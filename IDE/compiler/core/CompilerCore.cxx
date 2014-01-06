// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerCore.cxx
 */
// =============================================================================

// Compiler header files.
#include "CompilerCore.h"
#include "CompilerExpr.h"
#include "CompilerOptions.h"
#include "CompilerModules.h"
#include "CompilerStatement.h"
#include "CompilerMsgFilter.h"
#include "CompilerSerializer.h"
#include "CompilerMsgObserver.h"
#include "CompilerMsgInterface.h"
#include "CompilerMessageStack.h"
#include "CompilerSemanticAnalyzer.h"

// OS compatibility.
#include "../../utilities/os/os.h"

// Standard header files.
#include <fstream>

// Used for i18n only.
#include <QObject>

CompilerCore::CompilerCore ( CompilerMsgInterface * msgInterface )
{
    m_opts             = NULL;
    m_msgObserver      = NULL;
    m_rootStatement    = NULL;
    m_semanticAnalyzer = NULL;

    m_messageStack = new CompilerMessageStack;
    m_msgInterface = new CompilerMsgFilter ( this, msgInterface );

    m_fileNumber = -1;
}

CompilerCore::~CompilerCore()
{
    delete m_msgInterface;
    delete m_messageStack;

    if ( NULL != m_semanticAnalyzer )
    {
        delete m_semanticAnalyzer;
    }
    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
    }
}

bool CompilerCore::compile ( LangId lang,
                             TargetArch arch,
                             CompilerOptions * opts,
                             bool genSimData )
{
    m_lang = lang;
    m_arch = arch;
    m_opts = opts;
    m_simulatorData.m_genSimData = genSimData;

    bool result = startCompilation();
    resetCompilerCore();
    return result;
}

inline bool CompilerCore::startCompilation()
{
    using namespace CompilerModules;

    try
    {
        resetCompilerCore();

        if ( false == checkOptions() )
        {
            return false;
        }

        m_opts->normalizeFilePaths();
        m_opts->clearOutputFiles();

        m_msgInterface->m_messageLimit = m_opts->m_messageLimit;
        m_basePath = boost::filesystem::path(m_opts->m_sourceFiles[0]).parent_path();

        std::string errStr;
        ModEmplStatCode statusCode = employModule ( m_lang, m_arch, this, m_semanticAnalyzer, &errStr );

        switch ( statusCode )
        {
            case MESC_OK:
                return m_success;
            case MESC_IO_ERROR:
                coreMessage ( MT_ERROR, QObject::tr("unable to open file: ").toStdString() + errStr );
                return false;
            case MESC_ARCH_NOT_SUPPORTED:
                coreMessage ( MT_ERROR,
                               QObject::tr("architecture not supported for the selected language").toStdString() );
                return false;
            case MESC_LANG_NOT_SUPPORTED:
                coreMessage ( MT_ERROR, QObject::tr("programming language not supported").toStdString() );
                return false;
            case MESC_UNKNOWN_ERROR:
                return false;
        }
    }
    catch ( const boost::system::error_code & e )
    {
        coreMessage ( MT_ERROR, QObject::tr("failure: %1").arg(e.message().c_str()).toStdString() );
        return false;
    }
    catch ( const boost::filesystem::filesystem_error & e )
    {
        coreMessage ( MT_ERROR, QObject::tr("failure: %1").arg(e.what()).toStdString() );
        return false;
    }

    return false;
}

DbgFile * CompilerCore::getSimDbg()
{
    return m_simulatorData.m_simDbg;
}

DataFile * CompilerCore::getSimData()
{
    return m_simulatorData.m_simData;
}

inline bool CompilerCore::checkOptions()
{
    if ( CompilerBase::LI_INVALID == m_lang )
    {
        coreMessage ( MT_ERROR, QObject::tr("programming language not specified").toStdString() );
        return false;
    }

    if ( CompilerBase::TA_INVALID == m_arch )
    {
        coreMessage ( MT_ERROR, QObject::tr("target architecture not specified").toStdString() );
        return false;
    }

    if ( true == m_opts->m_sourceFiles.empty() )
    {
        coreMessage ( MT_ERROR, QObject::tr("source code file not specified").toStdString() );
        return false;
    }
    else
    {
        for ( const auto file : m_opts->m_sourceFiles )
        {
            if ( true == file.empty() )
            {
                coreMessage ( MT_ERROR, QObject::tr("empty string used as source code file name").toStdString() );
                return false;
            }
        }
    }

    return true;
}

inline std::string CompilerCore::msgType2str ( MessageType type )
{
    switch ( type )
    {
        case MT_INVALID:
            // This should never happen; when the control flow reaches this point, there is a bug in the compiler!
            return "";
        case MT_GENERAL:
            return QObject::tr("Message: ").toStdString();
        case MT_ERROR:
            m_success = false;
            return QObject::tr("Error: ").toStdString();
        case MT_WARNING:
            return QObject::tr("Warning: ").toStdString();
        case MT_REMARK:
            return QObject::tr("Remark: ").toStdString();
    }

    return "";
}

std::string CompilerCore::locationToStr ( const CompilerSourceLocation & location,
                                          bool main ) const
{
    std::string result;

    if ( ( false == main ) && ( -1 != location.m_origin ) )
    {
        bool first = true;
        std::string prefix;
        std::vector<const CompilerSourceLocation *> locationBackTr;
        m_locationTracker.traverse(location, &locationBackTr);
        for ( const auto i : locationBackTr )
        {
            if ( false == first )
            {
                prefix += " ==> ";
            }
            first = false;
            result += locationToStr ( *i, main );
        }
        return result;
    }

    if ( -1 == location.m_fileNumber || location.m_fileNumber >= (int)m_openedFiles.size() )
    {
        return "";
    }

    result += boost::filesystem::make_relative ( m_basePath, getFileName(location.m_fileNumber) ).string();

    char tmp[100];
    sprintf ( tmp,
              ":%d.%d-%d.%d",
              location.m_lineStart,
              location.m_colStart,
              location.m_lineEnd,
              location.m_colEnd );

    result.append(tmp);
    if ( true == main )
    {
        result.append(": ");
    }

    return result;
}

inline void CompilerCore::coreMessage ( MessageType type,
                                        const std::string & text )
{
    m_msgInterface->message(msgType2str(type) + text + ".", type);
}

void CompilerCore::coreMessage ( const CompilerSourceLocation & location,
                                 MessageType type,
                                 const std::string & text,
                                 bool forceAsUnique,
                                 bool noObserver )
{
    if ( -1 != location.m_origin )
    {
        std::string prefixNormal;
        std::string prefixObserver;
        std::vector<const CompilerSourceLocation *> locationBackTr;
        m_locationTracker.traverse(location, &locationBackTr, true);

        for ( const auto tracePoint : locationBackTr )
        {
            if ( -1 == tracePoint->m_origin )
            {
                coreMessage ( *tracePoint, type, prefixNormal + text, forceAsUnique, true );
                prefixNormal += "==> ";
            }
            else if ( NULL != m_msgObserver )
            {
                m_msgObserver->message(*tracePoint, type, prefixObserver + text);
                prefixObserver += "==> ";
            }
        }

        return;
    }

    if ( ( false == forceAsUnique )
           &&
         ( false == m_messageStack->isUnique(location, type, text, m_opts->m_briefMsgOutput) ) )
    {
        return;
    }

    if ( ( false == noObserver ) && ( NULL != m_msgObserver ) )
    {
        m_msgObserver->message(location, type, text);
    }

    m_msgInterface->message ( ( locationToStr(location, true) + msgType2str(type) + text + "." ), type );
}

void CompilerCore::preprocessorMessage ( const CompilerSourceLocation & location,
                                         MessageType type,
                                         const std::string & text,
                                         bool forceAsUnique )
{
    coreMessage(location, type, text, forceAsUnique);
}

void CompilerCore::lexerMessage ( const CompilerSourceLocation & location,
                                  MessageType type,
                                  const std::string & text,
                                  bool forceAsUnique )
{
    coreMessage(location, type, text, forceAsUnique);
}

void CompilerCore::parserMessage ( const CompilerSourceLocation & location,
                                   MessageType type,
                                   const std::string & text,
                                   bool forceAsUnique )
{
    coreMessage(location, type, text, forceAsUnique);
}

void CompilerCore::semanticMessage ( const CompilerSourceLocation & location,
                                     MessageType type,
                                     const std::string & text,
                                     bool forceAsUnique )
{
    coreMessage(location, type, text, forceAsUnique);
}

bool CompilerCore::successful() const
{
    return m_success;
}

inline void CompilerCore::setOpenedFile ( const std::string & filename,
                                          FILE ** fileHandle )
{
    int idx = getFileNumber(filename);

    m_filename = filename;
    if ( -1 != idx )
    {
        m_fileNumber = idx;
        if ( NULL != fileHandle )
        {
            if ( NULL == m_openedFiles[idx].second )
            {
                m_openedFiles[idx].second = (*fileHandle);
            }
            else
            {
                fclose(*fileHandle);
                (*fileHandle) = m_openedFiles[idx].second;
                rewind(*fileHandle);
            }
        }
    }
    else
    {
        m_fileNumber = m_openedFiles.size();
        m_openedFiles.push_back(std::make_pair(filename, *fileHandle));
    }
}

void CompilerCore::registerMsgObserver ( CompilerMsgObserver * observer )
{
    m_msgObserver = observer;
    m_msgObserver->setMaxNumberOfMessages(m_opts->m_messageLimit);
}

CompilerStatement * CompilerCore::loadDevSpecCode ( const std::string & deviceName,
                                                    CompilerBase::DevSpecLoaderFlag * flag )
{
    if ( true == m_devSpecCodeLoaded )
    {
        if ( deviceName != m_device )
        {
            coreMessage ( MT_WARNING, QObject::tr ( "cannot set device to %1, device specification code is already "
                                                    "loaded for %2")
                                                  . arg ( deviceName.c_str() )
                                                  . arg ( m_device.c_str() )
                                                  . toStdString() );
        }

        if ( NULL != flag )
        {
            *flag = DSLF_ALREADY_LOADED;
        }
        return NULL;
    }
    m_device = deviceName;
    m_devSpecCodeLoaded = true;

    std::string fileName = getBaseIncludeDir() + deviceName + PRECOMPILED_CODE_EXTENSION;

    if ( false == boost::filesystem::is_regular_file(fileName) )
    {
        if ( NULL != flag )
        {
            *flag = DSLF_DOES_NOT_EXIST;
        }
        return NULL;
    }

    CompilerStatement * result = loadPrecompiledCode(fileName, true);
    if ( NULL == result )
    {
        if ( NULL != flag )
        {
            *flag = DSLF_UNABLE_TO_READ;
        }
        coreMessage ( MT_ERROR, QObject::tr ( "unable to read file: `%1'" ).arg( fileName.c_str() ).toStdString() );
    }

    m_semanticAnalyzer->setDevice(deviceName);

    if ( NULL != flag )
    {
        *flag = DSLF_OK;
    }

    return result;
}

std::string CompilerCore::getBaseName()
{
    return boost::filesystem::path(m_opts->m_sourceFiles[0]).filename().string();
}

std::string CompilerCore::getBaseIncludeDir()
{
    using namespace boost::filesystem;

    std::string result = m_baseIncludeDir;

    switch ( m_lang )
    {
        case LI_INVALID:
            return "";
        case LI_C:
            result += "/C";
            break;
        case LI_ASM:
            result += "/assembler";
            break;
    }
    switch ( m_arch )
    {
        case TA_INVALID:
            return "";
        case TA_AVR8:
            result += "/avr8";
            break;
        case TA_PIC8:
            result += "/pic8";
            break;
        case TA_MCS51:
            result += "/mcs51";
            break;
        case TA_PICOBLAZE:
            result += "/PicoBlaze";
            break;
    }

    result += "/";

    return system_complete(path(result).make_preferred()).string();
}

CompilerStatement * CompilerCore::loadPrecompiledCode ( const std::string & fileName,
                                                        bool hide )
{
    try
    {
        std::ifstream file ( fileName, (std::ios_base::in | std::ios_base::binary) );
        CompilerSerializer deserializer(file, m_openedFiles, &m_locationTracker, m_lang, m_arch, hide);
        CompilerStatement * result = new CompilerStatement(deserializer);

        if ( true == file.bad() )
        {
            result->completeDelete();
            result = NULL;
        }

        return result;
    }
    catch ( const CompilerSerializer::Exception & e )
    {
        return NULL;
    }
}

bool CompilerCore::savePrecompiledCode ( const std::string & fileName,
                                         const CompilerStatement * code )
{
    try
    {
        std::ofstream file ( fileName, (std::ios_base::out | std::ios_base::binary) );
        CompilerSerializer serializer(file, m_openedFiles, &m_locationTracker, m_lang, m_arch);
        code->serializeTree(serializer);
        return ( !file.bad() );
    }
    catch ( const CompilerSerializer::Exception & )
    {
        return false;
    }
}

void CompilerCore::closeInputFiles()
{
    for ( std::vector<std::pair<std::string,FILE*>>::iterator it = m_openedFiles.begin();
          m_openedFiles.end() != it;
          it++ )
    {
        if ( NULL != it->second )
        {
            fclose(it->second);
            it->second = NULL;
        }
    }
}

inline void CompilerCore::resetCompilerCore()
{
    if ( NULL != m_msgObserver )
    {
        m_msgObserver->reset();
        m_msgObserver = NULL;
    }
    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
        m_rootStatement = NULL;
    }
    if ( NULL != m_semanticAnalyzer )
    {
        delete m_semanticAnalyzer;
        m_semanticAnalyzer = NULL;
    }

    // Close all opened input files.
    for ( std::vector<std::pair<std::string,FILE*>>::const_iterator it = m_openedFiles.cbegin();
          m_openedFiles.cend() != it;
          it++ )
    {
        if ( NULL != it->second )
        {
            fclose(it->second);
        }
    }

    m_success = true;
    m_devSpecCodeLoaded = false;

    m_messageStack->clear();
    m_msgInterface->clear();
    m_basePath.clear();

    m_filename.clear();
    m_openedFiles.clear();
    m_fileNameStack.clear();
    m_locationTracker.clear();

    resetCompilerParserInterface();
}

FILE * CompilerCore::fileOpen ( const std::string & filename,
                                std::string * finalFilename,
                                bool acyclic )
{
    using namespace boost::filesystem;

    std::string absoluteFileName;
    path filenamePath = path(makeHomeSafe(filename)).make_preferred();
    path basePath = path(m_filename).parent_path();

    if ( true == filenamePath.is_absolute() )
    {
        absoluteFileName = filenamePath.string();
    }
    else
    {
        if ( true == is_regular_file(basePath / filenamePath) )
        {
            absoluteFileName = (basePath / filenamePath).string();
        }
        else
        {
            for ( std::vector<std::string>::const_iterator it = m_opts->m_includePath.cbegin();
                  it != m_opts->m_includePath.cend();
                  it++ )
            {
                // Determinate absolute include path.
                path includePath = path(*it).make_preferred();
                if ( false == includePath.is_absolute() )
                {
                    includePath = ( basePath / includePath );
                }

                if ( ( true == is_directory(includePath) ) && ( true == is_regular_file(includePath / filenamePath) ) )
                {
                    absoluteFileName = (includePath / filenamePath).string();
                    break;
                }
            }
            return NULL;
        }
    }

    if ( true == acyclic )
    {
        for ( std::vector<std::string>::const_iterator it = m_fileNameStack.cbegin();
              it != m_fileNameStack.cend();
              ++it )
        {
            if ( *it == absoluteFileName )
            {
                coreMessage ( MT_ERROR, QObject::tr ( "file `%1' is already opened, you might have an "
                                                      "\"include\" loop in your code" )
                                                    .arg(absoluteFileName.c_str()).toStdString() );
                return NULL;
            }
        }
    }

    FILE * fileHandle = fopen(absoluteFileName.c_str(), "r");
    if ( NULL == fileHandle )
    {
        return NULL;
    }
    if ( false == pushFileName(absoluteFileName, &fileHandle) )
    {
        fclose(fileHandle);
        return NULL;
    }

    if ( NULL != finalFilename )
    {
        *finalFilename = absoluteFileName;
    }
    return fileHandle;
}

bool CompilerCore::pushFileName ( const std::string & filename,
                                  FILE ** fileHandle )
{
    if ( ( -1 != m_opts->m_maxInclusion )
           &&
         ( m_fileNameStack.size() >= (size_t)(m_opts->m_maxInclusion) ) )
    {
        coreMessage ( MT_ERROR, QObject::tr ( "maximum include level (%1) reached" )
                                            .arg(m_opts->m_maxInclusion).toStdString() );
        fclose(*fileHandle);
        return false;
    }

    setOpenedFile(filename, fileHandle);
    m_fileNameStack.push_back(filename);

    return true;
}

void CompilerCore::popFileName()
{
    m_fileNameStack.pop_back();
    setOpenedFile(m_fileNameStack.back());
}

int CompilerCore::getFileNumber() const
{
    return m_fileNumber;
}

int CompilerCore::getFileNumber ( unsigned int uplevel ) const
{
    if ( 0 == uplevel )
    {
        return m_fileNumber;
    }
    else
    {
        if ( m_fileNameStack.size() <= uplevel )
        {
            return -1;
        }
        else
        {
            return getFileNumber ( m_fileNameStack [ m_fileNameStack.size() - uplevel - 1 ] );
        }
    }
}

int CompilerCore::getFileNumber ( const std::string & filename ) const
{
    int result = -1;
    for ( unsigned int i = 0; i < m_openedFiles.size(); i++ )
    {
        if ( filename == m_openedFiles[i].first )
        {
            result = i;
        }
    }
    return result;
}

void CompilerCore::processCodeTree ( CompilerStatement * codeTree )
{
    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
        m_rootStatement = NULL;
    }

    if ( true == m_opts->m_syntaxCheckOnly )
    {
        codeTree->completeDelete();
        return;
    }

    m_rootStatement = new CompilerStatement();
    if ( NULL != codeTree )
    {
        m_rootStatement -> appendLink ( codeTree -> first() );
    }

    if ( NULL == m_semanticAnalyzer )
    {
        coreMessage ( MT_ERROR, QObject::tr ( "semantic analyzer is missing" ).toStdString() );
        return;
    }

    if ( false == m_opts->m_prcTarget.empty() )
    {
        if ( false == savePrecompiledCode ( m_opts->m_prcTarget, m_rootStatement ) )
        {
            coreMessage ( MT_ERROR, QObject::tr ( "unable to save precompiled code" ).toStdString() );
        }
    }

    if ( false == m_opts->m_device.empty() )
    {
        DevSpecLoaderFlag loaderFlag;
        CompilerStatement * devSpecCode = loadDevSpecCode(m_opts->m_device, &loaderFlag);
        if ( NULL == devSpecCode )
        {
            if ( DSLF_DOES_NOT_EXIST == loaderFlag )
            {
                coreMessage ( MT_ERROR, QObject::tr ( "device not supported: `%1'" )
                                                    . arg ( m_opts->m_device.c_str() )
                                                    . toStdString() );
            }
            return;
        }
        else
        {
            m_rootStatement->first()->insertLink(devSpecCode->next());
            delete devSpecCode;
        }
    }

    m_semanticAnalyzer->process(m_rootStatement);
}

const std::vector<std::pair<std::string,FILE*>> & CompilerCore::listSourceFiles() const
{
    return m_openedFiles;
}

const std::string & CompilerCore::getFileName ( int fileNumber ) const
{
    return m_openedFiles.at(fileNumber).first;
}

CompilerLocationTracker & CompilerCore::locationTrack()
{
    return m_locationTracker;
}

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
    m_opts             = nullptr;
    m_msgObserver      = nullptr;
    m_rootStatement    = nullptr;
    m_semanticAnalyzer = nullptr;

    m_messageStack = new CompilerMessageStack;
    m_msgInterface = new CompilerMsgFilter ( this, msgInterface );

    m_fileNumber = -1;
}

CompilerCore::~CompilerCore()
{
    delete m_msgInterface;
    delete m_messageStack;

    if ( nullptr != m_semanticAnalyzer )
    {
        delete m_semanticAnalyzer;
    }

    if ( nullptr != m_rootStatement )
    {
        m_rootStatement->completeDelete();
    }
}

void CompilerCore::setup ( LangId lang,
                           TargetArch arch,
                           CompilerOptions * opts,
                           bool genSimData )
{
    resetCompilerCore();

    m_lang = lang;
    m_arch = arch;
    m_opts = opts;

    m_simulatorData.m_genSimData = genSimData;
    m_msgInterface->m_messageLimit = opts->m_messageLimit;

    m_basePath = boost::filesystem::path(opts->m_sourceFiles[0]).parent_path();
}

DbgFile * CompilerCore::getSimDbg()
{
    return m_simulatorData.m_simDbg;
}

DataFile * CompilerCore::getSimData()
{
    return m_simulatorData.m_simData;
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
    const CompilerSourceLocation * loc = &location;

    while ( true )
    {
        bool prefix = false;
        if ( ( -1 != loc->m_fileNumber ) && ( loc->m_fileNumber < (int) m_openedFiles.size() ) )
        {
            result += boost::filesystem::make_relative ( m_basePath, getFileName(loc->m_fileNumber) ).string();

            char tmp [ 32 ];
            sprintf ( tmp,
                    ":%d.%d-%d.%d",
                    loc->m_lineStart,
                    loc->m_colStart,
                    loc->m_lineEnd,
                    loc->m_colEnd );

            result += tmp;
            prefix = true;
        }

        if ( ( false == main ) && ( -1 != loc->m_origin ) )
        {
            if ( true == prefix )
            {
                result += " ==> ";
            }
            loc = &( m_locationTracker.getLocation(loc->m_origin) );
            continue;
        }

        break;
    }

    return result;
}

void CompilerCore::coreMessage ( MessageType type,
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
    {
        bool ignoreLocation = m_opts->m_briefMsgOutput;
        if ( true == forceAsUnique )
        {
            ignoreLocation = false;
        }
        if ( false == m_messageStack->isUnique ( location, type, text, ignoreLocation ) )
        {
            return;
        }
    }

    {
        std::string prefix;
        const CompilerSourceLocation * loc = &location;

        if ( ( false == noObserver ) && ( nullptr != m_msgObserver ) )
        {
            while ( true )
            {
                m_msgObserver->message(*loc, type, prefix + text);

                if ( -1 != loc->m_origin )
                {
                    prefix += "==> ";
                    loc = &( m_locationTracker.getLocation(loc->m_origin) );
                    continue;
                }

                break;
            }
        }
    }

    if ( false == location.isSet() )
    {
        m_msgInterface->message ( ( msgType2str(type) + text + "." ), type );
    }
    else if ( ( -1 != location.m_fileNumber ) && ( location.m_fileNumber < (int) m_openedFiles.size() ) )
    {
        m_msgInterface->message ( ( locationToStr(location) + ": " + msgType2str(type) + text + "." ), type );
    }
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

void CompilerCore::printSummary ( const std::string & text )
{
    m_msgInterface->message(text, MT_REMARK);
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
        if ( nullptr != fileHandle )
        {
            if ( nullptr == m_openedFiles[idx].second )
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
        m_openedFiles.push_back({filename, *fileHandle});
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

        if ( nullptr != flag )
        {
            *flag = DSLF_ALREADY_LOADED;
        }
        return nullptr;
    }
    m_device = deviceName;
    m_devSpecCodeLoaded = true;

    std::string fileName = getBaseIncludeDir() + deviceName + PRECOMPILED_CODE_EXTENSION;

    if ( false == boost::filesystem::is_regular_file(fileName) )
    {
        if ( nullptr != flag )
        {
            *flag = DSLF_DOES_NOT_EXIST;
        }
        return nullptr;
    }

    CompilerStatement * result = loadPrecompiledCode(fileName, true);
    if ( nullptr == result )
    {
        if ( nullptr != flag )
        {
            *flag = DSLF_UNABLE_TO_READ;
        }
        coreMessage ( MT_ERROR, QObject::tr ( "unable to read file: `%1'" ).arg( fileName.c_str() ).toStdString() );
    }

    m_semanticAnalyzer->setDevice(deviceName);

    if ( nullptr != flag )
    {
        *flag = DSLF_OK;
    }

    return result;
}

std::string CompilerCore::getBaseName ( bool withExtension )
{
    if ( true == withExtension )
    {
        return boost::filesystem::path(m_opts->m_sourceFiles[0]).filename().string();
    }
    else
    {
        return boost::filesystem::path(m_opts->m_sourceFiles[0]).filename().stem().string();
    }
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
        case TA_ADAPTABLE:
            result += "/Adaptable";
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
            result = nullptr;
        }

        return result;
    }
    catch ( const CompilerSerializer::Exception & e )
    {
        return nullptr;
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
        return ( false == file.bad() );
    }
    catch ( const CompilerSerializer::Exception & )
    {
        return false;
    }
}

void CompilerCore::closeInputFiles()
{
    for ( auto & file : m_openedFiles )
    {
        if ( nullptr != file.second )
        {
            fclose(file.second);
            file.second = nullptr;
        }
    }
}

inline void CompilerCore::resetCompilerCore()
{
    if ( nullptr != m_msgObserver )
    {
        m_msgObserver->reset();
        m_msgObserver = nullptr;
    }
    if ( nullptr != m_rootStatement )
    {
        m_rootStatement->completeDelete();
        m_rootStatement = nullptr;
    }
    if ( nullptr != m_semanticAnalyzer )
    {
        delete m_semanticAnalyzer;
        m_semanticAnalyzer = nullptr;
    }

    // Close all opened input files.
    for ( const auto & file : m_openedFiles )
    {
        if ( nullptr != file.second )
        {
            fclose(file.second);
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
    m_locationMap.clear();

    resetCompilerParserInterface();
}

FILE * CompilerCore::fileOpen ( const std::string & filename,
                                std::string * finalFilename,
                                bool acyclic,
                                bool systemOnly )
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
        if ( ( false == systemOnly ) && ( true == is_regular_file(basePath / filenamePath) ) )
        {
            absoluteFileName = canonical(basePath / filenamePath).string();
        }
        else
        {
            bool usingSystemPaths = false;
            const std::vector<std::string> * includePaths = &(m_opts->m_includePath);
            if ( true == systemOnly )
            {
                usingSystemPaths = true;
                includePaths = &(m_opts->m_includePathSystem);
            }

            while ( true )
            {
                for ( const auto & iPath : *includePaths )
                {
                    // Determinate absolute include path.
                    path includePath = path(iPath).make_preferred();
                    if ( false == includePath.is_absolute() )
                    {
                        includePath = ( basePath / includePath );
                    }

                    if ( ( true == is_directory(includePath) ) && ( true == is_regular_file(includePath / filenamePath) ) )
                    {
                        absoluteFileName = canonical(includePath / filenamePath).string();
                        break;
                    }
                }

                if ( ( true == absoluteFileName.empty() ) && ( false == usingSystemPaths ) )
                {
                    usingSystemPaths = true;
                    includePaths = &(m_opts->m_includePathSystem);
                    continue;
                }

                break;
            }

            if ( true == absoluteFileName.empty() )
            {
                std::string ipats;

                if ( false == systemOnly )
                {
                    for ( const auto & iPath : m_opts->m_includePath )
                    {
                        if ( false == ipats.empty() )
                        {
                            ipats += ", ";
                        }
                        ipats += '`' + iPath + '\'';
                    }
                }

                for ( const auto & iPath : m_opts->m_includePathSystem )
                {
                    if ( false == ipats.empty() )
                    {
                        ipats += ", ";
                    }
                    ipats += '`' + iPath + '\'';
                }

                if ( false == systemOnly )
                {
                    coreMessage ( MT_ERROR, QObject::tr ( "unable to locate file `%1' in base path `%2', or include "
                                                          "path(s): %3" )
                                                        . arg ( (char*) filenamePath.c_str() )
                                                        . arg ( (char*) basePath.c_str() )
                                                        . arg ( ipats.c_str() )
                                                        . toStdString() );
                }
                else
                {
                    coreMessage ( MT_ERROR, QObject::tr ( "unable to locate file `%1' in include path(s): %2" )
                                                        . arg ( (char*) filenamePath.c_str() )
                                                        . arg ( ipats.c_str() )
                                                        . toStdString() );
                }

                return nullptr;
            }
        }
    }

    if ( true == acyclic )
    {
        for ( const auto & file : m_fileNameStack )
        {
            if ( file == absoluteFileName )
            {
                coreMessage ( MT_ERROR, QObject::tr ( "file `%1' is already opened, you might have an "
                                                      "inclusion loop in your code" )
                                                    .arg(absoluteFileName.c_str()).toStdString() );
                return nullptr;
            }
        }
    }

    FILE * fileHandle = fopen(absoluteFileName.c_str(), "r");
    if ( nullptr == fileHandle )
    {
        return nullptr;
    }
    if ( false == pushFileName(absoluteFileName, &fileHandle) )
    {
        return nullptr;
    }

    if ( nullptr != finalFilename )
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
        coreMessage ( MT_ERROR, QObject::tr ( "maximum file inclusion depth (%1) reached" )
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

void CompilerCore::setVirtualFileName ( const std::string & filename )
{
    for ( unsigned int i = 0; i < m_openedFiles.size(); i++ )
    {
        if ( filename == m_openedFiles[i].first )
        {
            m_fileNumber = i;
            return;
        }
    }

    m_fileNumber = m_openedFiles.size();
    m_openedFiles.push_back({filename, nullptr});
}

void CompilerCore::processCodeTree ( CompilerStatement * codeTree )
{
    if ( nullptr != m_rootStatement )
    {
        m_rootStatement->completeDelete();
    }

    m_rootStatement = new CompilerStatement();

    if ( nullptr != codeTree )
    {
        m_rootStatement -> appendLink ( codeTree -> first() );
    }
}

void CompilerCore::startSemanticAnalysis()
{
    if ( true == m_opts->m_syntaxCheckOnly )
    {
        m_rootStatement->completeDelete();
        m_rootStatement = nullptr;
        return;
    }

    if ( false == m_opts->m_prcTarget.empty() )
    {
        if ( false == savePrecompiledCode ( m_opts->m_prcTarget, m_rootStatement ) )
        {
            coreMessage ( MT_ERROR, QObject::tr ( "unable to save precompiled code" ).toStdString() );
        }
    }

    if ( false == m_success )
    {
        coreMessage ( MT_WARNING, QObject::tr ( "semantic analysis was not executed due to error(s)" )
                                              . toStdString() );
        return;
    }

    if ( nullptr == m_semanticAnalyzer )
    {
        coreMessage ( MT_ERROR, QObject::tr ( "semantic analyzer is missing" ).toStdString() );
        return;
    }

    if ( false == m_opts->m_device.empty() )
    {
        if ( TA_ADAPTABLE == m_arch )
        {
            m_semanticAnalyzer->setDevice(m_opts->m_device);
        }
        else
        {
            DevSpecLoaderFlag loaderFlag;
            CompilerStatement * devSpecCode = loadDevSpecCode(m_opts->m_device, &loaderFlag);
            if ( nullptr == devSpecCode )
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
    }

    m_semanticAnalyzer->process(m_rootStatement);
}

const std::vector<std::pair<std::string,FILE*>> & CompilerCore::listSourceFiles() const
{
    return m_openedFiles;
}

const std::string & CompilerCore::getFileName ( int fileNumber ) const
{
    if ( -1 == fileNumber )
    {
        return m_openedFiles.back().first;
    }
    else
    {
        return m_openedFiles.at(fileNumber).first;
    }
}

CompilerLocationTracker & CompilerCore::locationTrack()
{
    return m_locationTracker;
}

CompilerLocationMap & CompilerCore::locationMap()
{
    return m_locationMap;
}

void CompilerCore::setSemanticAnalyzer ( CompilerSemanticAnalyzer * semanticAnalyzer )
{
    m_semanticAnalyzer = semanticAnalyzer;
}

const boost::filesystem::path & CompilerCore::getBasePath() const
{
    return m_basePath;
}

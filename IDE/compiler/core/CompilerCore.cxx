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
#include <cstdio>
#include <sstream>
#include <fstream>

// Used for i18n only.
#include <QObject>

CompilerCore::CompilerCore ( CompilerMsgInterface * msgInterface )
                           : m_msgInterface ( new CompilerMsgFilter(this, msgInterface, MAX_MESSAGES) )
{
    m_opts = NULL;
    m_msgObserver = NULL;
    m_semanticAnalyzer = NULL;
    m_rootStatement = NULL;
    m_messageStack = new CompilerMessageStack;
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

        {
            using namespace boost::filesystem;
            m_basePath = system_complete(path(m_opts->m_sourceFile).parent_path().make_preferred());
        }

        ModEmplStatCode statusCode = employModule ( m_lang, m_arch, this, m_semanticAnalyzer );

        switch ( statusCode )
        {
            case MESC_OK:
                return true;
            case MESC_IO_ERROR:
                localMessage ( MT_ERROR, QObject::tr("unable to open file: ").toStdString() + m_opts->m_sourceFile );
                return false;
            case MESC_ARCH_NOT_SUPPORTED:
                localMessage ( MT_ERROR,
                               QObject::tr("architecture not supported for the selected language").toStdString() );
                return false;
            case MESC_LANG_NOT_SUPPORTED:
                localMessage ( MT_ERROR, QObject::tr("programming language not supported").toStdString() );
                return false;
            case MESC_UNKNOWN_ERROR:
                return false;
        }
    }
    catch ( const boost::system::error_code & e )
    {
        localMessage ( MT_ERROR, QObject::tr("failure: %1").arg(e.message().c_str()).toStdString() );
        return false;
    }
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
        localMessage ( MT_ERROR, QObject::tr("programming language not specified").toStdString() );
        return false;
    }

    if ( CompilerBase::TA_INVALID == m_arch )
    {
        localMessage ( MT_ERROR, QObject::tr("target architecture not specified").toStdString() );
        return false;
    }

    if ( true == m_opts->m_sourceFile.empty() )
    {
        localMessage ( MT_ERROR, QObject::tr("source code file not specified").toStdString() );
        return false;
    }

    return true;
}

inline std::string CompilerCore::msgType2str ( MessageType type )
{
    switch ( type )
    {
        case MT_INVALID:
            // This should never happen; when the control flow reaches this point, there is bug in the compiler!
            return "";
        case MT_GENERAL:
            return "";
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

inline void CompilerCore::localMessage ( MessageType type,
                                         const std::string & text )
{
    m_msgInterface->message(msgType2str(type) + text + ".", type);
}

void CompilerCore::localMessage ( const CompilerSourceLocation & location,
                                  MessageType type,
                                  const std::string & text )
{
    if ( false == m_messageStack->isUnique(location, type, text) )
    {
        return;
    }

    if ( NULL != m_msgObserver )
    {
        m_msgObserver->message(location, type, text);
    }

    std::stringstream msgText;
    msgText << m_filename;
    if ( location.m_lineStart > 0 )
    {
        msgText << ":" << location.m_lineStart << "." << location.m_colStart;
        if ( location.m_lineStart == location.m_lineEnd )
        {
            if ( location.m_colStart != location.m_colEnd )
            {
                msgText << "-" << location.m_colEnd;
            }
        }
        else
        {
            msgText << "-" << location.m_lineEnd << "." << location.m_colEnd;
        }
    }

    msgText << ": " << msgType2str(type) << text << ".";
    m_msgInterface->message(msgText.str(), type);
}

void CompilerCore::parserMessage ( const CompilerSourceLocation & location,
                                   MessageType type,
                                   const std::string & text )
{
    localMessage(location, type, text);
}

void CompilerCore::lexerMessage ( const CompilerSourceLocation & location,
                                  MessageType type,
                                  const std::string & text )
{
    localMessage(location, type, text);
}

void CompilerCore::compilerMessage ( const CompilerSourceLocation & location,
                                     MessageType type,
                                     const std::string & text )
{
    localMessage(location, type, text);
}

bool CompilerCore::successful() const
{
    return m_success;
}

void CompilerCore::compilerMessage ( MessageType type,
                                     const std::string & text )
{
    std::stringstream msgText;

    switch ( type )
    {
        case MT_INVALID:
            // This should never happen; when the control flow reaches this point, there is bug in the compiler!
            return;
        case MT_GENERAL:
            break;
        case MT_ERROR:
            msgText << QObject::tr("error: ").toStdString();
            m_success = false;
            break;
        case MT_WARNING:
            msgText << QObject::tr("warning: ").toStdString();
            break;
        case MT_REMARK:
            msgText << QObject::tr("remark: ").toStdString();
            break;
    }

    msgText << text << ".";
    m_msgInterface -> message ( msgText.str(), type );
}

inline void CompilerCore::setFileName ( const std::string & filename )
{
    int idx = getFileNumber(filename);

    m_filename = filename;
    if ( -1 != idx )
    {
        m_fileNumber = idx;
    }
    else
    {
        m_fileNumber = m_fileNames.size();
        m_fileNames.push_back(filename);
    }
}

void CompilerCore::registerMsgObserver ( CompilerMsgObserver * observer )
{
    m_msgObserver = observer;
    m_msgObserver->setMaxNumberOfMessages(MAX_MESSAGES);
}

CompilerStatement * CompilerCore::loadDevSpecCode ( const std::string & deviceName,
                                                    CompilerBase::DevSpecLoaderFlag * flag )
{
    if ( true == m_devSpecCodeLoaded )
    {
        localMessage ( MT_WARNING, QObject::tr ( "cannot set device to %1, device specification code is already loaded "
                                                 "for %2")
                                               . arg ( deviceName.c_str() )
                                               . arg ( m_device.c_str() )
                                               . toStdString() );
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
        localMessage ( MT_ERROR, QObject::tr ( "unable to read file: `%1'" ).arg( fileName.c_str() ).toStdString() );
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
    return boost::filesystem::path(m_opts->m_sourceFile).filename().string();
}

std::string CompilerCore::getBaseIncludeDir()
{
    using namespace boost::filesystem;

    std::string result = m_baseIncludeDir;

    switch ( m_lang )
    {
        case LI_INVALID:
            return "";
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
        CompilerSerializer deserializer(file, m_fileNames, m_lang, m_arch, hide);
        CompilerStatement * result = new CompilerStatement(deserializer);

        if ( true == file.bad() )
        {
            result->completeDelete();
            result = NULL;
        }

        return result;
    }
    catch ( CompilerSerializer::Exception & e )
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
        CompilerSerializer serializer(file, m_fileNames, m_lang, m_arch);
        code->serializeTree(serializer);
        return ( !file.bad() );
    }
    catch ( CompilerSerializer::Exception & )
    {
        return false;
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

    m_success = true;
    m_devSpecCodeLoaded = false;

    m_messageStack->reset();
    m_msgInterface->reset();
    m_basePath.clear();

    m_fileNameStack.clear();
    m_fileNames.clear();
    m_filename.clear();

    resetCompilerParserInterface();
}

FILE * CompilerCore::fileOpen ( const std::string & filename,
                                bool acyclic )
{
    using namespace boost::filesystem;

    std::string absoluteFileName;
    path filenamePath = path(filename).make_preferred();
    path basePath = system_complete(path(m_filename).parent_path().make_preferred());

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
                if ( true == is_regular_file(path(*it) / filenamePath) )
                {
                    absoluteFileName = (path(*it).make_preferred() / filenamePath).string();
                    break;
                }
            }
            return NULL;
        }
    }

    if ( true == acyclic )
    {
        for ( std::vector<std::string>::const_iterator it = m_fileNameStack.begin();
              it != m_fileNameStack.end();
              ++it )
        {
            if ( absoluteFileName == *it )
            {
                localMessage ( MT_ERROR, QObject::tr ( "file %1 is already opened, you might have an "
                                                       "\"include\" loop in your code" )
                                                     .arg(absoluteFileName.c_str()).toStdString() );
                return NULL;
            }
        }
    }

    if ( false == pushFileName(absoluteFileName) )
    {
        return NULL;
    }

    return fopen(absoluteFileName.c_str(), "r");
}

bool CompilerCore::pushFileName ( const std::string & filename )
{
    if ( ( -1 != m_opts->m_maxInclusion )
           &&
         ( m_fileNameStack.size() >= (size_t)(m_opts->m_maxInclusion) ) )
    {
        localMessage ( MT_ERROR, QObject::tr ( "maximum include level (%1) reached" )
                                             .arg(m_opts->m_maxInclusion).toStdString() );
        return false;
    }

    setFileName(filename);
    m_fileNameStack.push_back(filename);

    return true;
}

void CompilerCore::popFileName()
{
    m_fileNameStack.pop_back();
    setFileName(m_fileNameStack.back());
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
    for ( unsigned int i = 0; i < m_fileNames.size(); i++ )
    {
        if ( filename == m_fileNames[i] )
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
        localMessage ( MT_ERROR, QObject::tr ( "semantic analyzer is missing" ).toStdString() );
        return;
    }

    if ( false == m_opts->m_prcTarget.empty() )
    {
        if ( false == savePrecompiledCode ( m_opts->m_prcTarget, m_rootStatement ) )
        {
            localMessage ( MT_ERROR, QObject::tr ( "unable to save precompiled code" ).toStdString() );
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
                localMessage ( MT_ERROR, QObject::tr ( "device not supported: `%1'" )
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

const std::vector<std::string> & CompilerCore::listSourceFiles() const
{
    return m_fileNames;
}

const std::string & CompilerCore::getFileName ( int fileNumber ) const
{
    return m_fileNames.at(fileNumber);
}

std::string CompilerCore::locationToStr ( const CompilerSourceLocation & location ) const
{
    if ( -1 == location.m_fileNumber || location.m_fileNumber >= (int)m_fileNames.size() )
    {
        return "";
    }

    std::string result = boost::filesystem::make_relative ( m_basePath, getFileName(location.m_fileNumber) ).string();

    char tmp[100];
    sprintf ( tmp,
              ":%d.%d-%d.%d", 
              location.m_lineStart,
              location.m_colStart,
              location.m_lineEnd,
              location.m_colEnd );

    result.append(tmp);
    return result;
}

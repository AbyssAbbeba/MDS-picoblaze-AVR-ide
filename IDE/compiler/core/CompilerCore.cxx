// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerCore.cxx
 */
// =============================================================================

// Compiler header files.
#include "CompilerCore.h"
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerMsgInterface.h"
#include "CompilerOptions.h"
#include "CompilerSemanticAnalyzer.h"
#include "CompilerMsgObserver.h"
#include "CompilerMsgFilter.h"
#include "CompilerSerializer.h"

// OS compatibility.
#include "../../utilities/os/os.h"

// Standard header files.
#include <sstream>
#include <cstdio>
#include <fstream>

// Include all implemented semantic analyzers we have in this compiler collection.
#include "asm/avr8/AsmAvr8SemanticAnalyzer.h"
#include "asm/pic8/AsmPic8SemanticAnalyzer.h"
#include "asm/mcs51/AsmMcs51SemanticAnalyzer.h"
#include "asm/kcpsm3/AsmKcpsm3SemanticAnalyzer.h"

// Used for i18n only.
#include <QObject>

CompilerCore::CompilerCore ( CompilerMsgInterface * msgInterface )
                           : m_msgInterface ( new CompilerMsgFilter(this, msgInterface, MAX_MESSAGES) )
{
    m_opts = NULL;
    m_msgObserver = NULL;
    m_semanticAnalyzer = NULL;
    m_rootStatement = NULL;
    m_fileNumber = -1;
}

CompilerCore::~CompilerCore()
{
    delete m_msgInterface;
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

        if ( true == setupSemanticAnalyzer() )
        {
            return startLexerAndParser();
        }

        return false;
    }
    catch ( boost::system::error_code & e )
    {
        m_msgInterface->message ( QObject::tr("Failure: %1.").arg(e.message().c_str()).toStdString(),
                                  MT_ERROR );
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
        m_msgInterface->message ( QObject::tr("Programming language not specified.").toStdString(),
                                  MT_ERROR );
        return false;
    }

    if ( CompilerBase::TA_INVALID == m_arch )
    {
        m_msgInterface->message ( QObject::tr("Target architecture not specified.").toStdString(),
                                  MT_ERROR );
        return false;
    }

    if ( true == m_opts->m_sourceFile.empty() )
    {
        m_msgInterface->message ( QObject::tr("Source code file not specified.").toStdString(),
                                  MT_ERROR );
        return false;
    }

    return true;
}

inline bool CompilerCore::setupSemanticAnalyzer()
{
    switch ( m_lang )
    {
        case LI_ASM:
            switch ( m_arch )
            {
                case TA_AVR8:
                    m_semanticAnalyzer = new AsmAvr8SemanticAnalyzer ( this, m_opts );
                    break;
                case TA_PIC8:
                    m_semanticAnalyzer = new AsmPic8SemanticAnalyzer ( this, m_opts );
                    break;
                case TA_MCS51:
                    m_semanticAnalyzer = new AsmMcs51SemanticAnalyzer ( this, m_opts );
                    break;
                case TA_KCPSM3:
                    m_semanticAnalyzer = new AsmKcpsm3SemanticAnalyzer ( this, m_opts );
                    break;
                default:
                    m_msgInterface->message ( QObject::tr ( "Architecture not supported for the selected language." )
                                                          . toStdString(),
                                              MT_ERROR );
                    return false;
            }
            break;
        default:
            m_msgInterface->message ( QObject::tr("Programming language not supported.").toStdString(),
                                      MT_ERROR );
            return false;
    }

    return true;
}

inline bool CompilerCore::startLexerAndParser()
{

    FILE * sourceFile = fileOpen ( m_opts->m_sourceFile );
    yyscan_t yyscanner; // Pointer to the lexer context

    if ( NULL == sourceFile )
    {
        m_msgInterface->message ( QObject::tr("Error: unable to open file: ").toStdString() + m_opts->m_sourceFile,
                                  MT_ERROR );
        return false;
    }

    switch ( m_lang )
    {
        case LI_ASM:
            switch ( m_arch )
            {
                case TA_AVR8:
                    avr8lexer_lex_init_extra ( this, &yyscanner );
                    avr8lexer_set_in ( sourceFile, yyscanner );
                    if ( true == m_success )
                    {
                        avr8parser_parse ( yyscanner, this );
                    }
                    avr8lexer_lex_destroy ( yyscanner );
                    break;
                case TA_PIC8:
                    pic8lexer_lex_init_extra ( this, &yyscanner );
                    pic8lexer_set_in ( sourceFile, yyscanner );
                    if ( true == m_success )
                    {
                        pic8parser_parse ( yyscanner, this );
                    }
                    pic8lexer_lex_destroy ( yyscanner );
                    break;
                case TA_MCS51:
                    mcs51lexer_lex_init_extra ( this, &yyscanner );
                    mcs51lexer_set_in ( sourceFile, yyscanner );
                    if ( true == m_success )
                    {
                        mcs51parser_parse ( yyscanner, this );
                    }
                    mcs51lexer_lex_destroy ( yyscanner );
                    break;
                case TA_KCPSM3:
                    kcpsm3lexer_lex_init_extra ( this, &yyscanner );
                    kcpsm3lexer_set_in ( sourceFile, yyscanner );
                    if ( true == m_success )
                    {
                        kcpsm3parser_parse ( yyscanner, this );
                    }
                    kcpsm3lexer_lex_destroy ( yyscanner );
                    break;
                default:
                    m_msgInterface->message ( QObject::tr ( "Architecture not supported for the selected language." )
                                                          . toStdString(),
                                              MT_ERROR );
                    return false;
            }
            break;
        default:
            m_msgInterface->message ( QObject::tr("Programming language not supported.").toStdString(),
                                      MT_ERROR );
            return false;
    }

    fclose(sourceFile);
    return m_success;
}

void CompilerCore::parserMessage ( CompilerSourceLocation location,
                                   MessageType type,
                                   const std::string & text )
{
    if ( NULL != m_msgObserver )
    {
        m_msgObserver->message(location, type, text);
    }

    std::stringstream msgText;

    std::string msgType;
    switch ( type )
    {
        case MT_INVALID:
            // This should never happen; when the control flow reaches this point, there is bug in the compiler!
            return;
        case MT_GENERAL:
            break;
        case MT_ERROR:
            msgType = QObject::tr("error: ").toStdString();
            m_success = false;
            break;
        case MT_WARNING:
            msgType = QObject::tr("warning: ").toStdString();
            break;
        case MT_REMARK:
            msgType = QObject::tr("remark: ").toStdString();
            break;
    }

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

    msgText << ": " << msgType << text << ".";
    m_msgInterface->message(msgText.str(), type);
}

void CompilerCore::lexerMessage ( CompilerSourceLocation location,
                                  MessageType type,
                                  const std::string & text )
{
    parserMessage(location, type, text);
}

void CompilerCore::compilerMessage ( CompilerSourceLocation location,
                                     MessageType type,
                                     const std::string & text )
{
    parserMessage(location, type, text);
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
        m_msgInterface->message ( QObject::tr("Error: device specification code is already loaded.").toStdString(),
                                  MT_ERROR );
        if ( NULL != flag )
        {
            *flag = DSLF_ALREADY_LOADED;
        }
        return NULL;
    }
    m_devSpecCodeLoaded = true;

    boost::filesystem::path fileName = m_baseIncludeDir;
    fileName = system_complete(fileName.make_preferred());

    switch ( m_lang )
    {
        case LI_INVALID:
            return NULL;
        case LI_ASM:
            fileName /= "assembler";
            break;
    }
    switch ( m_arch )
    {
        case TA_INVALID:
            return NULL;
        case TA_AVR8:
            fileName /= "avr8";
            break;
        case TA_PIC8:
            fileName /= "pic8";
            break;
        case TA_MCS51:
            fileName /= "mcs51";
            break;
        case TA_KCPSM3:
            fileName /= "PicoBlaze";
            break;
    }
    fileName /= deviceName + PRECOMPILED_CODE_EXTENSION;

    if ( false == boost::filesystem::is_regular_file(fileName) )
    {
        if ( NULL != flag )
        {
            *flag = DSLF_DOES_NOT_EXIST;
        }
        return NULL;
    }

    CompilerStatement * result = loadPrecompiledCode(fileName.string(), true);
    if ( NULL == result )
    {
        if ( NULL != flag )
        {
            *flag = DSLF_UNABLE_TO_READ;
        }
        m_msgInterface->message ( QObject::tr ( "Error: unable to read file `%1'." )
                                                . arg ( fileName.c_str() )
                                                . toStdString(),
                                  MT_ERROR );
    }

    if ( NULL != flag )
    {
        *flag = DSLF_OK;
    }

    return result;
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
        return ( !file.fail() );
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
                m_msgInterface->message ( QObject::tr ( "Error: file %1 is already opened, you might have an "
                                                        "\"include\" loop in your code." )
                                                      .arg(absoluteFileName.c_str()).toStdString(),
                                          MT_ERROR );
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
        m_msgInterface->message ( QObject::tr ( "Error: maximum include level (%1) reached." )
                                              .arg(m_opts->m_maxInclusion).toStdString(),
                                  MT_ERROR );
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

void CompilerCore::syntaxAnalysisComplete ( CompilerStatement * codeTree )
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
        m_msgInterface->message ( QObject::tr ( "semantic analyzer is missing" ).toStdString(), MT_ERROR );
        return;
    }

    if ( false == m_opts->m_prcTarget.empty() )
    {
        if ( false == savePrecompiledCode ( m_opts->m_prcTarget, m_rootStatement ) )
        {
            m_msgInterface->message ( QObject::tr ( "unable to save precompiled code" ).toStdString(), MT_ERROR );
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
                m_msgInterface->message ( QObject::tr ( "Device not supported: `%1'" )
                                                      . arg ( m_opts->m_device.c_str() )
                                                      . toStdString(),
                                          MT_ERROR );
            }
            else if ( DSLF_ALREADY_LOADED == loaderFlag )
            {
                m_msgInterface->message ( QObject::tr ( "Device specification code is already "
                                                        "loaded" ).toStdString(),
                                          MT_ERROR );
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

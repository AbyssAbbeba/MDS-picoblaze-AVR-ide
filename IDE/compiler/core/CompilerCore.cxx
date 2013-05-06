// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
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
#include "SemanticAnalyzer.h"

// Standard header files.
#include <sstream>
#include <cstdio>

// Include all implemented semantic analyzers we have in this compiler collection.
#include "asm/avr8/AsmAvr8SemanticAnalyzer.h"
#include "asm/pic8/AsmPic8SemanticAnalyzer.h"
#include "asm/mcs51/AsmMcs51SemanticAnalyzer.h"
#include "asm/kcpsm3/AsmKcpsm3SemanticAnalyzer.h"

#include <QObject> // Used for i18n only

#include <iostream>

CompilerCore::CompilerCore ( CompilerMsgInterface * msgInterface )
                           : m_msgInterface(msgInterface)
{
    m_semanticAnalyzer = NULL;
    m_rootStatement = NULL;
    m_fileNumber = -1;
}

CompilerCore::~CompilerCore()
{
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
                             CompilerOptions * opts )
{
    resetCompilerCore();
    m_opts = opts;

    if ( false == checkOptions(lang, arch) )
    {
        return false;
    }

    if ( true == setupSemanticAnalyzer(lang, arch) )
    {
        return startLexerAndParser(lang, arch);
    }

    return false;
}

inline bool CompilerCore::checkOptions ( LangId lang,
                                         TargetArch arch )
{
    if ( CompilerBase::LI_INVALID == lang )
    {
        m_msgInterface->message ( QObject::tr("Programming language not specified.").toStdString(),
                                  MT_ERROR );
        return false;
    }

    if ( CompilerBase::TA_INVALID == arch )
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

inline bool CompilerCore::setupSemanticAnalyzer ( LangId lang,
                                                  TargetArch arch )
{
    switch ( lang )
    {
        case LI_ASM:
            switch ( arch )
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
                    m_msgInterface->message ( QObject::tr("Architecture not supported for the selected language.").toStdString(),
                                              MT_ERROR );
                    return false;
            }
            break;
        default:
            m_msgInterface->message ( QObject::tr("Programming language not supported.").toStdString(),
                                      MT_ERROR );
            return false;
    }

    return m_success;
}

inline bool CompilerCore::startLexerAndParser ( LangId lang,
                                                TargetArch arch )
{

    FILE * sourceFile = fileOpen ( m_opts->m_sourceFile.c_str() );
    yyscan_t yyscanner; // Pointer to the lexer context

    if ( NULL == sourceFile )
    {
        m_msgInterface->message ( QObject::tr("Error: unable to open file: ").toStdString() + m_opts->m_sourceFile,
                                  MT_ERROR );
        return false;
    }

    switch ( lang )
    {
        case LI_ASM:
            switch ( arch )
            {
                case TA_AVR8:
                    avr8lexer_lex_init_extra ( this, &yyscanner );
                    avr8lexer_set_in ( sourceFile, yyscanner );
                    avr8parser_parse ( yyscanner, this );
                    avr8lexer_lex_destroy ( yyscanner );
                    break;
                case TA_PIC8:
                    pic8lexer_lex_init_extra ( this, &yyscanner );
                    pic8lexer_set_in ( sourceFile, yyscanner );
                    pic8parser_parse ( yyscanner, this );
                    pic8lexer_lex_destroy ( yyscanner );
                    break;
                case TA_MCS51:
                    mcs51lexer_lex_init_extra ( this, &yyscanner );
                    mcs51lexer_set_in ( sourceFile, yyscanner );
                    mcs51parser_parse ( yyscanner, this );
                    mcs51lexer_lex_destroy ( yyscanner );
                    break;
                case TA_KCPSM3:
                    kcpsm3lexer_lex_init_extra ( this, &yyscanner );
                    kcpsm3lexer_set_in ( sourceFile, yyscanner );
                    kcpsm3parser_parse ( yyscanner, this );
                    kcpsm3lexer_lex_destroy ( yyscanner );
                    break;
                default:
                    m_msgInterface->message ( QObject::tr("Architecture not supported for the selected language.").toStdString(),
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

void CompilerCore::parserMessage ( SourceLocation location,
                                   MessageType type,
                                   const std::string & text )
{
    std::stringstream msgText;

    std::string msgType;
    switch ( type )
    {
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

void CompilerCore::lexerMessage ( SourceLocation location,
                                  MessageType type,
                                  const std::string & text )
{
    parserMessage(location, type, text);
}

void CompilerCore::compilerMessage ( SourceLocation location,
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

inline void CompilerCore::resetCompilerCore()
{
    m_success = true;
    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
        m_rootStatement = NULL;
    }
    if ( NULL != m_semanticAnalyzer )
    {
        delete m_semanticAnalyzer;
    }

    m_fileNameStack.clear();
    m_fileNames.clear();
    m_filename.clear();

    resetCompilerParserInterface();
}

FILE * CompilerCore::fileOpen ( const std::string & filename,
                                bool acyclic )
{
    if ( true == acyclic )
    {
        for ( std::vector<std::string>::const_iterator it = m_fileNameStack.begin();
              it != m_fileNameStack.end();
              ++it )
        {
            if ( filename == *it )
            {
                m_msgInterface->message ( QObject::tr ( "Error: file %1 is already opened, you might have an "
                                                        "\"include\" loop in your code." )
                                                      .arg(filename.c_str()).toStdString(),
                                          MT_ERROR );
                return NULL;
            }
        }
    }

    if ( false == pushFileName(filename) )
    {
        return NULL;
    }

    return fopen(filename.c_str(), "r");
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
    }
    m_rootStatement = new CompilerStatement();
    if ( NULL != codeTree )
    {
        m_rootStatement -> appendLink ( codeTree -> first() );
    }
    if ( NULL == m_semanticAnalyzer )
    {
        m_msgInterface->message ( QObject::tr ( "Semantic analyzer is missing!" ).toStdString(), MT_ERROR );
        return;
    }
    else
    {
        m_semanticAnalyzer->process(m_rootStatement);
    }
}

const std::vector<std::string> & CompilerCore::listSourceFiles() const
{
    return m_fileNames;
}

const std::string & CompilerCore::getFileName ( int fileNumber ) const
{
    return m_fileNames.at(fileNumber);
}

std::string CompilerCore::locationToStr ( const CompilerBase::SourceLocation & location ) const
{
    std::string result = getFileName(location.m_fileNumber);

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

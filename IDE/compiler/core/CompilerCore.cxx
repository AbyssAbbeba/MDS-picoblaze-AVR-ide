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

#include "CompilerCore.h"

#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerMsgInterface.h"
#include "CompilerOptions.h"

#include <sstream>
#include <cstdio>
#include <iostream> // DEBUG

#include <QObject> // Used for i18n only

CompilerCore::CompilerCore ( CompilerMsgInterface * msgInterface )
                           : m_msgInterface(msgInterface)
{
    m_semanticAnalyser = NULL;
    m_rootStatement = NULL;
    m_fileNumber = -1;
}

CompilerCore::~CompilerCore()
{
    if ( NULL != m_semanticAnalyser )
    {
        delete m_semanticAnalyser;
    }
    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
    }
}

bool CompilerCore::compile ( LangId lang,
                             TargetArch arch,
                             CompilerOptions * const opts,
                             const std::string & filename )
{
    resetCompilerCore();

    FILE * sourceFile = fileOpen(filename.c_str());
    yyscan_t yyscanner; // Pointer to the lexer context

    if ( NULL == sourceFile )
    {
        m_msgInterface->message ( QObject::tr("Error: unable to open file: ").toStdString() + m_filename,
                                  MT_ERROR );
        return false;
    }

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
    }

    fclose(sourceFile);
    return m_success;
}

inline bool CompilerCore::setupSemanticAnalyser ( LangId lang,
                                                  TargetArch arch,
                                                  CompilerOptions * const opts,
                                                  const std::string & filename )
{
    m_semanticAnalyser = new AsmAvr8SemanticAnalyser(opts, filename);
    m_semanticAnalyser = new AsmPic8SemanticAnalyser(opts, filename);
    m_semanticAnalyser = new AsmMcs51SemanticAnalyser(opts, filename);
}

inline bool CompilerCore::startLexerAndParser ( LangId lang,
                                                TargetArch arch,
                                                CompilerOptions * const opts,
                                                const std::string & filename )
{
    
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

void CompilerCore::setFileName ( const std::string & filename )
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
        delete m_rootStatement;
        m_rootStatement = NULL;
    }
    if ( NULL != m_semanticAnalyser )
    {
        delete m_semanticAnalyser;
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
    if ( m_fileNameStack.size() >= 10 /* TODO: replace this with some variable */ )
    {
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
    std::cout << ">>> syntaxAnalysisComplete:\n";

    if ( NULL != m_rootStatement )
    {
        m_rootStatement->completeDelete();
    }
    m_rootStatement = codeTree;
    if ( NULL != m_rootStatement )
    {
        m_rootStatement = m_rootStatement->first();
    }
    std::cout << m_rootStatement;

    if ( NULL == m_semanticAnalyser )
    {
        m_msgInterface->message ( QObject::tr ( "Semantic analyser missing!" ), MT_ERROR );
        return;
    }
    else
    {
        m_semanticAnalyser->process(codeTree);
    }
}

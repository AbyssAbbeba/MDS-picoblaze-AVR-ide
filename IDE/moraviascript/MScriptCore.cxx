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
 * @ingroup MoraviaScript
 * @file MScriptCore.cxx
 */
// =============================================================================

#include "MScriptCore.h"

// Include lexer prototypes (they are used by the core to initialize and destroy a lexer)
#include "moraviaScriptParser.h"
#include "moraviaScriptLexer.h"

// MScript language interpreter header files.
#include "MScriptExpr.h"

// Standard header files.
#include <iostream>

// Parser prototype (the core uses it to initiate syntactical analysis).
int moraviaScriptParser_parse ( yyscan_t yyscanner,
                                MScriptParserInterface * core );


MScriptCore::MScriptCore ( MScriptStrategy * strategy,
                           const std::string & scriptCode )
                         : m_strategy ( strategy )
{
    m_codeTree = NULL;
    loadScript(scriptCode);
}

MScriptCore::MScriptCore ( MScriptStrategy * strategy,
                           FILE * sourceFile,
                           const std::string & fileName )
                         :
                           m_strategy ( strategy )
{
    m_codeTree = NULL;
    loadScript(sourceFile, fileName);
}

MScriptCore::~MScriptCore()
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }
}

bool MScriptCore::loadScript ( const std::string & scriptCode )
{
    unloadScript();

    /*
     * Initiate lexical and syntax analysis of the source code.
     */
    yyscan_t yyscanner; // Pointer to the lexer context
    moraviaScriptLexer_lex_init_extra ( this, &yyscanner );
    YY_BUFFER_STATE bufferState = moraviaScriptLexer__scan_string ( scriptCode.c_str(), yyscanner );
    if ( true == m_success )
    {
        moraviaScriptParser_parse ( yyscanner, this );
    }
    moraviaScriptLexer__delete_buffer ( bufferState, yyscanner );
    moraviaScriptLexer_lex_destroy ( yyscanner );

    return m_success;
}

bool MScriptCore::loadScript ( FILE * sourceFile,
                               const std::string & fileName )
{
    unloadScript();
    m_files.push_back(fileName);
    return loadFile(sourceFile);
}

bool MScriptCore::loadFile ( FILE * file )
{
    yyscan_t yyscanner; // Pointer to the lexer context
    moraviaScriptLexer_lex_init_extra ( this, &yyscanner );
    moraviaScriptLexer_set_in ( file, yyscanner );
    if ( true == m_success )
    {
        moraviaScriptParser_parse ( yyscanner, this );
    }
    moraviaScriptLexer_lex_destroy ( yyscanner );

    return m_success;
}

void MScriptCore::unloadScript()
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
        m_codeTree = NULL;
    }
    m_messages.clear();
    m_files.clear();
    m_success = true;
}

bool MScriptCore::executeStep()
{
    step();
    return true;
}

bool MScriptCore::executeRun()
{
    while ( true == step() );
    return true;
}

std::vector<std::string> & MScriptCore::getMessages()
{
    return m_messages;
}

void MScriptCore::clearMessages()
{
    m_messages.clear();
}

void MScriptCore::parserMessage ( const MScriptSrcLocation & location,
                                  MScriptBase::MessageType type,
                                  const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text );

    if ( type == MT_ERROR )
    {
        throw MScriptRunTimeError();
    }
}

void MScriptCore::lexerMessage ( const MScriptSrcLocation & location,
                                 MScriptBase::MessageType type,
                                 const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text );
}

void MScriptCore::interpreterMessage ( const MScriptSrcLocation & location,
                                       MScriptBase::MessageType type,
                                       const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text );
}

MScriptBase * MScriptCore::getCoreBase()
{
    return this;
}

void MScriptCore::syntaxAnalysisComplete ( MScriptStatement * codeTree )
{
    if ( NULL != m_codeTree )
    {
        m_includedCode = codeTree;
        return;
    }

    m_codeTree->completeDelete();
    m_codeTree = ( new MScriptStatement(MScriptSrcLocation(), MScriptStmtTypes::STMT_ROOT) ) -> appendLink ( codeTree );
    std::cout << m_codeTree;

    init(m_codeTree);
}

int MScriptCore::getFileNumber ( const std::string & fileName )
{
    int result = 0;
    for ( std::vector<std::string>::const_iterator i = m_files.cbegin();
          i != m_files.cend();
          i++ )
    {
        if ( fileName == *i )
        {
            return result;
        }
        result++;
    }

    return -1;
}

void MScriptCore::rewriteFileNumbers ( MScriptStatement * code,
                                       int fileNumber ) const
{
    for ( MScriptStatement * node = code;
          NULL != node;
          node = node->next() )
    {
        node->m_location.m_file = fileNumber;
        if ( NULL != node->branch() )
        {
            rewriteFileNumbers ( node->branch(), fileNumber );
        }

        for ( MScriptExpr * arg = node->args();
              NULL != arg;
              arg = arg->next() )
        {
            rewriteFileNumbers ( arg, fileNumber );
        }
    }
}

void MScriptCore::rewriteFileNumbers ( MScriptExpr * expr,
                                       int fileNumber ) const
{
    expr->m_location.m_file = fileNumber;

    MScriptValue * value = &( expr->m_lValue );
    for ( int i = 0; i < 2; i++ )
    {
        if ( MScriptValue::TYPE_EXPR == value->m_type )
        {
            rewriteFileNumbers ( value->m_data.m_expr, fileNumber );
        }

        value = &( expr->m_rValue );
    }
}

MScriptStatement * MScriptCore::include ( const MScriptSrcLocation & location,
                                          const std::string & fileName )
{
    int fileNumber = getFileNumber(fileName);
    if ( -1 == fileNumber )
    {
        fileNumber = (int) m_files.size();
        m_files.push_back(fileName);
    }
    FILE * file = fopen ( fileName.c_str(), "r" );
    if ( NULL == file )
    {
        interpreterMessage ( location,
                             MScriptBase::MT_ERROR,
                             QObject::tr ( "unable to open file: `%1'" )
                                         . arg ( fileName.c_str() )
                                         . toStdString() );
        return NULL;
    }

    loadFile(file);
    rewriteFileNumbers(m_includedCode, fileNumber);
    return m_includedCode;
}


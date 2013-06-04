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
 * @ingroup MoraviaScript
 * @file MScriptCore.cxx
 */
// =============================================================================

#include "MScriptCore.h"

// Include lexer prototypes (they are used by the core to initialize and destroy a lexer)
#include "moraviaScriptParser.h"
#include "moraviaScriptLexer.h"

// Standard header files
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
                           FILE * sourceFile )
                         : m_strategy ( strategy )
{
    m_codeTree = NULL;
    loadScript(sourceFile);
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

bool MScriptCore::loadScript ( FILE * sourceFile )
{
    unloadScript();

    /*
     * Initiate lexical and syntax analysis of the source code.
     */
    yyscan_t yyscanner; // Pointer to the lexer context
    moraviaScriptLexer_lex_init_extra ( this, &yyscanner );
    moraviaScriptLexer_set_in ( sourceFile, yyscanner );
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

void MScriptCore::parserMessage ( MScriptSrcLocation location,
                                  MScriptBase::MessageType type,
                                  const std::string & text )
{
    m_messages.push_back(location.toString() + " " + msgTypeToStr(type) + ": " + text );
}

void MScriptCore::lexerMessage ( MScriptSrcLocation location,
                                 MScriptBase::MessageType type,
                                 const std::string & text )
{
    m_messages.push_back(location.toString() + " " + msgTypeToStr(type) + ": " + text );
}

void MScriptCore::interpretMessage ( MScriptSrcLocation location,
                                     MScriptBase::MessageType type,
                                     const std::string & text )
{
    m_messages.push_back(location.toString() + " " + msgTypeToStr(type) + ": " + text );
}

void MScriptCore::syntaxAnalysisComplete ( MScriptStatement * codeTree )
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }

    m_codeTree = ( new MScriptStatement(MScriptSrcLocation(), MScriptStmtTypes::STMT_ROOT) ) -> appendLink ( codeTree );
    std::cout << m_codeTree;

    init(m_codeTree);
}

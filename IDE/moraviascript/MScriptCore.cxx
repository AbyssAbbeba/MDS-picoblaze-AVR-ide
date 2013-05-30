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
                           const std::string & script )
                         : m_strategy ( strategy )
{
    m_codeTree = NULL;
    loadScript(script);
}

MScriptCore::~MScriptCore()
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }
}

bool MScriptCore::loadScript ( const std::string & script )
{
    unloadScript();

    /*
     * Initiate lexical and syntax analysis of the source code.
     */
    yyscan_t yyscanner; // Pointer to the lexer context
    moraviaScriptLexer_lex_init_extra ( this, &yyscanner );
    YY_BUFFER_STATE bufferState = moraviaScriptLexer__scan_string ( script.c_str(), yyscanner );
    if ( true == m_success )
    {
        moraviaScriptParser_parse ( yyscanner, this );
    }
    moraviaScriptLexer__delete_buffer ( bufferState, yyscanner );
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

}

bool MScriptCore::executeRun()
{

}

std::vector<std::string> & MScriptCore::getMessages()
{
    return m_messages;
}

void MScriptCore::clearMessages()
{
    m_messages.clear();
}

void MScriptCore::syntaxAnalysisComplete ( MScriptStatement * codeTree )
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }

    m_codeTree = ( new MScriptStatement() ) -> appendLink ( codeTree );
    std::cout << m_codeTree;
}

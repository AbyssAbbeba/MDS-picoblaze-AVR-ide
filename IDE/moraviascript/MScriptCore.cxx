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
 * @ingroup MoraviaScript
 * @file MScriptCore.cxx
 */
// =============================================================================

#include "MScriptCore.h"

// Include lexer prototypes (they are used by the core to initialize and destroy a lexer)
#include "MScriptParser.h"
#include "MScriptLexer.h"

// MScript language interpreter header files.
#include "MScriptExpr.h"
#include "MScriptStrategy.h"

// Standard header files.
#include <iostream>

// Parser prototype (the core uses it to initiate syntactical analysis).
int MScriptParser_parse ( yyscan_t yyscanner,
                          MScriptParserInterface * core );


MScriptCore::MScriptCore ( MScriptStrategy * strategy,
                           const std::string & scriptCode )
                         : m_strategy ( strategy )
{
    m_strategy->m_core = this;
    m_codeTree = nullptr;
    loadScript(scriptCode);
}

MScriptCore::MScriptCore ( MScriptStrategy * strategy,
                           FILE * sourceFile,
                           const std::string & fileName )
                         :
                           m_strategy ( strategy )
{
    m_strategy->m_core = this;
    m_codeTree = nullptr;
    loadScript(sourceFile, fileName);
}

MScriptCore::~MScriptCore()
{
    if ( nullptr != m_codeTree )
    {
        m_codeTree->completeDelete();
    }
}

bool MScriptCore::loadScript ( const std::string & scriptCode )
{
    unloadScript();
    return loadCode(scriptCode);
}

bool MScriptCore::loadScript ( FILE * sourceFile,
                               const std::string & fileName )
{
    unloadScript();
    m_files.push_back(fileName);
    return loadFile(sourceFile);
}

bool MScriptCore::loadCode ( const std::string & scriptCode )
{
    yyscan_t yyscanner; // Pointer to the lexer context
    MScriptLexer_lex_init_extra ( this, &yyscanner );
    YY_BUFFER_STATE bufferState = MScriptLexer__scan_string ( scriptCode.c_str(), yyscanner );
    if ( true == m_success )
    {
        MScriptParser_parse ( yyscanner, this );
    }
    MScriptLexer__delete_buffer ( bufferState, yyscanner );
    MScriptLexer_lex_destroy ( yyscanner );

    return m_success;
}

bool MScriptCore::loadFile ( FILE * file )
{
    yyscan_t yyscanner; // Pointer to the lexer context
    MScriptLexer_lex_init_extra ( this, &yyscanner );
    MScriptLexer_set_in ( file, yyscanner );
    if ( true == m_success )
    {
        MScriptParser_parse ( yyscanner, this );
    }
    MScriptLexer_lex_destroy ( yyscanner );

    return m_success;
}

void MScriptCore::unloadScript()
{
    if ( nullptr != m_codeTree )
    {
        m_codeTree->completeDelete();
        m_codeTree = nullptr;
    }
    m_messages.clear();
    m_files.clear();
    m_success = true;
}

void MScriptCore::parserMessage ( const MScriptSrcLocation & location,
                                  MScriptBase::MessageType type,
                                  const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text + "." );

    if ( type == MT_ERROR )
    {
        throw MScriptRunTimeError();
    }
}

void MScriptCore::lexerMessage ( const MScriptSrcLocation & location,
                                 MScriptBase::MessageType type,
                                 const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text + "." );

    if ( type == MT_ERROR )
    {
        throw MScriptRunTimeError();
    }
}

void MScriptCore::interpreterMessage ( const MScriptSrcLocation & location,
                                       MScriptBase::MessageType type,
                                       const std::string & text )
{
    m_messages.push_back(location.toString(this) + " " + msgTypeToStr(type) + ": " + text + "." );

    if ( type == MT_ERROR )
    {
        throw MScriptRunTimeError();
    }
}

void MScriptCore::strategyMessage ( MScriptBase::MessageType type,
                                    const std::string & text )
{
    m_messages.push_back(msgTypeToStr(type) + ": " + text + "." );

    if ( type == MT_ERROR )
    {
        throw MScriptRunTimeError();
    }
}

MScriptBase * MScriptCore::getCoreBase()
{
    return this;
}

MScriptStrategy * MScriptCore::getStrategy()
{
    return m_strategy;
}

void MScriptCore::syntaxAnalysisComplete ( MScriptStatement * codeTree )
{
    if ( nullptr != m_codeTree )
    {
        m_includedCode = codeTree;
        return;
    }

    m_codeTree->completeDelete();
    m_codeTree = ( new MScriptStatement(MScriptSrcLocation(), MScriptStmtTypes::STMT_ROOT) ) -> appendLink ( codeTree );

    std::cout << "========================================================================================================================\n" << "== syntaxAnalysisComplete ( ... )                                                                                     ==\n" << "========================================================================================================================\n" << m_codeTree<< "========================================================================================================================\n\n\n";

    init(m_codeTree);
}

int MScriptCore::getFileNumber ( const std::string & fileName )
{
    int result = 0;
    for ( const auto & file : m_files )
    {
        if ( fileName == file )
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
          nullptr != node;
          node = node->next() )
    {
        node->m_location.m_file = fileNumber;
        if ( nullptr != node->branch() )
        {
            rewriteFileNumbers ( node->branch(), fileNumber );
        }

        for ( MScriptExpr * arg = node->args();
              nullptr != arg;
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

void MScriptCore::locationRelativeTo ( MScriptStatement * code,
                                       const MScriptSrcLocation & location )
{
    for ( MScriptStatement * node = code;
          nullptr != node;
          node = node->next() )
    {
        node->m_location.m_line[0]   += ( location.m_line[0] - 1 );
        node->m_location.m_line[0]   += ( location.m_line[1] - 1 );
        node->m_location.m_column[0] += ( location.m_column[0] - 1 );
        node->m_location.m_column[1] += ( location.m_column[1] - 1 );
        node->m_location.m_file       = location.m_file;

        if ( nullptr != node->branch() )
        {
            locationRelativeTo ( node->branch(), location );
        }

        for ( MScriptExpr * arg = node->args();
              nullptr != arg;
              arg = arg->next() )
        {
            locationRelativeTo ( arg, location );
        }
    }
}

void MScriptCore::locationRelativeTo ( MScriptExpr * expr,
                                       const MScriptSrcLocation & location )
{
    expr->m_location.m_line[0]   += ( location.m_line[0] - 1 );
    expr->m_location.m_line[0]   += ( location.m_line[1] - 1 );
    expr->m_location.m_column[0] += ( location.m_column[0] - 1 );
    expr->m_location.m_column[1] += ( location.m_column[1] - 1 );
    expr->m_location.m_file       = location.m_file;

    MScriptValue * value = &( expr->m_lValue );
    for ( int i = 0; i < 2; i++ )
    {
        if ( MScriptValue::TYPE_EXPR == value->m_type )
        {
            locationRelativeTo ( value->m_data.m_expr, location );
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
    if ( nullptr == file )
    {
        interpreterMessage ( location,
                             MScriptBase::MT_ERROR,
                             QObject::tr ( "unable to open file: `%1'" )
                                         . arg ( fileName.c_str() )
                                         . toStdString() );
        return nullptr;
    }

    loadFile(file);
    rewriteFileNumbers(m_includedCode, fileNumber);
    return m_includedCode;
}

MScriptStatement * MScriptCore::insertCode ( const MScriptSrcLocation & location,
                                             const std::string & code )
{
    loadCode(code);
    locationRelativeTo(m_includedCode, location);
    return m_includedCode;
}

MScriptFuncTable * MScriptCore::getFuncTbl()
{
    return m_funcTable;
}

MScriptVarTable * MScriptCore::getVarTbl()
{
    return m_varTable;
}

MScriptNamespaces * MScriptCore::getNs()
{
    return m_namespaces;
}

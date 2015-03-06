// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCTreeDecoder.cxx
 */
// =============================================================================

#include "CompilerCTreeDecoder.h"

// Standard headers.
#include<iostream>//DEBUG

// Compiler header files.
#include "CompilerOptions.h"
#include "CompilerStatement.h"
#include "CompilerSemanticInterface.h"

// C compiler header files.
#include "CompilerCSymbolTable.h"
#include "CompilerCDeclaration.h"

CompilerCTreeDecoder::CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                             CompilerOptions           * opts,
                                             CompilerCSymbolTable      * symbolTable,
                                             CompilerCExprProcessor    * exprProcessor )
                                           :
                                             m_compilerCore ( compilerCore ),
                                             m_opts ( opts ),
                                             m_symbolTable ( symbolTable ),
                                             m_exprProcessor ( exprProcessor )
{
}

CompilerCTreeDecoder::~CompilerCTreeDecoder()
{
}

void CompilerCTreeDecoder::processCodeTree ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case C_DECLARATION:
                processDeclaration(node->args());
                break;
            case C_FUNCTION_DEF:
                processFuncDef(node);
                break;
            default:
                unexpectedNode(node->location());
                return;
        }
    }
}

/*
(
    ( datatype
        (0x10000 datatype
            ( 's' pair
                (
                    (0x8 datatype) declaration
                        (pair 'n')
                ) | (
                    (0x80 datatype) declaration
                        (pair
                            (
                                ('d') [array]
                                    ( declaration )
                            )
                        )
                )
            )
        )
    ) declaration
)
*/

inline void CompilerCTreeDecoder::processDeclaration ( CompilerExpr * declaration )
{
    std::cout << "C_DECLARATION:" << declaration << '\n';
    std::cout << "left="<<declaration->lVal()<<'\n';
    std::cout << "op="<<declaration->oper()<<'\n';
    std::cout << "right="<<declaration->rVal()<<'\n'<<'\n';

    bool isTypedef;
    CompilerCDeclaration * absDeclar = resolveDeclaration(isTypedef, declaration);
    if ( nullptr != absDeclar )
    {
        if ( true == isTypedef )
        {
            m_symbolTable->newDataType(absDeclar);
        }
        else
        {
            m_symbolTable->newSymbol(absDeclar);
        }
    }
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDeclaration ( bool & isTypedef,
                                                                  const CompilerExpr * declaration )
{
    if ( CompilerExpr::OPER_DECLARATION != declaration->oper() )
    {
        unexpectedNode(declaration->location());
        return nullptr;
    }
}

inline void CompilerCTreeDecoder::processFuncDef ( CompilerStatement * definition )
{
    std::cout << "C_FUNCTION_DEF:"<<definition->args()<<'\n';
}

inline void CompilerCTreeDecoder::processExpressions ( CompilerExpr * expr )
{
    std::cout << "processExpressions\n";
//     m_exprProcessor->something(...);
}

inline void CompilerCTreeDecoder::unexpectedNode ( const CompilerSourceLocation & location )
{
    m_compilerCore->semanticMessage ( location,
                                      CompilerBase::MT_ERROR,
                                      QObject::tr ( "internal error, please report this as a bug in the compiler "
                                                    "implementation" ).toStdString() );
}

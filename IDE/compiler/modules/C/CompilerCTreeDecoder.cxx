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

CompilerCTreeDecoder::CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                             CompilerOptions           * opts,
                                             CompilerCSymbolTable      * symbolTable )
                                           :
                                             m_compilerCore ( compilerCore ),
                                             m_opts ( opts ),
                                             m_symbolTable ( symbolTable )
{
}

CompilerCTreeDecoder::~CompilerCTreeDecoder()
{
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
void CompilerCTreeDecoder::processDeclarations ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case C_DECLARATION:
std::cout << "C_DECLARATION:" << node->args() << '\n';
std::cout << "left="<<node->args()->lVal()<<'\n';
std::cout << "op="<<node->args()->oper()<<'\n';
std::cout << "right="<<node->args()->rVal()<<'\n'<<'\n';
                break;
            case C_FUNCTION_DEF:
                std::cout << "C_FUNCTION_DEF\n";
                break;
            default:
                unexpectedNode(node);
                return;
        }
    }
}

inline void CompilerCTreeDecoder::unexpectedNode ( CompilerStatement * node )
{
    m_compilerCore->semanticMessage ( node->location(),
                                      CompilerBase::MT_ERROR,
                                      QObject::tr ( "internal error, please report this as a bug in the compiler "
                                                    "implementation" ).toStdString() );
}

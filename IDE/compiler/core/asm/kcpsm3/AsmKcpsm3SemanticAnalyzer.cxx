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
 * @file AsmKcpsm3SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmKcpsm3SemanticAnalyzer.h"
#include "StatementTypes.h"

#include <iostream> // DEBUG

void AsmKcpsm3SemanticAnalyzer::process ( CompilerStatement * & codeTree )
{
    std::cout << "AsmKcpsm3SemanticAnalyzer::process:\n" << codeTree;

    processDeclarations(codeTree);
}

inline void AsmKcpsm3SemanticAnalyzer::processDeclarations ( CompilerStatement * & node )
{
    using namespace StatementTypes;

    node = node->first();
    while ( NULL != node->next() )
    {
        std::cout << "type: " << node->type() << "\n";
        switch ( (int) node->type() )
        {
            case ASMAVR8_DIR_EQU:
                std::cout << "    >>> EQU " << node->type() << "\n";

                addToSymbolTable(node->args()->at(0)->copyChainLink(), node->args()->at(1)->copyChainLink());
                break;
        }

        node = node->next();
    }
}

inline void AsmKcpsm3SemanticAnalyzer::addToSymbolTable ( CompilerExpr * const name,
                                                        CompilerExpr * const value )
{
    std::cout << "addToSymbolTable ( " << name << ", " << value << " );\n";
}

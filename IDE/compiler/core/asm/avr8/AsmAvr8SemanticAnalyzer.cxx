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
 * @ingroup Avr8Asm
 * @file AsmAvr8SemanticAnalyzer.cxx
 */
// =============================================================================

#include "AsmAvr8SemanticAnalyzer.h"

#include <iostream> // DEBUG

void AsmAvr8SemanticAnalyzer::process ( CompilerStatement * codeTree )
{
    std::cout << "AsmAvr8SemanticAnalyzer::process:\n" << codeTree;

    processDeclarations(codeTree);
}

/*
            ASMAVR8_DIR_IF,                 ///<
            ASMAVR8_DIR_IFN,                ///<
            ASMAVR8_DIR_IFDEF,              ///<
            ASMAVR8_DIR_IFNDEF,             ///<
            ASMAVR8_DIR_ELSEIFB,            ///<
            ASMAVR8_DIR_ELSEIFNB,           ///<
            ASMAVR8_DIR_ELSE,               ///<
            ASMAVR8_DIR_ELSEIF,             ///<
            ASMAVR8_DIR_ELSEIFN,            ///<
            ASMAVR8_DIR_ELSEIFDEF,          ///<
            ASMAVR8_DIR_ELSEIFNDEF,         ///<
            ASMAVR8_DIR_IFNB,               ///<
            ASMAVR8_DIR_IFB,                ///<

            ASMAVR8_DIR_LOCAL,              ///<
            ASMAVR8_DIR_SKIP,               ///<
            ASMAVR8_DIR_EXITM,              ///<
            ASMAVR8_DIR_REPT,               ///<
            ASMAVR8_DIR_MACRO,              ///<
            ASMAVR8_DIR_BYTE,               ///<
            ASMAVR8_DIR_CSEG,               ///<
            ASMAVR8_DIR_DB,                 ///<
            ASMAVR8_DIR_DEF,                ///<
            ASMAVR8_DIR_DEVICE,             ///<
            ASMAVR8_DIR_DSEG,               ///<
            ASMAVR8_DIR_DW,                 ///<
//             ASMAVR8_DIR_EQU,                ///<
            ASMAVR8_DIR_ESEG,               ///<
            ASMAVR8_DIR_EXIT,               ///<
            ASMAVR8_DIR_INCLUDE,            ///<
            ASMAVR8_DIR_LIST,               ///<
            ASMAVR8_DIR_LISTMAC,            ///<
            ASMAVR8_DIR_NOLIST,             ///<
            ASMAVR8_DIR_ORG,                ///<
            ASMAVR8_DIR_SET,                ///<
*/

inline void AsmAvr8SemanticAnalyzer::processDeclarations ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

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

inline void AsmAvr8SemanticAnalyzer::addToSymbolTable ( CompilerExpr * const name,
                                                        CompilerExpr * const value )
{
    std::cout << "addToSymbolTable ( " << name << ", " << value << " );\n";
}

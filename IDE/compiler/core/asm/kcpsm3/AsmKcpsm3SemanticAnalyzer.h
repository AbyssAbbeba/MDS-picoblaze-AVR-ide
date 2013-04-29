// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SemanticAnalyzer.h
 */
// =============================================================================

#ifndef ASMKCPSM3SEMANTICANALYSER_H
#define ASMKCPSM3SEMANTICANALYSER_H

#include "../../CompilerSemanticInterface.h"
#include "../../CompilerStatement.h"
#include "../../CompilerExpr.h"

#include "../AsmMachineCodeGen.h"

#include "AsmKcpsm3SymbolTable.h"
#include "AsmKcpsm3CodeListing.h"
#include "AsmKcpsm3InstructionSet.h"

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3SemanticAnalyzer
 */
class AsmKcpsm3SemanticAnalyzer : public CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3SemanticAnalyzer ( CompilerCore * compilerCore,
                                    CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~AsmKcpsm3SemanticAnalyzer();

    protected:
        /**
         * @brief Forbidden constructor
         */
        AsmKcpsm3SemanticAnalyzer() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void process ( CompilerStatement * codeTree );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        inline void phase1 ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in,out] codeTree
         */
        inline void phase2 ( CompilerStatement * codeTree );

    ////    Protected Attributes    ////
    protected:
        ///
        AsmKcpsm3SymbolTable * m_symbolTable;

        ///
        AsmMachineCodeGen * m_machineCode;

        ///
        AsmKcpsm3CodeListing * m_codeListing;

        ///
        AsmKcpsm3InstructionSet * m_instructionSet;
};

#endif // ASMKCPSM3SEMANTICANALYSER_H

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
 * @file AsmKcpsm3InstructionSet.h
 */
// =============================================================================

#ifndef ASMKCPSM3INSTRUCTIONSET_H
#define ASMKCPSM3INSTRUCTIONSET_H

// Forward declarations
class CompilerStatement;

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"

// KCPSM3 assembler semantic analyzer header files.
#include "AsmKcpsm3SymbolTable.h"

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3InstructionSet
 */
class AsmKcpsm3InstructionSet
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum OPER_BIT_SIZE
        {
            OBS_AAA = 10, ///< Address
            OBS_SXY =  4, ///< Register
            OBS_KK  =  8, ///< Constant
            OBS_PP  =  8, ///< Port ID
            OBS_SS  =  6  ///< Scratch Pad RAM
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3InstructionSet ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * opts )
                                : m_compilerCore ( compilerCore ),
                                  m_opts ( opts ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] stmt
         * @param[in,out] symbolTable
         * @return
         */
        int resolveOPcode ( CompilerStatement * stmt,
                            AsmKcpsm3SymbolTable * symbolTable );

        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        bool isInstruction ( const CompilerStatement * const stmt ) const;

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;
};

#endif // ASMKCPSM3INSTRUCTIONSET_H

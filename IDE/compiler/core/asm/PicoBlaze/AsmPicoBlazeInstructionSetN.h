// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeInstructionSetN.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEINSTRUCTIONSETN_H
#define ASMPICOBLAZEINSTRUCTIONSETN_H

// Forward declarations.
class AsmPicoBlazeSymbolTable;
class AsmPicoBlazeInstructionSet;

// Common compiler header files.
#include "../../CompilerStatement.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeInstructionSetN
 */
class AsmPicoBlazeInstructionSetN
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmPicoBlazeInstructionSetN();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] stmt
         * @return -1 == instruction not supported, valid OPCODE otherwise.
         */
        virtual int resolveOPcode ( const CompilerStatement * stmt ) const = 0;

        /**
         * @brief
         * @param[in,out] instructionSet
         * @param[in,out] symbolTable
         */
        void init ( AsmPicoBlazeInstructionSet * instructionSet,
                    AsmPicoBlazeSymbolTable * symbolTable );

    ////    Protected Attributes    ////
    protected:
        ///
        AsmPicoBlazeSymbolTable * m_symbolTable;

        ///
        AsmPicoBlazeInstructionSet * m_instructionSet;
};

#endif // ASMPICOBLAZEINSTRUCTIONSETN_H

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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeInstructionSetN.cxx
 */
// =============================================================================

#include "AsmPicoBlazeInstructionSetN.h"


AsmPicoBlazeInstructionSetN::AsmPicoBlazeInstructionSetN()
{
    m_symbolTable = nullptr;
    m_instructionSet = nullptr;
}

void AsmPicoBlazeInstructionSetN::init ( AsmPicoBlazeInstructionSet * instructionSet,
                                         AsmSymbolTable * symbolTable )
{
    m_instructionSet = instructionSet;
    m_symbolTable = symbolTable;
}

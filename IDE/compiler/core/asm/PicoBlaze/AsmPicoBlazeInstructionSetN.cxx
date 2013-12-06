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
    m_symbolTable = NULL;
    m_instructionSet = NULL;
}

void AsmPicoBlazeInstructionSetN::init ( AsmPicoBlazeInstructionSet * instructionSet,
                                         AsmPicoBlazeSymbolTable * symbolTable )
{
    m_instructionSet = instructionSet;
    m_symbolTable = symbolTable;
}

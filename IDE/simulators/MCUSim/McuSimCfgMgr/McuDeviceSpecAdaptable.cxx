// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpecAdaptable.cxx
 */
// =============================================================================

#include "McuDeviceSpecAdaptable.h"
#include "MCUSim.h"

McuDeviceSpecAdaptable::McuDeviceSpecAdaptable ( const AdjSimProcDef & procDef )
                                               : McuDeviceSpec ( MCUSim::Arch::ARCH_ADAPTABLE ),
                                                 m_procDef ( procDef )
{
}

bool McuDeviceSpecAdaptable::setupSimulator ( AdaptableSimConfig & mcuConfig ) const
{
    mcuConfig.m_family = MCUSimBase::FAMILY_ADAPTABLE_0;
    mcuConfig.m_name = m_procDef.m_name;

    mcuConfig.m_configStack->m_size = m_procDef.m_stack.m_size;
    mcuConfig.m_configStack->m_useDesignatedStack = m_procDef.m_stack.m_useDesignatedStack;
    mcuConfig.m_configStack->m_simpleStack.m_operation = AdaptableSimStack::Config::SimpleStack::Operation ( m_procDef.m_stack.m_simpleStack.m_operation );
    mcuConfig.m_configStack->m_simpleStack.m_content.m_space = AdaptableSimStack::Config::SimpleStack::Space ( m_procDef.m_stack.m_simpleStack.m_content.m_space );
    mcuConfig.m_configStack->m_simpleStack.m_content.m_offset = m_procDef.m_stack.m_simpleStack.m_content.m_offset;
    mcuConfig.m_configStack->m_simpleStack.m_pointer.m_space = AdaptableSimStack::Config::SimpleStack::Space ( m_procDef.m_stack.m_simpleStack.m_pointer.m_space );
    mcuConfig.m_configStack->m_simpleStack.m_pointer.m_address = m_procDef.m_stack.m_simpleStack.m_pointer.m_address;
    mcuConfig.m_configStack->m_simpleStack.m_pointer.m_maxSize = m_procDef.m_stack.m_simpleStack.m_pointer.m_maxSize;
    mcuConfig.m_configStack->m_simpleStack.m_pointer.m_indirect = m_procDef.m_stack.m_simpleStack.m_pointer.m_indirect;

    mcuConfig.m_configRegisters->m_size = m_procDef.m_memory.m_register.m_size;
    mcuConfig.m_configRegisters->m_banks = m_procDef.m_memory.m_register.m_banks;

    mcuConfig.m_configDataMemory->m_size = m_procDef.m_memory.m_data.m_size;

    mcuConfig.m_configStatusFlags->m_backupWhenInterrupted = m_procDef.m_flags.m_backupWhenInterrupted;
    mcuConfig.m_configStatusFlags->m_autoDisableInterrupts = m_procDef.m_flags.m_autoDisableInterrupts;
    mcuConfig.m_configStatusFlags->m_autoEnableInterrupts = m_procDef.m_flags.m_autoEnableInterrupts;
    mcuConfig.m_configStatusFlags->m_hasZero = m_procDef.m_flags.m_hasZero;
    mcuConfig.m_configStatusFlags->m_hasCarry = m_procDef.m_flags.m_hasCarry;
    mcuConfig.m_configStatusFlags->m_hasOverflow = m_procDef.m_flags.m_hasOverflow;
    mcuConfig.m_configStatusFlags->m_hasNegative = m_procDef.m_flags.m_hasNegative;
    mcuConfig.m_configStatusFlags->m_hasHalfCarry = m_procDef.m_flags.m_hasHalfCarry;
    mcuConfig.m_configStatusFlags->m_hasParity = m_procDef.m_flags.m_hasParity;

    mcuConfig.m_configProgramMemory->m_size = m_procDef.m_memory.m_program.m_size;
    switch ( m_procDef.m_memory.m_program.m_word )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B:
            mcuConfig.m_configProgramMemory->m_wordSize = AdaptableSimProgramMemory::WORD_1B;
            break;
        case AdjSimProcDef::Memory::Program::WORD_2B:
            mcuConfig.m_configProgramMemory->m_wordSize = AdaptableSimProgramMemory::WORD_2B;
            break;
        case AdjSimProcDef::Memory::Program::WORD_3B:
            mcuConfig.m_configProgramMemory->m_wordSize = AdaptableSimProgramMemory::WORD_3B;
            break;
    }
    switch ( m_procDef.m_memory.m_program.m_endian )
    {
        case AdjSimProcDef::Memory::Program::END_BIG:
            mcuConfig.m_configProgramMemory->m_endian = AdaptableSimProgramMemory::END_BIG;
            break;
        case AdjSimProcDef::Memory::Program::END_LITTLE:
            mcuConfig.m_configProgramMemory->m_endian = AdaptableSimProgramMemory::END_LITTLE;
            break;
    }

    mcuConfig.m_configInstructionSet->m_pcMax = ( m_procDef.m_memory.m_program.m_size - 1 );
    mcuConfig.m_configInstructionSet->m_interruptVector = m_procDef.m_interruptVector;
    mcuConfig.m_configInstructionSet->m_opCodeSize = m_procDef.m_instructionSet.front().m_opCode.size();

    unsigned short id = 0;
    mcuConfig.m_configInstructionSet->m_instructions.clear();
    for ( const AdjSimProcDef::Instruction & inst : m_procDef.m_instructionSet )
    {
        AdaptableSimInstruction instruction;

        instruction.m_code = 0;
        instruction.m_mask = 0;

        unsigned int bitNumber = 0;
        for ( const AdjSimProcDef::Instruction::OpCodeBit bit : inst.m_opCode )
        {
            switch ( bit )
            {
                case AdjSimProcDef::Instruction::OCB_ZERO:
                    instruction.m_mask |= ( 1 << bitNumber );
                    break;
                case AdjSimProcDef::Instruction::OCB_ONE:
                    instruction.m_code |= ( 1 << bitNumber );
                    instruction.m_mask |= ( 1 << bitNumber );
                    break;
                case AdjSimProcDef::Instruction::OCB_DONT_CARE:
                    break;
            }
            bitNumber++;
        }

        instruction.m_id = (unsigned short) id++;
        instruction.m_timeP = (unsigned short) inst.m_time.m_positive;
        instruction.m_timeN = (unsigned short) inst.m_time.m_negative;
        instruction.m_nextP = (unsigned short) inst.m_next.m_positive;
        instruction.m_nextN = (unsigned short) inst.m_next.m_negative;
        instruction.m_operation = translateOP(inst.m_operation);

        instruction.m_parameters.m_data = 0;
        instruction.m_fixedOperands.m_data = 0;

        unsigned int operNumber = 0;
        for ( const AdjSimProcDef::Instruction::Operand & oper : inst.m_operands )
        {
            instruction.m_parameters.m_data |= ( (unsigned int) oper.m_type << ( operNumber * 4 ) );

            for ( int perm : oper.m_OPCodePermutation )
            {
                instruction.m_permutation[operNumber].push_back((unsigned char)perm);
            }
            for ( unsigned int i = instruction.m_permutation[operNumber].size(); i < (unsigned int) oper.m_size; i++ )
            {
                instruction.m_permutation[operNumber].push_back(0);
            }

            if ( -1 != oper.m_fixedValue )
            {
                instruction.m_fixedOperands.m_data |= ( oper.m_fixedValue << ( 16 * operNumber ) );
                instruction.m_fixedOperands.m_data |= ( 1 << ( 24 + operNumber ) );
            }

            operNumber++;
        }

        for ( int i = 0; i < AdjSimProcDef::Instruction::FLAG__MAX__; i++ )
        {
            instruction.m_parameters.m_data |= ( (int) inst.m_flags[i] ) << ( 12 + 2 * i );
        }

        instruction.m_parameters.m_data |= (uint64_t) inst.m_condition.m_flag << 28;
        instruction.m_parameters.m_data |= (uint64_t) inst.m_condition.m_type << 32;

        if ( AdjSimProcDef::Instruction::OperParameters::P_EVEN == inst.m_operParameters.m_parity )
        {
            instruction.m_parameters.m_data |= 1ULL << 33;
        }

        instruction.m_parameters.m_data |= ( inst.m_operParameters.m_ignoreCarryFlag ? ( 1ULL << 34 ) : 0 );


        if ( AdjSimProcDef::Instruction::OperParameters::RES_STORE_IN_3RD == inst.m_operParameters.m_result )
        {
            instruction.m_parameters.m_data |= 1ULL << 35;
        }
        else if ( AdjSimProcDef::Instruction::OperParameters::RES_DISCARD == inst.m_operParameters.m_result )
        {
            instruction.m_parameters.m_data |= 1ULL << 36;
        }

        if ( -1 != inst.m_next.m_positive )
        {
            instruction.m_parameters.m_data |= 1ULL << 37;
        }

        if ( -1 != inst.m_next.m_negative )
        {
            instruction.m_parameters.m_data |= 1ULL << 38;
        }

        instruction.m_parameters.m_data |= ( inst.m_operParameters.m_ignoreZeroFlag  ? ( 1ULL << 39 ) : 0 );

        mcuConfig.m_configInstructionSet->m_instructions.push_back(instruction);
    }

    return true;
}

inline AdaptableSimOperationID::ID McuDeviceSpecAdaptable::translateOP ( AdjSimProcDef::Instruction::Operation o ) const
{
    using namespace AdaptableSimOperationID;

    switch ( o )
    {
        case AdjSimProcDef::Instruction::OP_NONE:               return OP_NONE;
        case AdjSimProcDef::Instruction::OP_ABS_JUMP:           return OP_ABS_JUMP;
        case AdjSimProcDef::Instruction::OP_ABS_CALL:           return OP_ABS_CALL;
        case AdjSimProcDef::Instruction::OP_REL_JUMP:           return OP_REL_JUMP;
        case AdjSimProcDef::Instruction::OP_REL_CALL:           return OP_REL_CALL;
        case AdjSimProcDef::Instruction::OP_OFS_JUMP:           return OP_OFS_JUMP;
        case AdjSimProcDef::Instruction::OP_OFS_CALL:           return OP_OFS_CALL;
        case AdjSimProcDef::Instruction::OP_IDX_JUMP:           return OP_IDX_JUMP;
        case AdjSimProcDef::Instruction::OP_IDX_CALL:           return OP_IDX_CALL;
        case AdjSimProcDef::Instruction::OP_RETURN:             return OP_RETURN;
        case AdjSimProcDef::Instruction::OP_ISR_RETURN:         return OP_ISR_RETURN;
        case AdjSimProcDef::Instruction::OP_SET_BANK:           return OP_SET_BANK;
        case AdjSimProcDef::Instruction::OP_MOVE:               return OP_MOVE;
        case AdjSimProcDef::Instruction::OP_CB_MOVE:            return OP_CB_MOVE;
        case AdjSimProcDef::Instruction::OP_MOVE_BIT:           return OP_MOVE_BIT;
        case AdjSimProcDef::Instruction::OP_CB_MOVE_BIT:        return OP_CB_MOVE_BIT;
        case AdjSimProcDef::Instruction::OP_SWAP:               return OP_SWAP;
        case AdjSimProcDef::Instruction::OP_CB_SWAP:            return OP_CB_SWAP;
        case AdjSimProcDef::Instruction::OP_SWAP_BIT:           return OP_SWAP_BIT;
        case AdjSimProcDef::Instruction::OP_CB_SWAP_BIT:        return OP_CB_SWAP_BIT;
        case AdjSimProcDef::Instruction::OP_CPL:                return OP_CPL;
        case AdjSimProcDef::Instruction::OP_BIT_TEST:           return OP_BIT_TEST;
        case AdjSimProcDef::Instruction::OP_ADD:                return OP_ADD;
        case AdjSimProcDef::Instruction::OP_SUB:                return OP_SUB;
        case AdjSimProcDef::Instruction::OP_AND:                return OP_AND;
        case AdjSimProcDef::Instruction::OP_OR:                 return OP_OR;
        case AdjSimProcDef::Instruction::OP_XOR:                return OP_XOR;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_0:       return OP_SHIFT_LEFT_0;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_0:      return OP_SHIFT_RIGHT_0;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_1:       return OP_SHIFT_LEFT_1;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_1:      return OP_SHIFT_RIGHT_1;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_R:       return OP_SHIFT_LEFT_R;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_R:      return OP_SHIFT_RIGHT_R;
        case AdjSimProcDef::Instruction::OP_SHIFT_LEFT_C:       return OP_SHIFT_LEFT_C;
        case AdjSimProcDef::Instruction::OP_SHIFT_RIGHT_C:      return OP_SHIFT_RIGHT_C;
        case AdjSimProcDef::Instruction::OP_ROTATE_LEFT:        return OP_ROTATE_LEFT;
        case AdjSimProcDef::Instruction::OP_ROTATE_RIGHT:       return OP_ROTATE_RIGHT;
    }

    // Just to eliminate one compiler (GCC) warning.
    return OP_NONE;
}

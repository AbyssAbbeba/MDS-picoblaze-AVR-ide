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
 * @ingroup AdaptableSim
 * @file AdaptableSimOperations.cxx
 */
// =============================================================================

#include "AdaptableSimOperations.h"

#include "AdaptableSimIO.h"
#include "AdaptableSimStack.h"
#include "AdaptableSimRegisters.h"
#include "AdaptableSimDataMemory.h"
#include "AdaptableSimStatusFlags.h"
#include "AdaptableSimInstruction.h"
#include "AdaptableSimProgramMemory.h"
#include "AdaptableSimInterruptController.h"

bool AdaptableSimOperations::operationSwitch ( AdaptableSimInstruction::OperationID operation,
                                               unsigned int operands [ AdaptableSimInstruction::OPERANDS_MAX ],
                                               const AdaptableSimInstruction & inst )
{
    const AdaptableSimInstruction::OperParam parameters = inst.m_parameters;

    if ( AdaptableSimInstruction::OperParam::C_NONE != parameters.condition() )
    {
        bool flag = m_statusFlags->getFlag(parameters.conditionFlag());
        if ( AdaptableSimInstruction::OperParam::C_POSITIVE == parameters.condition() )
        {
            if ( false == flag )
            {
                return false;
            }
        }
        else if ( true == flag )
        {
            return false;
        }
    }

    switch ( operation )
    {
        case AdaptableSimInstruction::OP_NONE:
            break;
        case AdaptableSimInstruction::OP_ABS_JUMP:
            instAbsoluteJump(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_ABS_CALL:
            instAbsoluteCall(parameters, operands[0]);
            break;
        case AdaptableSimInstruction::OP_REL_JUMP:
            instRelativeJump(parameters, operands[0], inst.m_permutation[0]);
            break;
        case AdaptableSimInstruction::OP_REL_CALL:
            instRelativeCall(parameters, operands[0], inst.m_permutation[0]);
            break;
        case AdaptableSimInstruction::OP_OFS_JUMP:
            instOffsetJump(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_OFS_CALL:
            instOffsetCall(parameters, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_IDX_JUMP:
            instIndexJump(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_IDX_CALL:
            instIndexCall(parameters, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_RETURN:
            instReturn();
            break;
        case AdaptableSimInstruction::OP_BIT_TEST:
            if ( false == instBitTest(parameters, operands[0], operands[1]) )
            {
                return false;
            }
            break;
        case AdaptableSimInstruction::OP_ISR_RETURN:
            instIsrReturn();
            break;
        case AdaptableSimInstruction::OP_SET_BANK:
            instSetBank(operands[0]);
            break;
        case AdaptableSimInstruction::OP_MOVE:
            instMove(inst, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_MOVE:
            instCbMove(inst, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_MOVE_BIT:
            instMoveBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_MOVE_BIT:
            instCbMoveBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_SWAP:
            instSwap(inst, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_SWAP:
            instCbSwap(inst, operands[0], operands[1]);
            break;
        case AdaptableSimInstruction::OP_SWAP_BIT:
            instSwapBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_CB_SWAP_BIT:
            instCbSwapBit(parameters, operands[0], operands[1], operands[1]);
            break;
        case AdaptableSimInstruction::OP_ADD:
            instAdd(inst, operands[0], operands[1], operands[2]);
            break;
        case AdaptableSimInstruction::OP_SUB:
            instAdd(inst, operands[0], operands[1], operands[2], true);
            break;
        case AdaptableSimInstruction::OP_AND:
            instLogOper(inst, operands[0], operands[1], operands[2], LOG_AND);
            break;
        case AdaptableSimInstruction::OP_OR:
            instLogOper(inst, operands[0], operands[1], operands[2], LOG_OR);
            break;
        case AdaptableSimInstruction::OP_XOR:
            instLogOper(inst, operands[0], operands[1], operands[2], LOG_XOR);
            break;
        case AdaptableSimInstruction::OP_CPL:
            instCpl(inst, operands[0]);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_0:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHL0);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_0:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHR0);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_1:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHL1);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_1:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHR1);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_R:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHLR);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_R:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHRR);
            break;
        case AdaptableSimInstruction::OP_SHIFT_LEFT_C:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHLC);
            break;
        case AdaptableSimInstruction::OP_SHIFT_RIGHT_C:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_SHRC);
            break;
        case AdaptableSimInstruction::OP_ROTATE_LEFT:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_RL);
            break;
        case AdaptableSimInstruction::OP_ROTATE_RIGHT:
            instShiftRotateOper(inst, operands[0], operands[1], operands[2], SRO_RR);
            break;
    }

    for ( int  i = 0; i < 8; i++ )
    {
        switch ( parameters.flagAttr( AdaptableSimInstruction::OperParam::ProcFlag(i) ) )
        {
            case AdaptableSimInstruction::OperParam::P_SET:
                m_statusFlags->setFlag(AdaptableSimInstruction::OperParam::ProcFlag(i), true);
                break;
            case AdaptableSimInstruction::OperParam::P_CLEAR:
                m_statusFlags->setFlag(AdaptableSimInstruction::OperParam::ProcFlag(i), false);
                break;
            default:
                break;
        }
    }

    return true;
}

unsigned int AdaptableSimOperations::getValue ( unsigned int addrVal,
                                                AdaptableSimInstruction::OperParam::AddressingMode addrMode )
{
    switch ( addrMode )
    {
        case AdaptableSimInstruction::OperParam::A_IMMEDIATE:
            return addrVal;
        case AdaptableSimInstruction::OperParam::A_REG_DIR:
            return m_registers->read(addrVal);
        case AdaptableSimInstruction::OperParam::A_REG_INDR:
            return m_registers->read(m_registers->read(addrVal));
        case AdaptableSimInstruction::OperParam::A_DATA_DIR:
            return m_dataMemory->read(addrVal);
        case AdaptableSimInstruction::OperParam::A_DATA_INDR:
            return m_dataMemory->read(m_dataMemory->read(addrVal));
        case AdaptableSimInstruction::OperParam::A_PROGRAM:
            return m_programMemory->read(addrVal);
        case AdaptableSimInstruction::OperParam::A_PORT:
            return m_io->read(addrVal);
    }

    // Control flow should never reach this point.
    return 0;
}

void AdaptableSimOperations::setValue ( unsigned int destination,
                                        AdaptableSimInstruction::OperParam::AddressingMode addrMode,
                                        unsigned int value )
{
    switch ( addrMode )
    {
        case AdaptableSimInstruction::OperParam::A_IMMEDIATE:
            // Invalid addressing mode.
            break;
        case AdaptableSimInstruction::OperParam::A_REG_DIR:
            m_registers->write(destination, value);
        case AdaptableSimInstruction::OperParam::A_REG_INDR:
            m_registers->write(m_registers->read(destination), value);
        case AdaptableSimInstruction::OperParam::A_DATA_DIR:
            m_dataMemory->write(destination, value);
        case AdaptableSimInstruction::OperParam::A_DATA_INDR:
            m_dataMemory->write(m_dataMemory->read(destination), value);
        case AdaptableSimInstruction::OperParam::A_PROGRAM:
            m_programMemory->write(destination, value);
        case AdaptableSimInstruction::OperParam::A_PORT:
            m_io->write(destination, value);
    }
}

inline unsigned int AdaptableSimOperations::setFlagsAndTrim ( unsigned int source,
                                                              AdaptableSimInstruction::OperParam parameters,
                                                              const std::vector<unsigned char> & permutation )
{
    unsigned int size = getSize(permutation);

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_ZERO ) )
    {
        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_ZERO, ( 0 == source ) );
    }

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_CARRY ) )
    {
        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_CARRY, (bool) ( source & ( 1 << size ) ) );
    }

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_NEGATIVE ) )
    {
        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_NEGATIVE,
                                 (bool) ( source & ( 1 << ( size - 1) ) ) );
    }

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_HALF_CARRY ) )
    {
        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_HALF_CARRY,
                                 (bool) ( source & ( 1 << ( size / 2 ) ) ) );
    }

    unsigned int mask = 0;
    for ( unsigned int i = 0; i < size; i++ )
    {
        mask |= ( 1 << i );
    }

    source &= mask;

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_PARITY ) )
    {
        bool parity = parameters.evenParity();

        for ( unsigned int i = 0; i < sizeof(unsigned int); i++ )
        {
            if ( source & ( 1 << i ) )
            {
                parity = !parity;
            }
        }

        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_PARITY, parity );
    }

    return source;
}

inline unsigned int AdaptableSimOperations::getSize ( const std::vector<unsigned char> & permutation )
{
    int result = -1;

    for ( unsigned char i : permutation )
    {
        if ( (int) i > result )
        {
            result = i;
        }
    }

    return (unsigned int) ++result;
}

inline int AdaptableSimOperations::toSigned ( unsigned int value,
                                              unsigned int size )
{
    if ( 0 == size )
    {
        return 0;
    }

    if ( value & ( 1 << ( size - 1 ) ) )
    {
        for ( unsigned int i = size; i < sizeof(unsigned int); i++ )
        {
            value |= ( 1 << i );
        }
    }

    return (int) value;
}

inline void AdaptableSimOperations::instAbsoluteJump ( AdaptableSimInstruction::OperParam parameters,
                                                       unsigned int address )
{
    setProgramCounter ( getValue(address, parameters.addressingMode(0)) );
}

inline void AdaptableSimOperations::instAbsoluteCall ( AdaptableSimInstruction::OperParam parameters,
                                                       unsigned int address )
{
    logEvent ( EVENT_CPU_CALL, m_pc );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( getValue(address, parameters.addressingMode(0)) );
    m_actSubprogCounter++;
}

inline void AdaptableSimOperations::instRelativeJump ( AdaptableSimInstruction::OperParam parameters,
                                                       unsigned int address,
                                                       const std::vector<unsigned char> & permutation )
{
    incrPc(toSigned(getValue(address, parameters.addressingMode(0)), getSize(permutation)));
}

inline void AdaptableSimOperations::instRelativeCall ( AdaptableSimInstruction::OperParam parameters,
                                                       unsigned int address,
                                                       const std::vector<unsigned char> & permutation )
{
    logEvent ( EVENT_CPU_CALL, m_pc );
    m_stack->pushOnStack ( m_pc );
    incrPc(toSigned(getValue(address, parameters.addressingMode(0)), getSize(permutation)));
    m_actSubprogCounter++;
}

inline void AdaptableSimOperations::instOffsetJump ( AdaptableSimInstruction::OperParam parameters,
                                                     unsigned int address,
                                                     unsigned int offset )
{
    setProgramCounter(getValue(address, parameters.addressingMode(0)) + getValue(offset, parameters.addressingMode(1)));
}

inline void AdaptableSimOperations::instOffsetCall ( AdaptableSimInstruction::OperParam parameters,
                                                     unsigned int address,
                                                     unsigned int offset )
{
    logEvent ( EVENT_CPU_CALL, m_pc );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter(getValue(address, parameters.addressingMode(0)) + getValue(offset, parameters.addressingMode(1)));
    m_actSubprogCounter++;
}

inline void AdaptableSimOperations::instIndexJump ( AdaptableSimInstruction::OperParam parameters,
                                                    unsigned int address,
                                                    unsigned int index,
                                                    unsigned int idxShift )
{
    setProgramCounter ( getValue(address, parameters.addressingMode(0))
                        + ( getValue(index, parameters.addressingMode(1))
                            << getValue(idxShift, parameters.addressingMode(2) ) ) );
}

inline void AdaptableSimOperations::instIndexCall ( AdaptableSimInstruction::OperParam parameters,
                                                    unsigned int address,
                                                    unsigned int index,
                                                    unsigned int idxShift )
{
    logEvent ( EVENT_CPU_CALL, m_pc );
    m_stack->pushOnStack ( m_pc );
    setProgramCounter ( getValue(address, parameters.addressingMode(0))
                        + ( getValue(index, parameters.addressingMode(1))
                        << getValue(idxShift, parameters.addressingMode(2) ) ) );
    m_actSubprogCounter++;
}

inline bool AdaptableSimOperations::instBitTest ( AdaptableSimInstruction::OperParam parameters,
                                                  unsigned int value,
                                                  unsigned int bitNumber )
{
    return (bool) ( getValue(value, parameters.addressingMode(0))
                    & ( 1 << getValue(bitNumber, parameters.addressingMode(1)) ) );
}

inline void AdaptableSimOperations::instReturn()
{
    if ( 0 == m_actSubprogCounter )
    {
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_RET, m_pc );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN, m_pc );
        m_actSubprogCounter--;
    }
    setProgramCounter ( m_stack->popFromStack() );
}

inline void AdaptableSimOperations::instSetBank ( unsigned int bank )
{
    m_registers->setBank ( bank ? 1 : 0 );
}

inline void AdaptableSimOperations::instMove ( const AdaptableSimInstruction & inst,
                                               unsigned int destination,
                                               unsigned int source )
{
    setValue ( destination,
               inst.m_parameters.addressingMode(0),
               setFlagsAndTrim ( getValue(source, inst.m_parameters.addressingMode(1)),
                                 inst.m_parameters,
                                 inst.m_permutation[0] ) );
}

inline void AdaptableSimOperations::instCbMove ( const AdaptableSimInstruction & inst,
                                                 unsigned int destination,
                                                 unsigned int source )
{
    int bank = m_registers->getBank();
    source = setFlagsAndTrim ( getValue(source, inst.m_parameters.addressingMode(1)),
                               inst.m_parameters,
                               inst.m_permutation[0] );
    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );
    setValue ( destination, inst.m_parameters.addressingMode(0), source );
    m_registers->setBank(bank);
}

inline void AdaptableSimOperations::instSwap ( const AdaptableSimInstruction & inst,
                                               unsigned int destination,
                                               unsigned int source )
{
    const unsigned int dstVal = getValue ( destination, inst.m_parameters.addressingMode(0) );
    const unsigned int srcVal = getValue ( source,      inst.m_parameters.addressingMode(1) );

    setValue ( source,
               inst.m_parameters.addressingMode(1),
               setFlagsAndTrim ( dstVal, inst.m_parameters, inst.m_permutation[1] ) );

    setValue ( destination,
               inst.m_parameters.addressingMode(0),
               setFlagsAndTrim ( srcVal, inst.m_parameters, inst.m_permutation[0] ) );
}

inline void AdaptableSimOperations::instCbSwap ( const AdaptableSimInstruction & inst,
                                                 unsigned int destination,
                                                 unsigned int source )
{
    int bank = m_registers->getBank();
    const unsigned int srcVal = getValue ( source,      inst.m_parameters.addressingMode(1) );
    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );
    const unsigned int dstVal = getValue ( destination, inst.m_parameters.addressingMode(0) );
    m_registers->setBank(bank);
    setValue ( source,
               inst.m_parameters.addressingMode(1),
               setFlagsAndTrim ( dstVal, inst.m_parameters, inst.m_permutation[1] ) );
    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );
    setValue ( destination,
               inst.m_parameters.addressingMode(0),
               setFlagsAndTrim ( srcVal, inst.m_parameters, inst.m_permutation[0] ) );
    m_registers->setBank(bank);
}

inline void AdaptableSimOperations::instMoveBit ( AdaptableSimInstruction::OperParam parameters,
                                                  unsigned int destination,
                                                  unsigned int source,
                                                  unsigned int bitNumbers )
{
    const unsigned int dstVal = getValue ( destination, parameters.addressingMode(0) );
    const unsigned int srcVal = getValue ( source,      parameters.addressingMode(1) );

    const unsigned int dstBitNo = ( 0xf0 & bitNumbers ) >> 4;
    const unsigned int srcBitNo = ( 0x0f & bitNumbers );

    bool srcBit = (bool) ( srcVal & ( 1 << srcBitNo ) );

    setValue ( destination,
               parameters.addressingMode(0),
               ( ( dstVal & ~( 1 << dstBitNo ) ) |  ( srcBit ? 1 : 0 ) << dstBitNo ) );
}

inline void AdaptableSimOperations::instCbMoveBit ( AdaptableSimInstruction::OperParam parameters,
                                                    unsigned int destination,
                                                    unsigned int source,
                                                    unsigned int bitNumbers )
{
    int bank = m_registers->getBank();

    const unsigned int srcVal = getValue ( source, parameters.addressingMode(1) );
    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );

    const unsigned int dstVal = getValue ( destination, parameters.addressingMode(0) );

    const unsigned int dstBitNo = ( 0xf0 & bitNumbers ) >> 4;
    const unsigned int srcBitNo = ( 0x0f & bitNumbers );

    bool srcBit = (bool) ( srcVal & ( 1 << srcBitNo ) );

    setValue ( destination,
               parameters.addressingMode(0),
               ( ( dstVal & ~( 1 << dstBitNo ) ) |  ( srcBit ? 1 : 0 ) << dstBitNo ) );

    m_registers->setBank(bank);
}

inline void AdaptableSimOperations::instSwapBit ( AdaptableSimInstruction::OperParam parameters,
                                                  unsigned int destination,
                                                  unsigned int source,
                                                  unsigned int bitNumbers )
{
    const unsigned int dstVal = getValue ( destination, parameters.addressingMode(0) );
    const unsigned int srcVal = getValue ( source,      parameters.addressingMode(1) );

    const unsigned int dstBitNo = ( 0xf0 & bitNumbers ) >> 4;
    const unsigned int srcBitNo = ( 0x0f & bitNumbers );

    bool dstBit = (bool) ( dstVal & ( 1 << dstBitNo ) );
    bool srcBit = (bool) ( srcVal & ( 1 << srcBitNo ) );

    setValue ( destination,
               parameters.addressingMode(0),
               ( ( dstVal & ~( 1 << dstBitNo ) ) |  ( srcBit ? 1 : 0 ) << dstBitNo ) );

    setValue ( source,
               parameters.addressingMode(1),
               ( ( srcVal & ~( 1 << srcBitNo ) ) |  ( dstBit ? 1 : 0 ) << srcBitNo ) );
}

inline void AdaptableSimOperations::instCbSwapBit ( AdaptableSimInstruction::OperParam parameters,
                                                    unsigned int destination,
                                                    unsigned int source,
                                                    unsigned int bitNumbers )
{
    int bank = m_registers->getBank();

    const unsigned int srcVal = getValue ( source, parameters.addressingMode(1) );

    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );
    const unsigned int dstVal = getValue ( destination, parameters.addressingMode(0) );
    m_registers->setBank(bank);

    const unsigned int dstBitNo = ( 0xf0 & bitNumbers ) >> 4;
    const unsigned int srcBitNo = ( 0x0f & bitNumbers );

    bool dstBit = (bool) ( dstVal & ( 1 << dstBitNo ) );
    bool srcBit = (bool) ( srcVal & ( 1 << srcBitNo ) );

    setValue ( source,
               parameters.addressingMode(1),
               ( ( srcVal & ~( 1 << srcBitNo ) ) |  ( dstBit ? 1 : 0 ) << srcBitNo ) );

    m_registers->setBank( ( 0 == bank ) ? 1 : 0 );
    setValue ( destination,
               parameters.addressingMode(0),
               ( ( dstVal & ~( 1 << dstBitNo ) ) |  ( srcBit ? 1 : 0 ) << dstBitNo ) );
    m_registers->setBank(bank);
}

inline void AdaptableSimOperations::instIsrReturn()
{
    // Chech whether there is an ISR in progress to return from.
    if ( m_statusFlags->getInterrupted() <= 0 )
    {
        m_statusFlags -> setInterrupted ( 1 );
        logEvent ( MCUSimEventLogger::FLAG_HI_PRIO, EVENT_CPU_ERR_INVALID_RETI );
    }
    else
    {
        logEvent ( EVENT_CPU_RETURN_FROM_ISR, m_pc );
        m_interruptController->returni();
    }

    // Return from ISR (Interrupt Service Routine).
    setProgramCounter ( m_stack->popFromStack() );
    m_statusFlags -> returni();
}

inline void AdaptableSimOperations::instAdd ( const AdaptableSimInstruction & inst,
                                              unsigned int opr0,
                                              unsigned int opr1,
                                              unsigned int opr2,
                                              bool subtract )
{
    unsigned int val0 = getValue(opr0, inst.m_parameters.addressingMode(0));
    unsigned int val1 = getValue(opr1, inst.m_parameters.addressingMode(1));

    if ( true == subtract )
    {
        val1 = -val1;
    }

    unsigned int result = ( val0 + val1 );

    if ( false == inst.m_parameters.ignoreCarry() )
    {
        if ( true == m_statusFlags->getFlag ( AdaptableSimInstruction::OperParam::F_CARRY ) )
        {
            result++;
        }
    }

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == inst.m_parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_OVERFLOW ) )
    {
        int msbNo0 = getSize(inst.m_permutation[0]);
        int msbNo1 = getSize(inst.m_permutation[1]);
        int msbNo2 = msbNo0;

        if ( true == inst.m_parameters.separateResult() )
        {
            msbNo2 = getSize(inst.m_permutation[2]);
        }

        const bool msb0 = val0   & ( 1 << ( msbNo0 - 1 ) );
        const bool msb1 = val1   & ( 1 << ( msbNo1 - 1 ) );
        const bool msb2 = result & ( 1 << ( msbNo2 - 1 ) );

        bool overflow = false;
        if ( msb0 == msb1 )
        {
            if ( msb0 != msb2 )
            {
                overflow = true;
            }
        }

        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_OVERFLOW, overflow );
    }

    if ( true == inst.m_parameters.separateResult() )
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[2]);
    }
    else
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[0]);
    }

    if ( false == inst.m_parameters.discartResult() )
    {
        if ( true == inst.m_parameters.separateResult() )
        {
            setValue ( opr2, inst.m_parameters.addressingMode(2), result );
        }
        else
        {
            setValue ( opr0, inst.m_parameters.addressingMode(0), result );
        }
    }
}

inline void AdaptableSimOperations::instLogOper ( const AdaptableSimInstruction & inst,
                                                  unsigned int opr0,
                                                  unsigned int opr1,
                                                  unsigned int opr2,
                                                  LogOperation operation )
{
    unsigned int val0 = getValue(opr0, inst.m_parameters.addressingMode(0));
    unsigned int val1 = getValue(opr1, inst.m_parameters.addressingMode(1));

    unsigned int result;

    switch ( operation )
    {
        case LOG_AND:
            result = ( val0 & val1 );
            break;
        case LOG_OR:
            result = ( val0 | val1 );
            break;
        case LOG_XOR:
            result = ( val0 ^ val1 );
            break;
    }

    if ( true == inst.m_parameters.separateResult() )
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[2]);
    }
    else
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[0]);
    }

    if ( false == inst.m_parameters.discartResult() )
    {
        if ( true == inst.m_parameters.separateResult() )
        {
            setValue ( opr2, inst.m_parameters.addressingMode(2), result );
        }
        else
        {
            setValue ( opr0, inst.m_parameters.addressingMode(0), result );
        }
    }
}

inline void AdaptableSimOperations::instCpl ( const AdaptableSimInstruction & inst,
                                              unsigned int operand )
{

    unsigned int result = setFlagsAndTrim ( ~ getValue ( operand,
                                                         inst.m_parameters.addressingMode(0) ),
                                            inst.m_parameters,
                                            inst.m_permutation[0] );

    if ( false == inst.m_parameters.discartResult() )
    {
        setValue ( operand,
                   inst.m_parameters.addressingMode(0),
                   setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[0]) );
    }
}

inline void AdaptableSimOperations::instShiftRotateOper ( const AdaptableSimInstruction & inst,
                                                          unsigned int opr0,
                                                          unsigned int opr1,
                                                          unsigned int opr2,
                                                          ShiftRotateOper operation )
{
    unsigned int size   = getSize(inst.m_permutation[0]);
    unsigned int result = getValue(opr0, inst.m_parameters.addressingMode(0));
    unsigned int by     = getValue(opr1, inst.m_parameters.addressingMode(1));

    bool carry = m_statusFlags->getFlag(AdaptableSimInstruction::OperParam::F_CARRY);


    switch ( operation )
    {
        case SRO_SHL0:
        case SRO_SHR0:
        case SRO_SHL1:
        case SRO_SHR1:
        case SRO_SHLR:
        case SRO_SHRR:
            break;

        case SRO_SHLC:
            if ( true == carry )
            {
                operation = SRO_SHL1;
            }
            else
            {
                operation = SRO_SHL0;
            }
            break;

        case SRO_SHRC:
            if ( true == carry )
            {
                operation = SRO_SHR1;
            }
            else
            {
                operation = SRO_SHR0;
            }
            break;

        case SRO_RL:
            if ( result & ( 1 << ( size - 1 ) ) )
            {
                operation = SRO_SHL1;
            }
            else
            {
                operation = SRO_SHL0;
            }
            break;

        case SRO_RR:
            if ( result & 1 )
            {
                operation = SRO_SHR1;
            }
            else
            {
                operation = SRO_SHR0;
            }
            break;
    }

    switch ( operation )
    {
        case SRO_SHL0:
            result <<= by;
            carry = (bool) (result & ( 1 << size ) );
            break;
        case SRO_SHR0:
            carry = (bool) ( result & ( 1 << ( by - 1 ) ) );
            result >>= by;
            break;
        case SRO_SHL1:
            result <<= by;
            result |= ( ( 1 << by ) - 1 );
            carry = (bool) (result & ( 1 << size ) );
            break;
        case SRO_SHR1:
            carry = (bool) ( result & ( 1 << ( by - 1 ) ) );
            result >>= by;
            result |= ~( ( ( 1 << size ) - 1 ) >> by );
            break;
        case SRO_SHLR:
        {
            bool lsb = (bool) ( result & 1 );
            result <<= by;
            if ( true == lsb )
            {
                result |= ( ( 1 << by ) - 1 );
            }
            carry = (bool) (result & ( 1 << size ) );
            break;
        }
        case SRO_SHRR:
        {
            bool msb = (bool) ( result & ( 1 << ( size - 1 ) ) );
            carry = (bool) ( result & ( 1 << ( by - 1 ) ) );
            result >>= by;
            if ( true == msb )
            {
                result |= ~( ( ( 1 << size ) - 1 ) >> by );
            }
            break;
        }

        case SRO_SHLC:
        case SRO_SHRC:
        case SRO_RL:
        case SRO_RR:
            break;
    }

    if ( true == inst.m_parameters.separateResult() )
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[2]);
    }
    else
    {
        result = setFlagsAndTrim(result, inst.m_parameters, inst.m_permutation[0]);
    }

    if ( false == inst.m_parameters.discartResult() )
    {
        if ( true == inst.m_parameters.separateResult() )
        {
            setValue ( opr2, inst.m_parameters.addressingMode(2), result );
        }
        else
        {
            setValue ( opr0, inst.m_parameters.addressingMode(0), result );
        }
    }

    if ( AdaptableSimInstruction::OperParam::P_AFFECT
         == inst.m_parameters.flagAttr ( AdaptableSimInstruction::OperParam::F_CARRY) )
    {
        m_statusFlags->setFlag ( AdaptableSimInstruction::OperParam::F_CARRY, carry );
    }
}

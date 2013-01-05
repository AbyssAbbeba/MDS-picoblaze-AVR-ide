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
 * @ingroup AVR8
 * @file AVR8InstructionSet.cxx
 */
// =============================================================================

#include "AVR8InstructionSet.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8DES.h"
#include "AVR8FusesAndLocks.h"
#include "AVR8InterruptController.h"
#include "AVR8SystemControl.h"
#include "AVR8BootLoader.h"

int ( AVR8InstructionSet :: * const AVR8InstructionSet::m_opCodeDispatchTable [ 64 ] ) ( const unsigned int opCode ) =
{
    &AVR8InstructionSet::instOPCode_000000, // opCode = 0000 00xx xxxx xxxx
    &AVR8InstructionSet::inst_CPC_Rd_Rr,    // opCode = 0000 01xx xxxx xxxx
    &AVR8InstructionSet::inst_SBC_Rd_Rr,    // opCode = 0000 10xx xxxx xxxx
    &AVR8InstructionSet::inst_ADD_Rd_Rr,    // opCode = 0000 11xx xxxx xxxx
    &AVR8InstructionSet::inst_CPSE_Rd_Rr,   // opCode = 0001 00xx xxxx xxxx
    &AVR8InstructionSet::inst_CP_Rd_Rr,     // opCode = 0001 01xx xxxx xxxx
    &AVR8InstructionSet::inst_SUB_Rd_Rr,    // opCode = 0001 10xx xxxx xxxx
    &AVR8InstructionSet::inst_ADC_Rd_Rr,    // opCode = 0001 11xx xxxx xxxx
    &AVR8InstructionSet::inst_AND_Rd_Rr,    // opCode = 0010 00xx xxxx xxxx
    &AVR8InstructionSet::inst_EOR_Rd_Rr,    // opCode = 0010 01xx xxxx xxxx
    &AVR8InstructionSet::inst_OR_Rd_Rr,     // opCode = 0010 10xx xxxx xxxx
    &AVR8InstructionSet::inst_MOV_Rd_Rr,    // opCode = 0010 11xx xxxx xxxx
    &AVR8InstructionSet::inst_CPI_Rd_K,     // opCode = 0011 00xx xxxx xxxx
    &AVR8InstructionSet::inst_CPI_Rd_K,     // opCode = 0011 01xx xxxx xxxx
    &AVR8InstructionSet::inst_CPI_Rd_K,     // opCode = 0011 10xx xxxx xxxx
    &AVR8InstructionSet::inst_CPI_Rd_K,     // opCode = 0011 11xx xxxx xxxx
    &AVR8InstructionSet::inst_SBCI_Rd_K,    // opCode = 0100 00xx xxxx xxxx
    &AVR8InstructionSet::inst_SBCI_Rd_K,    // opCode = 0100 01xx xxxx xxxx
    &AVR8InstructionSet::inst_SBCI_Rd_K,    // opCode = 0100 10xx xxxx xxxx
    &AVR8InstructionSet::inst_SBCI_Rd_K,    // opCode = 0100 11xx xxxx xxxx
    &AVR8InstructionSet::inst_SUBI_Rd_K,    // opCode = 0101 00xx xxxx xxxx
    &AVR8InstructionSet::inst_SUBI_Rd_K,    // opCode = 0101 01xx xxxx xxxx
    &AVR8InstructionSet::inst_SUBI_Rd_K,    // opCode = 0101 10xx xxxx xxxx
    &AVR8InstructionSet::inst_SUBI_Rd_K,    // opCode = 0101 11xx xxxx xxxx
    &AVR8InstructionSet::inst_ORI_Rd_K,     // opCode = 0110 00xx xxxx xxxx
    &AVR8InstructionSet::inst_ORI_Rd_K,     // opCode = 0110 01xx xxxx xxxx
    &AVR8InstructionSet::inst_ORI_Rd_K,     // opCode = 0110 10xx xxxx xxxx
    &AVR8InstructionSet::inst_ORI_Rd_K,     // opCode = 0110 11xx xxxx xxxx
    &AVR8InstructionSet::inst_ANDI_Rd_K,    // opCode = 0111 00xx xxxx xxxx
    &AVR8InstructionSet::inst_ANDI_Rd_K,    // opCode = 0111 01xx xxxx xxxx
    &AVR8InstructionSet::inst_ANDI_Rd_K,    // opCode = 0111 10xx xxxx xxxx
    &AVR8InstructionSet::inst_ANDI_Rd_K,    // opCode = 0111 11xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1000 00xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1000 01xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1000 10xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1000 11xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_100100,// opCode = 1001 00xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_100101,// opCode = 1001 01xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_100110,// opCode = 1001 10xx xxxx xxxx
    &AVR8InstructionSet::inst_MUL_Rd_Rr,    // opCode = 1001 11xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1010 00xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1010 01xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1010 10xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_10x0xx,// opCode = 1010 11xx xxxx xxxx
    &AVR8InstructionSet::inst_IN,           // opCode = 1011 00xx xxxx xxxx
    &AVR8InstructionSet::inst_IN,           // opCode = 1011 01xx xxxx xxxx
    &AVR8InstructionSet::inst_OUT_A_Rr,     // opCode = 1011 10xx xxxx xxxx
    &AVR8InstructionSet::inst_OUT_A_Rr,     // opCode = 1011 11xx xxxx xxxx
    &AVR8InstructionSet::inst_RJMP_k,       // opCode = 1100 00xx xxxx xxxx
    &AVR8InstructionSet::inst_RJMP_k,       // opCode = 1100 01xx xxxx xxxx
    &AVR8InstructionSet::inst_RJMP_k,       // opCode = 1100 10xx xxxx xxxx
    &AVR8InstructionSet::inst_RJMP_k,       // opCode = 1100 11xx xxxx xxxx
    &AVR8InstructionSet::inst_RCALL_k,      // opCode = 1101 00xx xxxx xxxx
    &AVR8InstructionSet::inst_RCALL_k,      // opCode = 1101 01xx xxxx xxxx
    &AVR8InstructionSet::inst_RCALL_k,      // opCode = 1101 10xx xxxx xxxx
    &AVR8InstructionSet::inst_RCALL_k,      // opCode = 1101 11xx xxxx xxxx
    &AVR8InstructionSet::inst_LDI_Rd_K,     // opCode = 1110 00xx xxxx xxxx
    &AVR8InstructionSet::inst_LDI_Rd_K,     // opCode = 1110 01xx xxxx xxxx
    &AVR8InstructionSet::inst_LDI_Rd_K,     // opCode = 1110 10xx xxxx xxxx
    &AVR8InstructionSet::inst_LDI_Rd_K,     // opCode = 1110 11xx xxxx xxxx
    &AVR8InstructionSet::inst_BRBS_s_k,     // opCode = 1111 00xx xxxx xxxx
    &AVR8InstructionSet::inst_BRBC_s_k,     // opCode = 1111 01xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_111110,// opCode = 1111 10xx xxxx xxxx
    &AVR8InstructionSet::instOPCode_111111  // opCode = 1111 11xx xxxx xxxx
};

int ( AVR8InstructionSet :: * const AVR8InstructionSet::m_opCodeDispatchTable_100100 [ 32 ] ) ( const unsigned int opCode ) =
{
    &AVR8InstructionSet::inst_LDS_Rd_k,     // opCode = 1001 000x xxxx 0000
    &AVR8InstructionSet::inst_LD_Rd_Zplus,  // opCode = 1001 000x xxxx 0001
    &AVR8InstructionSet::inst_LD_Rd_minusZ, // opCode = 1001 000x xxxx 0010
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 000x xxxx 0011
    &AVR8InstructionSet::inst_LPM_Rd_Z,     // opCode = 1001 000x xxxx 0100
    &AVR8InstructionSet::inst_LPM_Rd_Zplus, // opCode = 1001 000x xxxx 0101
    &AVR8InstructionSet::inst_ELPM_Rd_Z,    // opCode = 1001 000x xxxx 0110
    &AVR8InstructionSet::inst_ELPM_Rd_Zplus,// opCode = 1001 000x xxxx 0111
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 000x xxxx 1000
    &AVR8InstructionSet::inst_LD_Rd_Yplus,  // opCode = 1001 000x xxxx 1001
    &AVR8InstructionSet::inst_LD_Rd_minusY, // opCode = 1001 000x xxxx 1010
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 000x xxxx 1011
    &AVR8InstructionSet::inst_LD_Rd_X,      // opCode = 1001 000x xxxx 1100
    &AVR8InstructionSet::inst_LD_Rd_Xplus,  // opCode = 1001 000x xxxx 1101
    &AVR8InstructionSet::inst_LD_Rd_minusX, // opCode = 1001 000x xxxx 1110
    &AVR8InstructionSet::inst_POP_Rd,       // opCode = 1001 000x xxxx 1111
    &AVR8InstructionSet::inst_STS_k_Rr,     // opCode = 1001 001x xxxx 0000
    &AVR8InstructionSet::inst_ST_Zplus_Rr,  // opCode = 1001 001x xxxx 0001
    &AVR8InstructionSet::inst_ST_minusZ_Rr, // opCode = 1001 001x xxxx 0010
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 001x xxxx 0011
    &AVR8InstructionSet::inst_XCH_Z_Rd,     // opCode = 1001 001x xxxx 0100
    &AVR8InstructionSet::inst_LAS_Z_Rd,     // opCode = 1001 001x xxxx 0101
    &AVR8InstructionSet::inst_LAC_Z_Rd,     // opCode = 1001 001x xxxx 0110
    &AVR8InstructionSet::inst_LAT_Z_Rd,     // opCode = 1001 001x xxxx 0111
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 001x xxxx 1000
    &AVR8InstructionSet::inst_ST_Yplus_Rr,  // opCode = 1001 001x xxxx 1001
    &AVR8InstructionSet::inst_ST_minusY_Rr, // opCode = 1001 001x xxxx 1010
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 001x xxxx 1011
    &AVR8InstructionSet::inst_ST_X_Rr,      // opCode = 1001 001x xxxx 1100
    &AVR8InstructionSet::inst_ST_Xplus_Rr,  // opCode = 1001 001x xxxx 1101
    &AVR8InstructionSet::inst_ST_minusX_Rr, // opCode = 1001 001x xxxx 1110
    &AVR8InstructionSet::inst_PUSH_Rr       // opCode = 1001 001x xxxx 1111
};

int ( AVR8InstructionSet :: * const AVR8InstructionSet::m_opCodeDispatchTable_1001_010x_xxxx_100x [ 64 ] ) ( const unsigned int opCode ) =
{
    &AVR8InstructionSet::inst_SEC,          // opCode = 1001 0100 0000 1000
    &AVR8InstructionSet::inst_IJMP,         // opCode = 1001 0100 0000 1001
    &AVR8InstructionSet::inst_SEZ,          // opCode = 1001 0100 0001 1000
    &AVR8InstructionSet::inst_EIJMP,        // opCode = 1001 0100 0001 1001
    &AVR8InstructionSet::inst_SEN,          // opCode = 1001 0100 0010 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0010 1001
    &AVR8InstructionSet::inst_SEV,          // opCode = 1001 0100 0011 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0011 1001
    &AVR8InstructionSet::inst_SES,          // opCode = 1001 0100 0100 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0100 1001
    &AVR8InstructionSet::inst_SEH,          // opCode = 1001 0100 0101 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0101 1001
    &AVR8InstructionSet::inst_SET,          // opCode = 1001 0100 0110 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0110 1001
    &AVR8InstructionSet::inst_SEI,          // opCode = 1001 0100 0111 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 0111 1001
    &AVR8InstructionSet::inst_CLC,          // opCode = 1001 0100 1000 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1000 1001
    &AVR8InstructionSet::inst_CLZ,          // opCode = 1001 0100 1001 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1001 1001
    &AVR8InstructionSet::inst_CLN,          // opCode = 1001 0100 1010 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1010 1001
    &AVR8InstructionSet::inst_CLV,          // opCode = 1001 0100 1011 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1011 1001
    &AVR8InstructionSet::inst_CLS,          // opCode = 1001 0100 1100 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1100 1001
    &AVR8InstructionSet::inst_CLH,          // opCode = 1001 0100 1101 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1101 1001
    &AVR8InstructionSet::inst_CLT,          // opCode = 1001 0100 1110 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1110 1001
    &AVR8InstructionSet::inst_CLI,          // opCode = 1001 0100 1111 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0100 1111 1001
    &AVR8InstructionSet::inst_RET,          // opCode = 1001 0101 0000 1000
    &AVR8InstructionSet::inst_ICALL,        // opCode = 1001 0101 0000 1001
    &AVR8InstructionSet::inst_RETI,         // opCode = 1001 0101 0001 1000
    &AVR8InstructionSet::inst_EICALL,       // opCode = 1001 0101 0001 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0010 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0010 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0011 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0011 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0100 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0100 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0101 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0101 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0110 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0110 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0111 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 0111 1001
    &AVR8InstructionSet::inst_SLEEP,        // opCode = 1001 0101 1000 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1000 1001
    &AVR8InstructionSet::inst_BREAK,        // opCode = 1001 0101 1001 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1001 1001
    &AVR8InstructionSet::inst_WDR,          // opCode = 1001 0101 1010 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1010 1001
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1011 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1011 1001
    &AVR8InstructionSet::inst_LPM,          // opCode = 1001 0101 1100 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1100 1001
    &AVR8InstructionSet::inst_ELPM,         // opCode = 1001 0101 1101 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1101 1001
    &AVR8InstructionSet::inst_SPM,          // opCode = 1001 0101 1110 1000
    &AVR8InstructionSet::instInvalid,       // opCode = 1001 0101 1110 1001
    &AVR8InstructionSet::inst_SPM_Zplus,    // opCode = 1001 0101 1111 1000
    &AVR8InstructionSet::instInvalid        // opCode = 1001 0101 1111 1001
};

AVR8InstructionSet * AVR8InstructionSet::link ( MCUSim::EventLogger     * eventLogger,
                                                AVR8ProgramMemory       * programMemory,
                                                AVR8DataMemory          * dataMemory,
                                                MCUSim::Mode            * processorMode,
                                                AVR8Sim::SleepMode      * sleepMode,
                                                AVR8FusesAndLocks       * fusesAndLocks,
                                                AVR8InterruptController * interruptController,
                                                AVR8SystemControl       * systemControl,
                                                AVR8Sim::HaltMode       * haltMode,
                                                AVR8BootLoader          * bootLoader )
{
    MCUSim::CPU::link(eventLogger);

    m_programMemory = programMemory;
    m_dataMemory = dataMemory;
    m_processorMode = processorMode;
    m_sleepMode = sleepMode;
    m_fusesAndLocks = fusesAndLocks;
    m_interruptController = interruptController;
    m_systemControl = systemControl;
    m_haltMode = haltMode;
    m_bootLoader = bootLoader;

    return this;
}

void AVR8InstructionSet::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_INITIAL_VALUES:
            resetToInitialValues();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void AVR8InstructionSet::mcuReset()
{
    if ( false == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_BOOTRST] )
    {
        m_pc = 0;
    }
    else
    {
        m_pc = m_bootLoader->getBootAddress();
    }
    m_actSubprogCounter = 0;
    m_lastInstruction = AVR8InsNames::INS_NONE;
}

inline void AVR8InstructionSet::resetToInitialValues()
{
    mcuReset();

    for ( int i = 0; i < AVR8InsNames::INS__MAX__; i++ )
    {
        m_instructionCounter[i] = 0;
    }
}

inline void AVR8InstructionSet::incrPc ( const int val )
{
    m_pc += val;
    while ( m_pc > m_config.m_pcMax )
    {
        m_pc -= m_config.m_pcMax;
        logEvent(EVENT_CPU_PC_OVERFLOW);
    }
    while ( m_pc < 0 )
    {
        m_pc += m_config.m_pcMax;
        logEvent(EVENT_CPU_PC_UNDERFLOW);
    }
}

int AVR8InstructionSet::execInstruction()
{
    int result = -1;
    int pcOrig = m_pc;
    unsigned int opCode = m_programMemory->readRaw(m_pc);
    incrPc();

    if ( opCode & 0xffff0000 )
    {
        if ( true == m_config.m_ignoreUndefinedOpCodes )
        {
            return -1;
        }
        else
        {
            opCode &= 0x0ffff;
        }
    }

    result = ( this->*(m_opCodeDispatchTable[opCode >> 10]) ) ( opCode );

    if ( pcOrig != m_pc )
    {
        logEvent(EVENT_CPU_PC_CHANGED, m_pc);
    }

    return result;
}

int AVR8InstructionSet::instOPCode_000000 ( const unsigned int opCode )
{
    // opCode = 0000 00xx ...
    if ( opCode & 0x0200 )
    {
        // opCode = 0000 001x ...
        if ( opCode & 0x0100 )
        {
            // opCode = 0000 0011 ...
            if ( opCode & 0x0080 )
            {
                // opCode = 0000 0011 1xxx ...
                if ( opCode & 0x0008 )
                {
                    // opCode = 0000 0011 1xxx 1xxx
                    return inst_FMULSU_Rd_Rr(opCode);
                }
                else
                {
                    // opCode = 0000 0011 1xxx 0xxx
                    return inst_FMULS_Rd_Rr(opCode);
                }
            }
            else
            {
                // opCode = 0000 0011 0xxx ...
                if ( opCode & 0x0008 )
                {
                    // opCode = 0000 0011 0xxx 1xxx
                    return inst_FMUL_Rd_Rr(opCode);
                }
                else
                {
                    // opCode = 0000 0011 0xxx 0xxx
                    return inst_MULSU_Rd_Rr(opCode);
                }
            }
        }
        else
        {
            // opCode = 0000 0010 ...
            return inst_MULS_Rd_Rr(opCode);
        }
    }
    else
    {
        // opCode = 0000 000x ...
        if ( opCode & 0x0100 )
        {
            // opCode = 0000 0001 ...
            return inst_MOVW_Rd_Rr(opCode);
        }
        else
        {
            // opCode = 0000 0000 ...
            if ( 0 == opCode )
            {
                return inst_NOP(opCode);
            }
            else
            {
                return instInvalid(opCode);
            }
        }
    }
}
int AVR8InstructionSet::instOPCode_10x0xx ( const unsigned int opCode )
{
    /*
     * opCode = 10x0 xxxx ...
     *
     * NOTE: There are two mutually exclusive meanings of this OP code.
     */

    if ( true == m_config.m_availableInstructions[AVR8InsNames::SPECI_STS_K_RR_16] )
    {
        if ( opCode & 0x2000 )
        {
            // opCode = 1010 ...
            if ( opCode & 0x0800 )
            {
                // opCode = 1010 1xxx ...
                return inst_STS_k_Rr_16b(opCode);
            }
            else
            {
                // opCode = 1010 0xxx ...
                return inst_LDS_Rd_k_16b(opCode);
            }
        }
        else
        {
            // opCode = 1000 ...
            return instInvalid(opCode);
        }
    }

    if ( opCode & 0x0200 )
    {
        // opCode = 10x0 xx1x ...
        if ( opCode & 0x0008 )
        {
            // opCode = 10x0 xx1x xxxx 1xxx
            return inst_STD_Yq_Rr(opCode);
        }
        else
        {
            // opCode = 10x0 xx1x xxxx 0xxx
            return inst_STD_Zq_Rr(opCode);
        }
    }
    else
    {
        // opCode = 10x0 xx0x ...
        if ( opCode & 0x0008 )
        {
            // opCode = 10x0 xx0x xxxx 1xxx
            return inst_LDD_Rd_Yq(opCode);
        }
        else
        {
            // opCode = 10x0 xx0x xxxx 0xxx
            return inst_LDD_Rd_Zq(opCode);
        }
    }
}

int AVR8InstructionSet::instOPCode_100100 ( const unsigned int opCode )
{
    // opcode = 1001 00i- ---- iiii, use i as index to the dispatch table
    int i = (opCode & 0x000f) | ((opCode & 0x200) >> 5);
    return (this->*(m_opCodeDispatchTable_100100[i]))(opCode);
}

int AVR8InstructionSet::instOPCode_100101 ( const unsigned int opCode )
{
    // opCode = 1001 01xx ...

    // Shortcut for some of the most frequently used instuctions.
    if ( 0x9408 == (opCode & 0xfe0e) )
    {
        // opCode = 1001 010x xxxx 100x
        goto label_1001_010x_xxxx_100x;
    }

    if ( opCode & 0x0200 )
    {
        // opCode = 1001 011x ...
        if ( opCode & 0x0100 )
        {
            // opCode = 1001 0111 ...
            return inst_SBIW_Rd_1_Rd_K(opCode);
        }
        else
        {
            // opCode = 1001 0110 ...
            return inst_ADIW_Rd_1_Rd_K(opCode);
        }
    }
    else
    {
        // opCode = 1001 010x ...
        if ( opCode & 0x0008 )
        {
            // opCode = 1001 010x xxxx 1xxx
            if ( opCode & 0x0004 )
            {
                // opCode = 1001 010x xxxx 11xx
                if ( opCode & 0x0002 )
                {
                    // opCode = 1001 010x xxxx 111x
                    return inst_CALL_k(opCode);
                }
                else
                {
                    // opCode = 1001 010x xxxx 110x
                    return inst_JMP_k(opCode);
                }
            }
            else
            {
                // opCode = 1001 010x xxxx 10xx
                if ( opCode & 0x0002 )
                {
                    // opCode = 1001 010x xxxx 101x
                    if ( opCode & 0x0001 )
                    {
                        // opCode = 1001 010x xxxx 1011
                        if ( opCode & 0x0100 )
                        {
                            // opCode = 1001 0101 xxxx 1011
                            return instInvalid(opCode);
                        }
                        else
                        {
                            // opCode = 1001 0100 xxxx 1011
                            return inst_DES_K(opCode);
                        }
                    }
                    else
                    {
                        // opCode = 1001 010x xxxx 1010
                        return inst_DEC_Rd(opCode);
                    }
                }
                else
                {
                    label_1001_010x_xxxx_100x:

                    // opcode = 1001 010i iiii 100i, use i as index to the dispatch table
                    int i = ((opCode & 0x01f0) >> 4) | (opCode & 0x0001);
                    return (this->*(m_opCodeDispatchTable_1001_010x_xxxx_100x[i]))(opCode);
                }
            }
        }
        else
        {
            // opCode = 1001 010x xxxx 0xxx
            if ( opCode & 0x0004 )
            {
                // opCode = 1001 010x xxxx 01xx
                if ( opCode & 0x0002 )
                {
                    // opCode = 1001 010x xxxx 011x
                    if ( opCode & 0x0001 )
                    {
                        // opCode = 1001 010x xxxx 0111
                        return inst_ROR_Rd(opCode);
                    }
                    else
                    {
                        // opCode = 1001 010x xxxx 0110
                        return inst_LSR_Rd(opCode);
                    }
                }
                else
                {
                    // opCode = 1001 010x xxxx 000x
                    if ( opCode & 0x0001 )
                    {
                        // opCode = 1001 010x xxxx 0101
                        return inst_ASR_Rd(opCode);
                    }
                    else
                    {
                        // opCode = 1001 010x xxxx 0100
                        return instInvalid(opCode);
                    }
                }
            }
            else
            {
                // opCode = 1001 010x xxxx 00xx
                if ( opCode & 0x0002 )
                {
                    // opCode = 1001 010x xxxx 001x
                    if ( opCode & 0x0001 )
                    {
                        // opCode = 1001 010x xxxx 0011
                        return inst_INC_Rd(opCode);
                    }
                    else
                    {
                        // opCode = 1001 010x xxxx 0010
                        return inst_SWAP_Rd(opCode);
                    }
                }
                else
                {
                    // opCode = 1001 010x xxxx 000x
                    if ( opCode & 0x0001 )
                    {
                        // opCode = 1001 010x xxxx 0001
                        return inst_NEG_Rd(opCode);
                    }
                    else
                    {
                        // opCode = 1001 010x xxxx 0000
                        return inst_COM_Rd(opCode);
                    }
                }
            }
        }
    }
}

int AVR8InstructionSet::instOPCode_100110 ( const unsigned int opCode )
{
    // opCode = 1001 10xx ...
    if ( opCode & 0x0200 )
    {
        // opCode = 1001 101x ...
        if ( opCode & 0x0100 )
        {
            // opCode = 1001 1011 ...
            return inst_SBIS_A_b(opCode);
        }
        else
        {
            // opCode = 1001 1010 ...
            return inst_SBI_A_b(opCode);
        }
    }
    else
    {
        // opCode = 1001 100x ...
        if ( opCode & 0x0100 )
        {
            // opCode = 1001 1001 ...
            return inst_SBIC_A_b(opCode);
        }
        else
        {
            // opCode = 1001 1000 ...
            return inst_CBI_A_b(opCode);
        }
    }
}

int AVR8InstructionSet::instOPCode_111110 ( const unsigned int opCode )
{
    // opCode = 1111 11xx ...
    if ( opCode & 0x0008 )
    {
        // opCode = 1111 10xx xxxx 1xxx
        return instInvalid(opCode);
    }
    else
    {
        if ( opCode & 0x0200 )
        {
            // opCode = 1111 101x ...
            return inst_BST_Rd_b(opCode);
        }
        else
        {
            // opCode = 1111 100x ...
            return inst_BLD_Rd_b(opCode);
        }
    }
}

int AVR8InstructionSet::instOPCode_111111 ( const unsigned int opCode )
{
    // opCode = 1111 11xx ...
    if ( opCode & 0x0008 )
    {
        // opCode = 1111 11xx xxxx 1xxx
        return instInvalid(opCode);
    }
    else
    {
        if ( opCode & 0x0200 )
        {
            // opCode = 1111 111x ...
            return inst_SBRS_Rr_b(opCode);
        }
        else
        {
            // opCode = 1111 110x ...
            return inst_SBRC_Rr_b(opCode);
        }
    }
}

inline void AVR8InstructionSet::instructionEnter ( AVR8InsNames::Instructions instName )
{
    m_instructionCounter[instName]++;
    m_lastInstruction = instName;
}

inline bool AVR8InstructionSet::isInstruction32b ( const unsigned int opCode ) const
{
    /*
     * Only the following instructions are 32b:
     *
     * LDS Rd, k            1001 000d dddd 0000 \
     *                      kkkk kkkk kkkk kkkk - Load Direct from Data Space
     * STS k, Rr            1001 001d dddd 0000 \
     *                      kkkk kkkk kkkk kkkk - Store Direct to Data Space
     * CALL k               1001 010k kkkk 111k \
     *                      kkkk kkkk kkkk kkkk - Long Call to a Subroutine
     * JMP k                1001 010k kkkk 110k \
     *                      kkkk kkkk kkkk kkkk - Jump
     */

    if (
         0x9000 == (opCode & 0xf800)     // opCode = 1001 0xxx xxxx xxxx
             &&
         0x0600 != (opCode & 0x0600)     // opCode ≠ xxxx x11x xxxx xxxx
             &&
         (                               // (opCode = xxxx xxxx xxxx 11xx) v (opCode = xxxx xxxx xxxx 0000)
             0x00C0 == (opCode & 0x00c0)
                 ||
             0x0000 == (opCode & 0x000f)
         )
       )
    {
        return true;
    }
    else
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

int AVR8InstructionSet::instInvalid ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_NONE);

    logEvent(EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode);
    return -1;
}

/*
 * OP Code: 1001 001d dddd 1111 - Push Register on Stack
 * Operation: STACK ← Rd
 */
int AVR8InstructionSet::inst_PUSH_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_PUSH] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_PUSH);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_PUSH_Rr);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    m_dataMemory->pushOnStack(m_dataMemory->read(addrRd));

    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

/*
 * OP Code: 1001 000d dddd 1111 - Pop Register from Stack
 * Operation: Rd ← STACK
 */
int AVR8InstructionSet::inst_POP_Rd ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_POP] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_POP);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_POP_Rd);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    m_dataMemory->write(addrRd, m_dataMemory->popFromStack());

    return 2;
}

/*
 * OP Code: 0000 11rd dddd rrrr - Add without Carry
 * Operation: Rd ← Rd + Rr
 */
int AVR8InstructionSet::inst_ADD_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ADD_Rd_Rr);

    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valRd = m_dataMemory->read(addrRd);

    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    register unsigned int result = (valRr + valRd);

    // Clear all status flags except for 'I' and 'T'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T );

    // Evaluate new status flags
    if ( 0x100 & result )
    {
        result &= 0xff;
        sreg |= AVR8RegNames::SREG_C;
    }
    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z;
    }
    if ( 0x80 & result )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if (
         ( ( valRr & 0x80 ) == ( valRd  & 0x80 ) )
             &&
         ( ( valRr & 0x80 ) != ( result & 0x80 ) )
       )
    {
        // V ← 1; S ← (N ⊕ V): ⊕ 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }
    if (
         ( (valRr & 0x08) &&  (valRd  & 0x08) )
             ||
         ( (valRr & 0x08) && !(result & 0x08) )
             ||
         ( (valRd & 0x08) && !(result & 0x08) )
       )
    {
        sreg |= AVR8RegNames::SREG_H;
    }

    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, result);

    return 1;
}

/*
 * OP Code: 1001 0111 KKdd KKKK - Subtract Immediate from Word
 * Operation: Rd+1:Rd ← Rd+1:Rd - K
 */
inline int AVR8InstructionSet::inst_SBIW_Rd_1_Rd_K ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_SBIW] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_SBIW);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_SBIW_Rd_1_Rd_K);

    // Operands
    const unsigned int valK = ((opCode & 0x00C0) >> 2) | (opCode & 0x000f);
    unsigned int addrRd = ((opCode & 0x0030) >> 3) | 0x19; // d ∈ {24,26,28,30}, Rd+1

    // Registers
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    unsigned int valRd = m_dataMemory->read(addrRd);        // Rd high byte
    valRd <<= 8;
    valRd |= m_dataMemory->read(--addrRd);                  // Rd low byte

    // Perform the operation
    unsigned int result = ( valRd - valK );

    // Clear all status flags except for 'I', 'T', and 'H'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H );

    // Evaluate new status flags
    if ( 0x10000 & result )
    {
        result &= 0xffff;
        sreg ^= AVR8RegNames::SREG_C;   // C ← 1
    }
    if ( 0 == result )
    {
        sreg ^= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    if ( 0x8000 & result )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if ( (valRd & 0x80) && !(result & 0x8000) )
    {
        // V ← 1; S ← (N ⊕ V): ⊕ 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);      // Rd high byte
    m_dataMemory->write(addrRd, (valRd));                   // Rd low byte
    m_dataMemory->write(++addrRd, (valRd));                 // Rd high byte

    // This takes two cycles to execute
    return 2;
}

template <bool withCarry>
    inline unsigned int AVR8InstructionSet::instSub8b ( unsigned int leftVal,
                                                        unsigned int rightVal )
{
    // Status flags register
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    register unsigned int result = ( leftVal - rightVal );
    if ( true == withCarry )
    {
        if ( AVR8RegNames::SREG_C & sreg ) {
            result --;
        }
    }

    // Clear all status flags except for 'I' and 'T'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T );

    // Evaluate new status flags
    if ( 0x100 & result )
    {
        result &= 0xff;
        sreg |= AVR8RegNames::SREG_C;   // C ← 1
    }
    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    if ( 0x80 & result )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if (
         (  ( leftVal & 0x80 ) && !( rightVal & 0x80 ) && !( result & 0x80 ) )
             ||
         ( !( leftVal & 0x80 ) &&  ( rightVal & 0x80 ) &&  ( result & 0x80 ) )
       )
    {
        // V ← 1; S ← (N ⊕ V): ⊕ 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }
    if (
         (  (rightVal & 0x08) && !(leftVal & 0x08) )
             ||
         (  (rightVal & 0x08) &&  (result  & 0x08) )
             ||
         ( !(leftVal  & 0x08) &&  (result  & 0x08) )
       )
    {
        sreg |= AVR8RegNames::SREG_H;   // H ← 1
    }

    // Write new status flags
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);

    // Return the result of subtraction
    return result;
}

/*
 * OP Code: 0100 KKKK dddd KKKK - Subtract Immediate with Carry
 * Operation: Rd ← Rd - K - C
 */
int AVR8InstructionSet::inst_SBCI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBCI_Rd_K);

    // Operands
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31
    const unsigned int valK = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);

    // Registers
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation, and write the result
    m_dataMemory->write(addrRd, instSub8b<true>(valRd, valK));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0101 KKKK dddd KKKK - Subtract Immediate
 * Operation: Rd ← Rd - K
 */
int AVR8InstructionSet::inst_SUBI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SUBI_Rd_K);

    // Operands
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31
    const unsigned int valK = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);

    // Registers
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation, and write the result
    m_dataMemory->write(addrRd, instSub8b<false>(valRd, valK));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0001 10rd dddd rrrr - Subtract without Carry
 * Operation: Rd ← Rd - Rr
 */
int AVR8InstructionSet::inst_SUB_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SUB_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation, and write the result
    m_dataMemory->write(addrRd, instSub8b<false>(valRd, valRr));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0000 10rd dddd rrrr - Subtract with Carry
 * Operation: Rd ← Rd - Rr - C
 */
int AVR8InstructionSet::inst_SBC_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBC_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation, and write the result
    m_dataMemory->write(addrRd, instSub8b<true>(valRd, valRr));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0001 01rd dddd rrrr - Compare
 * Operation: Rd - Rr
 */
int AVR8InstructionSet::inst_CP_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_CP_Rd_Rr);

    // Operands
    unsigned int valRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    unsigned int valRd = (opCode & 0x01f0) >> 4;

    // Registers
    valRr = m_dataMemory->read(valRr);
    valRd = m_dataMemory->read(valRd);

    // Perform the operation, and DO NOT write the result to Rd
    instSub8b<false>(valRd, valRr);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0000 01rd dddd rrrr - Compare with Carry
 * Operation: Rd - Rr - C
 */
int AVR8InstructionSet::inst_CPC_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_CPC_Rd_Rr);

    // Operands
    unsigned int valRd = (opCode & 0x01f0) >> 4;
    unsigned int valRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);

    // Registers
    valRd = m_dataMemory->read(valRd);
    valRr = m_dataMemory->read(valRr);

    // Perform the operation, and DO NOT write the result to Rd
    instSub8b<true>(valRd, valRr);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0011 KKKK dddd KKKK - Compare with Immediate
 * Operation: Rd - K
 */
int AVR8InstructionSet::inst_CPI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_CPI_Rd_K);

    // Operands
    const unsigned int valK = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);
    unsigned int valRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31

    // Registers
    valRd = m_dataMemory->read(valRd);

    // Perform the operation, and DO NOT write the result to Rd
    instSub8b<false>(valRd, valK);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 001r rrrr 0100 - Exchange
 * Operation: (Z) ← Rd, Rd ← (Z)
 */
int AVR8InstructionSet::inst_XCH_Z_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_XCH_Z_Rd);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Get address pointed by the Z register
    unsigned int addrZ = m_dataMemory->read(AVR8RegNames::ZH);
    addrZ <<= 8;
    addrZ |= m_dataMemory->read(AVR8RegNames::ZL);

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valZ = m_dataMemory->read(addrZ);

    // Write results
    m_dataMemory->write(addrZ, valRr); // (Z) ← Rd
    m_dataMemory->write(addrRr, valZ); // Rd ← (Z)

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0001 00rd dddd rrrr - Compare Skip if Equal
 * Operation: If Rd = Rr then PC ← PC + 2 (or 3) else PC ← PC + 1
 */
int AVR8InstructionSet::inst_CPSE_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_CPSE_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation
    if ( valRr == valRd )
    {
        if ( true == isInstruction32b(m_pc) )
        {
            incrPc(2);
            return 3;
        }
        else
        {
            incrPc(1);
            return 2;
        }
    }
    else
    {
        return 1;
    }
}

/*
 * OP Code: 1111 110r rrrr 0bbb - Skip if Bit in Register is Cleared
 * Operation: If Rr(b) = 0 then PC ← PC + 2 (or 3) else PC ← PC + 1
 */
inline int AVR8InstructionSet::inst_SBRC_Rr_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBRC_Rr_b);

    // Operands
    const unsigned int valb   = (0x01 << (opCode & 0x0007)); // bit mask
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);

    // Perform the operation
    if ( 0 == (valRr & valb) )
    {
        if ( true == isInstruction32b(m_pc) )
        {
            incrPc(2);
            return 3;
        }
        else
        {
            incrPc(1);
            return 2;
        }
    }
    else
    {
        return 1;
    }
}

/*
 * OP Code: 1111 111r rrrr 0bbb - Skip if Bit in Register is Set
 * Operation: If Rr(b) = 1 then PC ← PC + 2 (or 3) else PC ← PC + 1
 */
inline int AVR8InstructionSet::inst_SBRS_Rr_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBRS_Rr_b);

    // Operands
    const unsigned int valb   = (0x01 << (opCode & 0x0007)); // bit mask
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);

    // Perform the operation
    if ( 0 != (valRr & valb) )
    {
        if ( true == isInstruction32b(m_pc) )
        {
            incrPc(2);
            return 3;
        }
        else
        {
            incrPc(1);
            return 2;
        }
    }
    else
    {
        return 1;
    }
}

/*
 * OP Code: 1001 1011 AAAA Abbb - Skip if Bit in I/O Register is Set
 * Operation: If I/O(A,b) = 1 then PC ← PC + 2 (or 3) else PC ← PC + 1
 */
inline int AVR8InstructionSet::inst_SBIS_A_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBIS_A_b);

    // Operands
    const unsigned int valb  = (0x01 << (opCode & 0x0007));         // Bit mask
    const unsigned int addrA = 0x20 + ((opCode & 0x00f8) >> 3);     // 0x20..

    // Registers
    const unsigned int valA = m_dataMemory->read(addrA);

    // Perform the operation
    if ( 0 != (valA & valb) )
    {
        if ( true == isInstruction32b(m_pc) )
        {
            incrPc(2);

            if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
            {
                return 4;
            }
            else
            {
                return 3;
            }
        }
        else
        {
            incrPc(1);
            if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
            {
                return 3;
            }
            else
            {
                return 2;
            }
        }
    }
    else
    {
        if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
}

/*
 * OP Code: 1001 1001 AAAA Abbb - Skip if Bit in I/O Register is Cleared
 * Operation: If Rr(b) = 0 then PC ← PC + 2 (or 3) else PC ← PC + 1
 */
inline int AVR8InstructionSet::inst_SBIC_A_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBIC_A_b);

    // Operands
    const unsigned int valb  = (0x01 << (opCode & 0x0007));         // Bit mask
    const unsigned int addrA = 0x20 + ((opCode & 0x00f8) >> 3);     // 0x20..

    // Registers
    const unsigned int valA = m_dataMemory->read(addrA);

    // Perform the operation
    if ( 0 == (valA & valb) )
    {
        if ( true == isInstruction32b(m_pc) )
        {
            incrPc(2);

            if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
            {
                return 4;
            }
            else
            {
                return 3;
            }
        }
        else
        {
            incrPc(1);
            if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
            {
                return 3;
            }
            else
            {
                return 2;
            }
        }
    }
    else
    {
        if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
}

/*
 * OP Code: 0001 11rd dddd rrrr - Add with Carry.
 * Operation: Rd ← Rd + Rr + C
 */
int AVR8InstructionSet::inst_ADC_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ADC_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRr = m_dataMemory->read(addrRr);
    const unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    register unsigned int result = (valRr + valRd + ((sreg & AVR8RegNames::SREG_C) ? 1 : 0));

    // Clear all status flags except for 'I' and 'T'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T );

    // Evaluate new status flags
    if ( 0x100 & result )
    {
        result &= 0xff;
        sreg |= AVR8RegNames::SREG_C;
    }
    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z;
    }
    if ( 0x80 & result )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if (
         ( (valRr & 0x80) == (valRd  & 0x80) )
             &&
         ( (valRr & 0x80) != (result & 0x80) )
       )
    {
        // V ← 1; S ← (N ⊕ V): ⊕ 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }
    if (
         ( (valRr & 0x08) &&  (valRd  & 0x08) )
             ||
         ( (valRr & 0x08) && !(result & 0x08) )
             ||
         ( (valRd & 0x08) && !(result & 0x08) )
       )
    {
        sreg |= AVR8RegNames::SREG_H;
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, result);

    return 1;
}

int AVR8InstructionSet::inst_AND_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_AND_Rd_Rr);

    // OP Code: 0010 00rd dddd rrrr - Logical AND.
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    unsigned int valRd = m_dataMemory->read(addrRd);

    // Perform the operation
    valRd &= m_dataMemory->read(addrRr);

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C );

    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;
    }
    if ( 0x80 & valRd )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg |= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    return 1;
}

/*
 * OP Code: 0010 01rd dddd rrrr - Exclusive OR
 * Operation: Rd ← Rd ⊕ Rr
 */
int AVR8InstructionSet::inst_EOR_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_EOR_Rd_Rr);

    // Operands
    const unsigned int addrRr = ( ( opCode & 0x0200 ) >> 5 ) | ( opCode & 0x000f );
    const unsigned int addrRd = ( ( opCode & 0x01f0 ) >> 4 );

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg  = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd ^= m_dataMemory->read(addrRr);

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C);

    // Evaluate new status flags
    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    if ( 0x80 & valRd )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0010 10rd dddd rrrr - Logical OR
 * Operation: Rd ← Rd v Rr
 */
int AVR8InstructionSet::inst_OR_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_OR_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg  = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd |= m_dataMemory->read(addrRr);

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C);

    // Evaluate new status flags
    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    if ( 0x80 & valRd )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0010 11rd dddd rrrr - Copy Register
 * Operation: Rd ← Rr
 */
int AVR8InstructionSet::inst_MOV_Rd_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_MOV_Rd_Rr);

    // Operands
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    m_dataMemory->write(addrRd, m_dataMemory->read(addrRr));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0000 0001 dddd rrrr - Copy Register Word
 * Operation: Rd+1:Rd ← Rr+1:Rr
 */
inline int AVR8InstructionSet::inst_MOVW_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_MOVW] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_MOVW);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_MOVW_Rd_Rr);

    // Operands
    unsigned int addrRr = (opCode & 0x000f) << 1;   // r ∈ {0,2,...,30}
    unsigned int addrRd = (opCode & 0x00f0) >> 3;   // d ∈ {0,2,...,30}

    /*
     * Perform the operation
     */
    m_dataMemory->write(addrRd, m_dataMemory->read(addrRr));        // Low byte
    addrRd++;
    addrRr++;
    m_dataMemory->write(addrRd, m_dataMemory->read(addrRr));        // High byte

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0110 KKKK dddd KKKK - Logical OR with Immediatee
 * Operation: Rd ← Rd v K
 */
int AVR8InstructionSet::inst_ORI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ORI_Rd_K);

    // Operands
    const unsigned int valK   = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg  = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd |= valK;

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C);

    // Evaluate new status flags
    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    if ( 0x80 & valRd )
    {
        // N ← 1;  S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 0111 KKKK dddd KKKK - Logical AND with Immediate
 * Operation: Rd ← Rd • K
 */
int AVR8InstructionSet::inst_ANDI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ANDI_Rd_K);

    // Operands
    const unsigned int valK   = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31

    // Registers
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    const unsigned int result = m_dataMemory->read(addrRd) | valK;

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C );

    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z; // Z ← 1
    }
    if ( 0x80 & result )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg |= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, result);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1011 0AAd dddd AAAA - Load an I/O Location to Register
 * Operation: Rd ← I/O(A)
 */
int AVR8InstructionSet::inst_IN ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_IN);

    // Operands
    const unsigned int addrA  = 0x20 + (((opCode & 0x0600) >> 5) | (opCode & 0x000F));
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    m_dataMemory->write(addrRd, m_dataMemory->read(addrA));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1011 1AAr rrrr AAAA - Store Register to I/O Location
 * Operation: I/O(A) ← Rd
 */
int AVR8InstructionSet::inst_OUT_A_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_OUT_A_Rr);

    // Operands
    const unsigned int addrA  = 0x20 + (((opCode & 0x0600) >> 5) | (opCode & 0x000F));
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    m_dataMemory->write(addrA, m_dataMemory->read(addrRd));

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1100 kkkk kkkk kkkk -  Relative Jump
 * Operation: PC ← PC + k + 1
 */
int AVR8InstructionSet::inst_RJMP_k ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_RJMP_k);

    // Operands
    int valK = unsigned(opCode & 0x0fff);

    // Execute jump
    if ( valK & 0x800 )
    {
        valK -= 0x1000;
    }
    incrPc(valK);

    return 2;
}

/*
 * OP Code: 1101 kkkk kkkk kkkk - Relative Call to Subroutine
 * Operation:
 * PC ← PC + k + 1  Devices with 16 bits PC, 128K bytes Program memory maximum.
 * PC ← PC + k + 1  Devices with 22 bits PC, 8M bytes Program memory maximum.
 */
int AVR8InstructionSet::inst_RCALL_k ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_RCALL_k);

    // Operands
    int valK = unsigned(opCode & 0x0fff);

    // Determinate how many machine cycles this takes
    int cycles;
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 2;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 3;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }
    else if ( MCUSim::FAMILY_TINYAVR /* Reduced code tinyAVR */ == m_config.m_family )
    {
        cycles = 4;
    }
    else
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 3;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 4;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }

    // Log the event
    logEvent(EVENT_CPU_CALL, m_pc, AVR8InsNames::SPECI_RCALL);

    // Push current PC value onto the stack
    m_dataMemory->pushOnStack(m_pc & 0xff);
    m_pc >>= 8;
    m_dataMemory->pushOnStack(m_pc & 0xff);
    if ( m_config.m_pcWidth > PCWIDTH_16 )
    {
        m_pc >>= 8;
        m_dataMemory->pushOnStack(m_pc & 0xff);
    }

    // Execute subprogram call
    m_actSubprogCounter++;
    if ( valK & 0x800 )
    {
        valK -= 0x1000;
    }
    incrPc(valK);

    return cycles;
}

/*
 * OP Code: 1110 KKKK dddd KKKK - Load Immediate
 * Operation: Rd ← K
 */
int AVR8InstructionSet::inst_LDI_Rd_K ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LDI_Rd_K);

    // Operands
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10; // 16 ≤ d ≤ 31
    const unsigned int valK   = ((opCode & 0x0f00) >> 4) | (opCode & 0x000f);

    m_dataMemory->write(addrRd, valK);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1111 00kk kkkk ksss - Branch if Bit in SREG is Set
 * Operation: if SREG(s) = 1 then PC ← PC + k + 1, else PC ← PC + 1
 */
int AVR8InstructionSet::inst_BRBS_s_k ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BRBS_s_k);

    // Operand s
    const unsigned int maskS = 0x01 << (opCode & 0x0007);

    // Registers
    const unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    if ( maskS & sreg )
    {
        /*
         * Bit SREG(s) is set --> jump
         */

        int valK = unsigned((opCode & 0x0Cf8) >> 3);    // Operand k

        // Convert 7b valK to any size signed integer
        if ( valK & 0x40 /* 0x40 = sign bit */ )
        {
            // Negative 7b number, make it any size negative
            valK -= 0x80;
        }

        incrPc(valK);   // Execute jump
        return 2;       // Jump takes 2 cycles
    }
    else
    {
        // Bit SREG(s) is cleared --> continue normally
        return 1;
    }
}

/*
 * OP Code: 1111 01kk kkkk ksss - Branch if Bit in SREG is Cleared
 * Operation: if SREG(s) = 0 then PC ← PC + k + 1, else PC ← PC + 1
 */
int AVR8InstructionSet::inst_BRBC_s_k ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BRBC_s_k);

    // Operands
    const unsigned int maskS = 0x01 << (opCode & 0x0007);

    // Registers
    const unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    if ( maskS & sreg )
    {
        // Bit SREG(s) is set --> continue normally
        return 1;
    }
    else
    {
        /*
         * Bit SREG(s) is cleared --> jump
         */

        int valK = unsigned((opCode & 0x0Cf8) >> 3);    // Operand k

        // Convert 7b valK to any size signed integer
        if ( valK & 0x40 /* 0x40 = sign bit */ ) {
            // Negative 7b number, make it any size negative
            valK -= 0x80;
        }

        incrPc(valK);   // Execute jump
        return 2;       // Jump takes 2 cycles
    }
}

/*
 * OP Code: 1001 0101 0000 1001 - Indirect Call to Subroutine
 * Operation: PC(15:0) ← Z(15:0), PC(21:16) ← 0
 */
int AVR8InstructionSet::inst_ICALL(const unsigned int)
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_ICALL] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_ICALL);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_ICALL);

    // Retrieve and compute the destination address
    unsigned int addr = m_dataMemory->read(AVR8RegNames::ZL);       // addr(7:0)
    addr |= m_dataMemory->read(AVR8RegNames::ZL) << 8;              // addr(15:8)

    // Determinate how many machine cycles this takes
    int cycles;
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 2;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 3;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }
    else
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 3;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 4;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }

    // Log the event
    logEvent(EVENT_CPU_CALL, m_pc, AVR8InsNames::SPECI_ICALL);

    // Push current PC value onto the stack
    m_dataMemory->pushOnStack(m_pc & 0xff);
    m_pc >>= 8;
    m_dataMemory->pushOnStack(m_pc & 0xff);
    if ( m_config.m_pcWidth > PCWIDTH_16 )
    {
        m_pc >>= 8;
        m_dataMemory->pushOnStack(m_pc & 0xff);
    }

    // Execute subprogram call
    m_actSubprogCounter++;
    m_pc = addr;

    return cycles;
}

/*
 * OP Code: 1001 0101 0001 1001 - Extended Indirect Call to Subroutine
 * Operation: PC(15:0) ← Z(15:0), PC(21:16) ← EIND
 */
int AVR8InstructionSet::inst_EICALL ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_EICALL] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_EICALL);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_EICALL);

    // Retrieve and compute the destination address
    unsigned int addr = m_dataMemory->read(AVR8RegNames::EIND);     // addr(21:16)
    addr |= m_dataMemory->read(AVR8RegNames::ZH) << 8;              // addr(15:8)
    addr |= m_dataMemory->read(AVR8RegNames::ZL);                   // addr(7:0)

    // Log the event
    logEvent(EVENT_CPU_CALL, m_pc, AVR8InsNames::SPECI_EICALL);

    // Push current PC value onto the stack
    m_dataMemory->pushOnStack(m_pc & 0xff);
    m_pc >>= 8;
    m_dataMemory->pushOnStack(m_pc & 0xff);
    m_pc >>= 8;
    m_dataMemory->pushOnStack(m_pc & 0xff);

    // Execute subprogram call
    m_actSubprogCounter++;
    m_pc = addr;

    // Determinate how many machine cycles this takes
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        return 3;
    }
    else
    {
        return 4;
    }
}

/*
 * OP Code: 1001 0100 0000 1001 - Indirect Jump
 * Operation: PC(15:0) ← Z(15:0), PC(21:16) ← 0
 */
int AVR8InstructionSet::inst_IJMP ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_IJMP] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_IJMP);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_IJMP);

    // Retrieve and compute the destination address
    unsigned int addr = m_dataMemory->read(AVR8RegNames::ZL);       // addr(7:0)
    addr |= m_dataMemory->read(AVR8RegNames::ZH) << 8;              // addr(15:8)

    // Execute jump
    m_pc = addr;

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 1001 0100 0001 1001 - Extended Indirect Jump
 * Operation: PC(15:0) ← Z(15:0), PC(21:16) ← EIND
 */
int AVR8InstructionSet::inst_EIJMP ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_EIJMP] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_EIJMP);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_EIJMP);

    // Retrieve and compute the destination address
    unsigned int addr = m_dataMemory->read(AVR8RegNames::EIND);     // addr(21:16)
    addr <<= 8;
    addr |= m_dataMemory->read(AVR8RegNames::ZH);                   // addr(15:8)
    addr <<= 8;
    addr |= m_dataMemory->read(AVR8RegNames::ZL);                   // addr(7:0)

    // Execute jump
    m_pc = addr;

    // This takes two cycles to execute
    return 2;
}

template<int instruction, bool destR0, bool postIncrement, bool extendedLoad>
    inline int AVR8InstructionSet::instLoadProgMemory ( const unsigned int opCode )
{

    if ( false == m_config.m_availableInstructions[instruction] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, instruction);
        return -1;
    }

    // Destination register
    unsigned int addrRd;
    if ( true == destR0 )
    {
        addrRd = AVR8RegNames::R0;
    }
    else
    {
        addrRd = (opCode & 0x01f0) >> 4;
    }

    // Check validity of Rd address, in case of post-incrementation
    if ( true == postIncrement )
    {
        if ( (AVR8RegNames::R30 == addrRd) | (AVR8RegNames::R31 == addrRd) )
        {
            // Invalid address, generate random result
            logEvent(EVENT_CPU_ERR_INVALID_OPSET, m_pc, AVR8InsNames::SPECI_ELPM_Rd_Zplus);

            // Write fake result
            m_dataMemory->write(addrRd, (0xff & m_programMemory->getUndefVal()));

            // Abort (3 cycles taken)
            return 3;
        }
    }

    // Load source address from RAMPZ:Z registers
    unsigned int aux = 0;
    if ( true == extendedLoad )
    {
        aux |= m_dataMemory->read(AVR8RegNames::RAMPZ);
        aux <<= 8;
    }
    aux = m_dataMemory->read(AVR8RegNames::ZH);
    aux <<= 8;
    aux |= m_dataMemory->read(AVR8RegNames::ZL);

    // Increment the RAMPZ:Z registers, if requested
    if ( true == postIncrement )
    {
        unsigned int tmp;
        if ( true == extendedLoad )
        {
            tmp = (aux + 1) & 0xffffff;
        }
        else
        {
            tmp = (aux + 1) & 0xffff;
        }
        m_dataMemory->write(AVR8RegNames::ZL, (tmp & 0xff));
        tmp >>= 8;
        m_dataMemory->write(AVR8RegNames::ZH, (tmp & 0xff));
        if ( m_programMemory->size() > 0xffff )
        {
            tmp >>= 8;
            m_dataMemory->write(AVR8RegNames::RAMPZ, (tmp & 0xff));
        }
    }

    // Load either low or high byte of program memory word pointed the Z register
    if ( aux & 0x1)
    {
        // Load high byte
        aux >>= 1;
        aux = m_bootLoader->lpmRead(aux);
        aux >>= 8;
    }
    else
    {
        // Load low byte
        aux >>= 1;
        aux = m_bootLoader->lpmRead(aux);
        aux &= 0xff;
    }

    // Write result to an Rd register
    m_dataMemory->write(addrRd, aux);

    // This takes three cycles to execute
    return 3;
}

/*
 * OP Code: 1001 000d dddd 0100 - Load Program Memory
 * Operation: Rd ← (Z)
 */
int AVR8InstructionSet::inst_LPM_Rd_Z ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LPM_Rd_Z);
    return instLoadProgMemory<AVR8InsNames::SPECI_LPM_Rd_Z>(opCode);
}

/*
 * OP Code: 1001 0101 1100 1000 - Load Program Memory
 * Operation: R0 ← (Z)
 */
int AVR8InstructionSet::inst_LPM ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_LPM);
    return instLoadProgMemory<AVR8InsNames::SPECI_LPM, true>(0);
}

/*
 * OP Code: 1001 000d dddd 0101 - Load Program Memory
 * Operation: Rd ← (Z), Z ← Z + 1
 */
int AVR8InstructionSet::inst_LPM_Rd_Zplus ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LPM_Rd_Zplus);
    return instLoadProgMemory<AVR8InsNames::SPECI_LPM_Rd_Zplus, false, true>(opCode);
}

/*
 * OP Code: 1001 0101 1101 1000 - Extended Load Program Memory (ELPM)
 * Operation: R0 ← (RAMPZ:Z)
 */
int AVR8InstructionSet::inst_ELPM ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_ELPM);
    return instLoadProgMemory<AVR8InsNames::SPECI_ELPM, true, false, true>(0);
}

/*
 * OP Code: 1001 000d dddd 0110 - Extended Load Program Memory (ELPM Rd, Z)
 * Operation: Rd ← (RAMPZ:Z)
 */
int AVR8InstructionSet::inst_ELPM_Rd_Z ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ELPM_Rd_Z);
    return instLoadProgMemory<AVR8InsNames::SPECI_ELPM_Rd_Z, false, false, true>(opCode);
}

/*
 * OP Code: 1001 000d dddd 0111 - Extended Load Program Memory (ELPM Rd, Z+)
 * Operation: Rd ← (RAMPZ:Z), (RAMPZ:Z) ← (RAMPZ:Z) +1
 *
 * Result of these combinations is undefined:
 *   - ELPM r30, Z+             // r30 has the same address as Z low
 *   - ELPM r31, Z+             // r31 has the same address as Z high
 */
int AVR8InstructionSet::inst_ELPM_Rd_Zplus ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ELPM_Rd_Zplus);
    return instLoadProgMemory<AVR8InsNames::SPECI_ELPM_Rd_Zplus, false, true, true>(opCode);
}

/*
 * OP Code: 1001 0101 1000 1000 - SLEEP
 * Operation: Enter a sleep mode
 */
int AVR8InstructionSet::inst_SLEEP ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SLEEP);

    // Available sleep modes:
    // ----------------------
    // SM2  SM1     SM0     Sleep Mode
    // 0    0       0       Idle
    // 0    0       1       ADC Noise Reduction
    // 0    1       0       Power-down
    // 0    1       1       Power-save
    // 1    0       0       Reserved
    // 1    0       1       Reserved
    // 1    1       0       Standby
    //
    // Note: Standby mode is only available with external crystals or resonators.

    // Determinate the sleep mode
    const unsigned int mcucr = m_dataMemory->read(AVR8RegNames::MCUCR);

    if ( AVR8RegNames::MCUCR_SE & mcucr )
    {
        // Go to the designated sleep mode
        *m_sleepMode = AVR8Sim::SleepMode ( mcucr & (AVR8RegNames::MCUCR_SM2 | AVR8RegNames::MCUCR_SM1 | AVR8RegNames::MCUCR_SM0) );
        *m_processorMode = MCUSim::MD_SLEEP;
        logEvent(EVENT_CPU_MODE_CHANGED, m_pc, AVR8InsNames::INS_SLEEP);
    }
    else
    {
        // Sleep mode disabled
        logEvent(EVENT_CPU_INST_IGNORED, m_pc, AVR8InsNames::INS_SLEEP);
    }

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 0101 1001 1000 - Break
 * Operation: On-chip Debug system break
 */
int AVR8InstructionSet::inst_BREAK ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_BREAK] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_BREAK);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_BREAK);

    if ( ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_JTAGEN] )
             ||
         ( true == (*m_fusesAndLocks)[AVR8FusesAndLocks::FUSE_OCDEN] ) )
    {
        *m_processorMode = MCUSim::MD_STOPPED;
        *m_haltMode = AVR8Sim::HALTM_BREAK;
        logEvent(EVENT_CPU_MODE_CHANGED, m_pc, AVR8InsNames::INS_BREAK);
    }
    else
    {
        logEvent(EVENT_CPU_INST_IGNORED, m_pc, AVR8InsNames::INS_BREAK);
    }
    return 1;
}

/*
 * OP Code: 1001 0101 1010 1000 - Watchdog Reset
 * Operation: WD timer reset
 */
int AVR8InstructionSet::inst_WDR ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_WDR);

    // Reset the watchdog timer
    m_systemControl->watchDogReset();

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 0100 0sss 1000 - Bit Clear in SREG
 * Operation: SREG(s) ← 0
 */
int AVR8InstructionSet::inst_BSET_s ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BSET_s);

    const unsigned int valS = (opCode & 0x0070) >> 4;
    const unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG) | ~(0x01 << valS);
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    return 1;
}

/*
 * OP Code: 1001 0100 1sss 1000 - Bit Clear in SREG
 * Operation: SREG(s) ← 0
 */
int AVR8InstructionSet::inst_BCLR_s ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BCLR_s);

    const unsigned int valS = (opCode & 0x0070) >> 4;
    const unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG) & ~(0x01 << valS);
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    return 1;
}

template<int bitMask>
    inline int AVR8InstructionSet::instSregSetBit()
{
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    sreg |= bitMask;
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    return 1;
}

template<int bitMask>
    inline int AVR8InstructionSet::instSregClearBit()
{
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);
    sreg &= ~bitMask;
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    return 1;
}

/*
 * OP Code: 1001 0100 0000 1000 - Set Carry Flag
 * Operation: C ← 1
 */
int AVR8InstructionSet::inst_SEC ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEC);
    return instSregSetBit<AVR8RegNames::SREG_C>();
}

/*
 * OP Code: 1001 0100 0001 1000 - Set Zero Flag
 * Operation: Z ← 1
 */
int AVR8InstructionSet::inst_SEZ ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEZ);
    return instSregSetBit<AVR8RegNames::SREG_Z>();
}

/*
 * OP Code: 1001 0100 0010 1000 - Set Negative Flag
 * Operation: N ← 1
 */
int AVR8InstructionSet::inst_SEN ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEN);
    return instSregSetBit<AVR8RegNames::SREG_N>();
}

/*
 * OP Code: 1001 0100 0011 1000 - Set Overflow Flag
 * Operation: V ← 1
 */
int AVR8InstructionSet::inst_SEV ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEV);
    return instSregSetBit<AVR8RegNames::SREG_V>();
}

/*
 * OP Code: 1001 0100 0100 1000 - Set Signed Flag
 * Operation: S ← 1
 */
int AVR8InstructionSet::inst_SES ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SES);
    return instSregSetBit<AVR8RegNames::SREG_S>();
}

/*
 * OP Code: 1001 0100 0101 1000 - Set Half Carry Flag
 * Operation: H ← 1
 */
int AVR8InstructionSet::inst_SEH ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEH);
    return instSregSetBit<AVR8RegNames::SREG_H>();
}

/*
 * OP Code: 1001 0100 0110 1000 - Set T Flag
 * Operation: T ← 1
 */
int AVR8InstructionSet::inst_SET ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SET);
    return instSregSetBit<AVR8RegNames::SREG_T>();
}

/*
 * OP Code: 1001 0100 0111 1000 - Set Global Interrupt Flag
 * Operation: I ← 1
 */
int AVR8InstructionSet::inst_SEI ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_SEI);
    return instSregSetBit<AVR8RegNames::SREG_I>();
}

/*
 * OP Code: 1001 0100 1000 1000 - Clear Carry Flag
 * Operation: C ← 0
 */
int AVR8InstructionSet::inst_CLC ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLC);
    return instSregClearBit<AVR8RegNames::SREG_C>();
}

/*
 * OP Code: 1001 0100 1001 1000 - Clear Zero Flag
 * Operation: Z ← 0
 */
int AVR8InstructionSet::inst_CLZ ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLZ);
    return instSregClearBit<AVR8RegNames::SREG_Z>();
}

/*
 * OP Code: 1001 0100 1010 1000 - Clear Negative Flag
 * Operation: N ← 0
 */
int AVR8InstructionSet::inst_CLN ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLN);
    return instSregClearBit<AVR8RegNames::SREG_N>();
}

/*
 * OP Code: 1001 0100 1011 1000 - Clear Overflow Flag
 * Operation: V ← 0
 */
int AVR8InstructionSet::inst_CLV ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLV);
    return instSregClearBit<AVR8RegNames::SREG_V>();
}

/*
 * OP Code: 1001 0100 1100 1000 - Clear Signed Flag
 * Operation: S ← 0
 */
int AVR8InstructionSet::inst_CLS ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLS);
    return instSregClearBit<AVR8RegNames::SREG_S>();
}

/*
 * OP Code: 1001 0100 1101 1000 - Clear Half Carry Flag
 * Operation: H ← 0
 */
int AVR8InstructionSet::inst_CLH ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLH);
    return instSregClearBit<AVR8RegNames::SREG_H>();
}

/*
 * OP Code: 1001 0100 1110 1000 - Clear T Flag
 * Operation: T ← 0
 */
int AVR8InstructionSet::inst_CLT ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLT);
    return instSregClearBit<AVR8RegNames::SREG_T>();
}

/*
 * OP Code: 1001 0100 1111 1000 - Clear Global Interrupt Flag
 * Operation: I ← 0
 */
int AVR8InstructionSet::inst_CLI ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_CLI);
    return instSregClearBit<AVR8RegNames::SREG_I>();
}

/*
 * OP Code: 0000 0000 0000 0000 - No Operation
 * Operation: (no operation)
 */
inline int AVR8InstructionSet::inst_NOP ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_NOP);
    return 1;
}

/*
 * OP Code: 1001 0110 KKdd KKKK - Add Immediate to Word
 * Operation: Rd+1:Rd ← Rd+1:Rd + K
 */
inline int AVR8InstructionSet::inst_ADIW_Rd_1_Rd_K ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_ADIW] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_ADIW);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_ADIW_Rd_1_Rd_K);

    // Operands
    const unsigned int valK = ((opCode & 0x00C0) >> 2) | (opCode & 0x000f);
    unsigned int addrRd = ((opCode & 0x0030) >> 3) | 0x18; // d ∈ {24,26,28,30}

    // Registers
    const unsigned int valRd0 = m_dataMemory->read(addrRd);
    const unsigned int valRd1 = m_dataMemory->read(++addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Auxiliary variables
    register unsigned int result = ((valRd1 << 8) + valRd0 + valK);

    // Clear all status flags except for 'I', 'T', and 'H'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H );

    // Evaluate status sreg
    if ( 0x10000 & result )
    {
        result &= 0xffff;
        sreg |= AVR8RegNames::SREG_C;
    }
    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z;
    }
    if ( 0x8000 & result )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg |= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if ( (result & 0x8000) && !(valRd1 & 0x80) )
    {
        // V ← 1; S ← (N ⊕ V): ⊕ 1 (V)
        sreg ^= ( AVR8RegNames::SREG_S | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, (result >> 8)); // Rd high
    m_dataMemory->write(--addrRd, (result & 0xff)); // Rd low

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 1001 010k kkkk 110k \
 *          kkkk kkkk kkkk kkkk - Jump
 * Operation: PC ← k
 */
inline int AVR8InstructionSet::inst_JMP_k ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_JMP] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_JMP);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_JMP_k);

    // Operand
    unsigned int valK = ((opCode & 0x01f0) << 13) | ((opCode & 0x0001) << 16);
    valK |= m_programMemory->read(m_pc); // Lower 16b are in separate word
    incrPc();

    // Check whether our program counter could even contain the destination address
    if ( (m_config.m_pcWidth > PCWIDTH_16) && (valK & 0xffff0000) )
    {
        logEvent(EVENT_CPU_ERR_INVALID_CALL, m_pc, AVR8InsNames::SPECI_CALL);
        valK &= 0xffff0000;
    }

    // Execute jump
    m_pc = valK;

    return 3;
}

/*
 * OP Code: 1001 0101 0000 1000 - Return from Subroutine
 * Operation:
 * PC(15:0) ← STACK Devices with 16 bits PC, 128K bytes Program memory maximum.
 * PC(21:0) ← STACK Devices with 22 bits PC, 8M bytes Program memory maximum.
 */
int AVR8InstructionSet::inst_RET ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_RET);

    // Load the return address from the stack
    unsigned int returnAddress = m_dataMemory->popFromStack() | (m_dataMemory->popFromStack() << 8);
    if ( m_config.m_pcWidth > PCWIDTH_16 )
    {
        returnAddress |= m_dataMemory->popFromStack() << 16;
    }

    // Execute return from subprogram
    if ( 0 == m_actSubprogCounter )
    {
        logEvent(EVENT_CPU_ERR_INVALID_RET, m_pc);
    }
    else
    {
        m_actSubprogCounter--;
    }
    m_pc = returnAddress;

    // Determinate hos many cycles this takes
    if ( PCWIDTH_22 == m_config.m_pcWidth )
    {
        return 5;
    }
    else
    {
        return 4;
    }
}

/*
 * OP Code: 1001 0101 0001 1000 - Return from Interrupt
 * Operation:
 * PC(15:0) ← STACK Devices with 16 bits PC, 128K bytes Program memory maximum.
 * PC(21:0) ← STACK Devices with 22 bits PC, 8M bytes Program memory maximum.
 */
int AVR8InstructionSet::inst_RETI ( const unsigned int )
{
    instructionEnter(AVR8InsNames::INS_RETI);

    // Load the return address from the stack
    unsigned int returnAddress = m_dataMemory->popFromStack() | (m_dataMemory->popFromStack() << 8);
    if ( m_config.m_pcWidth > PCWIDTH_16 )
    {
        returnAddress |= m_dataMemory->popFromStack() << 16;
    }

    // Execute return from subprogram
    if ( true == m_interruptController->interrupted() )
    {
        logEvent(EVENT_CPU_ERR_INVALID_RETI, m_pc);
    }
    else
    {
        // Make interrupt controller forget about the last interrupt
        m_interruptController->reti();
    }
    m_pc = returnAddress;

    // Determinate hos many cycles this takes
    if ( PCWIDTH_22 == m_config.m_pcWidth )
    {
        return 5;
    }
    else
    {
        return 4;
    }
}

/*
 * OP Code: 1001 010k kkkk 111k \
 *          kkkk kkkk kkkk kkkk - Long Call to a Subroutine
 * Operation: PC ← k
 */
inline int AVR8InstructionSet::inst_CALL_k ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_CALL] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_CALL);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_CALL_k);

    // Operand
    unsigned int valK = ((opCode & 0x01f0) << 13) | ((opCode & 0x0001) << 16);
    valK |= m_programMemory->read(m_pc); // Lower 16b are in separate word

    // Auxiliary variables
    int cycles; // Number of machine cycles

    // Log the event and increment PC (we have read one more word)
    logEvent(EVENT_CPU_CALL, m_pc, AVR8InsNames::SPECI_CALL);
    incrPc();

    // Determinate how many machine cycles this takes
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 3;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 4;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }
    else
    {
        if ( PCWIDTH_16 == m_config.m_pcWidth )
        {
            cycles = 4;
        }
        else if ( PCWIDTH_22 == m_config.m_pcWidth )
        {
            cycles = 5;
        }
        else
        {
            logEvent(EVENT_CPU_SYS_FATAL_ERROR, m_pc);
            return -1;
        }
    }
    // Check whether our program counter could even contain the destination address
    if ( (m_config.m_pcWidth > PCWIDTH_16) && (valK & 0xffff0000) )
    {
        logEvent(EVENT_CPU_ERR_INVALID_CALL, m_pc, AVR8InsNames::SPECI_CALL);
        valK &= 0xffff0000;
    }

    // Push current PC value onto the stack
    m_dataMemory->pushOnStack(m_pc & 0xff);
    m_pc >>= 8;
    m_dataMemory->pushOnStack(m_pc & 0xff);
    if ( m_config.m_pcWidth > PCWIDTH_16 )
    {
        m_pc >>= 8;
        m_dataMemory->pushOnStack(m_pc & 0xff);
    }

    // Execute subprogram call
    m_actSubprogCounter++;
    m_pc = valK;

    return cycles;
}

/*
 * OP Code: 1001 010d dddd 1010 - Decrement
 * Operation: Rd ← Rd - 1
 */
inline int AVR8InstructionSet::inst_DEC_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_DEC_Rd);

    // Operands
    unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd--;
    valRd &= 0xff;

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C );

    // Evaluate new status flags
    if ( 0 == valRd )
    {
        // Z ← 1
        sreg ^= AVR8RegNames::SREG_Z;
    }
    else if ( 0x7F == valRd )
    {
        /*
         * V flag is set if two’s complement overflow resulted from the
         * operation; cleared otherwise. Two’s complement overflow occurs
         * if and only if Rd was 0x80 before the operation (0x7F after).
         */

        // V ← 1; S ← (N ⊕ V): ← 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }

    if ( 0x80 & valRd )
    {
        // N ← 1; S ← (N ⊕ V): ⊕ 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 010d dddd 0011 - Increment
 * Operation: Rd ← Rd + 1
 */
inline int AVR8InstructionSet::inst_INC_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_INC_Rd);

    // Operands
    unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd++;
    valRd &= 0xff;

    // Clear all status flags except for 'I', 'T', 'H', and 'C'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H | AVR8RegNames::SREG_C );

    // Evaluate new status flags
    if ( 0 == valRd )
    {
        // Z ← 1
        sreg ^= AVR8RegNames::SREG_Z;
    }
    else if ( 0x80 == valRd )
    {
        /*
         * V flag is set if two’s complement overflow resulted from the
         * operation; cleared otherwise. Two’s complement overflow occurs
         * if and only if Rd was 0x7F before the operation (0x80 after).
         */

        // V ← 1; S ← (N ⊕ V): ← 1 (V)
        sreg ^= ( AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }

    if ( 0x80 & valRd )
    {
        // N ← 1; S ← (N ⊕ V): ⊕ 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 010d dddd 0000 - One’s Complement
 * Operation: Rd ← $FF - Rd
 */
inline int AVR8InstructionSet::inst_COM_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_COM_Rd);

    // Operands
    unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    valRd ^= 0xff;

    // Clear all status flags except for 'I', 'T', and 'H'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H );

    // Evaluate new status flags
    sreg |= AVR8RegNames::SREG_C;           // C ← 1
    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }

    if ( 0x80 & valRd )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg |= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 010d dddd 0001 - Two’s Complement
 * Operation: Rd ← $00 - Rd
 */
inline int AVR8InstructionSet::inst_NEG_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_NEG_Rd);

    // Operands
    unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Auxiliary variables
    register unsigned int result = valRd;

    // Perform the operation
    result ^= 0xff;
    result += 1;
    result &= 0xff;

    // Clear all status flags except for 'I', and 'T'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T );

    // Evaluate new status flags
    if ( 0 == result )
    {
        sreg ^= AVR8RegNames::SREG_Z;   // Z ← 1
    }
    else
    {
        sreg ^= AVR8RegNames::SREG_C;   // C ← 1
    }
    if ( 0x80 & result )
    {
        // N ← 1; S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if ( 0x80 == result )
    {
        // V ← 1; S ← (N ⊕ V): ← 1 (V)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_S );
    }
    if ( (0x08 & result) || (0x08 & valRd))
    {
        sreg ^= AVR8RegNames::SREG_H;   // H ← 1
    }


    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, result);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 010d dddd 0010 - Swap Nibbles
 * Operation: R(7:4) ← Rd(3:0), R(3:0) ← Rd(7:4)
 */
inline int AVR8InstructionSet::inst_SWAP_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SWAP_Rd);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    const unsigned int valRd = m_dataMemory->read(addrRd);

    // Auxiliary variables
    const unsigned int result = ((valRd & 0x0f) << 4) | ((valRd & 0xf0) >> 4);

    // Write results
    m_dataMemory->write(addrRd, result);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 010d dddd 0101 - Arithmetic Shift Right.
 * Operation:  ╷₇╷₆╷₅╷₄╷₃╷₂╷₁╷₀╷
 *             ┏━┳━┳━┳━┳━┳━┳━┳━┓
 *             ┗━┻━┻━┻━┻━┻━┻━┻━┛
 *             ╵↶╵→╵→╵→╵→╵→╵→╵→╵
 *               0→
 */
inline int AVR8InstructionSet::inst_ASR_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ASR_Rd);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Clear all status flags except for 'I', 'T', and 'H'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H );

    /*
     * Evaluate status sreg, and perform the shift operation
     */
    if ( 0x01 & valRd )
    {
        // C ← 1; V ← (N ⊕ C): ← 1 (C)
        sreg ^= ( AVR8RegNames::SREG_C | AVR8RegNames::SREG_V );
    }

    // Perform the shift.
    valRd >>= 1;

    if ( 0x40 & valRd )
    {
        // Preserve sign bit in the valRd.
        valRd ^= 0xc0;

        // N ← 1;  V ← (N ⊕ C): ⊕ 1 (N); S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );

        if ( AVR8RegNames::SREG_V & sreg )
        {
            // S ← (N ⊕ V): ⊕ 1 (V)
            sreg ^= AVR8RegNames::SREG_S;
        }
    }
    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    return 1;
}

/*
 * OP Code: 1001 010d dddd 0110 - Logical Shift Right
 * Operation:  ╷₇╷₆╷₅╷₄╷₃╷₂╷₁╷₀╷
 *             ┏━┳━┳━┳━┳━┳━┳━┳━┓
 *             ┗━┻━┻━┻━┻━┻━┻━┻━┛
 *         0 → ╵→╵→╵→╵→╵→╵→╵→╵→╵ → C
 */
inline int AVR8InstructionSet::inst_LSR_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LSR_Rd);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Clear all status flags except for 'I', 'T', and 'H'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T | AVR8RegNames::SREG_H );

    /*
     * Evaluate status sreg, and perform the shift operation
     */
    if ( 0x01 & valRd )
    {
        // C ← 1; V ← (N ⊕ C): ← 1 (C); S ← (N ⊕ V): ← 1 (V)
        sreg |= ( AVR8RegNames::SREG_C | AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }

    valRd >>= 1;    // Perform the shift.

    if ( 0 == valRd )
    {
        sreg |= AVR8RegNames::SREG_Z;   // Z ← 1
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    return 1;
}

/*
 * OP Code: 1001 010d dddd 0111 - Rotate Right through Carry
 * Operation:  ╷₇╷₆╷₅╷₄╷₃╷₂╷₁╷₀╷
 *             ┏━┳━┳━┳━┳━┳━┳━┳━┓
 *             ┗━┻━┻━┻━┻━┻━┻━┻━┛
 *         C → ╵→╵→╵→╵→╵→╵→╵→╵→╵ → C
 */
inline int AVR8InstructionSet::inst_ROR_Rd ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ROR_Rd);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Rd8 ← C
    if ( AVR8RegNames::SREG_C & sreg )
    {
        valRd |= 0x100;
    }

    // Clear all status flags except for 'I', and 'T'.
    sreg &= ( AVR8RegNames::SREG_I | AVR8RegNames::SREG_T );

    /*
     * Evaluate status sreg, and perform the rotation operation
     */

    // C ← Rd0
    if ( 0x01 & valRd )
    {
        // C ← 1; V ← (N ⊕ C): ← 1 (C); S ← (N ⊕ V): ← 1 (V)
        sreg ^= ( AVR8RegNames::SREG_C | AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }

    // Perform the rotation
    valRd >>= 1;

    if ( 0x80 & valRd )
    {
        // N ← 1; V ← (N ⊕ C): ← 1 (N); S ← (N ⊕ V): ← 1 (N)
        sreg ^= ( AVR8RegNames::SREG_N | AVR8RegNames::SREG_V | AVR8RegNames::SREG_S );
    }
    if ( 0 == valRd )
    {
        sreg ^= AVR8RegNames::SREG_Z;   // Z ← 1
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->write(addrRd, valRd);

    return 1;
}

/*
 * OP Code: 1111 100d dddd 0bbb - Bit Load from the T Flag in SREG to a Bit in Register.
 * Operation: Rd(b) ← T
 */
inline int AVR8InstructionSet::inst_BLD_Rd_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BLD_Rd_b);

    // Operands
    const unsigned int maskb  = (0x01 << (opCode & 0x0007));
    const unsigned int addrRd = ((opCode & 0x01f0) >> 4);

    // Registers
    unsigned int valRd = m_dataMemory->read(addrRd);
    const unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    if ( sreg & AVR8RegNames::SREG_T )
    {
        valRd |= maskb;
    }
    else
    {
        valRd &= maskb;
    }

    // Write result
    m_dataMemory->write(addrRd, valRd);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1111 101d dddd 0bbb - Bit Store from Bit in Register to T Flag in SREG.
 * Operation: T ← Rd(b)
 */
inline int AVR8InstructionSet::inst_BST_Rd_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_BST_Rd_b);

    // Operands
    const unsigned int maskb  = (0x01 << (opCode & 0x0007));
    const unsigned int addrRd = ((opCode & 0x01f0) >> 4);

    // Registers
    const unsigned int valRd = m_dataMemory->read(addrRd);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    // Perform the operation
    if ( valRd & maskb )
    {
        sreg |= AVR8RegNames::SREG_T;
    }
    else
    {
        sreg &= AVR8RegNames::SREG_T;
    }

    // Write result
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 1010 AAAA Abbb - Set Bit in I/O Register
 * Operation: I/O(A,b) ← 1
 */
inline int AVR8InstructionSet::inst_SBI_A_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_SBI_A_b);

    // Operands
    const unsigned int addrA = 0x20 + ((opCode & 0x00f8) >> 3);
    const unsigned int maskb = (0x01 << (opCode & 0x0007));

    // Registers
    unsigned int valA = m_dataMemory->read(addrA);

    // Clear the bit in (A)
    valA |= maskb;

    // Write result
    m_dataMemory->write(addrA, valA);

    // Evaluate how many cycles it takes to execute
    if ( (MCUSim::FAMILY_XMEGA == m_config.m_family) || (MCUSim::FAMILY_TINYAVR /* Reduced code tinyAVR */ == m_config.m_family) )
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

/*
 * OP Code: 1001 1000 AAAA Abbb - Clear Bit in I/O Register
 * Operation: I/O(A,b) ← 0
 */
inline int AVR8InstructionSet::inst_CBI_A_b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_CBI_A_b);

    // Operands
    const unsigned int addrA = 0x20 + ((opCode & 0x00f8) >> 3);
    const unsigned int maskb = (0x01 << (opCode & 0x0007));

    // Registers
    unsigned int valA = m_dataMemory->read(addrA);

    // Clear the bit in (A)
    valA &= ~maskb;

    // Write result
    m_dataMemory->write(addrA, valA);

    // Evaluate how many cycles it takes to execute
    if ( (MCUSim::FAMILY_XMEGA == m_config.m_family) || (MCUSim::FAMILY_TINYAVR /* Reduced code tinyAVR */ == m_config.m_family) )
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

/*
 * OP Code: 1001 001r rrrr 0101 - Load And Set
 * Operation: (Z) ← Rd v (Z), Rd ← (Z) ??? obscure ???
 */
int AVR8InstructionSet::inst_LAS_Z_Rd ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_LAS] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_LAS);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_LAS_Z_Rd);

    // Operands
    const unsigned int addrRr = ((opCode & 0x01f0) >> 4);

    // Registers
    unsigned int addrZ = m_dataMemory->read(AVR8RegNames::ZH);
    addrZ <<= 8;
    addrZ |= m_dataMemory->read(AVR8RegNames::ZL);
    unsigned int valZ = m_dataMemory->read(addrZ);
    unsigned int valRr = m_dataMemory->read(addrRr);

    // Perform the operation
    valRr |= valZ;

    // Write results
    m_dataMemory->write(addrRr, valZ);      // Exchanged with Z
    m_dataMemory->write(addrZ, valRr);      // Exchanged with modfied Rr

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 001r rrrr 0110 - Load And Clear
 * Operation: (Z) ← Rd • ($FF – (Z)) ??? what is that ???
 */
int AVR8InstructionSet::inst_LAC_Z_Rd ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_LAC] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_LAC);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_LAC_Z_Rd);

    // Operands
    const unsigned int addrRr = ((opCode & 0x01f0) >> 4);

    // Registers
    unsigned int addrZ = m_dataMemory->read(AVR8RegNames::ZH);
    addrZ <<= 8;
    addrZ |= m_dataMemory->read(AVR8RegNames::ZL);
    unsigned int valZ = m_dataMemory->read(addrZ);
    unsigned int valRr = m_dataMemory->read(addrRr);

    // Perform the operation
    valRr ^= 0xff;
    valRr &= valZ;

    // Write results
    m_dataMemory->write(addrRr, valZ);      // Exchanged with Z
    m_dataMemory->write(addrZ, valRr);      // Exchanged with modfied Rr

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1001 001r rrrr 0111 - Load And Toggle
 * Operation: (Z) ← Rd ⊕ (Z), Rd ← (Z) ??? obscure ???
 */
int AVR8InstructionSet::inst_LAT_Z_Rd ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_LAT] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_LAT);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_LAT_Z_Rd);

    // Operands
    const unsigned int addrRr = ((opCode & 0x01f0) >> 4);

    // Registers
    unsigned int addrZ = m_dataMemory->read(AVR8RegNames::ZH);
    addrZ <<= 8;
    addrZ |= m_dataMemory->read(AVR8RegNames::ZL);
    unsigned int valZ = m_dataMemory->read(addrZ);
    unsigned int valRr = m_dataMemory->read(addrRr);

    // Perform the operation
    valRr ^= valZ;

    // Write results
    m_dataMemory->write(addrRr, valZ);      // Exchanged with Z
    m_dataMemory->write(addrZ, valRr);      // Exchanged with modfied Rr

    // This takes one cycle to execute
    return 1;
}

template <int instruction, unsigned int pointerRegH, unsigned int pointerRegL, bool preDecrement, bool postIncrement>
    inline int AVR8InstructionSet::instLD ( unsigned int addrRd,
                                            int displacement )
{
    if ( false == m_config.m_availableInstructions[instruction] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, instruction);
        return -1;
    }

    bool accessToProgMemory = false; // TODO: Implement it (see page 89)
    bool accessToISRAM = false; // TODO: Implement it (see page 89)

    // Get 16b address pointed by the pointer register
    unsigned int pointer = m_dataMemory->read(pointerRegH);
    pointer <<= 8;
    pointer |= m_dataMemory->read(pointerRegL);

    // Conditional pre-decrement
    if ( true == preDecrement )
    {
        pointer--;
        pointer &= 0xffff;      // Preserve 16b value in case of underflow
    }

    // Use displacement
    if ( 0 != displacement )
    {
        if ( 0x40 & displacement )
        {
            // Convert 6b negative signed integer to any size negative int.
            displacement -= 0x80;
        }

        // Alter pointer
        pointer += displacement;
        pointer &= 0xffff;      // Preserve 16b value in case of under/over-flow
    }

    // Get value pointed by the pointer
    unsigned int pointedVal = m_dataMemory->read(pointer);

    // Conditional post-decrement
    if ( true == postIncrement )
    {
        pointer++;
        pointer &= 0xffff;      // Preserve 16b value in case of overflow
    }

    if ( true == preDecrement || true == postIncrement )
    {
        // Check validity of Rd address
        if ( pointerRegH == addrRd || pointerRegL == addrRd )
        {
            // Invalid address, generate random result
            logEvent(EVENT_CPU_ERR_INVALID_OPSET, m_pc, instruction);

            // Genereate fake results
            pointedVal = (0xff & m_programMemory->getUndefVal());
            pointer = m_programMemory->getUndefVal();
        }

        // Write back updated pointer
        m_dataMemory->write(pointerRegH, (pointer >> 8));
        m_dataMemory->write(pointerRegL, (pointer & 0xff));
    }

    // Write *pointer to Rd
    m_dataMemory->write(addrRd, pointedVal);

    // Determinate how many cycles this takes
    int cycles;
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        if ( true == postIncrement )
        {
            cycles = 1; // (ii.)
        }
        else if ( true == preDecrement )
        {
            cycles = 2; // (iii.)
        }
        else
        {
            cycles = 1; // (i.)
        }

        if ( true == accessToISRAM )
        {
            cycles++;
        }
    }
    else
    {
        if ( true == postIncrement )
        {
            cycles = 2; // (ii.)
        }
        else if ( true == preDecrement )
        {
            cycles = 2; // (iii.)

            if ( true == accessToProgMemory )
            {
                cycles++;
                // TODO: But if an interrupt occur (before the last clock cycle) no additional clock cycles is necessary when loading from the program memory. (see page 89)
            }
        }
        else
        {
            cycles = 1; // (i.)

            if ( true == accessToProgMemory )
            {
                cycles++;
                // TODO: But if an interrupt occur (before the last clock cycle) no additional clock cycles is necessary when loading from the program memory. (see page 89)
            }
        }
    }

    if ( 0 != displacement )
    {
        cycles++; // see instruction set specification for details ...
    }
    return cycles;
}

/*
 * OP Code: 1001 000d dddd 1100 - Load Indirect from Data Space to Register using Index X
 * Operation: Rd ← (X)
 */
int AVR8InstructionSet::inst_LD_Rd_X ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_X);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_X, AVR8RegNames::XH, AVR8RegNames::XL, false, false>(addrRd);
}

/*
 * OP Code: 1001 000d dddd 1101 - Load Indirect from Data Space to Register using Index X
 * Operation: Rd ← (X), X ← X + 1
 */
int AVR8InstructionSet::inst_LD_Rd_Xplus ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_Xplus);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_Xplus, AVR8RegNames::XH, AVR8RegNames::XL, false, true>(addrRd);
}

/*
 * OP Code: 1001 000d dddd 1110 - Load Indirect from Data Space to Register using Index X
 * Operation: X ← X - 1, Rd ← (X)
 */
int AVR8InstructionSet::inst_LD_Rd_minusX ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_minusX);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_minusX, AVR8RegNames::XH, AVR8RegNames::XL, true, false>(addrRd);
}

/*
 * OP Code: 1001 000d dddd 1001 - Load Indirect from Data Space to Register using Index Y
 * Operation: Rd ← (Y), Y ← Y + 1
 */
int AVR8InstructionSet::inst_LD_Rd_Yplus ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_Yplus);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_Yplus, AVR8RegNames::YH, AVR8RegNames::YL, false, true>(addrRd);
}

/*
 * OP Code: 1001 000d dddd 1010 - Load Indirect from Data Space to Register using Index Y
 * Operation: Y ← Y - 1, Rd ← (Y)
 */
int AVR8InstructionSet::inst_LD_Rd_minusY ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_minusY);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_minusY, AVR8RegNames::YH, AVR8RegNames::YL, true, false>(addrRd);
}

/*
 * OP Code: 10q0 qq0d dddd 1qqq - Load Indirect from Data Space to Register using Index Y
 * Operation: Rd ← (Y+q)
 */
inline int AVR8InstructionSet::inst_LDD_Rd_Yq ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LDD_Rd_Yq);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;
    const unsigned int valq   = ((opCode & 0x01f0) >> 8) | ((opCode & 0x0c00) >> 7) | (opCode & 0x0007);

    // Perform the operation
    if ( 0 == valq )
    {
        return instLD<AVR8InsNames::SPECI_LDD_Rd_Y, AVR8RegNames::YH, AVR8RegNames::YL, false, false>(addrRd);
    }
    else
    {
        // a different instruction ('AVR8InsNames::SPECI_LDD_Rd_Y' is not the same as 'AVR8InsNames::SPECI_LDD_Rd_Yq')
        return instLD<AVR8InsNames::SPECI_LDD_Rd_Yq, AVR8RegNames::YH, AVR8RegNames::YL, false, false>(addrRd, valq);
    }
}

/*
 * OP Code: 1001 000d dddd 0001 - Load Indirect From Data Space to Register using Index Z
 * Operation: Rd ← (Z), Z ← Z + 1
 */
int AVR8InstructionSet::inst_LD_Rd_Zplus ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_Zplus);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_Zplus, AVR8RegNames::ZH, AVR8RegNames::ZL, false, true>(addrRd);
}

/*
 * OP Code: 1001 000d dddd 0010 - Load Indirect From Data Space to Register using Index Z
 * Operation: Z ← Z - 1, Rd ← (Z)
 */
int AVR8InstructionSet::inst_LD_Rd_minusZ ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LD_Rd_minusZ);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instLD<AVR8InsNames::SPECI_LD_Rd_minusZ, AVR8RegNames::ZH, AVR8RegNames::ZL, true, false>(addrRd);
}

/*
 * OP Code: 10q0 qq0d dddd 0qqq - Load Indirect From Data Space to Register using Index Z
 * Operation: Rd ← (Z+q)
 */
inline int AVR8InstructionSet::inst_LDD_Rd_Zq ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LDD_Rd_Zq);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;
    const unsigned int valq   = ((opCode & 0x01f0) >> 8) | ((opCode & 0x0c00) >> 7) | (opCode & 0x0007);

    // Perform the operation
    if ( 0 == valq )
    {
        return instLD<AVR8InsNames::SPECI_LDD_Rd_Z, AVR8RegNames::ZH, AVR8RegNames::ZL, false, false>(addrRd);
    }
    else
    {
        return instLD<AVR8InsNames::SPECI_LDD_Rd_Zq, AVR8RegNames::ZH, AVR8RegNames::ZL, false, false>(addrRd, valq);
    }
}

/*
 * OP Code: 1001 11rd dddd rrrr - Multiply Unsigned
 * Operation: R1:R0 ← Rd × Rr (unsigned ← unsigned × unsigned)
 */
int AVR8InstructionSet::inst_MUL_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_MUL] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_MUL);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_MUL_Rd_Rr);

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;
    const unsigned int addrRr = ((opCode & 0x0200) >> 5) | (opCode & 0x000f);

    // Perform the operation
    instMul<false, false, false>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 0000 0010 dddd rrrr - Multiply Signed
 * Operation: R1:R0 ← Rd × Rr (signed ← signed × signed)
 */
inline int AVR8InstructionSet::inst_MULS_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_MULS] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_MULS);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_MULS_Rd_Rr);

    // Operands
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4) | 0x10;    // 16 ≤ d ≤ 31
    const unsigned int addrRr = (opCode & 0x000f) | 0x10;           // 16 ≤ r ≤ 31

    // Perform the operation
    instMul<true, true, false>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 0000 0011 0ddd 0rrr - Multiply Signed with Unsigned
 * Operation: R1:R0 ← Rd × Rr (signed ← signed × unsigned)
 */
inline int AVR8InstructionSet::inst_MULSU_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_MULSU] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_MULSU);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_MULSU_Rd_Rr);

    // Operands
    const unsigned int addrRd = ((opCode & 0x0070) >> 4) | 0x10;    // 16 ≤ d ≤ 23
    const unsigned int addrRr = (opCode & 0x0007) | 0x10;           // 16 ≤ r ≤ 23

    // Perform the operation
    instMul<true, false, false>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 0000 0011 0ddd 1rrr - Fractional Multiply Unsigned
 * Operation: R1:R0 ← Rd × Rr (unsigned (1.15) ← unsigned (1.7) × unsigned (1.7))
 *
 * This instruction performs the same operation as "MUL Rd, Rr" does, except for
 * that the result is left-shifted by one bit. This might seem strange but, in
 * fact, it's perfectly correct as long as the result fits in range [0,2>.
 */
inline int AVR8InstructionSet::inst_FMUL_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_FMUL] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_FMUL);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_FMUL_Rd_Rr);

    // Operands
    const unsigned int addrRd = ((opCode & 0x0070) >> 4) | 0x10;    // 16 ≤ d ≤ 23
    const unsigned int addrRr = (opCode & 0x0007) | 0x10;           // 16 ≤ r ≤ 23

    // Perform the operation
    instMul<false, false, true>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 0000 0011 1ddd 0rrr - Fractional Multiply Signed
 * Operation: R1:R0 ← Rd × Rr (signed (1.15) ← signed (1.7) × signed (1.7))
 *
 * This instruction performs the same operation as "FMUL Rd, Rr" does, except for
 * that the multiplicand and the multiplier undergo sign extension. Note that the
 * result fits in range [-1,1>, and -1 × -1 causes two's complement overflow,
 * thus produces an incorrect result (-1).
 */
inline int AVR8InstructionSet::inst_FMULS_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_FMULS] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_FMULS);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_FMULS_Rd_Rr);

    // Operands
    const unsigned int addrRd = ((opCode & 0x0070) >> 4) | 0x10;    // 16 ≤ d ≤ 23
    const unsigned int addrRr = (opCode & 0x0007) | 0x10;           // 16 ≤ r ≤ 23

    // Perform the operation
    instMul<true, true, true>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

/*
 * OP Code: 0000 0011 1ddd 1rrr - Fractional Multiply Signed with Unsigned
 * Operation: R1:R0 ← Rd × Rr (signed (1.15) ← signed (1.7) × unsigned (1.7))
 *
 * This instruction performs the same operation as "FMULS Rd, Rr" does, except for
 * that the multiplier does not undergo sign extension. Note that the result has to
 * fit in range [-1,1>, and so the operation might easily cause two's complement
 * overflow and thus produce an obscured result.
 */
inline int AVR8InstructionSet::inst_FMULSU_Rd_Rr ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_FMULSU] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_FMULSU);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_FMULSU_Rd_Rr);

    // Operands
    const unsigned int addrRd = ((opCode & 0x0070) >> 4) | 0x10;    // 16 ≤ d ≤ 23
    const unsigned int addrRr = (opCode & 0x0007) | 0x10;           // 16 ≤ r ≤ 23

    // Perform the operation
    instMul<true, false, true>(addrRd, addrRr);

    // This takes two cycles to execute
    return 2;
}

template<bool rdSigned, bool rrSigned, bool fractional>
    inline void AVR8InstructionSet::instMul ( const unsigned int addrRd,
                                              const unsigned int addrRr )
{
    // Registers
    int valRd = m_dataMemory->read(addrRd);
    int valRr = m_dataMemory->read(addrRr);
    register unsigned int sreg = m_dataMemory->readFast(AVR8RegNames::SREG);

    if ( true == rdSigned )
    {
        if ( 0x80 & valRd )
        {
            // Convert 8b negative signed integer to any size negative int.
            valRd -= 0x100;
        }
    }
    if ( true == rrSigned )
    {
        if ( 0x80 & valRr )
        {
            // Convert 8b negative signed integer to any size negative int.
            valRr -= 0x100;
        }
    }

    // Perform the operation (16b result is required even for signed multiplication)
    unsigned int result = 0xffff & signed(valRd * valRr);

    // Clear only Z and C status flags
    sreg &= ~( AVR8RegNames::SREG_Z | AVR8RegNames::SREG_C );

    // Evaluate status sreg
    if ( 0x8000 & result )
    {
        if ( true == fractional )
        {
            /*
             * Clear MSB, if set.
             *
             * Reason: later, we will shift the value by 1b left, so we have
             * to make sure that it won't exceed 16b boundary after the shift.
             */
            result &= 0x7fff;
        }
        sreg |= AVR8RegNames::SREG_C; // C ← 1
    }
    if ( 0 == result )
    {
        sreg |= AVR8RegNames::SREG_Z; // Z ← 1
    }
    if ( true == fractional )
    {
        // Left-shift the result by 1 bit, see "AVR201: Using the AVR® Hardware Multiplier" for details.
        result <<= 1;
    }

    // Write results
    m_dataMemory->writeFast(AVR8RegNames::SREG, sreg);
    m_dataMemory->writeFast(AVR8RegNames::R1, (result >> 8));       // The result - high byte
    m_dataMemory->writeFast(AVR8RegNames::R0, (result & 0xff));     // The result - low byte
}

template<int instruction, bool store>
    inline int AVR8InstructionSet::instLoadOrStore32b ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[instruction] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, instruction);
        return -1;
    }

    // Operands
    const unsigned int addrRd = (opCode & 0x01f0) >> 4;
    unsigned int addrK = m_programMemory->read(m_pc);
    incrPc(); // We have read one more word from the program memory

    if ( m_dataMemory->size() > 0xffff )
    {
        // Part with more than 64K of data memory
        addrK |= (m_dataMemory->read(AVR8RegNames::RAMPD) << 16);
    }

    // Perform the operation
    if ( true == store )
    {
        // Store
        m_dataMemory->write(addrK, m_dataMemory->read(addrRd));

        // This takes two cycles to execute
        return 2;
    }
    else
    {
        // Load
        m_dataMemory->write(addrRd, m_dataMemory->read(addrK));

        // Determinate how many cycles this takes to execute
        if ( (MCUSim::FAMILY_XMEGA == m_config.m_family) && (addrK >= 0x20) )
        {
            // XMEGA: 2 If the LDS instruction is accessing internal SRAM, one extra cycle is inserted.
            return 3;
        }
        else
        {
            return 2;
        }
    }
}

/*
 * OP Code: 1001 000d dddd 0000 \
 *          kkkk kkkk kkkk kkkk - Load Direct from Data Space (LDS Rd, k) (32b version)
 * Operation: Rd ← (k)
 */
int AVR8InstructionSet::inst_LDS_Rd_k ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LDS_Rd_k);
    return instLoadOrStore32b<AVR8InsNames::SPECI_LDS_Rd_k, false>(opCode);
}

/*
 * OP Code: 1001 001d dddd 0000 \
 *          kkkk kkkk kkkk kkkk - Store Direct to Data Space (STS k, Rr) (32b version)
 * Operation: (k) ← Rd
 */
int AVR8InstructionSet::inst_STS_k_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_STS_k_Rr);
    return instLoadOrStore32b<AVR8InsNames::SPECI_LDS_Rd_k, true>(opCode);
}

template<int instruction, bool store>
    inline int AVR8InstructionSet::instLoadOrStore16b ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[instruction] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, instruction);
        return -1;
    }

    /*
     * The address given in the instruction is coded to a data space address as follows:
     * ADDR[7:0] = ( ¬INST[8], INST[8], INST[10], INST[9], INST[3], INST[2], INST[1], INST[0] )
     */
    unsigned int addrK = ( ( opCode & 0x0100 ) ? 0x40 : 0x80 ) | // ( ¬INST[8], INST[8] ...
                         ( ( opCode & 0x0400 ) ? 0x20 : 0x00 ) | // ... INST[10] ...
                         ( ( opCode & 0x0200 ) ? 0x10 : 0x00 ) | // ... INST[9] ...
                           ( opCode & 0x000f );                  // ... INST[3], INST[2], INST[1], INST[0] )

    /*
     * 16 ≤ d ≤ 31, but registers r0..r15 are remapped to r16..r31.
     */
    const unsigned int addrRd = ((opCode & 0x00f0) >> 4);


    // Perform the operation
    if ( true == store )
    {
        // Store
        m_dataMemory->write(addrK, m_dataMemory->read(addrRd));
    }
    else
    {
        // Load
        m_dataMemory->write(addrRd, m_dataMemory->read(addrK));
    }

    // This takes one cycle to execute
    return 1;
}

/*
 * OP Code: 1010 0kkk dddd kkkk - Load Direct from Data Space (LDS Rd, k) (16b version)
 * Operation: Rd ← (k)
 */
inline int AVR8InstructionSet::inst_LDS_Rd_k_16b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_LDS_Rd_k_16b);
    return instLoadOrStore16b<AVR8InsNames::SPECI_LDS_Rd_k_16b, false>(opCode);
}

/*
 * OP Code: 1010 1kkk dddd kkkk - Store Direct to Data Space (STS k, Rr) (16b version)
 * Operation: (k) ← Rd
 */
inline int AVR8InstructionSet::inst_STS_k_Rr_16b ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_STS_k_Rr_16b);
    return instLoadOrStore16b<AVR8InsNames::SPECI_STS_k_Rr_16bb, true>(opCode);
}

template <int instruction, unsigned int pointerRegH, unsigned int pointerRegL, bool preDecrement, bool postIncrement>
    inline int AVR8InstructionSet::instST ( unsigned int addrRr,
                                            int displacement )
{
    if ( false == m_config.m_availableInstructions[instruction] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, instruction);
        return -1;
    }

    // Get 16b address pointed by the pointer register
    unsigned int pointer = m_dataMemory->read(pointerRegH);
    pointer <<= 8;
    pointer |= m_dataMemory->read(pointerRegL);

    // Conditional pre-decrement
    if ( true == preDecrement )
    {
        pointer--;
        pointer &= 0xffff;      // Preserve 16b value in case of underflow
    }

    // Use displacement
    if ( 0 != displacement )
    {
        if ( 0x40 & displacement )
        {
            // Convert 6b negative signed integer to any size negative int.
            displacement -= 0x80;
        }

        // Alter pointer
        pointer += displacement;
        pointer &= 0xffff;      // Preserve 16b value in case of under/over-flow
    }

    // Get value of Rd
    unsigned int valRd = m_dataMemory->read(addrRr);

    // Conditional post-decrement
    if ( true == postIncrement )
    {
        pointer++;
        pointer &= 0xffff;      // Preserve 16b value in case of overflow
    }

    if ( true == preDecrement || true == postIncrement )
    {
        // Check validity of Rd address
        if ( pointerRegH == addrRr || pointerRegL == addrRr )
        {
            // Invalid address, generate random result
            logEvent(EVENT_CPU_ERR_INVALID_OPSET, m_pc, instruction);

            // Genereate fake results
            valRd   = (0xff & m_programMemory->getUndefVal());
            pointer = m_programMemory->getUndefVal();
        }

        // Write back updated pointer
        m_dataMemory->write(pointerRegH, (pointer >> 8));
        m_dataMemory->write(pointerRegL, (pointer & 0xff));
    }

    // Write Rd to *pointer
    m_dataMemory->write(pointer, valRd);

    // Determinate how many cycles this takes
    if ( MCUSim::FAMILY_XMEGA == m_config.m_family )
    {
        if ( (true == preDecrement) || (0 != displacement) )
        {
            return 2; // (iii.), (iv.)
        }
        else
        {
            return 1; // (i.), (ii.)
        }
    }
    else if ( MCUSim::FAMILY_TINYAVR /* Reduced code tinyAVR */ == m_config.m_family )
    {
        if ( true == preDecrement )
        {
            return 2; // (iii.), there is no (iv.) on tinyAVR
        }
        else
        {
            return 1; // (i.), (ii.)
        }
    }
    else
    {
        return 2; // megaAVR
    }
}

/*
 * Variant A)
 *      OP Code: 10q0 qq1r rrrr 0qqq - Store Indirect From Register to Data Space using Index Z (STD Z+q, Rr)
 *      Operation: (Z+q) ← Rr
 * Variant B)
 *      OP Code: 1000 001r rrrr 0000 - Store Indirect From Register to Data Space using Index Z (ST Z, Rr)
 *      Operation: (Z) ← Rr
 */
inline int AVR8InstructionSet::inst_STD_Zq_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_STD_Zq_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;
    const unsigned int valq   = ((opCode & 0x01f0) >> 8) | ((opCode & 0x0c00) >> 7) | (opCode & 0x0007);

    // Perform the operation
    if ( 0 == valq )
    {
        return instST<AVR8InsNames::SPECI_STD_Rr_Z, AVR8RegNames::ZH, AVR8RegNames::ZL, false, false>(addrRr);
    }
    else
    {
        // a different instruction ('AVR8InsNames::SPECI_STD_Rr_Z' is not the same as 'AVR8InsNames::SPECI_STD_Rr_Zq')
        return instST<AVR8InsNames::SPECI_STD_Rr_Zq, AVR8RegNames::ZH, AVR8RegNames::ZL, false, false>(addrRr, valq);
    }
}

/*
 * Variant A)
 *      OP Code: 10q0 qq1r rrrr 1qqq - Store Indirect From Register to Data Space using Index Y (STD Y+q, Rr)
 *      Operation: (Y+q) ← Rr
 * Variant B)
 *      OP Code: 1000 001r rrrr 1000 - Store Indirect From Register to Data Space using Index Y (ST Y, Rr)
 *      Operation: (Y) ← Rr
 */
inline int AVR8InstructionSet::inst_STD_Yq_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_STD_Yq_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;
    const unsigned int valq   = ((opCode & 0x01f0) >> 8) | ((opCode & 0x0c00) >> 7) | (opCode & 0x0007);

    // Perform the operation
    if ( 0 == valq )
    {
        return instST<AVR8InsNames::SPECI_STD_Rr_Y, AVR8RegNames::YH, AVR8RegNames::YL, false, false>(addrRr);
    }
    else
    {
        // a different instruction ('AVR8InsNames::SPECI_STD_Rr_Z' is not the same as 'AVR8InsNames::SPECI_STD_Rr_Zq')
        return instST<AVR8InsNames::SPECI_STD_Rr_Yq, AVR8RegNames::YH, AVR8RegNames::YL, false, false>(addrRr, valq);
    }
}

/*
 * OP Code: 1001 001r rrrr 0001 - Store Indirect From Register to Data Space using Index Z (ST Z+, Rr)
 * Operation: (Z) ← Rr, Z ← Z + 1
 */
int AVR8InstructionSet::inst_ST_Zplus_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_Zplus_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_Zplus, AVR8RegNames::ZH, AVR8RegNames::ZL, false, true>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 0010 - Store Indirect From Register to Data Space using Index Z (ST -Z, Rr)
 * Operation: Z ← Z - 1, (Z) ← Rr
 */
int AVR8InstructionSet::inst_ST_minusZ_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_minusZ_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_minusZ, AVR8RegNames::ZH, AVR8RegNames::ZL, true, false>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 1001 - Store Indirect From Register to Data Space using Index Y (ST Y+, Rr)
 * Operation: (Y) ← Rr, Y ← Y + 1
 */
int AVR8InstructionSet::inst_ST_Yplus_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_Yplus_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_Yplus, AVR8RegNames::YH, AVR8RegNames::YL, false, true>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 1010 - Store Indirect From Register to Data Space using Index Y (ST -Y, Rr)
 * Operation: Y ← Y - 1, (Y) ← Rr
 */
int AVR8InstructionSet::inst_ST_minusY_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_minusY_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_minusY, AVR8RegNames::YH, AVR8RegNames::YL, true, false>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 1100 - Store Indirect From Register to Data Space using Index X (ST X, Rr)
 * Operation: (X) ← Rr
 */
int AVR8InstructionSet::inst_ST_X_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_X_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_X, AVR8RegNames::XH, AVR8RegNames::XL, false, false>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 1101 - Store Indirect From Register to Data Space using Index X (ST X+, Rr)
 * Operation: (X) ← Rr, X ← X + 1
 */
int AVR8InstructionSet::inst_ST_Xplus_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_Xplus_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_Xplus, AVR8RegNames::XH, AVR8RegNames::XL, false, true>(addrRr);
}

/*
 * OP Code: 1001 001r rrrr 1110 - Store Indirect From Register to Data Space using Index X (ST -X, Rr)
 * Operation: X ← X - 1, (X) ← Rr
 */
int AVR8InstructionSet::inst_ST_minusX_Rr ( const unsigned int opCode )
{
    instructionEnter(AVR8InsNames::INS_ST_minusX_Rr);

    // Operands
    const unsigned int addrRr = (opCode & 0x01f0) >> 4;

    // Perform the operation
    return instST<AVR8InsNames::SPECI_ST_Rr_minusX, AVR8RegNames::XH, AVR8RegNames::XL, true, false>(addrRr);
}

/*
 * OP Code: 1001 0101 1110 1000 - Store Program Memory (SPM)
 * Operation: Write to program memory (device specific)
 */
int AVR8InstructionSet::inst_SPM ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_SPM] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::INS_SPM);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_SPM);

    unsigned int addr, val;

    addr = m_dataMemory->read(AVR8RegNames::ZH);
    addr <<= 8;
    addr |= m_dataMemory->read(AVR8RegNames::ZL);

    val = m_dataMemory->read(AVR8RegNames::R1);
    val <<= 8;
    val |= m_dataMemory->read(AVR8RegNames::R0);

    return m_bootLoader->spmWrite(addr, val);
}

/*
 * OP Code: 1001 0101 1111 1000 - Store Program Memory (SPM Z+)
 * Operation: Write to program memory (device specific)
 */
int AVR8InstructionSet::inst_SPM_Zplus ( const unsigned int )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_SPM_Zplus] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::INS_SPM_Zplus);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_SPM_Zplus);

    int cycles;
    unsigned int addr, val;

    addr = m_dataMemory->read(AVR8RegNames::ZH);
    addr <<= 8;
    addr |= m_dataMemory->read(AVR8RegNames::ZL);

    val = m_dataMemory->read(AVR8RegNames::R1);
    val <<= 8;
    val |= m_dataMemory->read(AVR8RegNames::R0);

    cycles = m_bootLoader->spmWrite(addr, val);

    addr++;
    addr &= 0xffff;
    m_dataMemory->write(AVR8RegNames::ZL, (addr & 0xff));
    addr >>= 8;
    m_dataMemory->write(AVR8RegNames::ZH, addr);

    return cycles;
}

/*
 * OP Code: 1001 0100 KKKK 1011 - Data Encryption Standard
 * Operation:
 *   If H = 0 then Encrypt round (R7-R0, R15-R8, K)
 *   If H = 1 then Decrypt round (R7-R0, R15-R8, K)
 */
inline int AVR8InstructionSet::inst_DES_K ( const unsigned int opCode )
{
    if ( false == m_config.m_availableInstructions[AVR8InsNames::SPECI_DES] )
    {
        logEvent(EVENT_CPU_UNSUPPORTED_INST, m_pc, AVR8InsNames::SPECI_DES);
        return -1;
    }
    instructionEnter(AVR8InsNames::INS_DES);

    // ---------------------------------------------------------------------
    // Read inputs
    // ---------------------------------------------------------------------

    // Number of enciphering or deciphering round
    const unsigned int round = ((opCode & 0x00f0) >> 4);    // operand 'k'

    // Flag: decipher = true, encipher = false
    bool decipherFlag = m_dataMemory->readBitFast(AVR8RegNames::SREG, AVR8RegNames::SREG_H);

    // 64b data block (plaintext or ciphertext)
    uint64_t dataBlock = 0;
    for ( int i = AVR8RegNames::R7; i >= AVR8RegNames::R0; i-- )
    {
        dataBlock <<= 8;
        dataBlock |= m_dataMemory->readFast(i);
    }

    // Full 64b key block (56b effective, 8b parity)
    uint64_t keyBlock = 0;
    for ( int i = AVR8RegNames::R15; i >= AVR8RegNames::R8; i-- )
    {
        keyBlock <<= 8;
        keyBlock |= m_dataMemory->readFast(i);
    }

    // ---------------------------------------------------------------------
    // Encipher / decipher
    // ---------------------------------------------------------------------

    m_des->cipher(dataBlock, keyBlock, round, decipherFlag);

    // ---------------------------------------------------------------------
    // Write outputs
    // ---------------------------------------------------------------------

    // 64b data block (plaintext or ciphertext)
    for ( int i = AVR8RegNames::R0; i <= AVR8RegNames::R7; i++ )
    {
        m_dataMemory->writeFast(i, dataBlock & 0xff);
        dataBlock >>= 8;
    }

    // Full 64b key block (56b effective, 8b parity)
    for ( int i = AVR8RegNames::R8; i <= AVR8RegNames::R15; i++ )
    {
        m_dataMemory->writeFast(i, keyBlock & 0xff);
        keyBlock >>= 8;
    }

    // If the DES instruction is succeeding a non-DES instruction, an extra cycle is inserted.
    return ( ( AVR8InsNames::INS_DES == m_lastInstruction ) ? 1 : 2 );
}

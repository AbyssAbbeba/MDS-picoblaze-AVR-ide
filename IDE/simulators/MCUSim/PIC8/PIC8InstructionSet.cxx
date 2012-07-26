/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */


#include "PIC8InstructionSet.h"
#include "PIC8ProgramMemory.h"
#include "PIC8DataMemory.h"
#include "PIC8FusesAndLocks.h"
#include "PIC8InterruptController.h"
#include "PIC8SystemControl.h"
#include "PIC8BootLoader.h"

int (PIC8InstructionSet:: * const PIC8InstructionSet::m_opCodeDispatchTable[64])(const unsigned int opCode) = {
	&PIC8InstructionSet::instOPCode_000000,	// opCode = 00 0000 xxxx xxxx
	&PIC8InstructionSet::instOPCode_000001,	// opCode = 00 0001 xxxx xxxx
	&PIC8InstructionSet::inst_SUBWF,	// opCode = 00 0010 xxxx xxxx
	&PIC8InstructionSet::inst_DECF,		// opCode = 00 0011 xxxx xxxx
	&PIC8InstructionSet::inst_IORWF,	// opCode = 00 0100 xxxx xxxx
	&PIC8InstructionSet::inst_ANDWF,	// opCode = 00 0101 xxxx xxxx
	&PIC8InstructionSet::inst_XORWF,	// opCode = 00 0110 xxxx xxxx
	&PIC8InstructionSet::inst_ADDWF,	// opCode = 00 0111 xxxx xxxx
	&PIC8InstructionSet::inst_MOVF,		// opCode = 00 1000 xxxx xxxx
	&PIC8InstructionSet::inst_COMF,		// opCode = 00 1001 xxxx xxxx
	&PIC8InstructionSet::inst_INCF,		// opCode = 00 1010 xxxx xxxx
	&PIC8InstructionSet::inst_DECFSZ,	// opCode = 00 1011 xxxx xxxx
	&PIC8InstructionSet::inst_RRF,		// opCode = 00 1100 xxxx xxxx
	&PIC8InstructionSet::inst_RLF,		// opCode = 00 1101 xxxx xxxx
	&PIC8InstructionSet::inst_SWAPF,	// opCode = 00 1110 xxxx xxxx
	&PIC8InstructionSet::inst_INCFSZ,	// opCode = 00 1111 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0000 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0001 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0010 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0011 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0100 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0101 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0110 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 0111 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1000 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1001 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1010 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1011 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1100 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1101 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1110 xxxx xxxx
	&PIC8InstructionSet::instOPCode_01,	// opCode = 01 1111 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0000 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0001 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0010 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0011 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0100 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0101 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0110 xxxx xxxx
	&PIC8InstructionSet::inst_CALL,		// opCode = 10 0111 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1000 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1001 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1010 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1011 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1100 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1101 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1110 xxxx xxxx
	&PIC8InstructionSet::inst_GOTO,		// opCode = 10 1111 xxxx xxxx
	&PIC8InstructionSet::inst_MOVLW,	// opCode = 11 0000 xxxx xxxx
	&PIC8InstructionSet::inst_MOVLW,	// opCode = 11 0001 xxxx xxxx
	&PIC8InstructionSet::inst_MOVLW,	// opCode = 11 0010 xxxx xxxx
	&PIC8InstructionSet::inst_MOVLW,	// opCode = 11 0011 xxxx xxxx
	&PIC8InstructionSet::inst_RETLW,	// opCode = 11 0100 xxxx xxxx
	&PIC8InstructionSet::RETLW,		// opCode = 11 0101 xxxx xxxx
	&PIC8InstructionSet::inst_RETLW,	// opCode = 11 0110 xxxx xxxx
	&PIC8InstructionSet::inst_RETLW,	// opCode = 11 0111 xxxx xxxx
	&PIC8InstructionSet::inst_IORLW,	// opCode = 11 1000 xxxx xxxx
	&PIC8InstructionSet::inst_ANDLW,	// opCode = 11 1001 xxxx xxxx
	&PIC8InstructionSet::inst_XORLW,	// opCode = 11 1010 xxxx xxxx
	//11 1011 xxxx xxxx missing?
	&PIC8InstructionSet::instInvalid,	// opCode = 11 1011 xxxx xxxx
	&PIC8InstructionSet::inst_SUBLW,	// opCode = 11 1100 xxxx xxxx
	&PIC8InstructionSet::inst_SUBLW,	// opCode = 11 1101 xxxx xxxx
	&PIC8InstructionSet::inst_ADDLW,	// opCode = 11 1110 xxxx xxxx
	&PIC8InstructionSet::inst_ADDLW,	// opCode = 11 1111 xxxx xxxx
};



PIC8InstructionSet * PIC8InstructionSet::link(
		MCUSim::EventLogger	* eventLogger,
		PIC8ProgramMemory	* programMemory,
		PIC8DataMemory		* dataMemory,
		MCUSim::Mode		* processorMode,
		PIC8Sim::SleepMode	* sleepMode,
		PIC8FusesAndLocks	* fusesAndLocks,
		PIC8InterruptController	* interruptController,
		PIC8SystemControl	* systemControl,
		PIC8Sim::HaltMode	* haltMode,
		PIC8BootLoader		* bootLoader
) {
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

void PIC8InstructionSet::reset(MCUSim::ResetMode mode) {
	switch ( mode ) {
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

inline void PIC8InstructionSet::mcuReset() {
	if ( false == (*m_fusesAndLocks)[PIC8FusesAndLocks::FUSE_BOOTRST] ) {
		m_pc = 0;
	} else {
		m_pc = m_bootLoader->getBootAddress();
	}
	m_actSubprogCounter = 0;
	m_lastInstruction = PIC8InsNames::INS_NONE;
}

inline void PIC8InstructionSet::resetToInitialValues() {
	mcuReset();

	for ( int i = 0; i < PIC8InsNames::INS__MAX__; i++ ) {
		m_instructionCounter[i] = 0;
	}
}

inline void PIC8InstructionSet::incrPc(const int val) {
	m_pc += val;
	while ( m_pc > m_config.m_pcMax ) {
		m_pc -= m_config.m_pcMax;
		logEvent(EVENT_CPU_PC_OVERFLOW);
	}
	while ( m_pc < 0 ) {
		m_pc += m_config.m_pcMax;
		logEvent(EVENT_CPU_PC_UNDERFLOW);
	}
}

int PIC8InstructionSet::execInstruction() {
	const int pcOrig = m_pc;
	unsigned int opCode = m_programMemory->readRaw(m_pc);
	incrPc();

	if ( opCode & 0xffff0000 ) {
		if ( true == m_config.m_ignoreUndefinedOpCodes ) {
			return -1;
		} else {
			opCode &= 0x0ffff;
		}
	}

	if ( pcOrig != m_pc ) {
		logEvent(EVENT_CPU_PC_CHANGED, m_pc);
	}

	return (this->*(m_opCodeDispatchTable[opCode >> 12]))(opCode);
}

int PIC8InstructionSet::instOPCode_000000(const unsigned int opCode) {
	// opCode = 00 0000 xxxx xxxx
	if ( !(opCode & 0x080) ) {
		// opCode = 00 0000 0xxx xxxx
		if ( opCode & 0x01 ) {
			// opCode = 00 0000 0xxx xxx1
			if ( opCode & 0x02 ) {
				// opCode = 00 0000 0xxx xx11
				if ( 99 == opCode ) {
					// opCode = 00 0000 0110 0011
					return inst_SLEEP(opCode);
				} else {
					return instInvalid(opCode);
				}
			} else {
				// opCode = 00 0000 0xxx xx01
                                if ( 9 == opCode ) {
					// opCode = 00 0000 0000 1001
					return inst_RETFIE(opCode);
				} else {
					return instInvalid(opCode);
				}
			}
		} else {
			// opCode = 00 0000 0xxx xxx0
			if ( opCode & 0x04  ) {
				// opCode = 00 0000 xxxx x1x0
				if ( 100 == opCode ) {
					// opCode = 00 0000 0110 0100 
					return inst_CLRWDT(opCode);
				} else {
					return instInvalid(opCode);
				}
			} else {
				if ( opCode & 0x08 ) {
					// opCode = 00 0000 0xxx 10x0
					if ( 8 == opCode) {
						// opCode == 00 0000 0000 1000
						return inst_RETURN(opCode);
					} else {
						return instInvalid(opCode);
					}
				} else {
					//opCode = 00 0000 0xxx 00x0
					if ( ( opCode & 0x02 ) ||
					     ( opCode & 0x016 ) ) {
						return instInvalid(opCode);
					} else {
						// opCode = 00 0000 0xx0 0000
						return inst_NOP(opCode);
					}
				}
				
			}
		}
	}
	// opCode = 00 0000 1xxx xxxx
	return inst_MOVWF(opCode);
}


int PIC8InstructionSet::instOPCode_000001(const unsigned int opCode) {
	if ( opCode & 0x080 ) {
		//opCode = 00 0001 1xxx xxxx
		return inst_CLRF(opCode);
	}

	// opCode = 00 0001 0xxx xxxx
	return inst_CLRW(opCode);
}


int PIC8InstructionSet::instOPCode_01(const unsigned int opCode) {
	if ( opCode & 0x02 ) {
		if (opCode & 0x01) {
			// opCode = 01 11xx xxxx xxxx
			return inst_BTFSS(opCode);
		} else {
			// opCode = 01 10xx xxxx xxxx
			return inst_BTFSC(opCode);
		}
	} else {
		if (opCode & 0x01) {
			// opCode = 01 01xx xxxx xxxx
			return inst_BSF(opCode);
		}
	}
	// opCode = 01 00xx xxxx xxxx
	return inst_BCF(opCode);
}


inline void PIC8InstructionSet::instructionEnter(AVR8InsNames::Instructions instName) {
	m_instructionCounter[instName]++;
	m_lastInstruction = instName;
}


/*inline bool PIC8InstructionSet::isInstruction32b(const unsigned int opCode) const {
	//No 32b instructions
}*/












// -----------------------------------------------------------------------------
// INSTRUCTIONS
// -----------------------------------------------------------------------------

int PIC8InstructionSet::instInvalid(const unsigned int opCode) {
	instructionEnter(PIC8InsNames::INS_NONE);

	logEvent(EVENT_CPU_ERR_INVALID_OPCODE, m_pc, opCode);
	return -1;
}


//
// Byte-oriented file register operations
//
// Includes:
// ADDWF
// ANDWF
// CLRF
// CLRW
// COMF
// DECF
// DECFSZ
// INCF
// INCFSZ
// IORWF
// MOVF
// MOVWF
// NOP
// RLF
// RRF
// SUBWF
// SWAPF
// XORWF
//


/*
 * Instruction: ADDWF
 * OP Code: 00 0111 dfff ffff - Add W and f
 * Operation: 
 * Status affected: C, DZ, Z
 */
int PIC8InstructionSet::inst_ADDWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_ADDWF);

	// Operands
	//const unsigned int addrRd = (opCode & 0x01f0) >> 4;

	// Perform the operation

	return 1;
}


/*
 * Instruction: ANDWF
 * OP Code: 00 0101 dfff ffff - AND W and f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_ANDWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_ANDWF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: CLRF
 * OP Code: 00 0001 1fff ffff - Clear f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_CLRF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_CLRF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: CLRW
 * OP Code: 00 0111 0xxx xxxx - Clear W
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_CLRW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_CLRW);

	// Operands

	// Perform the operation

	return 1;
}

/*
 * Instruction: COMF
 * OP Code: 00 1001 dfff ffff - Complement f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_COMF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_COMF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: DECF
 * OP Code: 00 0011 dfff ffff - Decrement f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_DECF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_DECF);

	// Operands

	// Perform the operation

	return 1;
}

/*
 * Instruction: DECFSZ
 * OP Code: 00 1011 dfff ffff - Decrement f, skip if 0
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_DECFSZ(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_DECFSZ);

	// Operands

	// Perform the operation

	//return 2;
	return 1;
}


/*
 * Instruction: INCF
 * OP Code: 00 1010 dfff ffff - Increment f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_INCF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_INCF);

	// Operands

	// Perform the operation

	return 1;
}

/*
 * Instruction: INCFSZ
 * OP Code: 00 1111 dfff ffff - Increment f, skip if 0
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_INCFSZ(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_INCFSZ);

	// Operands

	// Perform the operation

	//return 2;
	return 1;
}


/*
 * Instruction: IORWF
 * OP Code: 00 0100 dfff ffff - Inclusive OR W with f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_IORWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_IORWF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: MOVF
 * OP Code: 00 1000 dfff ffff - Move f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_MOVF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_MOVF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: MOVWF
 * OP Code: 00 0000 1fff ffff - Inclusive OR W with f
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_MOVWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_MOVWF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: NOP
 * OP Code: 00 0000 0xx0 0000 - No operation
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_NOP(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_NOP);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: RLF
 * OP Code: 00 1101 dfff ffff - Rotate fLeft f through Carry
 * Operation: 
 * Status affected: C
 */
int PIC8InstructionSet::inst_RLF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_RLF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: RRF
 * OP Code: 00 1100 dfff ffff - Rotate Right f through Carry
 * Operation: 
 * Status affected: C
 */
int PIC8InstructionSet::inst_RRF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_RRF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: SUBWF
 * OP Code: 00 0010 dfff ffff - Substract W from f
 * Operation: 
 * Status affected: C,DC,Z
 */
int PIC8InstructionSet::inst_SUBWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_SUBWF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: SWAPF
 * OP Code: 00 1110 dfff ffff - Swap nibbles in f
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_SWAPF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_SWAPF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: XORWF
 * OP Code: 00 0110 dfff ffff - Exclusive OR W with f
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_XORWF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_XORWF);

	// Operands

	// Perform the operation

	return 1;
}


//
// Bit-oriented file register operations
//
// Includes:
// BCF
// BSF
// BTFSC
// BTFSS
//



/*
 * Instruction: BCF
 * OP Code: 01 00bb bfff ffff - Bit Clear f
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_BCF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_BCF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: BSF
 * OP Code: 01 01bb bfff ffff - Bit Set f
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_BSF(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_BSF);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: BTFSC
 * OP Code: 01 10bb bfff ffff - Bit Test f, Skip if Clear
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_BTFSC(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_BTFSC);

	// Operands

	// Perform the operation

	//return 2;
	return 1;
}


/*
 * Instruction: BTFSS
 * OP Code: 01 11bb bfff ffff - Bit Test f, Skip if Set
 * Operation: 
 * Status affected: 
 */
int PIC8InstructionSet::inst_BTFSS(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_BTFSS);

	// Operands

	// Perform the operation

	//return 2;
	return 1;
}


//
// Literal and control operations
//
// Includes:
// ADDLW
// ANDLW
// CALL
// CLRWDT
// GOTO
// IORLW
// MOVLW
// RETFIE
// RETLW
// RETURN
// SLEEP
// SUBLW
// XORLW
//


/*
 * Instruction: ADDLW
 * OP Code: 11 111x kkkk kkkk - Add literal and W
 * Operation: 
 * Status affected: C, DC, Z
 */
int PIC8InstructionSet::inst_ADDLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_ADDLW);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: ANDLW
 * OP Code: 11 1001 kkkk kkkk - AND literal with W
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_ANDLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_ANDLW);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: CALL
 * OP Code: 10 0kkk kkkk kkkk - Call subroutine
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_CALL(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_CALL);

	// Operands

	// Perform the operation

	return 2;
}


/*
 * Instruction: CLRWDT
 * OP Code: 00 0000 0110 0100 - Clear Watchdog Timer
 * Operation: 
 * Status affected: TO, PD
 */
int PIC8InstructionSet::inst_CLRWDT(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_CLRWDT);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: GOTO
 * OP Code: 10 1kkk kkkk kkkk - Go to address
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_GOTO(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_GOTO);

	// Operands

	// Perform the operation

	return 2;
}


/*
 * Instruction: IORLW
 * OP Code: 11 1000 kkkk kkkk - Inclusive OR literal with W
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_IORLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_IORLW);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: MOVLW
 * OP Code: 11 00xx kkkk kkkk - Mov literal to W
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_MOVLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_MOVLW);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: RETFIE
 * OP Code: 00 0000 0000 1001 - Return from interrupt
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_RETFIE(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_RETFIE);

	// Operands

	// Perform the operation

	return 2;
}


/*
 * Instruction: RETLW
 * OP Code: 11 01xx kkkk kkkk - Return with literal in W
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_RETLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_RETLW);

	// Operands

	// Perform the operation

	return 2;
}


/*
 * Instruction: RETURN
 * OP Code: 00 0000 0000 1000 - Return from subroutine
 * Operation: 
 * Status affected:
 */
int PIC8InstructionSet::inst_RETURN(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_RETURN);

	// Operands

	// Perform the operation

	return 2;
}


/*
 * Instruction: SLEEP
 * OP Code: 00 0000 0110 0011 - Go into standby mode
 * Operation: 
 * Status affected: TO, PD
 */
int PIC8InstructionSet::inst_SLEEP(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_SLEEP);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: SUBLW
 * OP Code: 11 110x kkkk kkkk - Substract W from literal
 * Operation: 
 * Status affected: C, DC, Z
 */
int PIC8InstructionSet::inst_SUBLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_SUBLW);

	// Operands

	// Perform the operation

	return 1;
}


/*
 * Instruction: XORLW
 * OP Code: 11 1010 kkkk kkkk - Exclusive OR literal with W
 * Operation: 
 * Status affected: Z
 */
int PIC8InstructionSet::inst_XORLW(const unsigned int opCode) {

	instructionEnter(PIC8InsNames::INS_XORLW);

	// Operands

	// Perform the operation

	return 1;
}

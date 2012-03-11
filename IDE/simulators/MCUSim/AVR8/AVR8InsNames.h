/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup AVR8
 * @file AVR8InsNames.h
 */

#ifndef AVR8INSNAMES_H
#define AVR8INSNAMES_H

/**
 * @brief
 * @ingroup AVR8
 * @namespace AVR8InsNames
 */
namespace AVR8InsNames {
	enum Instructions {
		INS_NONE = 0,

		INS_PUSH_Rr,		///< 1001 001d dddd 1111 - Push Register on Stack
		INS_POP_Rd,		///< 1001 000d dddd 1111 - Pop Register from Stack
		INS_ADD_Rd_Rr,		///< 0000 11rd dddd rrrr - Add without Carry
		INS_SBIW_Rd_1_Rd_K,	///< 1001 0111 KKdd KKKK - Subtract Immediate from Word
		INS_SBCI_Rd_K,		///< 0100 KKKK dddd KKKK - Subtract Immediate with Carry
		INS_SUBI_Rd_K,		///< 0101 KKKK dddd KKKK - Subtract Immediate
		INS_SUB_Rd_Rr,		///< 0001 10rd dddd rrrr - Subtract without Carry
		INS_SBC_Rd_Rr,		///< 0000 10rd dddd rrrr - Subtract with Carry
		INS_CP_Rd_Rr,		///< 0001 01rd dddd rrrr - Compare
		INS_CPC_Rd_Rr,		///< 0000 01rd dddd rrrr - Compare with Carry
		INS_CPI_Rd_K,		///< 0011 KKKK dddd KKKK - Compare with Immediate
		INS_XCH_Z_Rd,		///< 1001 001r rrrr 0100 - Exchange
		INS_CPSE_Rd_Rr,		///< 0001 00rd dddd rrrr - Compare Skip if Equal
		INS_SBRC_Rr_b,		///< 1111 110r rrrr 0bbb - Skip if Bit in Register is Cleared
		INS_SBRS_Rr_b,		///< 1111 111r rrrr 0bbb - Skip if Bit in Register is Set
		INS_SBIS_A_b,		///< 1001 1011 AAAA Abbb - Skip if Bit in I/O Register is Set
		INS_SBIC_A_b,		///< 1001 1001 AAAA Abbb - Skip if Bit in I/O Register is Cleared
		INS_ADC_Rd_Rr,		///< 0001 11rd dddd rrrr - Add with Carry.
		INS_AND_Rd_Rr,		///< 0001 11rd dddd rrrr - Add with Carry.
		INS_EOR_Rd_Rr,		///< 0010 01rd dddd rrrr - Exclusive OR
		INS_OR_Rd_Rr,		///< 0010 10rd dddd rrrr - Logical OR
		INS_MOV_Rd_Rr,		///< 0010 11rd dddd rrrr - Copy Register
		INS_MOVW_Rd_Rr,		///< 0000 0001 dddd rrrr - Copy Register Word
		INS_ORI_Rd_K,		///< 0110 KKKK dddd KKKK - Logical OR with Immediatee
		INS_ANDI_Rd_K,		///< 0111 KKKK dddd KKKK - Logical AND with Immediate
		INS_IN,			///< 1011 0AAd dddd AAAA - Load an I/O Location to Register
		INS_OUT_A_Rr,		///< 1011 1AAr rrrr AAAA - Store Register to I/O Location
		INS_RJMP_k,		///< 1101 kkkk kkkk kkkk - Relative Call to Subroutine
		INS_RCALL_k,		///< 1101 kkkk kkkk kkkk - Relative Call to Subroutine
		INS_LDI_Rd_K,		///< 1110 KKKK dddd KKKK - Load Immediate
		INS_BRBS_s_k,		///< 1111 00kk kkkk ksss - Branch if Bit in SREG is Set
		INS_BRBC_s_k,		///< 1111 01kk kkkk ksss - Branch if Bit in SREG is Cleared
		INS_ICALL,		///< 1001 0101 0000 1001 - Indirect Call to Subroutine
		INS_EICALL,		///< 1001 0101 0001 1001 - Extended Indirect Call to Subroutine
		INS_IJMP,		///< 1001 0100 0000 1001 - Indirect Jump
		INS_EIJMP,		///< 1001 0100 0001 1001 - Extended Indirect Jump
		INS_LPM_Rd_Z,		///< 1001 000d dddd 0100 - Load Program Memory
		INS_LPM,		///< 1001 0101 1100 1000 - Load Program Memory
		INS_LPM_Rd_Zplus,	///< 1001 000d dddd 0101 - Load Program Memory
		INS_ELPM,		///< 1001 0101 1101 1000 - Extended Load Program Memory (ELPM)
		INS_ELPM_Rd_Z,		///< 1001 000d dddd 0110 - Extended Load Program Memory (ELPM Rd, Z)
		INS_ELPM_Rd_Zplus,	///< 1001 000d dddd 0111 - Extended Load Program Memory (ELPM Rd, Z+)
		INS_SLEEP,		///< 1001 0101 1000 1000 - SLEEP
		INS_BREAK,		///< 1001 0101 1001 1000 - Break
		INS_WDR,		///< 1001 0101 1010 1000 - Watchdog Reset
		INS_BSET_s,		///< 1001 0100 0sss 1000 - Bit Clear in SREG
		INS_BCLR_s,		///< 1001 0100 1sss 1000 - Bit Clear in SREG
		INS_SEC,		///< 1001 0100 0000 1000 - Set Carry Flag
		INS_SEZ,		///< 1001 0100 0001 1000 - Set Zero Flag
		INS_SEN,		///< 1001 0100 0010 1000 - Set Negative Flag
		INS_SEV,		///< 1001 0100 0011 1000 - Set Overflow Flag
		INS_SES,		///< 1001 0100 0100 1000 - Set Signed Flag
		INS_SEH,		///< 1001 0100 0101 1000 - Set Half Carry Flag
		INS_SET,		///< 1001 0100 0110 1000 - Set T Flag
		INS_SEI,		///< 1001 0100 0111 1000 - Set Global Interrupt Flag
		INS_CLC,		///< 1001 0100 1000 1000 - Clear Carry Flag
		INS_CLZ,		///< 1001 0100 1001 1000 - Clear Zero Flag
		INS_CLN,		///< 1001 0100 1010 1000 - Clear Negative Flag
		INS_CLV,		///< 1001 0100 1011 1000 - Clear Overflow Flag
		INS_CLS,		///< 1001 0100 1100 1000 - Clear Signed Flag
		INS_CLH,		///< 1001 0100 1101 1000 - Clear Half Carry Flag
		INS_CLT,		///< 1001 0100 1110 1000 - Clear T Flag
		INS_CLI,		///< 1001 0100 1111 1000 - Clear Global Interrupt Flag
		INS_NOP,		///< 0000 0000 0000 0000 - No Operation
		INS_ADIW_Rd_1_Rd_K,	///< 1001 0110 KKdd KKKK - Add Immediate to Word
		INS_JMP_k,		///< 1001 010k kkkk 110k kkkk kkkk kkkk kkkk - Jump
		INS_RET,		///< 1001 0101 0000 1000 - Return from Subroutine
		INS_RETI,		///< 1001 0101 0001 1000 - Return from Interrupt
		INS_CALL_k,		///< 1001 010k kkkk 111k kkkk kkkk kkkk kkkk - Long Call to a Subroutine
		INS_DEC_Rd,		///< 1001 010d dddd 1010 - Decrement
		INS_INC_Rd,		///< 1001 010d dddd 0011 - Increment
		INS_COM_Rd,		///< 1001 010d dddd 0000 - One’s Complement
		INS_NEG_Rd,		///< 1001 010d dddd 0001 - Two’s Complement
		INS_SWAP_Rd,		///< 1001 010d dddd 0010 - Swap Nibbles
		INS_ASR_Rd,		///< 1001 010d dddd 0101 - Arithmetic Shift Right.
		INS_LSR_Rd,		///< 1001 010d dddd 0110 - Logical Shift Right
		INS_ROR_Rd,		///< 1001 010d dddd 0111 - Rotate Right through Carry
		INS_BLD_Rd_b,		///< 1111 100d dddd 0bbb - Bit Load from the T Flag in SREG to a Bit in Register.
		INS_BST_Rd_b,		///< 1111 101d dddd 0bbb - Bit Store from Bit in Register to T Flag in SREG.
		INS_SBI_A_b,		///< 1001 1010 AAAA Abbb - Set Bit in I/O Register
		INS_CBI_A_b,		///< 1001 1000 AAAA Abbb - Clear Bit in I/O Register
		INS_LAS_Z_Rd,		///< 1001 001r rrrr 0101 - Load And Set
		INS_LAC_Z_Rd,		///< 1001 001r rrrr 0110 - Load And Clear
		INS_LAT_Z_Rd,		///< 1001 001r rrrr 0111 - Load And Toggle
		INS_LD_Rd_X,		///< 1001 000d dddd 1100 - Load Indirect from Data Space to Register using Index X
		INS_LD_Rd_Xplus,	///< 1001 000d dddd 1101 - Load Indirect from Data Space to Register using Index X
		INS_LD_Rd_minusX,	///< 1001 000d dddd 1110 - Load Indirect from Data Space to Register using Index X
		INS_LD_Rd_Yplus,	///< 1001 000d dddd 1001 - Load Indirect from Data Space to Register using Index Y
		INS_LD_Rd_minusY,	///< 1001 000d dddd 1010 - Load Indirect from Data Space to Register using Index Y
		INS_LDD_Rd_Yq,		///< 10q0 qq0d dddd 1qqq - Load Indirect from Data Space to Register using Index Y
		INS_LD_Rd_Zplus,	///< 1001 000d dddd 0001 - Load Indirect From Data Space to Register using Index Z
		INS_LD_Rd_minusZ,	///< 1001 000d dddd 0010 - Load Indirect From Data Space to Register using Index Z
		INS_LDD_Rd_Zq,		///< 10q0 qq0d dddd 0qqq - Load Indirect From Data Space to Register using Index Z
		INS_MUL_Rd_Rr,		///< 1001 11rd dddd rrrr - Multiply Unsigned
		INS_MULS_Rd_Rr,		///< 0000 0010 dddd rrrr - Multiply Signed
		INS_MULSU_Rd_Rr,	///< 0000 0011 0ddd 0rrr - Multiply Signed with Unsigned
		INS_FMUL_Rd_Rr,		///< 0000 0011 0ddd 1rrr - Fractional Multiply Unsigned
		INS_FMULS_Rd_Rr,	///< 0000 0011 1ddd 0rrr - Fractional Multiply Signed
		INS_FMULSU_Rd_Rr,	///< 0000 0011 1ddd 1rrr - Fractional Multiply Signed with Unsigned
		INS_LDS_Rd_k,		///< 1001 000d dddd 0000 kkkk kkkk kkkk kkkk - Load Direct from Data Space (LDS Rd, k) (32b version)
		INS_STS_k_Rr,		///< 1001 001d dddd 0000 kkkk kkkk kkkk kkkk - Store Direct to Data Space (STS k, Rr) (32b version)
		INS_LDS_Rd_k_16b,	///< 1010 0kkk dddd kkkk - Load Direct from Data Space (LDS Rd, k) (16b version)
		INS_STS_k_Rr_16b,	///< 1010 1kkk dddd kkkk - Store Direct to Data Space (STS k, Rr) (16b version)
		INS_STD_Zq_Rr,		///< 1000 001r rrrr 0000 - Store Indirect From Register to Data Space using Index Z (ST Z, Rr)
		INS_STD_Yq_Rr,		///< 1000 001r rrrr 1000 - Store Indirect From Register to Data Space using Index Y (ST Y, Rr)
		INS_ST_Zplus_Rr,	///< 1001 001r rrrr 0001 - Store Indirect From Register to Data Space using Index Z (ST Z+, Rr)
		INS_ST_minusZ_Rr,	///< 1001 001r rrrr 0010 - Store Indirect From Register to Data Space using Index Z (ST -Z, Rr)
		INS_ST_Yplus_Rr,	///< 1001 001r rrrr 1001 - Store Indirect From Register to Data Space using Index Y (ST Y+, Rr)
		INS_ST_minusY_Rr,	///< 1001 001r rrrr 1010 - Store Indirect From Register to Data Space using Index Y (ST -Y, Rr)
		INS_ST_X_Rr,		///< 1001 001r rrrr 1100 - Store Indirect From Register to Data Space using Index X (ST X, Rr)
		INS_ST_Xplus_Rr,	///< 1001 001r rrrr 1101 - Store Indirect From Register to Data Space using Index X (ST X+, Rr)
		INS_ST_minusX_Rr,	///< 1001 001r rrrr 1110 - Store Indirect From Register to Data Space using Index X (ST -X, Rr)
		INS_SPM,		///< 1001 0101 1110 1000 - Store Program Memory (SPM)
		INS_SPM_Zplus,		///< 1001 0101 1111 1000 - Store Program Memory (SPM Z+)
		INS_DES,		///< 1001 0100 KKKK 1011 - Data Encryption Standard

		INS__MAX__
	};

	enum SpecificInstructions {
		SPECI_STS_K_RR_16 = 0,	///< 16b version of "STS k, Rr" and "LDS Rd, k"
		SPECI_ADIW,		///< Add Immediate to Word
		SPECI_CALL,		///< Long Call to a Subroutine
		SPECI_JMP,		///< Jump
		SPECI_BREAK,		///< Break
		SPECI_EICALL,		///< Extended Indirect Call to Subroutine
		SPECI_EIJMP,		///< Extended Indirect Jump
		SPECI_ICALL,		///< Indirect Call to Subroutine
		SPECI_IJMP,		///< Indirect Jump
		SPECI_DES,		///< Data Encryption Standard
		SPECI_ELPM_Rd_Zplus,	///< Extended Load Program Memory
		SPECI_ELPM_Rd_Z,	///< Extended Load Program Memory
		SPECI_ELPM,		///< Extended Load Program Memory

		SPECI_MUL,		///< Multiply Unsigned
		SPECI_MULS,		///< Multiply Signed
		SPECI_MULSU,		///< Multiply Signed with Unsigned
		SPECI_FMUL,		///< Fractional Multiply Unsigned
		SPECI_FMULS,		///< Fractional Multiply Signed
		SPECI_FMULSU,		///< Fractional Multiply Signed with Unsigned

		SPECI_LD_Rd_X,		///< 
		SPECI_LD_Rd_Xplus,	///<
		SPECI_LD_Rd_minusX,	///<

		SPECI_LDD_Rd_Y,		///< 
		SPECI_LD_Rd_Yplus,	///<
		SPECI_LD_Rd_minusY,	///<
		SPECI_LDD_Rd_Yq,	///<

		SPECI_LDD_Rd_Z,		///< 
		SPECI_LD_Rd_Zplus,	///<
		SPECI_LD_Rd_minusZ,	///<
		SPECI_LDD_Rd_Zq,	///<

		SPECI_ST_Rr_X,		///< 
		SPECI_ST_Rr_Xplus,	///<
		SPECI_ST_Rr_minusX,	///<

		SPECI_STD_Rr_Y,		///< 
		SPECI_ST_Rr_Yplus,	///<
		SPECI_ST_Rr_minusY,	///<
		SPECI_STD_Rr_Yq,	///<

		SPECI_STD_Rr_Z,		///< 
		SPECI_ST_Rr_Zplus,	///<
		SPECI_ST_Rr_minusZ,	///<
		SPECI_STD_Rr_Zq,	///<

		SPECI_LAS,		///< Load And Set
		SPECI_LAC,		///< Load And Clear
		SPECI_LAT,		///< Load And Toggle

		SPECI_SPM,		///< 1001 0101 1110 1000 - Store Program Memory (SPM)
		SPECI_SPM_Zplus,	///< 1001 0101 1111 1000 - Store Program Memory (SPM Z+)

		SPECI_POP,		///< 
		SPECI_PUSH,		///< 

		SPECI_MOVW,		///< Copy Register Word
		SPECI_SBIW,		///< Subtract Immediate from Word

		SPECI_RCALL,		///< Relative Call to Subroutine
		SPECI_LDS_Rd_k_16b,	///< Load Direct from Data Space (16b version)
		SPECI_LDS_Rd_k,		///< Load Direct from Data Space (32b version)

		SPECI_LPM,		///< Load Program Memory
		SPECI_LPM_Rd_Z,		///< Load Program Memory
		SPECI_LPM_Rd_Zplus,	///< Load Program Memory

		SPECI_STS_k_Rr_16bb,	///< Store Direct to Data Space (STS k, Rr) (16b version)
		SPECI_STS_k_Rr,		///< Store Direct to Data Space (STS k, Rr) (32b version)

		SPECI__MAX__
	};
};

#endif // AVR8INSNAMES_H

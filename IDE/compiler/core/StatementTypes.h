/**
 * @brief
 * C++  ...
 *
 * ...
 *
 *  See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup Assembler
 * @file StatementTypes.h
 */

#ifndef STATEMENTTYPES_H
#define STATEMENTTYPES_H

#include <ostream>

/**
 * @brief
 * @ingroup Assembler
 * @namespace StatementTypes
 */
namespace StatementTypes {
	enum StatementType {
		EMPTY_STATEMENT = 0,

		ASM51_MACRO,
		ASM51_LABEL,
		
		ASM51_DIR_MACRO,
		ASM51_DIR_DB,

		ASM51_INS_ACALL,

		ASM51_INS_ADD__A_IMM,
		ASM51_INS_ADD__A_DIR,
		ASM51_INS_ADD__A_IR0,
		ASM51_INS_ADD__A_IR1,
		ASM51_INS_ADD__A_R0,
		ASM51_INS_ADD__A_R1,
		ASM51_INS_ADD__A_R2,
		ASM51_INS_ADD__A_R3,
		ASM51_INS_ADD__A_R4,
		ASM51_INS_ADD__A_R5,
		ASM51_INS_ADD__A_R6,
		ASM51_INS_ADD__A_R7,

		ASM51_INS_ADDC__A_IMM,
		ASM51_INS_ADDC__A_DIR,
		ASM51_INS_ADDC__A_IR0,
		ASM51_INS_ADDC__A_IR1,
		ASM51_INS_ADDC__A_R0,
		ASM51_INS_ADDC__A_R1,
		ASM51_INS_ADDC__A_R2,
		ASM51_INS_ADDC__A_R3,
		ASM51_INS_ADDC__A_R4,
		ASM51_INS_ADDC__A_R5,
		ASM51_INS_ADDC__A_R6,
		ASM51_INS_ADDC__A_R7,

		ASM51_INS_AJMP__DIR,

		ASM51_INS_ANL__DIR_A,
		ASM51_INS_ANL__DIR_IMM,
		ASM51_INS_ANL__A_IMM,
		ASM51_INS_ANL__A_DIR,
		ASM51_INS_ANL__A_IR0,
		ASM51_INS_ANL__A_IR1,
		ASM51_INS_ANL__A_R0,
		ASM51_INS_ANL__A_R1,
		ASM51_INS_ANL__A_R2,
		ASM51_INS_ANL__A_R3,
		ASM51_INS_ANL__A_R4,
		ASM51_INS_ANL__A_R5,
		ASM51_INS_ANL__A_R6,
		ASM51_INS_ANL__A_R7,
		ASM51_INS_ANL__C_DIR,
		ASM51_INS_ANL__C_SL_DIR,

		ASM51_INS_CJNE__A_IMM_DIR,
		ASM51_INS_CJNE__A_DIR_DIR,
		ASM51_INS_CJNE__IR0_IMM_DIR,
		ASM51_INS_CJNE__IR1_IMM_DIR,
		ASM51_INS_CJNE__R0_IMM_DIR,
		ASM51_INS_CJNE__R1_IMM_DIR,
		ASM51_INS_CJNE__R2_IMM_DIR,
		ASM51_INS_CJNE__R3_IMM_DIR,
		ASM51_INS_CJNE__R4_IMM_DIR,
		ASM51_INS_CJNE__R5_IMM_DIR,
		ASM51_INS_CJNE__R6_IMM_DIR,
		ASM51_INS_CJNE__R7_IMM_DIR,

		ASM51_INS_CLR__DIR,
		ASM51_INS_CLR__C,
		ASM51_INS_CLR__A,

		ASM51_INS_CPL__DIR,
		ASM51_INS_CPL__C,
		ASM51_INS_CPL__A,

		ASM51_INS_DA__A,

		ASM51_INS_DEC__DIR,
		ASM51_INS_DEC__A,
		ASM51_INS_DEC__IR0,
		ASM51_INS_DEC__IR1,
		ASM51_INS_DEC__R0,
		ASM51_INS_DEC__R1,
		ASM51_INS_DEC__R2,
		ASM51_INS_DEC__R3,
		ASM51_INS_DEC__R4,
		ASM51_INS_DEC__R5,
		ASM51_INS_DEC__R6,
		ASM51_INS_DEC__R7,

		ASM51_INS_DIV__AB,

		ASM51_INS_DJNZ__DIR_DIR,
		ASM51_INS_DJNZ__R0_DIR,
		ASM51_INS_DJNZ__R1_DIR,
		ASM51_INS_DJNZ__R2_DIR,
		ASM51_INS_DJNZ__R3_DIR,
		ASM51_INS_DJNZ__R4_DIR,
		ASM51_INS_DJNZ__R5_DIR,
		ASM51_INS_DJNZ__R6_DIR,
		ASM51_INS_DJNZ__R7_DIR,

		ASM51_INS_INC__A,
		ASM51_INS_INC__DIR,
		ASM51_INS_INC__IR0,
		ASM51_INS_INC__IR1,
		ASM51_INS_INC__R0,
		ASM51_INS_INC__R1,
		ASM51_INS_INC__R2,
		ASM51_INS_INC__R3,
		ASM51_INS_INC__R4,
		ASM51_INS_INC__R5,
		ASM51_INS_INC__R6,
		ASM51_INS_INC__R7,
		ASM51_INS_INC__DPTR,

		ASM51_INS_JB__DIR_DIR,

		ASM51_INS_JBC__DIR_DIR,

		ASM51_INS_JC__DIR,


		ASM51_INS_JMP__IA_DPTR,
		ASM51_INS_JMP__DIR,

		ASM51_INS_JNB__DIR,

		ASM51_INS_JNC__DIR,

		ASM51_INS_JNZ__DIR,

		ASM51_INS_JZ__DIR,

		ASM51_INS_LCALL__DIR,

		ASM51_INS_LJMP__DIR,

		ASM51_INS_MOV__DIR_DIR,
		ASM51_INS_MOV__DIR_IMM,
		ASM51_INS_MOV__DIR_IR0,
		ASM51_INS_MOV__DIR_IR1,
		ASM51_INS_MOV__DIR_R0,
		ASM51_INS_MOV__DIR_R1,
		ASM51_INS_MOV__DIR_R2,
		ASM51_INS_MOV__DIR_R3,
		ASM51_INS_MOV__DIR_R4,
		ASM51_INS_MOV__DIR_R5,
		ASM51_INS_MOV__DIR_R6,
		ASM51_INS_MOV__DIR_R7,
		ASM51_INS_MOV__DIR_C,
		ASM51_INS_MOV__DIR_A,
		ASM51_INS_MOV__IR0_IMM,
		ASM51_INS_MOV__IR1_IMM,
		ASM51_INS_MOV__IR0_DIR,
		ASM51_INS_MOV__IR1_DIR,
		ASM51_INS_MOV__IR0_A,
		ASM51_INS_MOV__IR1_A,
		ASM51_INS_MOV__DPTR_IMM,
		ASM51_INS_MOV__R0_IMM,
		ASM51_INS_MOV__R1_IMM,
		ASM51_INS_MOV__R2_IMM,
		ASM51_INS_MOV__R3_IMM,
		ASM51_INS_MOV__R4_IMM,
		ASM51_INS_MOV__R5_IMM,
		ASM51_INS_MOV__R6_IMM,
		ASM51_INS_MOV__R7_IMM,
		ASM51_INS_MOV__R0_DIR,
		ASM51_INS_MOV__R1_DIR,
		ASM51_INS_MOV__R2_DIR,
		ASM51_INS_MOV__R3_DIR,
		ASM51_INS_MOV__R4_DIR,
		ASM51_INS_MOV__R5_DIR,
		ASM51_INS_MOV__R6_DIR,
		ASM51_INS_MOV__R7_DIR,
		ASM51_INS_MOV__R0_A,
		ASM51_INS_MOV__R1_A,
		ASM51_INS_MOV__R2_A,
		ASM51_INS_MOV__R3_A,
		ASM51_INS_MOV__R4_A,
		ASM51_INS_MOV__R5_A,
		ASM51_INS_MOV__R6_A,
		ASM51_INS_MOV__R7_A,
		ASM51_INS_MOV__A_IMM,
		ASM51_INS_MOV__A_IR0,
		ASM51_INS_MOV__A_IR1,
		ASM51_INS_MOV__A_R0,
		ASM51_INS_MOV__A_R1,
		ASM51_INS_MOV__A_R2,
		ASM51_INS_MOV__A_R3,
		ASM51_INS_MOV__A_R4,
		ASM51_INS_MOV__A_R5,
		ASM51_INS_MOV__A_R6,
		ASM51_INS_MOV__A_R7,
		ASM51_INS_MOV__A_DIR,
		ASM51_INS_MOV__C_DIR,

		ASM51_INS_MOVC__A_IA_DPTR,
		ASM51_INS_MOVC__A_IA_PC,

		ASM51_INS_MOVX__IDPTR_A,
		ASM51_INS_MOVX__IR0_A,
		ASM51_INS_MOVX__IR1_A,
		ASM51_INS_MOVX__A_IDPTR,
		ASM51_INS_MOVX__A_IR0,
		ASM51_INS_MOVX__A_IR1,

		ASM51_INS_MUL__AB,

		ASM51_INS_NOP,

		ASM51_INS_ORL__DIR_A,
		ASM51_INS_ORL__DIR_IMM,
		ASM51_INS_ORL__A_IMM,
		ASM51_INS_ORL__A_DIR,
		ASM51_INS_ORL__A_IR0,
		ASM51_INS_ORL__A_IR1,
		ASM51_INS_ORL__A_R0,
		ASM51_INS_ORL__A_R1,
		ASM51_INS_ORL__A_R2,
		ASM51_INS_ORL__A_R3,
		ASM51_INS_ORL__A_R4,
		ASM51_INS_ORL__A_R5,
		ASM51_INS_ORL__A_R6,
		ASM51_INS_ORL__A_R7,
		ASM51_INS_ORL__C_DIR,
		ASM51_INS_ORL__C_SL_DIR,

		ASM51_INS_POP__DIR,

		ASM51_INS_PUSH__DIR,

		ASM51_INS_RET,

		ASM51_INS_RETI,

		ASM51_INS_RL__A,

		ASM51_INS_RLC__A,

		ASM51_INS_RR__A,

		ASM51_INS_RRC__A,

		ASM51_INS_SETB__C,
		ASM51_INS_SETB__DIR,

		ASM51_INS_SJMP__DIR,

		ASM51_INS_SUBB__A_IMM,
		ASM51_INS_SUBB__A_DIR,
		ASM51_INS_SUBB__A_IR0,
		ASM51_INS_SUBB__A_IR1,
		ASM51_INS_SUBB__A_R0,
		ASM51_INS_SUBB__A_R1,
		ASM51_INS_SUBB__A_R2,
		ASM51_INS_SUBB__A_R3,
		ASM51_INS_SUBB__A_R4,
		ASM51_INS_SUBB__A_R5,
		ASM51_INS_SUBB__A_R6,
		ASM51_INS_SUBB__A_R7,

		ASM51_INS_SWAP__A,

		ASM51_INS_XCH__A_IR0,
		ASM51_INS_XCH__A_IR1,
		ASM51_INS_XCH__A_R0,
		ASM51_INS_XCH__A_R1,
		ASM51_INS_XCH__A_R2,
		ASM51_INS_XCH__A_R3,
		ASM51_INS_XCH__A_R4,
		ASM51_INS_XCH__A_R5,
		ASM51_INS_XCH__A_R6,
		ASM51_INS_XCH__A_R7,
		ASM51_INS_XCH__A_DIR,

		ASM51_INS_XCHD__A_IR0,
		ASM51_INS_XCHD__A_IR1,

		ASM51_INS_XRL__DIR_A,
		ASM51_INS_XRL__DIR_IMM,
		ASM51_INS_XRL__A_IMM,
		ASM51_INS_XRL__A_DIR,
		ASM51_INS_XRL__A_IR0,
		ASM51_INS_XRL__A_IR1,
		ASM51_INS_XRL__A_R0,
		ASM51_INS_XRL__A_R1,
		ASM51_INS_XRL__A_R2,
		ASM51_INS_XRL__A_R3,
		ASM51_INS_XRL__A_R4,
		ASM51_INS_XRL__A_R5,
		ASM51_INS_XRL__A_R6,
		ASM51_INS_XRL__A_R7,

		ASM51__MAX__
	};
};

// Debugging operators
std::ostream & operator << (std::ostream & out, const StatementTypes::StatementType type);

#endif // STATEMENTTYPES_H

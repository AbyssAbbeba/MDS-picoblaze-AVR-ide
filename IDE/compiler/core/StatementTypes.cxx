/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "StatementTypes.h"

std::ostream & operator << (std::ostream & out, const StatementTypes::StatementType type) {
	using namespace StatementTypes;

	switch ( type ) {
		case EMPTY_STATEMENT:			out << "EMPTY_STATEMENT";		break;

		case ASMAVR8_MACRO:			out << "ASMAVR8_MACRO";			break;
		case ASMAVR8_LABEL:			out << "ASMAVR8_LABEL";			break;
		case ASMAVR8_INCLUDE:			out << "ASMAVR8_INCLUDE";		break;
		case ASMAVR8_COND_ASM:			out << "ASMAVR8_COND_ASM";		break;
		case ASMAVR8_DIR_IF:			out << "ASMAVR8_DIR_IF";		break;
		case ASMAVR8_DIR_IFN:			out << "ASMAVR8_DIR_IFN";		break;
		case ASMAVR8_DIR_IFDEF:			out << "ASMAVR8_DIR_IFDEF";		break;
		case ASMAVR8_DIR_IFNDEF:		out << "ASMAVR8_DIR_IFNDEF";		break;
		case ASMAVR8_DIR_ELSEIFB:		out << "ASMAVR8_DIR_ELSEIFB";		break;
		case ASMAVR8_DIR_ELSEIFNB:		out << "ASMAVR8_DIR_ELSEIFNB";		break;
		case ASMAVR8_DIR_ELSE:			out << "ASMAVR8_DIR_ELSE";		break;
		case ASMAVR8_DIR_ELSEIF:		out << "ASMAVR8_DIR_ELSEIF";		break;
		case ASMAVR8_DIR_ELSEIFN:		out << "ASMAVR8_DIR_ELSEIFN";		break;
		case ASMAVR8_DIR_ELSEIFDEF:		out << "ASMAVR8_DIR_ELSEIFDEF";		break;
		case ASMAVR8_DIR_ELSEIFNDEF:		out << "ASMAVR8_DIR_ELSEIFNDEF";	break;
		case ASMAVR8_DIR_LOCAL:			out << "ASMAVR8_DIR_LOCAL";		break;
		case ASMAVR8_DIR_IFNB:			out << "ASMAVR8_DIR_IFNB";		break;
		case ASMAVR8_DIR_IFB:			out << "ASMAVR8_DIR_IFB";		break;
		case ASMAVR8_DIR_SKIP:			out << "ASMAVR8_DIR_SKIP";		break;
		case ASMAVR8_DIR_EXITM:			out << "ASMAVR8_DIR_EXITM";		break;
		case ASMAVR8_DIR_REPT:			out << "ASMAVR8_DIR_REPT";		break;
		case ASMAVR8_DIR_MACRO:			out << "ASMAVR8_DIR_MACRO";		break;
		case ASMAVR8_DIR_BYTE:			out << "ASMAVR8_DIR_BYTE";		break;
		case ASMAVR8_DIR_CSEG:			out << "ASMAVR8_DIR_CSEG";		break;
		case ASMAVR8_DIR_DB:			out << "ASMAVR8_DIR_DB";		break;
		case ASMAVR8_DIR_DEF:			out << "ASMAVR8_DIR_DEF";		break;
		case ASMAVR8_DIR_DEVICE:		out << "ASMAVR8_DIR_DEVICE";		break;
		case ASMAVR8_DIR_DSEG:			out << "ASMAVR8_DIR_DSEG";		break;
		case ASMAVR8_DIR_DW:			out << "ASMAVR8_DIR_DW";		break;
		case ASMAVR8_DIR_EQU:			out << "ASMAVR8_DIR_EQU";		break;
		case ASMAVR8_DIR_ESEG:			out << "ASMAVR8_DIR_ESEG";		break;
		case ASMAVR8_DIR_EXIT:			out << "ASMAVR8_DIR_EXIT";		break;
		case ASMAVR8_DIR_INCLUDE:		out << "ASMAVR8_DIR_INCLUDE";		break;
		case ASMAVR8_DIR_LIST:			out << "ASMAVR8_DIR_LIST";		break;
		case ASMAVR8_DIR_LISTMAC:		out << "ASMAVR8_DIR_LISTMAC";		break;
		case ASMAVR8_DIR_NOLIST:		out << "ASMAVR8_DIR_NOLIST";		break;
		case ASMAVR8_DIR_ORG:			out << "ASMAVR8_DIR_ORG";		break;
		case ASMAVR8_DIR_SET:			out << "ASMAVR8_DIR_SET";		break;
		case ASMAVR8_INS_ADD:			out << "ASMAVR8_INS_ADD";		break;
		case ASMAVR8_INS_ADC:			out << "ASMAVR8_INS_ADC";		break;
		case ASMAVR8_INS_ADIW:			out << "ASMAVR8_INS_ADIW";		break;
		case ASMAVR8_INS_SUB:			out << "ASMAVR8_INS_SUB";		break;
		case ASMAVR8_INS_SUBI:			out << "ASMAVR8_INS_SUBI";		break;
		case ASMAVR8_INS_SBC:			out << "ASMAVR8_INS_SBC";		break;
		case ASMAVR8_INS_SBCI:			out << "ASMAVR8_INS_SBCI";		break;
		case ASMAVR8_INS_SBIW:			out << "ASMAVR8_INS_SBIW";		break;
		case ASMAVR8_INS_AND:			out << "ASMAVR8_INS_AND";		break;
		case ASMAVR8_INS_ANDI:			out << "ASMAVR8_INS_ANDI";		break;
		case ASMAVR8_INS_OR:			out << "ASMAVR8_INS_OR";		break;
		case ASMAVR8_INS_ORI:			out << "ASMAVR8_INS_ORI";		break;
		case ASMAVR8_INS_EOR:			out << "ASMAVR8_INS_EOR";		break;
		case ASMAVR8_INS_COM:			out << "ASMAVR8_INS_COM";		break;
		case ASMAVR8_INS_NEG:			out << "ASMAVR8_INS_NEG";		break;
		case ASMAVR8_INS_SBR:			out << "ASMAVR8_INS_SBR";		break;
		case ASMAVR8_INS_CBR:			out << "ASMAVR8_INS_CBR";		break;
		case ASMAVR8_INS_INC:			out << "ASMAVR8_INS_INC";		break;
		case ASMAVR8_INS_DEC:			out << "ASMAVR8_INS_DEC";		break;
		case ASMAVR8_INS_TST:			out << "ASMAVR8_INS_TST";		break;
		case ASMAVR8_INS_CLR:			out << "ASMAVR8_INS_CLR";		break;
		case ASMAVR8_INS_SER:			out << "ASMAVR8_INS_SER";		break;
		case ASMAVR8_INS_MUL:			out << "ASMAVR8_INS_MUL";		break;
		case ASMAVR8_INS_RJMP:			out << "ASMAVR8_INS_RJMP";		break;
		case ASMAVR8_INS_IJMP:			out << "ASMAVR8_INS_IJMP";		break;
		case ASMAVR8_INS_JMP:			out << "ASMAVR8_INS_JMP";		break;
		case ASMAVR8_INS_RCALL:			out << "ASMAVR8_INS_RCALL";		break;
		case ASMAVR8_INS_ICALL:			out << "ASMAVR8_INS_ICALL";		break;
		case ASMAVR8_INS_CALL:			out << "ASMAVR8_INS_CALL";		break;
		case ASMAVR8_INS_RET:			out << "ASMAVR8_INS_RET";		break;
		case ASMAVR8_INS_RETI:			out << "ASMAVR8_INS_RETI";		break;
		case ASMAVR8_INS_CPSE:			out << "ASMAVR8_INS_CPSE";		break;
		case ASMAVR8_INS_CP:			out << "ASMAVR8_INS_CP";		break;
		case ASMAVR8_INS_CPC:			out << "ASMAVR8_INS_CPC";		break;
		case ASMAVR8_INS_CPI:			out << "ASMAVR8_INS_CPI";		break;
		case ASMAVR8_INS_SBRC:			out << "ASMAVR8_INS_SBRC";		break;
		case ASMAVR8_INS_SBRS:			out << "ASMAVR8_INS_SBRS";		break;
		case ASMAVR8_INS_SBIC:			out << "ASMAVR8_INS_SBIC";		break;
		case ASMAVR8_INS_SBIS:			out << "ASMAVR8_INS_SBIS";		break;
		case ASMAVR8_INS_BRBS:			out << "ASMAVR8_INS_BRBS";		break;
		case ASMAVR8_INS_BRBC:			out << "ASMAVR8_INS_BRBC";		break;
		case ASMAVR8_INS_BREQ:			out << "ASMAVR8_INS_BREQ";		break;
		case ASMAVR8_INS_BRNE:			out << "ASMAVR8_INS_BRNE";		break;
		case ASMAVR8_INS_BRCS:			out << "ASMAVR8_INS_BRCS";		break;
		case ASMAVR8_INS_BRCC:			out << "ASMAVR8_INS_BRCC";		break;
		case ASMAVR8_INS_BRSH:			out << "ASMAVR8_INS_BRSH";		break;
		case ASMAVR8_INS_BRLO:			out << "ASMAVR8_INS_BRLO";		break;
		case ASMAVR8_INS_BRMI:			out << "ASMAVR8_INS_BRMI";		break;
		case ASMAVR8_INS_BRPL:			out << "ASMAVR8_INS_BRPL";		break;
		case ASMAVR8_INS_BRGE:			out << "ASMAVR8_INS_BRGE";		break;
		case ASMAVR8_INS_BRLT:			out << "ASMAVR8_INS_BRLT";		break;
		case ASMAVR8_INS_BRHS:			out << "ASMAVR8_INS_BRHS";		break;
		case ASMAVR8_INS_BRHC:			out << "ASMAVR8_INS_BRHC";		break;
		case ASMAVR8_INS_BRTS:			out << "ASMAVR8_INS_BRTS";		break;
		case ASMAVR8_INS_BRTC:			out << "ASMAVR8_INS_BRTC";		break;
		case ASMAVR8_INS_BRVS:			out << "ASMAVR8_INS_BRVS";		break;
		case ASMAVR8_INS_BRVC:			out << "ASMAVR8_INS_BRVC";		break;
		case ASMAVR8_INS_BRIE:			out << "ASMAVR8_INS_BRIE";		break;
		case ASMAVR8_INS_BRID:			out << "ASMAVR8_INS_BRID";		break;
		case ASMAVR8_INS_MOV:			out << "ASMAVR8_INS_MOV";		break;
		case ASMAVR8_INS_LDI:			out << "ASMAVR8_INS_LDI";		break;
		case ASMAVR8_INS_LDS:			out << "ASMAVR8_INS_LDS";		break;
		case ASMAVR8_INS_LD:			out << "ASMAVR8_INS_LD";		break;
		case ASMAVR8_INS_STS:			out << "ASMAVR8_INS_STS";		break;
		case ASMAVR8_INS_ST:			out << "ASMAVR8_INS_ST";		break;
		case ASMAVR8_INS_STD:			out << "ASMAVR8_INS_STD";		break;
		case ASMAVR8_INS_LPM:			out << "ASMAVR8_INS_LPM";		break;
		case ASMAVR8_INS_IN:			out << "ASMAVR8_INS_IN";		break;
		case ASMAVR8_INS_OUT:			out << "ASMAVR8_INS_OUT";		break;
		case ASMAVR8_INS_PUSH:			out << "ASMAVR8_INS_PUSH";		break;
		case ASMAVR8_INS_POP:			out << "ASMAVR8_INS_POP";		break;
		case ASMAVR8_INS_LSL:			out << "ASMAVR8_INS_LSL";		break;
		case ASMAVR8_INS_LSR:			out << "ASMAVR8_INS_LSR";		break;
		case ASMAVR8_INS_ROL:			out << "ASMAVR8_INS_ROL";		break;
		case ASMAVR8_INS_ROR:			out << "ASMAVR8_INS_ROR";		break;
		case ASMAVR8_INS_ASR:			out << "ASMAVR8_INS_ASR";		break;
		case ASMAVR8_INS_SWAP:			out << "ASMAVR8_INS_SWAP";		break;
		case ASMAVR8_INS_BSET:			out << "ASMAVR8_INS_BSET";		break;
		case ASMAVR8_INS_BCLR:			out << "ASMAVR8_INS_BCLR";		break;
		case ASMAVR8_INS_SBI:			out << "ASMAVR8_INS_SBI";		break;
		case ASMAVR8_INS_CBI:			out << "ASMAVR8_INS_CBI";		break;
		case ASMAVR8_INS_BST:			out << "ASMAVR8_INS_BST";		break;
		case ASMAVR8_INS_BLD:			out << "ASMAVR8_INS_BLD";		break;
		case ASMAVR8_INS_SEC:			out << "ASMAVR8_INS_SEC";		break;
		case ASMAVR8_INS_CLC:			out << "ASMAVR8_INS_CLC";		break;
		case ASMAVR8_INS_SEN:			out << "ASMAVR8_INS_SEN";		break;
		case ASMAVR8_INS_CLN:			out << "ASMAVR8_INS_CLN";		break;
		case ASMAVR8_INS_SEZ:			out << "ASMAVR8_INS_SEZ";		break;
		case ASMAVR8_INS_CLZ:			out << "ASMAVR8_INS_CLZ";		break;
		case ASMAVR8_INS_SEI:			out << "ASMAVR8_INS_SEI";		break;
		case ASMAVR8_INS_CLI:			out << "ASMAVR8_INS_CLI";		break;
		case ASMAVR8_INS_SES:			out << "ASMAVR8_INS_SES";		break;
		case ASMAVR8_INS_CLS:			out << "ASMAVR8_INS_CLS";		break;
		case ASMAVR8_INS_SEV:			out << "ASMAVR8_INS_SEV";		break;
		case ASMAVR8_INS_CLV:			out << "ASMAVR8_INS_CLV";		break;
		case ASMAVR8_INS_SET:			out << "ASMAVR8_INS_SET";		break;
		case ASMAVR8_INS_CLT:			out << "ASMAVR8_INS_CLT";		break;
		case ASMAVR8_INS_SEH:			out << "ASMAVR8_INS_SEH";		break;
		case ASMAVR8_INS_CLH:			out << "ASMAVR8_INS_CLH";		break;
		case ASMAVR8_INS_NOP:			out << "ASMAVR8_INS_NOP";		break;
		case ASMAVR8_INS_SLEEP:			out << "ASMAVR8_INS_SLEEP";		break;
		case ASMAVR8_INS_WDR:			out << "ASMAVR8_INS_WDR";		break;

		case ASM51_LABEL:			out << "ASM51_LABEL";			break;
		case ASM51_MACRO:			out << "ASM51_MACRO";			break;
		case ASM51_INCLUDE:			out << "ASM51_INCLUDE";			break;
		case ASM51_COND_ASM:			out << "ASM51_COND_ASM";		break;
		case ASM51_DIR_MACRO:			out << "ASM51_DIR_MACRO";		break;
		case ASM51_DIR_DB:			out << "ASM51_DIR_DB";			break;
		case ASM51_DIR_BIT:			out << "ASM51_DIR_BIT";			break;
		case ASM51_DIR_BSEG:			out << "ASM51_DIR_BSEG";		break;
		case ASM51_DIR_BYTE:			out << "ASM51_DIR_BYTE";		break;
		case ASM51_DIR_CODE:			out << "ASM51_DIR_CODE";		break;
		case ASM51_DIR_CSEG:			out << "ASM51_DIR_CSEG";		break;
		case ASM51_DIR_DATA:			out << "ASM51_DIR_DATA";		break;
		case ASM51_DIR_DBIT:			out << "ASM51_DIR_DBIT";		break;
		case ASM51_DIR_DS:			out << "ASM51_DIR_DS";			break;
		case ASM51_DIR_DSEG:			out << "ASM51_DIR_DSEG";		break;
		case ASM51_DIR_DW:			out << "ASM51_DIR_DW";			break;
		case ASM51_DIR_ELSE:			out << "ASM51_DIR_ELSE";		break;
		case ASM51_DIR_ELSEIF:			out << "ASM51_DIR_ELSEIF";		break;
		case ASM51_DIR_ELSEIFDEF:		out << "ASM51_DIR_ELSEIFDEF";		break;
		case ASM51_DIR_ELSEIFN:			out << "ASM51_DIR_ELSEIFN";		break;
		case ASM51_DIR_ELSEIFNDEF:		out << "ASM51_DIR_ELSEIFNDEF";		break;
		case ASM51_DIR_END:			out << "ASM51_DIR_END";			break;
		case ASM51_DIR_EQU:			out << "ASM51_DIR_EQU";			break;
		case ASM51_DIR_FLAG:			out << "ASM51_DIR_FLAG";		break;
		case ASM51_DIR_IDATA:			out << "ASM51_DIR_IDATA";		break;
		case ASM51_DIR_IF:			out << "ASM51_DIR_IF";			break;
		case ASM51_DIR_IFDEF:			out << "ASM51_DIR_IFDEF";		break;
		case ASM51_DIR_IFN:			out << "ASM51_DIR_IFN";			break;
		case ASM51_DIR_IFNDEF:			out << "ASM51_DIR_IFNDEF";		break;
		case ASM51_DIR_ISEG:			out << "ASM51_DIR_ISEG";		break;
		case ASM51_DIR_LIST:			out << "ASM51_DIR_LIST";		break;
		case ASM51_DIR_NAME:			out << "ASM51_DIR_NAME";		break;
		case ASM51_DIR_NOLIST:			out << "ASM51_DIR_NOLIST";		break;
		case ASM51_DIR_ORG:			out << "ASM51_DIR_ORG";			break;
		case ASM51_DIR_REPT:			out << "ASM51_DIR_REPT";		break;
		case ASM51_DIR_SET:			out << "ASM51_DIR_SET";			break;
		case ASM51_DIR_SKIP:			out << "ASM51_DIR_SKIP";		break;
		case ASM51_DIR_USING:			out << "ASM51_DIR_USING";		break;
		case ASM51_DIR_XDATA:			out << "ASM51_DIR_XDATA";		break;
		case ASM51_DIR_XSEG:			out << "ASM51_DIR_XSEG";		break;
		case ASM51_DIR_LOCAL:			out << "ASM51_DIR_LOCAL";		break;
		case ASM51_DIR_ELSEIFNB:		out << "ASM51_DIR_ELSEIFNB";		break;
		case ASM51_DIR_EXITM:			out << "ASM51_DIR_EXITM";		break;
		case ASM51_DIR_ELSEIFB:			out << "ASM51_DIR_ELSEIFB";		break;
		case ASM51_DIR_IFNB:			out << "ASM51_DIR_IFNB";		break;
		case ASM51_DIR_IFB:			out << "ASM51_DIR_IFB";			break;
		case ASM51_CNT_COND:			out << "ASM51_CNT_COND";		break;
		case ASM51_CNT_NODEBUG:			out << "ASM51_CNT_NODEBUG";		break;
		case ASM51_CNT_NOSYMBOLS:		out << "ASM51_CNT_NOSYMBOLS";		break;
		case ASM51_CNT_CONDONLY:		out << "ASM51_CNT_CONDONLY";		break;
		case ASM51_CNT_NOTABS:			out << "ASM51_CNT_NOTABS";		break;
		case ASM51_CNT_NOGEN:			out << "ASM51_CNT_NOGEN";		break;
		case ASM51_CNT_SAVE:			out << "ASM51_CNT_SAVE";		break;
		case ASM51_CNT_DATE:			out << "ASM51_CNT_DATE";		break;
		case ASM51_CNT_NOXREF:			out << "ASM51_CNT_NOXREF";		break;
		case ASM51_CNT_LIST:			out << "ASM51_CNT_LIST";		break;
		case ASM51_CNT_NOLIST:			out << "ASM51_CNT_NOLIST";		break;
		case ASM51_CNT_PAGELENGTH:		out << "ASM51_CNT_PAGELENGTH";		break;
		case ASM51_CNT_SYMBOLS:			out << "ASM51_CNT_SYMBOLS";		break;
		case ASM51_CNT_DEBUG:			out << "ASM51_CNT_DEBUG";		break;
		case ASM51_CNT_MACRO:			out << "ASM51_CNT_MACRO";		break;
		case ASM51_CNT_NOMACRO:			out << "ASM51_CNT_NOMACRO";		break;
		case ASM51_CNT_PAGEWIDTH:		out << "ASM51_CNT_PAGEWIDTH";		break;
		case ASM51_CNT_TITLE:			out << "ASM51_CNT_TITLE";		break;
		case ASM51_CNT_PAGING:			out << "ASM51_CNT_PAGING";		break;
		case ASM51_CNT_EJECT:			out << "ASM51_CNT_EJECT";		break;
		case ASM51_CNT_MOD51:			out << "ASM51_CNT_MOD51";		break;
		case ASM51_CNT_NOMOD51:			out << "ASM51_CNT_NOMOD51";		break;
		case ASM51_CNT_PHILIPS:			out << "ASM51_CNT_PHILIPS";		break;
		case ASM51_CNT_WARNING:			out << "ASM51_CNT_WARNING";		break;
		case ASM51_CNT_ERROR:			out << "ASM51_CNT_ERROR";		break;
		case ASM51_CNT_NOBUILTIN:		out << "ASM51_CNT_NOBUILTIN";		break;
		case ASM51_CNT_NOPAGING:		out << "ASM51_CNT_NOPAGING";		break;
		case ASM51_CNT_XREF:			out << "ASM51_CNT_XREF";		break;
		case ASM51_CNT_NOCOND:			out << "ASM51_CNT_NOCOND";		break;
		case ASM51_CNT_GEN:			out << "ASM51_CNT_GEN";			break;
		case ASM51_CNT_GENONLY:			out << "ASM51_CNT_GENONLY";		break;
		case ASM51_CNT_RESTORE:			out << "ASM51_CNT_RESTORE";		break;
		case ASM51_INS_ACALL:			out << "ASM51_INS_ACALL";		break;
		case ASM51_INS_ADD__A_IMM:		out << "ASM51_INS_ADD__A_IMM";		break;
		case ASM51_INS_ADD__A_DIR:		out << "ASM51_INS_ADD__A_DIR";		break;
		case ASM51_INS_ADD__A_IR0:		out << "ASM51_INS_ADD__A_IR0";		break;
		case ASM51_INS_ADD__A_IR1:		out << "ASM51_INS_ADD__A_IR1";		break;
		case ASM51_INS_ADD__A_R0:		out << "ASM51_INS_ADD__A_R0";		break;
		case ASM51_INS_ADD__A_R1:		out << "ASM51_INS_ADD__A_R1";		break;
		case ASM51_INS_ADD__A_R2:		out << "ASM51_INS_ADD__A_R2";		break;
		case ASM51_INS_ADD__A_R3:		out << "ASM51_INS_ADD__A_R3";		break;
		case ASM51_INS_ADD__A_R4:		out << "ASM51_INS_ADD__A_R4";		break;
		case ASM51_INS_ADD__A_R5:		out << "ASM51_INS_ADD__A_R5";		break;
		case ASM51_INS_ADD__A_R6:		out << "ASM51_INS_ADD__A_R6";		break;
		case ASM51_INS_ADD__A_R7:		out << "ASM51_INS_ADD__A_R7";		break;
		case ASM51_INS_ADDC__A_IMM:		out << "ASM51_INS_ADDC__A_IMM";		break;
		case ASM51_INS_ADDC__A_DIR:		out << "ASM51_INS_ADDC__A_DIR";		break;
		case ASM51_INS_ADDC__A_IR0:		out << "ASM51_INS_ADDC__A_IR0";		break;
		case ASM51_INS_ADDC__A_IR1:		out << "ASM51_INS_ADDC__A_IR1";		break;
		case ASM51_INS_ADDC__A_R0:		out << "ASM51_INS_ADDC__A_R0";		break;
		case ASM51_INS_ADDC__A_R1:		out << "ASM51_INS_ADDC__A_R1";		break;
		case ASM51_INS_ADDC__A_R2:		out << "ASM51_INS_ADDC__A_R2";		break;
		case ASM51_INS_ADDC__A_R3:		out << "ASM51_INS_ADDC__A_R3";		break;
		case ASM51_INS_ADDC__A_R4:		out << "ASM51_INS_ADDC__A_R4";		break;
		case ASM51_INS_ADDC__A_R5:		out << "ASM51_INS_ADDC__A_R5";		break;
		case ASM51_INS_ADDC__A_R6:		out << "ASM51_INS_ADDC__A_R6";		break;
		case ASM51_INS_ADDC__A_R7:		out << "ASM51_INS_ADDC__A_R7";		break;
		case ASM51_INS_AJMP__DIR:		out << "ASM51_INS_AJMP__DIR";		break;
		case ASM51_INS_ANL__DIR_A:		out << "ASM51_INS_ANL__DIR_A";		break;
		case ASM51_INS_ANL__DIR_IMM:		out << "ASM51_INS_ANL__DIR_IMM";	break;
		case ASM51_INS_ANL__A_IMM:		out << "ASM51_INS_ANL__A_IMM";		break;
		case ASM51_INS_ANL__A_DIR:		out << "ASM51_INS_ANL__A_DIR";		break;
		case ASM51_INS_ANL__A_IR0:		out << "ASM51_INS_ANL__A_IR0";		break;
		case ASM51_INS_ANL__A_IR1:		out << "ASM51_INS_ANL__A_IR1";		break;
		case ASM51_INS_ANL__A_R0:		out << "ASM51_INS_ANL__A_R0";		break;
		case ASM51_INS_ANL__A_R1:		out << "ASM51_INS_ANL__A_R1";		break;
		case ASM51_INS_ANL__A_R2:		out << "ASM51_INS_ANL__A_R2";		break;
		case ASM51_INS_ANL__A_R3:		out << "ASM51_INS_ANL__A_R3";		break;
		case ASM51_INS_ANL__A_R4:		out << "ASM51_INS_ANL__A_R4";		break;
		case ASM51_INS_ANL__A_R5:		out << "ASM51_INS_ANL__A_R5";		break;
		case ASM51_INS_ANL__A_R6:		out << "ASM51_INS_ANL__A_R6";		break;
		case ASM51_INS_ANL__A_R7:		out << "ASM51_INS_ANL__A_R7";		break;
		case ASM51_INS_ANL__C_DIR:		out << "ASM51_INS_ANL__C_DIR";		break;
		case ASM51_INS_ANL__C_SL_DIR:		out << "ASM51_INS_ANL__C_SL_DIR";	break;
		case ASM51_INS_CJNE__A_IMM_DIR:		out << "ASM51_INS_CJNE__A_IMM_DIR";	break;
		case ASM51_INS_CJNE__A_DIR_DIR:		out << "ASM51_INS_CJNE__A_DIR_DIR";	break;
		case ASM51_INS_CJNE__IR0_IMM_DIR:	out << "ASM51_INS_CJNE__IR0_IMM_DIR";	break;
		case ASM51_INS_CJNE__IR1_IMM_DIR:	out << "ASM51_INS_CJNE__IR1_IMM_DIR";	break;
		case ASM51_INS_CJNE__R0_IMM_DIR:	out << "ASM51_INS_CJNE__R0_IMM_DIR";	break;
		case ASM51_INS_CJNE__R1_IMM_DIR:	out << "ASM51_INS_CJNE__R1_IMM_DIR";	break;
		case ASM51_INS_CJNE__R2_IMM_DIR:	out << "ASM51_INS_CJNE__R2_IMM_DIR";	break;
		case ASM51_INS_CJNE__R3_IMM_DIR:	out << "ASM51_INS_CJNE__R3_IMM_DIR";	break;
		case ASM51_INS_CJNE__R4_IMM_DIR:	out << "ASM51_INS_CJNE__R4_IMM_DIR";	break;
		case ASM51_INS_CJNE__R5_IMM_DIR:	out << "ASM51_INS_CJNE__R5_IMM_DIR";	break;
		case ASM51_INS_CJNE__R6_IMM_DIR:	out << "ASM51_INS_CJNE__R6_IMM_DIR";	break;
		case ASM51_INS_CJNE__R7_IMM_DIR:	out << "ASM51_INS_CJNE__R7_IMM_DIR";	break;
		case ASM51_INS_CLR__DIR:		out << "ASM51_INS_CLR__DIR";		break;
		case ASM51_INS_CLR__C:			out << "ASM51_INS_CLR__C";		break;
		case ASM51_INS_CLR__A:			out << "ASM51_INS_CLR__A";		break;
		case ASM51_INS_CPL__DIR:		out << "ASM51_INS_CPL__DIR";		break;
		case ASM51_INS_CPL__C:			out << "ASM51_INS_CPL__C";		break;
		case ASM51_INS_CPL__A:			out << "ASM51_INS_CPL__A";		break;
		case ASM51_INS_DA__A:			out << "ASM51_INS_DA__A";		break;
		case ASM51_INS_DEC__DIR:		out << "ASM51_INS_DEC__DIR";		break;
		case ASM51_INS_DEC__A:			out << "ASM51_INS_DEC__A";		break;
		case ASM51_INS_DEC__IR0:		out << "ASM51_INS_DEC__IR0";		break;
		case ASM51_INS_DEC__IR1:		out << "ASM51_INS_DEC__IR1";		break;
		case ASM51_INS_DEC__R0:			out << "ASM51_INS_DEC__R0";		break;
		case ASM51_INS_DEC__R1:			out << "ASM51_INS_DEC__R1";		break;
		case ASM51_INS_DEC__R2:			out << "ASM51_INS_DEC__R2";		break;
		case ASM51_INS_DEC__R3:			out << "ASM51_INS_DEC__R3";		break;
		case ASM51_INS_DEC__R4:			out << "ASM51_INS_DEC__R4";		break;
		case ASM51_INS_DEC__R5:			out << "ASM51_INS_DEC__R5";		break;
		case ASM51_INS_DEC__R6:			out << "ASM51_INS_DEC__R6";		break;
		case ASM51_INS_DEC__R7:			out << "ASM51_INS_DEC__R7";		break;
		case ASM51_INS_DIV__AB:			out << "ASM51_INS_DIV__AB";		break;
		case ASM51_INS_DJNZ__DIR_DIR:		out << "ASM51_INS_DJNZ__DIR_DIR";	break;
		case ASM51_INS_DJNZ__R0_DIR:		out << "ASM51_INS_DJNZ__R0_DIR";	break;
		case ASM51_INS_DJNZ__R1_DIR:		out << "ASM51_INS_DJNZ__R1_DIR";	break;
		case ASM51_INS_DJNZ__R2_DIR:		out << "ASM51_INS_DJNZ__R2_DIR";	break;
		case ASM51_INS_DJNZ__R3_DIR:		out << "ASM51_INS_DJNZ__R3_DIR";	break;
		case ASM51_INS_DJNZ__R4_DIR:		out << "ASM51_INS_DJNZ__R4_DIR";	break;
		case ASM51_INS_DJNZ__R5_DIR:		out << "ASM51_INS_DJNZ__R5_DIR";	break;
		case ASM51_INS_DJNZ__R6_DIR:		out << "ASM51_INS_DJNZ__R6_DIR";	break;
		case ASM51_INS_DJNZ__R7_DIR:		out << "ASM51_INS_DJNZ__R7_DIR";	break;
		case ASM51_INS_INC__A:			out << "ASM51_INS_INC__A";		break;
		case ASM51_INS_INC__DIR:		out << "ASM51_INS_INC__DIR";		break;
		case ASM51_INS_INC__IR0:		out << "ASM51_INS_INC__IR0";		break;
		case ASM51_INS_INC__IR1:		out << "ASM51_INS_INC__IR1";		break;
		case ASM51_INS_INC__R0:			out << "ASM51_INS_INC__R0";		break;
		case ASM51_INS_INC__R1:			out << "ASM51_INS_INC__R1";		break;
		case ASM51_INS_INC__R2:			out << "ASM51_INS_INC__R2";		break;
		case ASM51_INS_INC__R3:			out << "ASM51_INS_INC__R3";		break;
		case ASM51_INS_INC__R4:			out << "ASM51_INS_INC__R4";		break;
		case ASM51_INS_INC__R5:			out << "ASM51_INS_INC__R5";		break;
		case ASM51_INS_INC__R6:			out << "ASM51_INS_INC__R6";		break;
		case ASM51_INS_INC__R7:			out << "ASM51_INS_INC__R7";		break;
		case ASM51_INS_INC__DPTR:		out << "ASM51_INS_INC__DPTR";		break;
		case ASM51_INS_JB__DIR_DIR:		out << "ASM51_INS_JB__DIR_DIR";		break;
		case ASM51_INS_JBC__DIR_DIR:		out << "ASM51_INS_JBC__DIR_DIR";	break;
		case ASM51_INS_JC__DIR:			out << "ASM51_INS_JC__DIR";		break;
		case ASM51_INS_JMP__IA_DPTR:		out << "ASM51_INS_JMP__IA_DPTR";	break;
		case ASM51_INS_JMP__DIR:		out << "ASM51_INS_JMP__DIR";		break;
		case ASM51_INS_JNB__DIR:		out << "ASM51_INS_JNB__DIR";		break;
		case ASM51_INS_JNC__DIR:		out << "ASM51_INS_JNC__DIR";		break;
		case ASM51_INS_JNZ__DIR:		out << "ASM51_INS_JNZ__DIR";		break;
		case ASM51_INS_JZ__DIR:			out << "ASM51_INS_JZ__DIR";		break;
		case ASM51_INS_LCALL__DIR:		out << "ASM51_INS_LCALL__DIR";		break;
		case ASM51_INS_LJMP__DIR:		out << "ASM51_INS_LJMP__DIR";		break;
		case ASM51_INS_MOV__DIR_DIR:		out << "ASM51_INS_MOV__DIR_DIR";	break;
		case ASM51_INS_MOV__DIR_IMM:		out << "ASM51_INS_MOV__DIR_IMM";	break;
		case ASM51_INS_MOV__DIR_IR0:		out << "ASM51_INS_MOV__DIR_IR0";	break;
		case ASM51_INS_MOV__DIR_IR1:		out << "ASM51_INS_MOV__DIR_IR1";	break;
		case ASM51_INS_MOV__DIR_R0:		out << "ASM51_INS_MOV__DIR_R0";		break;
		case ASM51_INS_MOV__DIR_R1:		out << "ASM51_INS_MOV__DIR_R1";		break;
		case ASM51_INS_MOV__DIR_R2:		out << "ASM51_INS_MOV__DIR_R2";		break;
		case ASM51_INS_MOV__DIR_R3:		out << "ASM51_INS_MOV__DIR_R3";		break;
		case ASM51_INS_MOV__DIR_R4:		out << "ASM51_INS_MOV__DIR_R4";		break;
		case ASM51_INS_MOV__DIR_R5:		out << "ASM51_INS_MOV__DIR_R5";		break;
		case ASM51_INS_MOV__DIR_R6:		out << "ASM51_INS_MOV__DIR_R6";		break;
		case ASM51_INS_MOV__DIR_R7:		out << "ASM51_INS_MOV__DIR_R7";		break;
		case ASM51_INS_MOV__DIR_C:		out << "ASM51_INS_MOV__DIR_C";		break;
		case ASM51_INS_MOV__DIR_A:		out << "ASM51_INS_MOV__DIR_A";		break;
		case ASM51_INS_MOV__IR0_IMM:		out << "ASM51_INS_MOV__IR0_IMM";	break;
		case ASM51_INS_MOV__IR1_IMM:		out << "ASM51_INS_MOV__IR1_IMM";	break;
		case ASM51_INS_MOV__IR0_DIR:		out << "ASM51_INS_MOV__IR0_DIR";	break;
		case ASM51_INS_MOV__IR1_DIR:		out << "ASM51_INS_MOV__IR1_DIR";	break;
		case ASM51_INS_MOV__IR0_A:		out << "ASM51_INS_MOV__IR0_A";		break;
		case ASM51_INS_MOV__IR1_A:		out << "ASM51_INS_MOV__IR1_A";		break;
		case ASM51_INS_MOV__DPTR_IMM:		out << "ASM51_INS_MOV__DPTR_IMM";	break;
		case ASM51_INS_MOV__R0_IMM:		out << "ASM51_INS_MOV__R0_IMM";		break;
		case ASM51_INS_MOV__R1_IMM:		out << "ASM51_INS_MOV__R1_IMM";		break;
		case ASM51_INS_MOV__R2_IMM:		out << "ASM51_INS_MOV__R2_IMM";		break;
		case ASM51_INS_MOV__R3_IMM:		out << "ASM51_INS_MOV__R3_IMM";		break;
		case ASM51_INS_MOV__R4_IMM:		out << "ASM51_INS_MOV__R4_IMM";		break;
		case ASM51_INS_MOV__R5_IMM:		out << "ASM51_INS_MOV__R5_IMM";		break;
		case ASM51_INS_MOV__R6_IMM:		out << "ASM51_INS_MOV__R6_IMM";		break;
		case ASM51_INS_MOV__R7_IMM:		out << "ASM51_INS_MOV__R7_IMM";		break;
		case ASM51_INS_MOV__R0_DIR:		out << "ASM51_INS_MOV__R0_DIR";		break;
		case ASM51_INS_MOV__R1_DIR:		out << "ASM51_INS_MOV__R1_DIR";		break;
		case ASM51_INS_MOV__R2_DIR:		out << "ASM51_INS_MOV__R2_DIR";		break;
		case ASM51_INS_MOV__R3_DIR:		out << "ASM51_INS_MOV__R3_DIR";		break;
		case ASM51_INS_MOV__R4_DIR:		out << "ASM51_INS_MOV__R4_DIR";		break;
		case ASM51_INS_MOV__R5_DIR:		out << "ASM51_INS_MOV__R5_DIR";		break;
		case ASM51_INS_MOV__R6_DIR:		out << "ASM51_INS_MOV__R6_DIR";		break;
		case ASM51_INS_MOV__R7_DIR:		out << "ASM51_INS_MOV__R7_DIR";		break;
		case ASM51_INS_MOV__R0_A:		out << "ASM51_INS_MOV__R0_A";		break;
		case ASM51_INS_MOV__R1_A:		out << "ASM51_INS_MOV__R1_A";		break;
		case ASM51_INS_MOV__R2_A:		out << "ASM51_INS_MOV__R2_A";		break;
		case ASM51_INS_MOV__R3_A:		out << "ASM51_INS_MOV__R3_A";		break;
		case ASM51_INS_MOV__R4_A:		out << "ASM51_INS_MOV__R4_A";		break;
		case ASM51_INS_MOV__R5_A:		out << "ASM51_INS_MOV__R5_A";		break;
		case ASM51_INS_MOV__R6_A:		out << "ASM51_INS_MOV__R6_A";		break;
		case ASM51_INS_MOV__R7_A:		out << "ASM51_INS_MOV__R7_A";		break;
		case ASM51_INS_MOV__A_IMM:		out << "ASM51_INS_MOV__A_IMM";		break;
		case ASM51_INS_MOV__A_IR0:		out << "ASM51_INS_MOV__A_IR0";		break;
		case ASM51_INS_MOV__A_IR1:		out << "ASM51_INS_MOV__A_IR1";		break;
		case ASM51_INS_MOV__A_R0:		out << "ASM51_INS_MOV__A_R0";		break;
		case ASM51_INS_MOV__A_R1:		out << "ASM51_INS_MOV__A_R1";		break;
		case ASM51_INS_MOV__A_R2:		out << "ASM51_INS_MOV__A_R2";		break;
		case ASM51_INS_MOV__A_R3:		out << "ASM51_INS_MOV__A_R3";		break;
		case ASM51_INS_MOV__A_R4:		out << "ASM51_INS_MOV__A_R4";		break;
		case ASM51_INS_MOV__A_R5:		out << "ASM51_INS_MOV__A_R5";		break;
		case ASM51_INS_MOV__A_R6:		out << "ASM51_INS_MOV__A_R6";		break;
		case ASM51_INS_MOV__A_R7:		out << "ASM51_INS_MOV__A_R7";		break;
		case ASM51_INS_MOV__A_DIR:		out << "ASM51_INS_MOV__A_DIR";		break;
		case ASM51_INS_MOV__C_DIR:		out << "ASM51_INS_MOV__C_DIR";		break;
		case ASM51_INS_MOVC__A_IA_DPTR:		out << "ASM51_INS_MOVC__A_IA_DPTR";	break;
		case ASM51_INS_MOVC__A_IA_PC:		out << "ASM51_INS_MOVC__A_IA_PC";	break;
		case ASM51_INS_MOVX__IDPTR_A:		out << "ASM51_INS_MOVX__IDPTR_A";	break;
		case ASM51_INS_MOVX__IR0_A:		out << "ASM51_INS_MOVX__IR0_A";		break;
		case ASM51_INS_MOVX__IR1_A:		out << "ASM51_INS_MOVX__IR1_A";		break;
		case ASM51_INS_MOVX__A_IDPTR:		out << "ASM51_INS_MOVX__A_IDPTR";	break;
		case ASM51_INS_MOVX__A_IR0:		out << "ASM51_INS_MOVX__A_IR0";		break;
		case ASM51_INS_MOVX__A_IR1:		out << "ASM51_INS_MOVX__A_IR1";		break;
		case ASM51_INS_MUL__AB:			out << "ASM51_INS_MUL__AB";		break;
		case ASM51_INS_NOP:			out << "ASM51_INS_NOP";			break;
		case ASM51_INS_ORL__DIR_A:		out << "ASM51_INS_ORL__DIR_A";		break;
		case ASM51_INS_ORL__DIR_IMM:		out << "ASM51_INS_ORL__DIR_IMM";	break;
		case ASM51_INS_ORL__A_IMM:		out << "ASM51_INS_ORL__A_IMM";		break;
		case ASM51_INS_ORL__A_DIR:		out << "ASM51_INS_ORL__A_DIR";		break;
		case ASM51_INS_ORL__A_IR0:		out << "ASM51_INS_ORL__A_IR0";		break;
		case ASM51_INS_ORL__A_IR1:		out << "ASM51_INS_ORL__A_IR1";		break;
		case ASM51_INS_ORL__A_R0:		out << "ASM51_INS_ORL__A_R0";		break;
		case ASM51_INS_ORL__A_R1:		out << "ASM51_INS_ORL__A_R1";		break;
		case ASM51_INS_ORL__A_R2:		out << "ASM51_INS_ORL__A_R2";		break;
		case ASM51_INS_ORL__A_R3:		out << "ASM51_INS_ORL__A_R3";		break;
		case ASM51_INS_ORL__A_R4:		out << "ASM51_INS_ORL__A_R4";		break;
		case ASM51_INS_ORL__A_R5:		out << "ASM51_INS_ORL__A_R5";		break;
		case ASM51_INS_ORL__A_R6:		out << "ASM51_INS_ORL__A_R6";		break;
		case ASM51_INS_ORL__A_R7:		out << "ASM51_INS_ORL__A_R7";		break;
		case ASM51_INS_ORL__C_DIR:		out << "ASM51_INS_ORL__C_DIR";		break;
		case ASM51_INS_ORL__C_SL_DIR:		out << "ASM51_INS_ORL__C_SL_DIR";	break;
		case ASM51_INS_POP__DIR:		out << "ASM51_INS_POP__DIR";		break;
		case ASM51_INS_PUSH__DIR:		out << "ASM51_INS_PUSH__DIR";		break;
		case ASM51_INS_RET:			out << "ASM51_INS_RET";			break;
		case ASM51_INS_RETI:			out << "ASM51_INS_RETI";		break;
		case ASM51_INS_RL__A:			out << "ASM51_INS_RL__A";		break;
		case ASM51_INS_RLC__A:			out << "ASM51_INS_RLC__A";		break;
		case ASM51_INS_RR__A:			out << "ASM51_INS_RR__A";		break;
		case ASM51_INS_RRC__A:			out << "ASM51_INS_RRC__A";		break;
		case ASM51_INS_SETB__C:			out << "ASM51_INS_SETB__C";		break;
		case ASM51_INS_SETB__DIR:		out << "ASM51_INS_SETB__DIR";		break;
		case ASM51_INS_SJMP__DIR:		out << "ASM51_INS_SJMP__DIR";		break;
		case ASM51_INS_SUBB__A_IMM:		out << "ASM51_INS_SUBB__A_IMM";		break;
		case ASM51_INS_SUBB__A_DIR:		out << "ASM51_INS_SUBB__A_DIR";		break;
		case ASM51_INS_SUBB__A_IR0:		out << "ASM51_INS_SUBB__A_IR0";		break;
		case ASM51_INS_SUBB__A_IR1:		out << "ASM51_INS_SUBB__A_IR1";		break;
		case ASM51_INS_SUBB__A_R0:		out << "ASM51_INS_SUBB__A_R0";		break;
		case ASM51_INS_SUBB__A_R1:		out << "ASM51_INS_SUBB__A_R1";		break;
		case ASM51_INS_SUBB__A_R2:		out << "ASM51_INS_SUBB__A_R2";		break;
		case ASM51_INS_SUBB__A_R3:		out << "ASM51_INS_SUBB__A_R3";		break;
		case ASM51_INS_SUBB__A_R4:		out << "ASM51_INS_SUBB__A_R4";		break;
		case ASM51_INS_SUBB__A_R5:		out << "ASM51_INS_SUBB__A_R5";		break;
		case ASM51_INS_SUBB__A_R6:		out << "ASM51_INS_SUBB__A_R6";		break;
		case ASM51_INS_SUBB__A_R7:		out << "ASM51_INS_SUBB__A_R7";		break;
		case ASM51_INS_SWAP__A:			out << "ASM51_INS_SWAP__A";		break;
		case ASM51_INS_XCH__A_IR0:		out << "ASM51_INS_XCH__A_IR0";		break;
		case ASM51_INS_XCH__A_IR1:		out << "ASM51_INS_XCH__A_IR1";		break;
		case ASM51_INS_XCH__A_R0:		out << "ASM51_INS_XCH__A_R0";		break;
		case ASM51_INS_XCH__A_R1:		out << "ASM51_INS_XCH__A_R1";		break;
		case ASM51_INS_XCH__A_R2:		out << "ASM51_INS_XCH__A_R2";		break;
		case ASM51_INS_XCH__A_R3:		out << "ASM51_INS_XCH__A_R3";		break;
		case ASM51_INS_XCH__A_R4:		out << "ASM51_INS_XCH__A_R4";		break;
		case ASM51_INS_XCH__A_R5:		out << "ASM51_INS_XCH__A_R5";		break;
		case ASM51_INS_XCH__A_R6:		out << "ASM51_INS_XCH__A_R6";		break;
		case ASM51_INS_XCH__A_R7:		out << "ASM51_INS_XCH__A_R7";		break;
		case ASM51_INS_XCH__A_DIR:		out << "ASM51_INS_XCH__A_DIR";		break;
		case ASM51_INS_XCHD__A_IR0:		out << "ASM51_INS_XCHD__A_IR0";		break;
		case ASM51_INS_XCHD__A_IR1:		out << "ASM51_INS_XCHD__A_IR1";		break;
		case ASM51_INS_XRL__DIR_A:		out << "ASM51_INS_XRL__DIR_A";		break;
		case ASM51_INS_XRL__DIR_IMM:		out << "ASM51_INS_XRL__DIR_IMM";	break;
		case ASM51_INS_XRL__A_IMM:		out << "ASM51_INS_XRL__A_IMM";		break;
		case ASM51_INS_XRL__A_DIR:		out << "ASM51_INS_XRL__A_DIR";		break;
		case ASM51_INS_XRL__A_IR0:		out << "ASM51_INS_XRL__A_IR0";		break;
		case ASM51_INS_XRL__A_IR1:		out << "ASM51_INS_XRL__A_IR1";		break;
		case ASM51_INS_XRL__A_R0:		out << "ASM51_INS_XRL__A_R0";		break;
		case ASM51_INS_XRL__A_R1:		out << "ASM51_INS_XRL__A_R1";		break;
		case ASM51_INS_XRL__A_R2:		out << "ASM51_INS_XRL__A_R2";		break;
		case ASM51_INS_XRL__A_R3:		out << "ASM51_INS_XRL__A_R3";		break;
		case ASM51_INS_XRL__A_R4:		out << "ASM51_INS_XRL__A_R4";		break;
		case ASM51_INS_XRL__A_R5:		out << "ASM51_INS_XRL__A_R5";		break;
		case ASM51_INS_XRL__A_R6:		out << "ASM51_INS_XRL__A_R6";		break;
		case ASM51_INS_XRL__A_R7:		out << "ASM51_INS_XRL__A_R7";		break;
		case ASM51__MAX__:			out << "ASM51__MAX__";			break;
	}
	return out;
}

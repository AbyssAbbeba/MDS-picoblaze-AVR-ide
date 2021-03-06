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
 * @ingroup Compiler
 * @file CompilerStatementTypes.h
 */
// =============================================================================

#ifndef COMPILERSTATEMENTTYPES_H
#define COMPILERSTATEMENTTYPES_H

#include <ostream>

/**
 * @brief
 * @ingroup Assembler
 * @namespace CompilerStatementTypes
 */
namespace CompilerStatementTypes
{
    ////    Datatypes    ////

    /**
     * @brief
     */
    enum StatementType
    {
        EMPTY_STATEMENT = 0,                    ///<

        /// @name 8-bit AVR
        //@{
            ASMAVR8_MACRO,                      ///<
            ASMAVR8_LABEL,                      ///<
            ASMAVR8_INCLUDE,                    ///<
            ASMAVR8_COND_ASM,                   ///<

            ASMAVR8_DIR_IF,                     ///<
            ASMAVR8_DIR_IFN,                    ///<
            ASMAVR8_DIR_IFDEF,                  ///<
            ASMAVR8_DIR_IFNDEF,                 ///<
            ASMAVR8_DIR_ELSEIFB,                ///<
            ASMAVR8_DIR_ELSEIFNB,               ///<
            ASMAVR8_DIR_ELSE,                   ///<
            ASMAVR8_DIR_ELSEIF,                 ///<
            ASMAVR8_DIR_ELSEIFN,                ///<
            ASMAVR8_DIR_ELSEIFDEF,              ///<
            ASMAVR8_DIR_ELSEIFNDEF,             ///<
            ASMAVR8_DIR_LOCAL,                  ///<
            ASMAVR8_DIR_IFNB,                   ///<
            ASMAVR8_DIR_IFB,                    ///<
            ASMAVR8_DIR_SKIP,                   ///<
            ASMAVR8_DIR_EXITM,                  ///<
            ASMAVR8_DIR_REPT,                   ///<
            ASMAVR8_DIR_MACRO,                  ///<
            ASMAVR8_DIR_BYTE,                   ///<
            ASMAVR8_DIR_CSEG,                   ///<
            ASMAVR8_DIR_DB,                     ///<
            ASMAVR8_DIR_DEF,                    ///<
            ASMAVR8_DIR_DEVICE,                 ///<
            ASMAVR8_DIR_DSEG,                   ///<
            ASMAVR8_DIR_DW,                     ///<
            ASMAVR8_DIR_EQU,                    ///<
            ASMAVR8_DIR_ESEG,                   ///<
            ASMAVR8_DIR_EXIT,                   ///<
            ASMAVR8_DIR_INCLUDE,                ///<
            ASMAVR8_DIR_LIST,                   ///<
            ASMAVR8_DIR_LISTMAC,                ///<
            ASMAVR8_DIR_NOLIST,                 ///<
            ASMAVR8_DIR_ORG,                    ///<
            ASMAVR8_DIR_SET,                    ///<

            ASMAVR8_INS_ADD,                    ///<
            ASMAVR8_INS_ADC,                    ///<
            ASMAVR8_INS_ADIW,                   ///<
            ASMAVR8_INS_SUB,                    ///<
            ASMAVR8_INS_SUBI,                   ///<
            ASMAVR8_INS_SBC,                    ///<
            ASMAVR8_INS_SBCI,                   ///<
            ASMAVR8_INS_SBIW,                   ///<
            ASMAVR8_INS_AND,                    ///<
            ASMAVR8_INS_ANDI,                   ///<
            ASMAVR8_INS_OR,                     ///<
            ASMAVR8_INS_ORI,                    ///<
            ASMAVR8_INS_EOR,                    ///<
            ASMAVR8_INS_COM,                    ///<
            ASMAVR8_INS_NEG,                    ///<
            ASMAVR8_INS_SBR,                    ///<
            ASMAVR8_INS_CBR,                    ///<
            ASMAVR8_INS_INC,                    ///<
            ASMAVR8_INS_DEC,                    ///<
            ASMAVR8_INS_TST,                    ///<
            ASMAVR8_INS_CLR,                    ///<
            ASMAVR8_INS_SER,                    ///<
            ASMAVR8_INS_MUL,                    ///<
            ASMAVR8_INS_RJMP,                   ///<
            ASMAVR8_INS_IJMP,                   ///<
            ASMAVR8_INS_JMP,                    ///<
            ASMAVR8_INS_RCALL,                  ///<
            ASMAVR8_INS_ICALL,                  ///<
            ASMAVR8_INS_CALL,                   ///<
            ASMAVR8_INS_RET,                    ///<
            ASMAVR8_INS_RETI,                   ///<
            ASMAVR8_INS_CPSE,                   ///<
            ASMAVR8_INS_CP,                     ///<
            ASMAVR8_INS_CPC,                    ///<
            ASMAVR8_INS_CPI,                    ///<
            ASMAVR8_INS_SBRC,                   ///<
            ASMAVR8_INS_SBRS,                   ///<
            ASMAVR8_INS_SBIC,                   ///<
            ASMAVR8_INS_SBIS,                   ///<
            ASMAVR8_INS_BRBS,                   ///<
            ASMAVR8_INS_BRBC,                   ///<
            ASMAVR8_INS_BREQ,                   ///<
            ASMAVR8_INS_BRNE,                   ///<
            ASMAVR8_INS_BRCS,                   ///<
            ASMAVR8_INS_BRCC,                   ///<
            ASMAVR8_INS_BRSH,                   ///<
            ASMAVR8_INS_BRLO,                   ///<
            ASMAVR8_INS_BRMI,                   ///<
            ASMAVR8_INS_BRPL,                   ///<
            ASMAVR8_INS_BRGE,                   ///<
            ASMAVR8_INS_BRLT,                   ///<
            ASMAVR8_INS_BRHS,                   ///<
            ASMAVR8_INS_BRHC,                   ///<
            ASMAVR8_INS_BRTS,                   ///<
            ASMAVR8_INS_BRTC,                   ///<
            ASMAVR8_INS_BRVS,                   ///<
            ASMAVR8_INS_BRVC,                   ///<
            ASMAVR8_INS_BRIE,                   ///<
            ASMAVR8_INS_BRID,                   ///<
            ASMAVR8_INS_MOV,                    ///<
            ASMAVR8_INS_LDI,                    ///<
            ASMAVR8_INS_LDS,                    ///<
            ASMAVR8_INS_LD_X,                   ///<
            ASMAVR8_INS_LD_Xp,                  ///<
            ASMAVR8_INS_LD_mX,                  ///<
            ASMAVR8_INS_LD_Y,                   ///<
            ASMAVR8_INS_LD_Yp,                  ///<
            ASMAVR8_INS_LD_mY,                  ///<
            ASMAVR8_INS_LD_Z,                   ///<
            ASMAVR8_INS_LD_Zp,                  ///<
            ASMAVR8_INS_LD_mZ,                  ///<
            ASMAVR8_INS_LDD_Y,                  ///<
            ASMAVR8_INS_LDD_Z,                  ///<
            ASMAVR8_INS_STS,                    ///<
            ASMAVR8_INS_ST_X,                   ///<
            ASMAVR8_INS_ST_Xp,                  ///<
            ASMAVR8_INS_ST_mX,                  ///<
            ASMAVR8_INS_ST_Y,                   ///<
            ASMAVR8_INS_ST_Yp,                  ///<
            ASMAVR8_INS_ST_mY,                  ///<
            ASMAVR8_INS_ST_Z,                   ///<
            ASMAVR8_INS_ST_Zp,                  ///<
            ASMAVR8_INS_ST_mZ,                  ///<
            ASMAVR8_INS_STD_Y,                  ///<
            ASMAVR8_INS_STD_Z,                  ///<
            ASMAVR8_INS_LPM,                    ///<
            ASMAVR8_INS_IN,                     ///<
            ASMAVR8_INS_OUT,                    ///<
            ASMAVR8_INS_PUSH,                   ///<
            ASMAVR8_INS_POP,                    ///<
            ASMAVR8_INS_LSL,                    ///<
            ASMAVR8_INS_LSR,                    ///<
            ASMAVR8_INS_ROL,                    ///<
            ASMAVR8_INS_ROR,                    ///<
            ASMAVR8_INS_ASR,                    ///<
            ASMAVR8_INS_SWAP,                   ///<
            ASMAVR8_INS_BSET,                   ///<
            ASMAVR8_INS_BCLR,                   ///<
            ASMAVR8_INS_SBI,                    ///<
            ASMAVR8_INS_CBI,                    ///<
            ASMAVR8_INS_BST,                    ///<
            ASMAVR8_INS_BLD,                    ///<
            ASMAVR8_INS_SEC,                    ///<
            ASMAVR8_INS_CLC,                    ///<
            ASMAVR8_INS_SEN,                    ///<
            ASMAVR8_INS_CLN,                    ///<
            ASMAVR8_INS_SEZ,                    ///<
            ASMAVR8_INS_CLZ,                    ///<
            ASMAVR8_INS_SEI,                    ///<
            ASMAVR8_INS_CLI,                    ///<
            ASMAVR8_INS_SES,                    ///<
            ASMAVR8_INS_CLS,                    ///<
            ASMAVR8_INS_SEV,                    ///<
            ASMAVR8_INS_CLV,                    ///<
            ASMAVR8_INS_SET,                    ///<
            ASMAVR8_INS_CLT,                    ///<
            ASMAVR8_INS_SEH,                    ///<
            ASMAVR8_INS_CLH,                    ///<
            ASMAVR8_INS_NOP,                    ///<
            ASMAVR8_INS_SLEEP,                  ///<
            ASMAVR8_INS_WDR,                    ///<
        //@}

        /// @name 8-bit PIC
        //@{
            ASMPIC8_MACRO,                      ///<
            ASMPIC8_LABEL,                      ///<
            ASMPIC8_INCLUDE,                    ///<
            ASMPIC8_COND_ASM,                   ///<

            ASMPIC8_DIR_DEFINE,                 ///<
            ASMPIC8_DIR_UNDEFINE,               ///<
            ASMPIC8_DIR_DC_IFDEF,               ///<
            ASMPIC8_DIR_DC_IFNDEF,              ///<
            ASMPIC8_DIR_DC_ENDIF,               ///<
            ASMPIC8_DIR_CONSTANT,               ///<
            ASMPIC8_DIR_PROCESSOR,              ///<
            ASMPIC8_DIR_RADIX,                  ///<
            ASMPIC8_DIR_VARIABLE,               ///<
            ASMPIC8_DIR_WHILE,                  ///<
            ASMPIC8_DIR___BADRAM,               ///<
            ASMPIC8_DIR___BADROM,               ///<
            ASMPIC8_DIR___CONFIG,               ///<
            ASMPIC8_DIR_CONFIG,                 ///<
            ASMPIC8_DIR___IDLOCS,               ///<
            ASMPIC8_DIR___MAXRAM,               ///<
            ASMPIC8_DIR___MAXROM,               ///<
            ASMPIC8_DIR_CBLOCK,                 ///<
            ASMPIC8_DIR_DA,                     ///<
            ASMPIC8_DIR_DATA,                   ///<
            ASMPIC8_DIR_DE,                     ///<
            ASMPIC8_DIR_DB,                     ///<
            ASMPIC8_DIR_DT,                     ///<
            ASMPIC8_DIR_DW,                     ///<
            ASMPIC8_DIR_FILL,                   ///<
            ASMPIC8_DIR_RES,                    ///<
            ASMPIC8_DIR_ERROR,                  ///<
            ASMPIC8_DIR_ERRORLEVEL,             ///<
            ASMPIC8_DIR_LIST,                   ///<
            ASMPIC8_DIR_MESSG,                  ///<
            ASMPIC8_DIR_NOLIST,                 ///<
            ASMPIC8_DIR_PAGE,                   ///<
            ASMPIC8_DIR_SPACE,                  ///<
            ASMPIC8_DIR_SUBTITLE,               ///<
            ASMPIC8_DIR_TITLE,                  ///<
            ASMPIC8_DIR_EXPAND,                 ///<
            ASMPIC8_DIR_NOEXPAND,               ///<
            ASMPIC8_DIR_ACCESS_OVR,             ///<
            ASMPIC8_DIR_BANKISEL,               ///<
            ASMPIC8_DIR_BANKSEL,                ///<
            ASMPIC8_DIR_CODE,                   ///<
            ASMPIC8_DIR_CODE_PACK,              ///<
            ASMPIC8_DIR_EXTERN,                 ///<
            ASMPIC8_DIR_GLOBAL,                 ///<
            ASMPIC8_DIR_IDATA,                  ///<
            ASMPIC8_DIR_IDATA_ACS,              ///<
            ASMPIC8_DIR_PAGESEL,                ///<
            ASMPIC8_DIR_PAGESELW,               ///<
            ASMPIC8_DIR_UDATA,                  ///<
            ASMPIC8_DIR_UDATA_ACS,              ///<
            ASMPIC8_DIR_UDATA_OVR,              ///<
            ASMPIC8_DIR_UDATA_SHR,              ///<
            ASMPIC8_DIR_IF,                     ///<
            ASMPIC8_DIR_IFN,                    ///<
            ASMPIC8_DIR_IFDEF,                  ///<
            ASMPIC8_DIR_IFNDEF,                 ///<
            ASMPIC8_DIR_ELSEIFB,                ///<
            ASMPIC8_DIR_ELSEIFNB,               ///<
            ASMPIC8_DIR_ELSE,                   ///<
            ASMPIC8_DIR_ELSEIF,                 ///<
            ASMPIC8_DIR_ELSEIFN,                ///<
            ASMPIC8_DIR_ELSEIFDEF,              ///<
            ASMPIC8_DIR_ELSEIFNDEF,             ///<
            ASMPIC8_DIR_LOCAL,                  ///<
            ASMPIC8_DIR_IFNB,                   ///<
            ASMPIC8_DIR_IFB,                    ///<
            ASMPIC8_DIR_SKIP,                   ///<
            ASMPIC8_DIR_EXITM,                  ///<
            ASMPIC8_DIR_REPT,                   ///<
            ASMPIC8_DIR_MACRO,                  ///<
            ASMPIC8_DIR_EQU,                    ///<
            ASMPIC8_DIR_ORG,                    ///<
            ASMPIC8_DIR_SET,                    ///<
            ASMPIC8_DIR_ENDW,                   ///<
            ASMPIC8_DIR_END,                    ///<

            ASMPIC8_INS_ADDWF,                  ///<
            ASMPIC8_INS_ANDWF,                  ///<
            ASMPIC8_INS_CLRF,                   ///<
            ASMPIC8_INS_CLRW,                   ///<
            ASMPIC8_INS_COMF,                   ///<
            ASMPIC8_INS_DECF,                   ///<
            ASMPIC8_INS_DECFSZ,                 ///<
            ASMPIC8_INS_INCF,                   ///<
            ASMPIC8_INS_INCFSZ,                 ///<
            ASMPIC8_INS_IORWF,                  ///<
            ASMPIC8_INS_MOVF,                   ///<
            ASMPIC8_INS_MOVWF,                  ///<
            ASMPIC8_INS_NOP,                    ///<
            ASMPIC8_INS_RLF,                    ///<
            ASMPIC8_INS_RRF,                    ///<
            ASMPIC8_INS_SUBWF,                  ///<
            ASMPIC8_INS_SWAPF,                  ///<
            ASMPIC8_INS_XORWF,                  ///<
            ASMPIC8_INS_BCF,                    ///<
            ASMPIC8_INS_BSF,                    ///<
            ASMPIC8_INS_BTFSC,                  ///<
            ASMPIC8_INS_BTFSS,                  ///<
            ASMPIC8_INS_ADDLW,                  ///<
            ASMPIC8_INS_ANDLW,                  ///<
            ASMPIC8_INS_CALL,                   ///<
            ASMPIC8_INS_CLRWDT,                 ///<
            ASMPIC8_INS_GOTO,                   ///<
            ASMPIC8_INS_IORLW,                  ///<
            ASMPIC8_INS_MOVLW,                  ///<
            ASMPIC8_INS_RETFIE,                 ///<
            ASMPIC8_INS_RETLW,                  ///<
            ASMPIC8_INS_RETURN,                 ///<
            ASMPIC8_INS_SLEEP,                  ///<
            ASMPIC8_INS_SUBLW,                  ///<
            ASMPIC8_INS_XORLW,                  ///<
            ASMPIC8_INS_OPTION,                 ///<
            ASMPIC8_INS_TRIS,                   ///<
        //@}

        /// @name MCS-51
        //@{
            ASM51_MACRO,                        ///<
            ASM51_LABEL,                        ///<
            ASM51_INCLUDE,                      ///<
            ASM51_COND_ASM,                     ///<

            ASM51_DIR_MACRO,                    ///<
            ASM51_DIR_DB,                       ///<
            ASM51_DIR_BIT,                      ///<
            ASM51_DIR_BSEG,                     ///<
            ASM51_DIR_BYTE,                     ///<
            ASM51_DIR_CODE,                     ///<
            ASM51_DIR_CSEG,                     ///<
            ASM51_DIR_DATA,                     ///<
            ASM51_DIR_DBIT,                     ///<
            ASM51_DIR_DS,                       ///<
            ASM51_DIR_DSEG,                     ///<
            ASM51_DIR_DW,                       ///<
            ASM51_DIR_ELSE,                     ///<
            ASM51_DIR_ELSEIF,                   ///<
            ASM51_DIR_ELSEIFDEF,                ///<
            ASM51_DIR_ELSEIFN,                  ///<
            ASM51_DIR_ELSEIFNDEF,               ///<
            ASM51_DIR_END,                      ///<
            ASM51_DIR_EQU,                      ///<
            ASM51_DIR_FLAG,                     ///<
            ASM51_DIR_IDATA,                    ///<
            ASM51_DIR_IF,                       ///<
            ASM51_DIR_IFDEF,                    ///<
            ASM51_DIR_IFN,                      ///<
            ASM51_DIR_IFNDEF,                   ///<
            ASM51_DIR_ISEG,                     ///<
            ASM51_DIR_LIST,                     ///<
            ASM51_DIR_NAME,                     ///<
            ASM51_DIR_NOLIST,                   ///<
            ASM51_DIR_ORG,                      ///<
            ASM51_DIR_REPT,                     ///<
            ASM51_DIR_SET,                      ///<
            ASM51_DIR_SKIP,                     ///<
            ASM51_DIR_USING,                    ///<
            ASM51_DIR_XDATA,                    ///<
            ASM51_DIR_XSEG,                     ///<
            ASM51_DIR_LOCAL,                    ///<
            ASM51_DIR_ELSEIFNB,                 ///<
            ASM51_DIR_EXITM,                    ///<
            ASM51_DIR_ELSEIFB,                  ///<
            ASM51_DIR_IFNB,                     ///<
            ASM51_DIR_IFB,                      ///<

            ASM51_CNT_COND,                     ///<
            ASM51_CNT_NODEBUG,                  ///<
            ASM51_CNT_NOSYMBOLS,                ///<
            ASM51_CNT_CONDONLY,                 ///<
            ASM51_CNT_NOTABS,                   ///<
            ASM51_CNT_NOGEN,                    ///<
            ASM51_CNT_SAVE,                     ///<
            ASM51_CNT_DATE,                     ///<
            ASM51_CNT_NOXREF,                   ///<
            ASM51_CNT_LIST,                     ///<
            ASM51_CNT_NOLIST,                   ///<
            ASM51_CNT_PAGELENGTH,               ///<
            ASM51_CNT_SYMBOLS,                  ///<
            ASM51_CNT_DEBUG,                    ///<
            ASM51_CNT_MACRO,                    ///<
            ASM51_CNT_NOMACRO,                  ///<
            ASM51_CNT_PAGEWIDTH,                ///<
            ASM51_CNT_TITLE,                    ///<
            ASM51_CNT_PAGING,                   ///<
            ASM51_CNT_EJECT,                    ///<
            ASM51_CNT_MOD51,                    ///<
            ASM51_CNT_NOMOD51,                  ///<
            ASM51_CNT_PHILIPS,                  ///<
            ASM51_CNT_WARNING,                  ///<
            ASM51_CNT_ERROR,                    ///<
            ASM51_CNT_NOBUILTIN,                ///<
            ASM51_CNT_NOPAGING,                 ///<
            ASM51_CNT_XREF,                     ///<
            ASM51_CNT_GEN,                      ///<
            ASM51_CNT_NOCOND,                   ///<
            ASM51_CNT_GENONLY,                  ///<
            ASM51_CNT_RESTORE,                  ///<

            ASM51_INS_ACALL,                    ///<

            ASM51_INS_ADD__A_IMM,               ///<
            ASM51_INS_ADD__A_DIR,               ///<
            ASM51_INS_ADD__A_IR0,               ///<
            ASM51_INS_ADD__A_IR1,               ///<
            ASM51_INS_ADD__A_R0,                ///<
            ASM51_INS_ADD__A_R1,                ///<
            ASM51_INS_ADD__A_R2,                ///<
            ASM51_INS_ADD__A_R3,                ///<
            ASM51_INS_ADD__A_R4,                ///<
            ASM51_INS_ADD__A_R5,                ///<
            ASM51_INS_ADD__A_R6,                ///<
            ASM51_INS_ADD__A_R7,                ///<

            ASM51_INS_ADDC__A_IMM,              ///<
            ASM51_INS_ADDC__A_DIR,              ///<
            ASM51_INS_ADDC__A_IR0,              ///<
            ASM51_INS_ADDC__A_IR1,              ///<
            ASM51_INS_ADDC__A_R0,               ///<
            ASM51_INS_ADDC__A_R1,               ///<
            ASM51_INS_ADDC__A_R2,               ///<
            ASM51_INS_ADDC__A_R3,               ///<
            ASM51_INS_ADDC__A_R4,               ///<
            ASM51_INS_ADDC__A_R5,               ///<
            ASM51_INS_ADDC__A_R6,               ///<
            ASM51_INS_ADDC__A_R7,               ///<

            ASM51_INS_AJMP__DIR,                ///<

            ASM51_INS_ANL__DIR_A,               ///<
            ASM51_INS_ANL__DIR_IMM,             ///<
            ASM51_INS_ANL__A_IMM,               ///<
            ASM51_INS_ANL__A_DIR,               ///<
            ASM51_INS_ANL__A_IR0,               ///<
            ASM51_INS_ANL__A_IR1,               ///<
            ASM51_INS_ANL__A_R0,                ///<
            ASM51_INS_ANL__A_R1,                ///<
            ASM51_INS_ANL__A_R2,                ///<
            ASM51_INS_ANL__A_R3,                ///<
            ASM51_INS_ANL__A_R4,                ///<
            ASM51_INS_ANL__A_R5,                ///<
            ASM51_INS_ANL__A_R6,                ///<
            ASM51_INS_ANL__A_R7,                ///<
            ASM51_INS_ANL__C_DIR,               ///<
            ASM51_INS_ANL__C_SL_DIR,            ///<

            ASM51_INS_CJNE__A_IMM_DIR,          ///<
            ASM51_INS_CJNE__A_DIR_DIR,          ///<
            ASM51_INS_CJNE__IR0_IMM_DIR,        ///<
            ASM51_INS_CJNE__IR1_IMM_DIR,        ///<
            ASM51_INS_CJNE__R0_IMM_DIR,         ///<
            ASM51_INS_CJNE__R1_IMM_DIR,         ///<
            ASM51_INS_CJNE__R2_IMM_DIR,         ///<
            ASM51_INS_CJNE__R3_IMM_DIR,         ///<
            ASM51_INS_CJNE__R4_IMM_DIR,         ///<
            ASM51_INS_CJNE__R5_IMM_DIR,         ///<
            ASM51_INS_CJNE__R6_IMM_DIR,         ///<
            ASM51_INS_CJNE__R7_IMM_DIR,         ///<

            ASM51_INS_CLR__DIR,                 ///<
            ASM51_INS_CLR__C,                   ///<
            ASM51_INS_CLR__A,                   ///<

            ASM51_INS_CPL__DIR,                 ///<
            ASM51_INS_CPL__C,                   ///<
            ASM51_INS_CPL__A,                   ///<

            ASM51_INS_DA__A,                    ///<

            ASM51_INS_DEC__DIR,                 ///<
            ASM51_INS_DEC__A,                   ///<
            ASM51_INS_DEC__IR0,                 ///<
            ASM51_INS_DEC__IR1,                 ///<
            ASM51_INS_DEC__R0,                  ///<
            ASM51_INS_DEC__R1,                  ///<
            ASM51_INS_DEC__R2,                  ///<
            ASM51_INS_DEC__R3,                  ///<
            ASM51_INS_DEC__R4,                  ///<
            ASM51_INS_DEC__R5,                  ///<
            ASM51_INS_DEC__R6,                  ///<
            ASM51_INS_DEC__R7,                  ///<

            ASM51_INS_DIV__AB,                  ///<

            ASM51_INS_DJNZ__DIR_DIR,            ///<
            ASM51_INS_DJNZ__R0_DIR,             ///<
            ASM51_INS_DJNZ__R1_DIR,             ///<
            ASM51_INS_DJNZ__R2_DIR,             ///<
            ASM51_INS_DJNZ__R3_DIR,             ///<
            ASM51_INS_DJNZ__R4_DIR,             ///<
            ASM51_INS_DJNZ__R5_DIR,             ///<
            ASM51_INS_DJNZ__R6_DIR,             ///<
            ASM51_INS_DJNZ__R7_DIR,             ///<

            ASM51_INS_INC__A,                   ///<
            ASM51_INS_INC__DIR,                 ///<
            ASM51_INS_INC__IR0,                 ///<
            ASM51_INS_INC__IR1,                 ///<
            ASM51_INS_INC__R0,                  ///<
            ASM51_INS_INC__R1,                  ///<
            ASM51_INS_INC__R2,                  ///<
            ASM51_INS_INC__R3,                  ///<
            ASM51_INS_INC__R4,                  ///<
            ASM51_INS_INC__R5,                  ///<
            ASM51_INS_INC__R6,                  ///<
            ASM51_INS_INC__R7,                  ///<
            ASM51_INS_INC__DPTR,                ///<

            ASM51_INS_JB__DIR_DIR,              ///<

            ASM51_INS_JBC__DIR_DIR,             ///<

            ASM51_INS_JC__DIR,                  ///<


            ASM51_INS_JMP__IA_DPTR,             ///<
            ASM51_INS_JMP__DIR,                 ///<

            ASM51_INS_JNB__DIR,                 ///<

            ASM51_INS_JNC__DIR,                 ///<

            ASM51_INS_JNZ__DIR,                 ///<

            ASM51_INS_JZ__DIR,                  ///<

            ASM51_INS_LCALL__DIR,               ///<

            ASM51_INS_LJMP__DIR,                ///<

            ASM51_INS_MOV__DIR_DIR,             ///<
            ASM51_INS_MOV__DIR_IMM,             ///<
            ASM51_INS_MOV__DIR_IR0,             ///<
            ASM51_INS_MOV__DIR_IR1,             ///<
            ASM51_INS_MOV__DIR_R0,              ///<
            ASM51_INS_MOV__DIR_R1,              ///<
            ASM51_INS_MOV__DIR_R2,              ///<
            ASM51_INS_MOV__DIR_R3,              ///<
            ASM51_INS_MOV__DIR_R4,              ///<
            ASM51_INS_MOV__DIR_R5,              ///<
            ASM51_INS_MOV__DIR_R6,              ///<
            ASM51_INS_MOV__DIR_R7,              ///<
            ASM51_INS_MOV__DIR_C,               ///<
            ASM51_INS_MOV__DIR_A,               ///<
            ASM51_INS_MOV__IR0_IMM,             ///<
            ASM51_INS_MOV__IR1_IMM,             ///<
            ASM51_INS_MOV__IR0_DIR,             ///<
            ASM51_INS_MOV__IR1_DIR,             ///<
            ASM51_INS_MOV__IR0_A,               ///<
            ASM51_INS_MOV__IR1_A,               ///<
            ASM51_INS_MOV__DPTR_IMM,            ///<
            ASM51_INS_MOV__R0_IMM,              ///<
            ASM51_INS_MOV__R1_IMM,              ///<
            ASM51_INS_MOV__R2_IMM,              ///<
            ASM51_INS_MOV__R3_IMM,              ///<
            ASM51_INS_MOV__R4_IMM,              ///<
            ASM51_INS_MOV__R5_IMM,              ///<
            ASM51_INS_MOV__R6_IMM,              ///<
            ASM51_INS_MOV__R7_IMM,              ///<
            ASM51_INS_MOV__R0_DIR,              ///<
            ASM51_INS_MOV__R1_DIR,              ///<
            ASM51_INS_MOV__R2_DIR,              ///<
            ASM51_INS_MOV__R3_DIR,              ///<
            ASM51_INS_MOV__R4_DIR,              ///<
            ASM51_INS_MOV__R5_DIR,              ///<
            ASM51_INS_MOV__R6_DIR,              ///<
            ASM51_INS_MOV__R7_DIR,              ///<
            ASM51_INS_MOV__R0_A,                ///<
            ASM51_INS_MOV__R1_A,                ///<
            ASM51_INS_MOV__R2_A,                ///<
            ASM51_INS_MOV__R3_A,                ///<
            ASM51_INS_MOV__R4_A,                ///<
            ASM51_INS_MOV__R5_A,                ///<
            ASM51_INS_MOV__R6_A,                ///<
            ASM51_INS_MOV__R7_A,                ///<
            ASM51_INS_MOV__A_IMM,               ///<
            ASM51_INS_MOV__A_IR0,               ///<
            ASM51_INS_MOV__A_IR1,               ///<
            ASM51_INS_MOV__A_R0,                ///<
            ASM51_INS_MOV__A_R1,                ///<
            ASM51_INS_MOV__A_R2,                ///<
            ASM51_INS_MOV__A_R3,                ///<
            ASM51_INS_MOV__A_R4,                ///<
            ASM51_INS_MOV__A_R5,                ///<
            ASM51_INS_MOV__A_R6,                ///<
            ASM51_INS_MOV__A_R7,                ///<
            ASM51_INS_MOV__A_DIR,               ///<
            ASM51_INS_MOV__C_DIR,               ///<

            ASM51_INS_MOVC__A_IA_DPTR,          ///<
            ASM51_INS_MOVC__A_IA_PC,            ///<

            ASM51_INS_MOVX__IDPTR_A,            ///<
            ASM51_INS_MOVX__IR0_A,              ///<
            ASM51_INS_MOVX__IR1_A,              ///<
            ASM51_INS_MOVX__A_IDPTR,            ///<
            ASM51_INS_MOVX__A_IR0,              ///<
            ASM51_INS_MOVX__A_IR1,              ///<

            ASM51_INS_MUL__AB,                  ///<

            ASM51_INS_NOP,                      ///<

            ASM51_INS_ORL__DIR_A,               ///<
            ASM51_INS_ORL__DIR_IMM,             ///<
            ASM51_INS_ORL__A_IMM,               ///<
            ASM51_INS_ORL__A_DIR,               ///<
            ASM51_INS_ORL__A_IR0,               ///<
            ASM51_INS_ORL__A_IR1,               ///<
            ASM51_INS_ORL__A_R0,                ///<
            ASM51_INS_ORL__A_R1,                ///<
            ASM51_INS_ORL__A_R2,                ///<
            ASM51_INS_ORL__A_R3,                ///<
            ASM51_INS_ORL__A_R4,                ///<
            ASM51_INS_ORL__A_R5,                ///<
            ASM51_INS_ORL__A_R6,                ///<
            ASM51_INS_ORL__A_R7,                ///<
            ASM51_INS_ORL__C_DIR,               ///<
            ASM51_INS_ORL__C_SL_DIR,            ///<

            ASM51_INS_POP__DIR,                 ///<

            ASM51_INS_PUSH__DIR,                ///<

            ASM51_INS_RET,                      ///<

            ASM51_INS_RETI,                     ///<

            ASM51_INS_RL__A,                    ///<

            ASM51_INS_RLC__A,                   ///<

            ASM51_INS_RR__A,                    ///<

            ASM51_INS_RRC__A,                   ///<

            ASM51_INS_SETB__C,                  ///<
            ASM51_INS_SETB__DIR,                ///<

            ASM51_INS_SJMP__DIR,                ///<

            ASM51_INS_SUBB__A_IMM,              ///<
            ASM51_INS_SUBB__A_DIR,              ///<
            ASM51_INS_SUBB__A_IR0,              ///<
            ASM51_INS_SUBB__A_IR1,              ///<
            ASM51_INS_SUBB__A_R0,               ///<
            ASM51_INS_SUBB__A_R1,               ///<
            ASM51_INS_SUBB__A_R2,               ///<
            ASM51_INS_SUBB__A_R3,               ///<
            ASM51_INS_SUBB__A_R4,               ///<
            ASM51_INS_SUBB__A_R5,               ///<
            ASM51_INS_SUBB__A_R6,               ///<
            ASM51_INS_SUBB__A_R7,               ///<

            ASM51_INS_SWAP__A,                  ///<

            ASM51_INS_XCH__A_IR0,               ///<
            ASM51_INS_XCH__A_IR1,               ///<
            ASM51_INS_XCH__A_R0,                ///<
            ASM51_INS_XCH__A_R1,                ///<
            ASM51_INS_XCH__A_R2,                ///<
            ASM51_INS_XCH__A_R3,                ///<
            ASM51_INS_XCH__A_R4,                ///<
            ASM51_INS_XCH__A_R5,                ///<
            ASM51_INS_XCH__A_R6,                ///<
            ASM51_INS_XCH__A_R7,                ///<
            ASM51_INS_XCH__A_DIR,               ///<

            ASM51_INS_XCHD__A_IR0,              ///<
            ASM51_INS_XCHD__A_IR1,              ///<

            ASM51_INS_XRL__DIR_A,               ///<
            ASM51_INS_XRL__DIR_IMM,             ///<
            ASM51_INS_XRL__A_IMM,               ///<
            ASM51_INS_XRL__A_DIR,               ///<
            ASM51_INS_XRL__A_IR0,               ///<
            ASM51_INS_XRL__A_IR1,               ///<
            ASM51_INS_XRL__A_R0,                ///<
            ASM51_INS_XRL__A_R1,                ///<
            ASM51_INS_XRL__A_R2,                ///<
            ASM51_INS_XRL__A_R3,                ///<
            ASM51_INS_XRL__A_R4,                ///<
            ASM51_INS_XRL__A_R5,                ///<
            ASM51_INS_XRL__A_R6,                ///<
            ASM51_INS_XRL__A_R7,                ///<
        //@}

        /// @name PicoBlaze
        //@{
            ASMPICOBLAZE_DIR_MACRO,             ///<
            ASMPICOBLAZE_DIR_ENDM,              ///<
            ASMPICOBLAZE_DIR_EXITM,             ///<
            ASMPICOBLAZE_MACRO,                 ///<
            ASMPICOBLAZE_LABEL,                 ///<
            ASMPICOBLAZE_DIR_EQU,               ///<
            ASMPICOBLAZE_DIR_END,               ///<
            ASMPICOBLAZE_INCLUDE,               ///<
            ASMPICOBLAZE_DIR_ORG,               ///<
            ASMPICOBLAZE_DIR_SKIP,              ///<
            ASMPICOBLAZE_LOCAL,                 ///<
            ASMPICOBLAZE_DIR_SET,               ///<
            ASMPICOBLAZE_DIR_DEFINE,            ///<
            ASMPICOBLAZE_DIR_UNDEFINE,          ///<
            ASMPICOBLAZE_DIR_CODE,              ///<
            ASMPICOBLAZE_DIR_REG,               ///<
            ASMPICOBLAZE_DIR_STRING,            ///<
            ASMPICOBLAZE_DIR_PORT,              ///<
            ASMPICOBLAZE_DIR_PORTIN,            ///<
            ASMPICOBLAZE_DIR_PORTOUT,           ///<
            ASMPICOBLAZE_DIR_REPT,              ///<
            ASMPICOBLAZE_DIR_ENDR,              ///<
            ASMPICOBLAZE_DIR_WHILE,             ///<
            ASMPICOBLAZE_DIR_ENDW,              ///<
            ASMPICOBLAZE_DIR_LIST,              ///<
            ASMPICOBLAZE_DIR_NOLIST,            ///<
            ASMPICOBLAZE_DIR_EXPAND,            ///<
            ASMPICOBLAZE_DIR_NOEXPAND,          ///<
            ASMPICOBLAZE_DIR_TITLE,             ///<
            ASMPICOBLAZE_DIR_ENTITY,            ///<
            ASMPICOBLAZE_DIR_MESSG,             ///<
            ASMPICOBLAZE_DIR_ERROR,             ///<
            ASMPICOBLAZE_DIR_WARNING,           ///<
            ASMPICOBLAZE_DIR_DB,                ///<
            ASMPICOBLAZE_DIR_AUTOREG,           ///<
            ASMPICOBLAZE_DIR_AUTOSPR,           ///<
            ASMPICOBLAZE_DIR_DATA,              ///<
            ASMPICOBLAZE_DIR_LIMIT,             ///<
            ASMPICOBLAZE_DIR_DEVICE,            ///<
            ASMPICOBLAZE_DIR_FAILJMP,           ///<
            ASMPICOBLAZE_DIR_ORGSPR,            ///<
            ASMPICOBLAZE_DIR_INITSPR,           ///<
            ASMPICOBLAZE_DIR_MERGESPR,          ///<

            ASMPICOBLAZE_COND_ASM,              ///<
            ASMPICOBLAZE_DIR_IF,                ///<
            ASMPICOBLAZE_DIR_IFN,               ///<
            ASMPICOBLAZE_DIR_IFDEF,             ///<
            ASMPICOBLAZE_DIR_IFNDEF,            ///<
            ASMPICOBLAZE_DIR_ELSEIFB,           ///<
            ASMPICOBLAZE_DIR_ELSEIFNB,          ///<
            ASMPICOBLAZE_DIR_ELSE,              ///<
            ASMPICOBLAZE_DIR_ELSEIF,            ///<
            ASMPICOBLAZE_DIR_ELSEIFN,           ///<
            ASMPICOBLAZE_DIR_ELSEIFDEF,         ///<
            ASMPICOBLAZE_DIR_ELSEIFNDEF,        ///<
            ASMPICOBLAZE_DIR_IFB,               ///<
            ASMPICOBLAZE_DIR_IFNB,              ///<
            ASMPICOBLAZE_DIR_ENDIF,             ///<

            ASMPICOBLAZE_RT_COND,               ///<
            ASMPICOBLAZE_DIR_RTIF,              ///<
            ASMPICOBLAZE_DIR_RTELSEIF,          ///<
            ASMPICOBLAZE_DIR_RTELSE,            ///<
            ASMPICOBLAZE_DIR_RTENDIF,           ///<
            ASMPICOBLAZE_DIR_RTWHILE,           ///<
            ASMPICOBLAZE_DIR_RTENDW,            ///<
            ASMPICOBLAZE_DIR_RTFOR,             ///<
            ASMPICOBLAZE_DIR_RTENDF,            ///<

            ASMPICOBLAZE_DIR_OUTPUTK_STR_P,     ///<
            ASMPICOBLAZE_DIR_LD_RET_SX_STR,     ///<

            ASMPICOBLAZE_INS_JUMP_AAA,          ///<
            ASMPICOBLAZE_INS_JUMP_Z_AAA,        ///<
            ASMPICOBLAZE_INS_JUMP_NZ_AAA,       ///<
            ASMPICOBLAZE_INS_JUMP_C_AAA,        ///<
            ASMPICOBLAZE_INS_JUMP_NC_AAA,       ///<
            ASMPICOBLAZE_INS_CALL_AAA,          ///<
            ASMPICOBLAZE_INS_CALL_Z_AAA,        ///<
            ASMPICOBLAZE_INS_CALL_NZ_AAA,       ///<
            ASMPICOBLAZE_INS_CALL_C_AAA,        ///<
            ASMPICOBLAZE_INS_CALL_NC_AAA,       ///<
            ASMPICOBLAZE_INS_RETURN,            ///<
            ASMPICOBLAZE_INS_RETURN_Z,          ///<
            ASMPICOBLAZE_INS_RETURN_NZ,         ///<
            ASMPICOBLAZE_INS_RETURN_C,          ///<
            ASMPICOBLAZE_INS_RETURN_NC,         ///<
            ASMPICOBLAZE_INS_ADD_SX_KK,         ///<
            ASMPICOBLAZE_INS_ADD_SX_SY,         ///<
            ASMPICOBLAZE_INS_ADDCY_SX_KK,       ///<
            ASMPICOBLAZE_INS_ADDCY_SX_SY,       ///<
            ASMPICOBLAZE_INS_SUB_SX_KK,         ///<
            ASMPICOBLAZE_INS_SUB_SX_SY,         ///<
            ASMPICOBLAZE_INS_SUBCY_SX_KK,       ///<
            ASMPICOBLAZE_INS_SUBCY_SX_SY,       ///<
            ASMPICOBLAZE_INS_COMPARE_SX_KK,     ///<
            ASMPICOBLAZE_INS_COMPARE_SX_SY,     ///<
            ASMPICOBLAZE_INS_RETURNI_ENA,       ///<
            ASMPICOBLAZE_INS_RETURNI_DIS,       ///<
            ASMPICOBLAZE_INS_ENABLE_INT,        ///<
            ASMPICOBLAZE_INS_DISABLE_INT,       ///<
            ASMPICOBLAZE_INS_LOAD_SX_KK,        ///<
            ASMPICOBLAZE_INS_LOAD_SX_SY,        ///<
            ASMPICOBLAZE_INS_AND_SX_KK,         ///<
            ASMPICOBLAZE_INS_AND_SX_SY,         ///<
            ASMPICOBLAZE_INS_OR_SX_KK,          ///<
            ASMPICOBLAZE_INS_OR_SX_SY,          ///<
            ASMPICOBLAZE_INS_XOR_SX_KK,         ///<
            ASMPICOBLAZE_INS_XOR_SX_SY,         ///<
            ASMPICOBLAZE_INS_TEST_SX_KK,        ///<
            ASMPICOBLAZE_INS_TEST_SX_SY,        ///<
            ASMPICOBLAZE_INS_STORE_SX_SS,       ///<
            ASMPICOBLAZE_INS_STORE_SX_SY,       ///<
            ASMPICOBLAZE_INS_FETCH_SX_SS,       ///<
            ASMPICOBLAZE_INS_FETCH_SX_SY,       ///<
            ASMPICOBLAZE_INS_SR0_SX,            ///<
            ASMPICOBLAZE_INS_SR1_SX,            ///<
            ASMPICOBLAZE_INS_SRX_SX,            ///<
            ASMPICOBLAZE_INS_SRA_SX,            ///<
            ASMPICOBLAZE_INS_RR_SX,             ///<
            ASMPICOBLAZE_INS_SL0_SX,            ///<
            ASMPICOBLAZE_INS_SL1_SX,            ///<
            ASMPICOBLAZE_INS_SLX_SX,            ///<
            ASMPICOBLAZE_INS_SLA_SX,            ///<
            ASMPICOBLAZE_INS_RL_SX,             ///<
            ASMPICOBLAZE_INS_INPUT_SX_PP,       ///<
            ASMPICOBLAZE_INS_INPUT_SX_SY,       ///<
            ASMPICOBLAZE_INS_OUTPUT_SX_PP,      ///<
            ASMPICOBLAZE_INS_OUTPUT_SX_SY,      ///<
            ASMPICOBLAZE_INS_JUMP_SX_SY,        ///<
            ASMPICOBLAZE_INS_CALL_SX_SY,        ///<
            ASMPICOBLAZE_INS_LD_RET_SX_KK,      ///<
            ASMPICOBLAZE_INS_HWBUILD_SX,        ///<
            ASMPICOBLAZE_INS_STAR_SX_SY,        ///<
            ASMPICOBLAZE_INS_STAR_SX_KK,        ///<
            ASMPICOBLAZE_INS_TESTCY_SX_SY,      ///<
            ASMPICOBLAZE_INS_TESTCY_SX_KK,      ///<
            ASMPICOBLAZE_INS_COMPARECY_SX_SY,   ///<
            ASMPICOBLAZE_INS_COMPARECY_SX_KK,   ///<
            ASMPICOBLAZE_INS_REGBANK_A,         ///<
            ASMPICOBLAZE_INS_REGBANK_B,         ///<
            ASMPICOBLAZE_INS_OUTPUTK_KK_P,      ///<
        //@}

        ///@name C
        //@{
            C_LABEL,
            C_CASE,
            C_DEFAULT,

            C_COMPOUND,
            C_CRITICAL_COMPOUND,

            C_DECLARATION,
            C_EXPRESSION,
            C_NULL,

            C_IF_ELSE,
            C_IF,
            C_ELSE,
            C_SWITCH,

            C_WHILE,
            C_DO_WHILE,
            C_FOR,
            C_FOR_DECL,

            C_GOTO,
            C_CONTINUE,
            C_BREAK,
            C_RETURN,

            C_FUNCTION_DEF,
            C_INLINE_ASSEMBLY,
        //@}

        STATEMENTTYPE__MAX__                    ///<
    };
};

// Trace oprator.
std::ostream & operator << ( std::ostream & out,
                             const CompilerStatementTypes::StatementType type );

#endif // COMPILERSTATEMENTTYPES_H

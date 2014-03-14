/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file enums.h
 */


#ifndef ENUMS_H
#define ENUMS_H


typedef enum LangType
{
    LANG_ASM = 0,
    LANG_C
} LangType;


typedef enum ErrorCode
{
    ERR_OPENFILE = 2000,
    ERR_XML_CONTENT,
    ERR_XML_ASSIGN,
    
    ERR_NO_PROJECT,
    
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_MEM_WR_NONEXISTENT,
    ERR_MEM_RD_NONEXISTENT,
    ERR_MEM_WR_NOT_IMPLEMENTED,
    ERR_MEM_RD_NOT_IMPLEMENTED,
    ERR_CPU_PC_OVERFLOW,
    ERR_CPU_PC_UNDERFLOW,
    ERR_CPU_SYS_FATAL,
    ERR_CPU_INVALID_OPCODE,
    ERR_CPU_INVALID_JUMP,
    ERR_CPU_INVALID_CALL,
    ERR_CPU_INVALID_IRQ,
    ERR_CPU_INVALID_RET,
    ERR_CPU_INVALID_RETI,
    ERR_CPU_INVALID_OPSET,
    ERR_CPU_UNSUPPORTED_INST,
    ERR_CPU_INST_IGNORED,
    
    ERR_UNTRACKED_PROJECT,
    ERR_SIM_NOSTART,
    ERR_SIM_NOT_COMPILED,
    ERR_SIM_NOT_COMPILED_MODIFIED,
    ERR_NO_MAINFILE
} ErrorCode;


typedef enum SourceType
{
    PLAIN = 0,
    C,
    CPP,
    AVRASM,
    PICASM,
    PICOBLAZEASM
} SourceType;


#endif
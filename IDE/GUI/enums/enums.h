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
    ERR_MEM_RD_ACCESS_DENIED,
    ERR_MEM_WR_ACCESS_DENIED,
    ERR_MEM_RD_UNDEFINED,
    ERR_MEM_RD_DEFAULT,
    ERR_MEM_RD_WRITE_ONLY,
    ERR_MEM_WR_READ_ONLY,
    ERR_MEM_RD_PAR_WRITE_ONLY,
    ERR_MEM_WR_PAR_READ_ONLY,
    ERR_MEM_RD_RESERVED_READ,
    ERR_MEM_WR_RESERVED_WRITTEN,
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
    ERR_SIM_RECOMPILE_FAILED,
    ERR_NO_MAINFILE,
    ERR_COMPILE_DURING_SIMULATION,

    ERR_NO_LICENSE,

    ERR_END_OF_DOCUMENT,
    ERR_START_OF_DOCUMENT
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

typedef enum WidgetCode
{
    WBOOKMARKLIST = 0,
    WBREAKPOINTLIST,
    WCOMPILEINFO,
    WCODEEDITCODE,
    WSIMULATIONINFO,
    WANALYSVAR,
    WANALYSFUNC,
    WBOTTOMHIDE,
    WRIGHTHIDE,
    WASMMACROANALYSER,
    WOUTPUTWIDGET
} WidgetCode;


typedef enum CodeEditBottomWidget
{
    WFIND = 0,
    WFINDANDREPLACE,
    WJUMPTOLINE
} CodeEditBottomWidget;


#endif
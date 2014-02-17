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
    ERR_UNTRACKED_PROJECT,
    ERR_SIM_NOSTART,
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
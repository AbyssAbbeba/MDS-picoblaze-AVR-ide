#ifndef ENUMS_H
#define ENUMS_H


typedef enum LangType {
    LANG_ASM = 0,
    LANG_C
} LangType;


typedef enum ErrorCode {
    //remove d
    dERR_OPENFILE = 2000,
    dERR_XML_CONTENT,
    dERR_XML_ASSIGN,
    dERR_NO_PROJECT
} ErrorCode;


typedef enum SourceType {
    PLAIN = 0,
    C, CPP,
    AVRASM,
    PICASM,
    PICOBLAZEASM
} SourceType;


#endif
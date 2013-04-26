// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2013
 * @ingroup PIC8
 * @file PIC8InsNames.h
 */
// =============================================================================

#ifndef PIC8INSNAMES_H
#define PIC8INSNAMES_H

/**
 * @brief
 * @ingroup PIC8
 * @namespace PIC8InsNames
 */
namespace PIC8InsNames
{
    ////    Datatypes    ////

    /**
     * @brief
     */
    enum Instructions
    {
        INS_NONE = 0,   ///<
        INS_ADDWF,      ///<
        INS_ANDWF,      ///<
        INS_CLRF,       ///<
        INS_CLRW,       ///<
        INS_COMF,       ///<
        INS_DECF,       ///<
        INS_DECFSZ,     ///<
        INS_INCF,       ///<
        INS_INCFSZ,     ///<
        INS_IORWF,      ///<
        INS_MOVF,       ///<
        INS_MOVWF,      ///<
        INS_NOP,        ///<
        INS_RLF,        ///<
        INS_RRF,        ///<
        INS_SUBWF,      ///<
        INS_SWAPF,      ///<
        INS_XORWF,      ///<
        INS_BCF,        ///<
        INS_BSF,        ///<
        INS_BTFSC,      ///<
        INS_BTFSS,      ///<
        INS_ADDLW,      ///<
        INS_ANDLW,      ///<
        INS_CALL,       ///<
        INS_CLRWDT,     ///<
        INS_GOTO,       ///<
        INS_IORLW,      ///<
        INS_MOVLW,      ///<
        INS_RETFIE,     ///<
        INS_RETLW,      ///<
        INS_RETURN,     ///<
        INS_SLEEP,      ///<
        INS_SUBLW,      ///<
        INS_XORLW,      ///<

        INS__MAX__      ///<
    };
};

#endif // PIC8INSNAMES_H

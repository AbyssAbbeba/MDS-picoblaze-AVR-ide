// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeInsNames.h
 */
// =============================================================================

#ifndef PICOBLAZEINSNAMES_H
#define PICOBLAZEINSNAMES_H

/**
 * @brief
 * @ingroup PicoBlaze
 * @namespace PicoBlazeInsNames
 */
namespace PicoBlazeInsNames
{
    ////    Datatypes    ////

    /**
     * @brief
     */
    enum Instructions
    {
        INS_NONE = 0,    ///<

        INS_JUMP,        ///<
        INS_CALL,        ///<
        INS_RETURN,      ///<
        INS_ADD,         ///<
        INS_ADDCY,       ///<
        INS_SUB,         ///<
        INS_SUBCY,       ///<
        INS_COMPARE,     ///<
        INS_RETURNI,     ///<
        INS_ENABLE_INT,  ///<
        INS_DISABLE_INT, ///<
        INS_LOAD,        ///<
        INS_AND,         ///<
        INS_OR,          ///<
        INS_XOR,         ///<
        INS_TEST,        ///<
        INS_STORE,       ///<
        INS_FETCH,       ///<
        INS_SR0,         ///<
        INS_SR1,         ///<
        INS_SRX,         ///<
        INS_SRA,         ///<
        INS_RR,          ///<
        INS_SL0,         ///<
        INS_SL1,         ///<
        INS_SLX,         ///<
        INS_SLA,         ///<
        INS_RL,          ///<
        INS_INPUT,       ///<
        INS_OUTPUT,      ///<
        INS_OUTPUTK,     ///<
        INS_LD_RET,      ///<
        INS_HWBUILD,     ///<
        INS_STAR,        ///<
        INS_TESTCY,      ///<
        INS_COMPARECY,   ///<
        INS_REGBANK,     ///<

        INS__MAX__       ///<
    };
};

#endif // PICOBLAZEINSNAMES_H

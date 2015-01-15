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
 * @ingroup MCUSim
 * @file MCUSimCPU.h
 */
// =============================================================================

#ifndef MCUSIMCPU_H
#define MCUSIMCPU_H

#include "MCUSimSubsys.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimCPU
 */
class MCUSimCPU : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_CPU_SYS_FATAL_ERROR = 0,  ///<

            EVENT_CPU_ERR_UNDEFINED_OPCODE, ///<
            EVENT_CPU_ERR_INVALID_OPCODE,   ///<
            EVENT_CPU_ERR_INVALID_JUMP,     ///<
            EVENT_CPU_ERR_INVALID_CALL,     ///<
            EVENT_CPU_WRN_INVALID_IRQ,      ///< Invalid interrupt, i.e. nonsense IRQ (Interrupt ReQuest).
            EVENT_CPU_ERR_INVALID_RET,      ///< There is no active subprogram to return from
            EVENT_CPU_ERR_INVALID_RETI,     ///< There is no active interrupt to return from
            EVENT_CPU_ERR_INVALID_OPSET,    ///< undefined result of the operation due to invalid set of operands
            EVENT_CPU_UNSUPPORTED_INST,     ///<
            EVENT_CPU_INST_IGNORED,         ///< (because of settings)
            EVENT_CPU_MODE_CHANGED,         ///<
            EVENT_CPU_MODE_CHANGE_CANCELED, ///< (e.g. SLEEP right after interrupt request)
            EVENT_CPU_PC_OVERFLOW,          ///<
            EVENT_CPU_PC_UNDERFLOW,         ///<
            EVENT_CPU_PC_CHANGED,           ///<

            EVENT_CPU_CALL,                 ///<
            EVENT_CPU_RETURN,               ///<
            EVENT_CPU_IRQ,                  ///<
            EVENT_CPU_RETURN_FROM_ISR,      ///<

            EVENT_ACCUMULATOR_WRITE,        ///< Content of the processor accumultor register has been modified.

            EVENT_CPU__MAX__                ///<
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimCPU() {};

        /**
         * @brief
         * @param[in,out] eventLogger
         */
        MCUSimCPU ( MCUSimEventLogger * eventLogger ) : MCUSimSubsys ( eventLogger, ID_CPU ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual unsigned int getProgramCounter() const = 0;

        /**
         * @brief
         * @param[in] newPc
         */
        virtual void setProgramCounter ( unsigned int newPc ) = 0;

        /**
         * @brief Perform forced return from subroutine or ISR.
         */
        virtual void forceReturn() = 0;

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in,out] eventLogger
         */
        void link ( MCUSimEventLogger * eventLogger )
        {
            MCUSimSubsys::link(eventLogger, ID_CPU);
        }
};

#endif // MCUSIMCPU_H

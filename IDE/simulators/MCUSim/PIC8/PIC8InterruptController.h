// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8InterruptController.h
 */
// =============================================================================

#ifndef PIC8INTERRUPTCTRL_H
#define PIC8INTERRUPTCTRL_H

// Forward declarations
class PIC8DataMemory;
class PIC8InstructionSet;
class PIC8Stack;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8InterruptController
 */
class PIC8InterruptController : public MCUSim::Subsys
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int INTERRUPT_VECTOR = 0x04;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_INT_ENTERING_INTERRUPT, ///<
            EVENT_INT_LEAVING_INTERRUPT,  ///<

            EVENT_INT__MAX__              ///<
        };

        /**
         * @brief
         */
        enum InterruptVector
        {
            INTVEC_NONE = -1,   ///<

            INTVEC_RESET,       ///< External Pin (MCLR), Power-on Reset, and Watchdog Reset
            INTVEC_INT0,        ///< External interrupt RB0/INT pin
            INTVEC_TMR0,        ///< TMR0 overflow interrupt
            INTVEC_PORTB,       ///< PORTB change interrupts (pins RB7:RB4)
            INTVEC_EEPROM,      ///< Data EEPROM write complete interrupt

            INTVEC__MAX__       ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            bool m_possibleInterrupts [ INTVEC__MAX__ ];
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8InterruptController();

        /**
         * @brief
         */
        ~PIC8InterruptController();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] stack
         * @return
         */
        PIC8InterruptController * link ( MCUSim::EventLogger * eventLogger,
                                   MCUSim::Mode        * processorMode,
                                   PIC8DataMemory      * dataMemory,
                                   PIC8InstructionSet  * instructionSet,
                                   PIC8Stack           * stack );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @return -1: system reset requested; 0: do nothing; N: interrupt executed, takes N cycles
         */
        int autoInterrupt();

        /**
         * @brief
         * @return (false in case of an invalid return, i.e. there was no interrupt to return from)
         */
        bool retfie();

        /**
         * @brief
         * @param[in] interrupt
         */
        void genIntReq ( InterruptVector interrupt );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         * @return
         */
        inline InterruptVector detectPendingInterrupt();

        /**
         * @brief
         * @param[in] vector
         * @return
         */
        inline int executeInterrupt ( InterruptVector vector );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Private Attributes    ////
    public:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8InstructionSet * m_instructionSet;

            ///
            PIC8Stack * m_stack;
        //@}

        /**
         * @brief
         */
        MCUSim::Mode * m_processorMode;

        /**
         * @brief
         */
        int m_actInterruptCounter;

        /**
         * @brief
         */
        int m_interruptCounter [ INTVEC__MAX__ ];

        /**
         * @brief
         */
        bool m_intReqWithoutFlag [ INTVEC__MAX__ ];

        /**
         * @brief
         */
        InterruptVector m_interruptToExecute;
};

#endif // PIC8INTERRUPTCTRL_H

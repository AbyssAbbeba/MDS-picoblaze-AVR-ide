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
 * @ingroup PicoBlaze
 * @file PicoBlazeInterruptController.h
 */
// =============================================================================

#ifndef PICOBLAZEINTERRUPTCONTROLLER_H
#define PICOBLAZEINTERRUPTCONTROLLER_H

class PicoBlazeRegisters;

#include "../MCUSim.h"

#include "PicoBlazeInstructionSet.h"
#include "PicoBlazeStatusFlags.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeInterruptController
 */
class PicoBlazeInterruptController : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_INT_IRQ_DENIED,         ///< IRQ denied most probably means that interrupts are just disabled.
            EVENT_INT_ENTERING_INTERRUPT, ///<
            EVENT_INT_LEAVING_INTERRUPT,  ///<

            EVENT_INT__MAX__              ///<
        };

        /**
         * @brief
         */
        struct Config
        {
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeInterruptController();

        /**
         * @brief
         */
        ~PicoBlazeInterruptController();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] instructionSet
         * @param[in,out] statusFlags
         * @param[in,out] registers
         * @return
         */
        PicoBlazeInterruptController * link ( MCUSimEventLogger        * eventLogger,
                                              PicoBlazeInstructionSet  * instructionSet,
                                              PicoBlazeStatusFlags     * statusFlags,
                                              PicoBlazeRegisters       * m_registers );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief Interrupt request.
         */
        void irq();

        /**
         * @brief Return from ISR.
         */
        void returni();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return -1: system reset requested; 0: do nothing; N: interrupt executed, takes N cycles
         */
        inline int autoInterrupt();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void mcuReset();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Private Attributes    ////
    public:
        /// @name PicoBlaze simulator subsystems
        //@{
            ///
            PicoBlazeInstructionSet * m_instructionSet;

            ///
            PicoBlazeStatusFlags * m_statusFlags;

            ///
            PicoBlazeRegisters * m_registers;
        //@}

        ///
        bool m_irq;

        ///
        int m_regBank;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline int PicoBlazeInterruptController::autoInterrupt()
{
    if ( false == m_irq )
    {
        return 0;
    }
    else if ( true == m_statusFlags->getInte() )
    {
        m_irq = false;
        m_regBank = m_registers->getBank();

        m_statusFlags->interrupt();
        m_instructionSet->irq();

        logEvent ( EVENT_INT_ENTERING_INTERRUPT );
        return 1;
    }
    else
    {
        m_irq = false;
        logEvent ( EVENT_INT_IRQ_DENIED );
        return 0;
    }
}

#endif // PICOBLAZEINTERRUPTCONTROLLER_H

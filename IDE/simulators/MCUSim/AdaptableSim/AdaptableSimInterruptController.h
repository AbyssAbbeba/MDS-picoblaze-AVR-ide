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
 * @ingroup AdaptableSim
 * @file AdaptableSimInterruptController.h
 */
// =============================================================================

#ifndef ADAPTABLESIMINTERRUPTCONTROLLER_H
#define ADAPTABLESIMINTERRUPTCONTROLLER_H

#include "../MCUSim.h"

#include "AdaptableSimInstructionSet.h"
#include "AdaptableSimStatusFlags.h"

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimInterruptController
 */
class AdaptableSimInterruptController : public MCUSimSubsys
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
        AdaptableSimInterruptController();

        /**
         * @brief
         */
        ~AdaptableSimInterruptController();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] instructionSet
         * @param[in,out] statusFlags
         * @return
         */
        AdaptableSimInterruptController * link ( MCUSimEventLogger          * eventLogger,
                                                 AdaptableSimInstructionSet * instructionSet,
                                                 AdaptableSimStatusFlags    * statusFlags );

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
        /// @name AdaptableSim simulator subsystems
        //@{
            ///
            AdaptableSimInstructionSet * m_instructionSet;

            ///
            AdaptableSimStatusFlags * m_statusFlags;
        //@}

        ///
        bool m_irq;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline int AdaptableSimInterruptController::autoInterrupt()
{
    if ( false == m_irq )
    {
        return 0;
    }
    else if ( true == m_statusFlags->getInte() )
    {
        m_irq = false;
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

#endif // ADAPTABLESIMINTERRUPTCONTROLLER_H

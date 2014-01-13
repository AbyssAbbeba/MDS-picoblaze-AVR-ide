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
 * @ingroup PIC8
 * @file PIC8Timer0WdtPrescaller.h
 */
// =============================================================================

#ifndef PIC8TIMER0WDTPRESCALLER_H
#define PIC8TIMER0WDTPRESCALLER_H

// Forward declarations
class PIC8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Timer0WdtPrescaller
 */
class PIC8Timer0WdtPrescaller : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_PSC_PRESCALER_CHANGED, ///<

            EVENT_PSC__MAX__             ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            bool m_enabled;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8Timer0WdtPrescaller();

        /**
         * @brief
         */
        ~PIC8Timer0WdtPrescaller();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @return
         */
        PIC8Timer0WdtPrescaller * link ( MCUSimEventLogger * eventLogger,
                                         PIC8DataMemory      * dataMemory );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief
         */
        void clockCycles();

        /**
         * @brief
         */
        unsigned int getPrescalerValue() const;

        /**
         * @brief
         * @param[in] by
         */
        void increment ( unsigned int by = 1 );

        /**
         * @brief
         */
        void prescalerReset();

        /**
         * @brief
         * @return
         */
        unsigned int flagReadAndClear();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool isAssignedToTimer0() const
        {
            return m_assignedToTimer0;
        }

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
        /// @name PIC8 simulator subsystems
        //@{
            /// @brief
            PIC8DataMemory * m_dataMemory;
        //@}

        /**
         * @brief
         */
        bool m_assignedToTimer0;

        /**
         * @brief
         */
        unsigned int m_flag;

        /**
         * @brief
         */
        unsigned int m_value;

        /**
         * @brief
         */
        unsigned int m_ps;
};

#endif // PIC8TIMER0WDTPRESCALLER_H

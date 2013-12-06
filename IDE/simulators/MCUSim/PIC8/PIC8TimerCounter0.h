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
 * @file PIC8TimerCounter0.h
 */
// =============================================================================

#ifndef PIC8TIMER0_H
#define PIC8TIMER0_H

// Forward declarations
class PIC8DataMemory;
class PIC8IO;
class PIC8Timer0WdtPrescaller;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8TimerCounter0
 */
class PIC8TimerCounter0 : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
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
        PIC8TimerCounter0();

        /**
         * @brief
         */
        ~PIC8TimerCounter0();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] io
         * @param[in,out] timer0WdtPrescaller
         * @return
         */
        PIC8TimerCounter0 * link ( MCUSimEventLogger     * eventLogger,
                                   PIC8DataMemory          * dataMemory,
                                   PIC8IO                  * io,
                                   PIC8Timer0WdtPrescaller * timer0WdtPrescaller );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         */
        void clockCycles ( unsigned int cycles );

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
    private:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8DataMemory * m_dataMemory;

            ///
            PIC8IO * m_io;

            ///
            PIC8Timer0WdtPrescaller * m_timer0WdtPrescaller;
        //@}

        ///
        unsigned int m_valToIncrBy;

        ///
        bool m_lastClkIn;
};

#endif // PIC8TIMER0_H

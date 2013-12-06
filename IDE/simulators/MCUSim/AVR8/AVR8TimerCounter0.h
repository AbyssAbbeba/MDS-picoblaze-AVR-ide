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
 * @ingroup AVR8
 * @file AVR8TimerCounter0.h
 */
// =============================================================================

#ifndef AVR8TIMERCOUNTER0_H
#define AVR8TIMERCOUNTER0_H

// Forward declarations
class AVR8DataMemory;
class AVR8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8TimerCounter0
 */
class AVR8TimerCounter0 : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            bool m_enabled; ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8TimerCounter0() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] subsysId
         * @param[in,out] dataMemory
         * @param[in,out] m_io
         * @return
         */
        AVR8TimerCounter0 * link ( MCUSimEventLogger     * eventLogger,
                                   MCUSimSubsys::SubsysId  subsysId,
                                   AVR8DataMemory          * dataMemory,
                                   AVR8IO                  * m_io );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] numberOf
         */
        void clockCycles ( unsigned int numberOf );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool enabled()
        {
            return m_config.m_enabled;
        }

        /**
         * @brief
         * @return
         */
        unsigned int getPrescalerVal() const
        {
            return m_prescaler;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] number
         */
        inline void incrementWithDelay ( unsigned int number );

        /**
         * @brief
         * @param[in] number
         * @param[in] max
         * @return
         */
        inline unsigned int incrementPrescaler ( unsigned int number,
                                                 unsigned int max );

        /**
         * @brief
         * @param[in] number
         */
        inline void incrementTimer ( unsigned int number );

        /**
         * @brief
         */
        inline void resetPrescalerOnCond();

        /**
         * @brief
         */
        inline void clearDelayArray();

        /**
         * @brief
         */
        inline void sampleT0();

        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         */
        inline void determinateClockSource();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        /// @name AVR8 simulator subsystems
        //@{
            ///
            AVR8DataMemory * m_dataMemory;

            ///
            AVR8IO * m_io;
        //@}

        /**
         * @brief
         */
        static const unsigned int COUNTER_DELAY = 3;

        /**
         * @brief
         */
        unsigned int m_incrementWithDelay [ COUNTER_DELAY ];

        /**
         * @brief
         */
        unsigned int m_iwdIndexOut;

        /**
         * @brief
         */
        unsigned int m_iwdIndexIn;

        /**
         * @brief
         */
        unsigned int m_clockSource;

        /**
         * @brief
         */
        unsigned int m_prescaler;

        /**
         * @brief
         */
        bool m_t0Log;
};

#endif // AVR8TIMERCOUNTER0_H

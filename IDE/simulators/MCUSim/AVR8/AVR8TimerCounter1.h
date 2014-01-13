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
 * @file AVR8TimerCounter1.h
 */
// =============================================================================

#ifndef AVR8TIMERCOUNTER1_H
#define AVR8TIMERCOUNTER1_H

// Forward declarations
class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8TimerCounter1
 */
class AVR8TimerCounter1 : public MCUSimSubsys
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
        AVR8TimerCounter1() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] subsysId
         * @param[in,out] dataMemory
         * @return
         */
        AVR8TimerCounter1 * link ( MCUSimEventLogger     * eventLogger,
                                   MCUSimSubsys::SubsysId  subsysId,
                                   AVR8DataMemory          * dataMemory );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

//         /**
//          * @brief
//          * @param[in] numberOf
//          */
//         void clockCycles ( unsigned int numberOf );
//
//         /**
//          * @brief
//          * @param[in] timeStep
//          * @param[in] clockCycles
//          */
//         void timeStep ( float timeStep,
//                         unsigned int clockCycles = 0 );

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

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void resetToInitialValues();

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

    ////    Protected Attributes    ////
    protected:
        /// @name AVR8 simulator subsystems
        //@{
            ///
            AVR8DataMemory * m_dataMemory;
        //@}
};

#endif // AVR8TIMERCOUNTER1_H

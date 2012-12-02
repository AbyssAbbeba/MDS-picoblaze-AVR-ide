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
 * @ingroup AVR8
 * @file AVR8AnalogComparator.h
 */
// =============================================================================

#ifndef AVR8ANALOGCOMPARATOR_H
#define AVR8ANALOGCOMPARATOR_H

// Forward declarations
class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8AnalogComparator
 */
class AVR8AnalogComparator : public MCUSim::Subsys
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
        AVR8AnalogComparator() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @return
         */
        AVR8AnalogComparator * link ( MCUSim::EventLogger * eventLogger,
                                      AVR8DataMemory      * dataMemory );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

//         /**
//          * @brief
//          * @param numberOf: [In]
//          */
//         void clockCycles ( unsigned int numberOf );
//
//         /**
//          * @brief
//          * @param timeStep: [In]
//          * @param clockCycles: [In]
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

    ////    Public Attributes    ////
    public:
        Config m_config; ///<

    ////    Protected Attributes    ////
    protected:
        /// @name AVR8 simulator subsystems
        //@{
            AVR8DataMemory * m_dataMemory; ///<
        //@}

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
};

#endif // AVR8ANALOGCOMPARATOR_H

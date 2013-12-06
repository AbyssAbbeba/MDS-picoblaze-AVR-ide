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
 * @file AVR8Twi.h
 */
// =============================================================================

#ifndef AVR8TWI_H
#define AVR8TWI_H

// Forward declarations
class AVR8DataMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Twi
 */
class AVR8Twi : public MCUSimSubsys
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
        AVR8Twi() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @return
         */
        AVR8Twi * link ( MCUSimEventLogger * eventLogger,
                         AVR8DataMemory      * dataMemory );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

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

#endif // AVR8TWI_H

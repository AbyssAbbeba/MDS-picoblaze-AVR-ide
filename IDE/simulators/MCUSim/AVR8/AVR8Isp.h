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
 * @file AVR8Isp.h
 */
// =============================================================================

#ifndef AVR8ISP_H
#define AVR8ISP_H

// Forward declarations
class AVR8ProgramMemory;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8Isp
 */
class AVR8Isp : public MCUSim::Subsys
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
        AVR8Isp() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] programMemory
         * @return
         */
        AVR8Isp * link ( MCUSim::EventLogger * eventLogger,
                         AVR8ProgramMemory   * programMemory );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

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
            AVR8ProgramMemory * m_programMemory;
        //@}
};

#endif // AVR8ISP_H

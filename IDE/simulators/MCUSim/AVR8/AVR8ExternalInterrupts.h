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
 * @file AVR8ExternalInterrupts.h
 */
// =============================================================================

#ifndef AVR8EXTERNALINTERRUPTS_H
#define AVR8EXTERNALINTERRUPTS_H

// Forward declarations
class AVR8DataMemory;
class AVR8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8ExternalInterrupts
 */
class AVR8ExternalInterrupts : public MCUSim::Subsys
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
        AVR8ExternalInterrupts() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] io
         * @return
         */
        AVR8ExternalInterrupts * link ( MCUSim::EventLogger * eventLogger,
                                        AVR8DataMemory      * dataMemory,
                                        AVR8IO              * io );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        // This has to be called even in a sleep mode (watchdog clock).
        // This method cannot read I/O backwards, it has to be called upon any change on I/O.
        /**
         * @brief
         */
        void clockCycle();

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
            /// @brief
            AVR8DataMemory * m_dataMemory;
        //@}

        /// @brief
        AVR8IO * m_io;

        /// @brief
        bool m_int0prev;

        /// @brief
        bool m_int1prev;
};

#endif // AVR8EXTERNALINTERRUPTS_H

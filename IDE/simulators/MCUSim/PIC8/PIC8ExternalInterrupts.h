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
 * @ingroup PIC8
 * @file PIC8ExternalInterrupts.h
 */
// =============================================================================

#ifndef PIC8EXTERNALINTERRUPTS_H
#define PIC8EXTERNALINTERRUPTS_H

// Forward declarations
class PIC8DataMemory;
class PIC8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ExternalInterrupts
 */
class PIC8ExternalInterrupts : public MCUSim::Subsys
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
        PIC8ExternalInterrupts();

        /**
         * @brief
         */
        ~PIC8ExternalInterrupts();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] io
         * @return
         */
        PIC8ExternalInterrupts * link ( MCUSim::EventLogger * eventLogger,
                                        PIC8DataMemory      * dataMemory,
                                        PIC8IO              * io);

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

    ////    Inline Private Operations    ////
    private:

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
        //@}
};

#endif // PIC8EXTERNALINTERRUPTS_H

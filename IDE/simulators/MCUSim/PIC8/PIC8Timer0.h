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
 * @file PIC8Timer0.h
 */
// =============================================================================

#ifndef PIC8TIMER0_H
#define PIC8TIMER0_H

// Forward declarations
class PIC8DataMemory;
class PIC8IO;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Timer0
 */
class PIC8Timer0 : public MCUSim::Subsys
{
    ////    Public Datatypes    ////
    public:
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
        PIC8Timer0();

        /**
         * @brief
         */
        ~PIC8Timer0();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] io
         * @return
         */
        PIC8Timer0 * link ( MCUSim::EventLogger * eventLogger,
                            PIC8DataMemory * dataMemory,
                            PIC8IO * io );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;
};

#endif // PIC8TIMER0_H

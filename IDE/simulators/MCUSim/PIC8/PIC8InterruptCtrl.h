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
 * @file PIC8InterruptCtrl.h
 */
// =============================================================================

#ifndef PIC8INTERRUPTCTRL_H
#define PIC8INTERRUPTCTRL_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8InterruptCtrl
 */
class PIC8InterruptCtrl : public MCUSim::Subsys
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
        PIC8InterruptCtrl();

        /**
         * @brief
         */
        ~PIC8InterruptCtrl();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8InterruptCtrl * link ( MCUSim::EventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @return (false in case of an invalid return, i.e. there was no interrupt to return from)
         */
        bool retfie();

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;
};

#endif // PIC8INTERRUPTCTRL_H

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeStatusFlags.h
 */
// =============================================================================

#ifndef PICOBLAZESTATUSFLAGS_H
#define PICOBLAZESTATUSFLAGS_H


/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeStatusFlags
 */
class PicoBlazeStatusFlags
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeStatusFlags();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void reset();

        /**
         * @brief
         */
        void interrupt();

        /**
         * @brief
         */
        void returni();

    ////    Public Attributes    ////
    public:
        /**
         * @brief Counter of active interrupt service routines.
         *
         * 0 means no ISR active, 1 means ISR active, more than 1 means that there is a hell lot of a mess :-) but it is
         * still a valid condition of the processor operation, values lower that zero makes no sense.
         */
        int m_interrupted;

        /// Carry flag.
        bool m_carry;

        /// Zero flag.
        bool m_zero;

        /// Value of the Carry flag before interrupt (see PicoBlaze spec. for details).
        bool m_preCarry;

        /// Value of the Zero flag before interrupt (see PicoBlaze spec. for details).
        bool m_preZero;

        /// Interrupt enable flag.
        bool m_inte;
};

#endif // PICOBLAZESTATUSFLAGS_H

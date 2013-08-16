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
        /// Interrupt enable flag
        bool m_inte;

        ///
        int m_interrupted;

        ///
        bool m_carry;

        ///
        bool m_zero;

        ///
        bool m_preCarry;

        ///
        bool m_preZero;
};

#endif // PICOBLAZESTATUSFLAGS_H

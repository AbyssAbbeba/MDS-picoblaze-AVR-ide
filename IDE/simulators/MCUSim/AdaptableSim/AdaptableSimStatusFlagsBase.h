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
 * @ingroup AdaptableSim
 * @file AdaptableSimStatusFlagsBase.h
 */
// =============================================================================

#ifndef ADAPTABLESIMSTATUSFLAGSBASE_H
#define ADAPTABLESIMSTATUSFLAGSBASE_H

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimStatusFlagsBase
 */
class AdaptableSimStatusFlagsBase
{
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

        ///
        bool m_overflow;

        ///
        bool m_negative;

        ///
        bool m_halmCarry;

        ///
        bool m_flag0;

        ///
        bool m_flag1;

        /// Value of the Carry flag before interrupt (see AdaptableSim spec. for details).
        bool m_preCarry;

        /// Value of the Zero flag before interrupt (see AdaptableSim spec. for details).
        bool m_preZero;

        /// Interrupt enable flag.
        bool m_inte;
};

#endif // ADAPTABLESIMSTATUSFLAGSBASE_H

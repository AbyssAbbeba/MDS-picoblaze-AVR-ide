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
 * @ingroup PicoBlaze
 * @file PicoBlazeStatusFlags.h
 */
// =============================================================================

#ifndef PICOBLAZESTATUSFLAGS_H
#define PICOBLAZESTATUSFLAGS_H

#include "../MCUSim.h"
#include "PicoBlazeStatusFlagsBase.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeStatusFlags
 */
class PicoBlazeStatusFlags : public MCUSimSubsys,
                             public PicoBlazeStatusFlagsBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief Events generated by the subsystem.
         */
        enum Event
        {
            EVENT_FLAGS_Z_CHANGED,   ///<
            EVENT_FLAGS_C_CHANGED,   ///<
            EVENT_FLAGS_PZ_CHANGED,  ///<
            EVENT_FLAGS_PC_CHANGED,  ///<
            EVENT_FLAGS_IE_CHANGED,  ///<
            EVENT_FLAGS_INT_CHANGED, ///<

            EVENT_FLAGS__MAX__       ///<
        };

        /**
         * @brief Subsystem configuration.
         */
        struct Config
        {
        };

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
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeStatusFlags * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         */
        void interrupt();

        /**
         * @brief
         */
        void returni();

        /// @name Accessors methods.
        //@{
            /**
             * @brief
             * @return
             */
            int getInterrupted() const
            {
                return m_interrupted;
            }

            /**
             * @brief
             * @return
             */
            bool getCarry() const
            {
                return m_carry;
            }

            /**
             * @brief
             * @return
             */
            bool getZero() const
            {
                return m_zero;
            }

            /**
             * @brief
             * @return
             */
            bool getPreCarry() const
            {
                return m_preCarry;
            }

            /**
             * @brief
             * @return
             */
            bool getPreZero() const
            {
                return m_preZero;
            }

            /**
             * @brief
             * @return
             */
            bool getInte() const
            {
                return m_inte;
            }
        //@}

        /// @name Mutator methods.
        //@{
            /**
             * @brief
             * @return
             */
            void setInterrupted ( int value )
            {
                m_interrupted = value;
                logEvent(EVENT_FLAGS_INT_CHANGED, 0, value);
            }

            /**
             * @brief
             * @return
             */
            void setCarry ( bool flag )
            {
                m_carry = flag;
                logEvent(EVENT_FLAGS_C_CHANGED, 0, flag);
            }

            /**
             * @brief
             * @return
             */
            void setZero ( bool flag )
            {
                m_zero = flag;
                logEvent(EVENT_FLAGS_Z_CHANGED, 0, flag);
            }

            /**
             * @brief
             * @return
             */
            void setPreCarry ( bool flag )
            {
                m_preCarry = flag;
                logEvent(EVENT_FLAGS_PC_CHANGED, 0, flag);
            }

            /**
             * @brief
             * @return
             */
            void setPreZero ( bool flag )
            {
                m_preZero = flag;
                logEvent(EVENT_FLAGS_PZ_CHANGED, 0, flag);
            }

            /**
             * @brief
             * @return
             */
            void setInte ( bool flag )
            {
                m_inte = flag;
                logEvent(EVENT_FLAGS_IE_CHANGED, 0, flag);
            }
        //@}

    ////    Public Attributes    ////
    public:
        /// Subsystem configuration.
        Config m_config;
};

#endif // PICOBLAZESTATUSFLAGS_H

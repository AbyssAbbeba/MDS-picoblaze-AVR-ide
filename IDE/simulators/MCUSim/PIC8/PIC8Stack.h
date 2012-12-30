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
 * @file PIC8Stack.h
 */
// =============================================================================

#ifndef PIC8STACK_H
#define PIC8STACK_H

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Stack
 */
class PIC8Stack : public MCUSim::Subsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_STACK_OVERFLOW,  ///<
            EVENT_STACK_UNDERFLOW  ///<
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            Config()
            {
                m_size = 0;
            }

            ///
            unsigned int m_size;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8Stack();

        /**
         * @brief
         */
        ~PIC8Stack();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8Stack * link ( MCUSim::EventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param value
         */
        inline void pushOnStack ( unsigned int value );

        /**
         * @brief
         * @return
         */
        inline unsigned int popFromStack();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void resetToInitialValues();

        /**
         * @brief
         */
        inline void loadConfig();

        /**
         * @brief
         */
        inline void mcuReset();

    ////    Public Attributes    ////
    public:
        ///
        Config m_config;

    ////    Private Attributes    ////
    private:
        ///
        unsigned int * m_data;

        ///
        unsigned int m_position;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void PIC8Stack::pushOnStack ( unsigned int value )
{
    if ( m_config.m_size == m_position )
    {
        logEvent(EVENT_STACK_OVERFLOW, m_position, value);
        m_position = 0;
    }
    m_data[m_position++] = value;
}

inline unsigned int PIC8Stack::popFromStack()
{
    if ( 0 == m_position )
    {
        logEvent(EVENT_STACK_UNDERFLOW, m_position, -1);
        m_position = m_config.m_size;
    }
    return m_data[--m_position];
}

#endif // PIC8STACK_H

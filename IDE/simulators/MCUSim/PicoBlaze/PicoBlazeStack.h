// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeStack.h
 */
// =============================================================================

#ifndef PICOBLAZESTACK_H
#define PICOBLAZESTACK_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeStack
 */
class PicoBlazeStack : public MCUSim::Memory
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
                m_size = 31; // <-- default value for KCPSM3
            }

            ///
            unsigned int m_size;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeStack();

        /**
         * @brief
         */
        ~PicoBlazeStack();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeStack * link ( MCUSim::EventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSim::ResetMode mode );

        /**
         * @brief
         * @param[in] addr
         * @param[out] data
         * @return
         */
        MCUSim::RetCode directRead ( unsigned int addr,
                                     unsigned int & data ) const;

        /**
         * @brief
         * @param[in] addr
         * @param[in] data
         * @return
         */
        MCUSim::RetCode directWrite ( unsigned int addr,
                                      unsigned int data );

        /**
         * @brief
         * @param[in] newSize
         */
        void resize ( unsigned int newSize );

        /**
         * @brief
         * @param[in] file
         */
        void loadDataFile ( const DataFile * file );

        /**
         * @brief
         * @param[in,out] file
         */
        void storeInDataFile ( DataFile * file ) const;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        unsigned int size() const
        {
            return m_config.m_size;
        }

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

inline void PicoBlazeStack::pushOnStack ( unsigned int value )
{
    if ( m_config.m_size == m_position )
    {
        logEvent(EVENT_STACK_OVERFLOW, m_position, value);
        m_position = 0;
    }
    m_data[m_position++] = value;
}

inline unsigned int PicoBlazeStack::popFromStack()
{
    if ( 0 == m_position )
    {
        logEvent(EVENT_STACK_UNDERFLOW, m_position, -1);
        m_position = m_config.m_size;
    }
    return ( 0x3ff & m_data[--m_position] );
}

#endif // PICOBLAZESTACK_H

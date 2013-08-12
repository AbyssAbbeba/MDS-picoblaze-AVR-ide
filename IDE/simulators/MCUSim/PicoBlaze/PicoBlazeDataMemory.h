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
 * @file PicoBlazeDataMemory.h
 */
// =============================================================================

#ifndef PICOBLAZEDATAMEMORY_H
#define PICOBLAZEDATAMEMORY_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeDataMemory
 */
class PicoBlazeDataMemory : public MCUSimMemory
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            Config()
            {
                m_undefinedValue = -1;
            }

            int m_undefinedValue;           ///< -1 means random
            unsigned int m_size;            ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeDataMemory();

        /**
         * @brief
         */
        ~PicoBlazeDataMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeDataMemory * link ( MCUSimEventLogger * eventLogger );

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
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] file
         */
        void loadDataFile ( const DataFile * file );

        /**
         * @brief
         * @param[in] file
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
            return m_size;
        }

        /**
         * @brief
         * @return
         */
        inline unsigned int getUndefVal() const;

        /**
         * @name Regular memory access methods
         * @brief
         */
        //@{
            /**
             * @brief
             * @param[in] addr
             * @return
             */
            inline unsigned int read ( unsigned int addr );

            /**
             * @brief
             * @param[in] addr
             * @param[in] val
             */
            inline void write ( unsigned int addr,
                                unsigned int val );
        //@}

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
        /**
         * @brief
         */
        Config m_config;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        unsigned int m_size;

        /**
         * @brief
         */
        uint32_t * m_memory;

        /**
         * @brief
         */
        MCUSimEventLogger * m_eventLogger;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PicoBlazeDataMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() &  0xff );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & 0xff );
    }
}

inline void PicoBlazeDataMemory::write ( unsigned int addr,
                                         unsigned int val )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
        }
        else
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
        }
        return;
    }

    register uint32_t result = m_memory[addr];
    const uint32_t valueOrig = ( result & 0xff );

    result &= ( 0xffffff00 ^ MFLAG_DEFAULT );
    result |= ( 0xff & val );
    m_memory[addr] = result;

    logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

    if ( valueOrig != val)
    {
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
    }
}

inline unsigned int PicoBlazeDataMemory::read ( unsigned int addr )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
        }
        else
        {
            logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
        }
        return getUndefVal();
    }

    register uint32_t result = m_memory[addr];

    if ( result & MFLAG_DEFAULT )
    {
        logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
    }

    logEvent(EVENT_MEM_INF_RD_VAL_READ, addr);

    result &= 0x0ff;
    return result;
}

#endif // PICOBLAZEDATAMEMORY_H

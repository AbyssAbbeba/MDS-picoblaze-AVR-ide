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
 * @file PicoBlazeProgramMemory.h
 */
// =============================================================================

#ifndef PICOBLAZEPROGRAMMEMORY_H
#define PICOBLAZEPROGRAMMEMORY_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeProgramMemory
 */
class PicoBlazeProgramMemory : public MCUSimMemory
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
                m_size = 1024; // <-- default value for KCPSM3
            }

            int m_undefinedValue; ///< -1 means random
            unsigned int m_size;  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeProgramMemory();

        /**
         * @brief
         */
        ~PicoBlazeProgramMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeProgramMemory * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

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
         * @param[in] file
         */
        void storeInDataFile ( DataFile * file ) const;

        /**
         * @brief
         * @return
         */
        unsigned int getUndefVal() const;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        inline unsigned int size() const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline unsigned int readRaw ( unsigned int addr );

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
        uint32_t * m_memory;

        /**
         * @brief
         */
        unsigned int m_size;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PicoBlazeProgramMemory::size() const
{
    return m_size;
}

inline unsigned int PicoBlazeProgramMemory::readRaw ( unsigned int addr )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_RD_NONEXISTENT, addr);
            return getUndefVal();
        }
        else
        {
            logEvent(EVENT_MEM_INF_RD_ADDR_OVERFLOW, addr, ( addr % m_size ) );
            addr %= m_size;
        }
    }

    int result = m_memory[addr];
    if ( result & MFLAG_UNDEFINED )
    {
        logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
    }

    return result;
}

inline unsigned int PicoBlazeProgramMemory::read ( unsigned int addr )
{
    return ( 0x3ffff & readRaw(addr) );
}

inline void PicoBlazeProgramMemory::write ( unsigned int addr,
                                            unsigned int val )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(EVENT_MEM_ERR_WR_NONEXISTENT, addr);
            return;
        }
        else
        {
            logEvent(EVENT_MEM_INF_WR_ADDR_OVERFLOW, addr, ( addr % m_size ) );
            addr %= m_size;
        }
    }

    m_memory[addr] &= ( 0xff000000 ^ MFLAG_UNDEFINED );
    m_memory[addr] |= val;

    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

#endif // PICOBLAZEPROGRAMMEMORY_H

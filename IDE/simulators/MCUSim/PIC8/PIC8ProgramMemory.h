// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PIC8
 * @file PIC8ProgramMemory.h
 */
// =============================================================================

#ifndef PIC8PROGRAMMEMORY_H
#define PIC8PROGRAMMEMORY_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"
#include "PIC8ConfigWord.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ProgramMemory
 */
class PIC8ProgramMemory : public MCUSim::Memory
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

            int m_undefinedValue; ///< -1 means random
            unsigned int m_size;  ///<

            unsigned int m_configWordAddress;
            unsigned int m_idLocationsRange[2];
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8ProgramMemory();

        /**
         * @brief
         */
        ~PIC8ProgramMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8ProgramMemory * link ( MCUSim::EventLogger * eventLogger,
                                   PIC8ConfigWord * configWord );

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

        /**
         * @brief
         * @param[in] addr
         * @param[in] val
         * @return
         */
        inline bool handleSpecialAddressWR ( unsigned int addr,
                                             unsigned int val );

        /**
         * @brief
         * @param[in] addr
         * @param[out] result
         * @return
         */
        inline bool handleSpecialAddressRD ( unsigned int addr,
                                             unsigned int * result ) const;

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;


    ////    Protected Attributes    ////
    protected:
        /// @name PIC8 simulator subsystems
        //@{
            PIC8ConfigWord * m_configWord;
        //@}

        /**
         * @brief
         */
        uint32_t * m_memory;

        /**
         * @brief
         */
        unsigned int m_size;

        /**
         * @brief
         */
        unsigned int * m_idLocations;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PIC8ProgramMemory::size() const
{
    return m_size;
}

inline unsigned int PIC8ProgramMemory::readRaw ( unsigned int addr )
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

inline unsigned int PIC8ProgramMemory::read ( unsigned int addr )
{
    unsigned int result;
    if ( false == handleSpecialAddressRD(addr, &result) )
    {
        result = readRaw(addr);
    }
    return ( 0x3fff & result );
}

inline void PIC8ProgramMemory::write ( unsigned int addr,
                                       unsigned int val )
{
    if ( true == handleSpecialAddressWR(addr, val) )
    {
        return;
    }

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

inline bool PIC8ProgramMemory::handleSpecialAddressRD ( unsigned int addr,
                                                        unsigned int * result ) const
{
    if ( m_config.m_configWordAddress == addr )
    {
        *result = m_configWord->getWord();
        return true;
    }
    else if ( m_config.m_idLocationsRange[0] <= addr && addr >= m_config.m_idLocationsRange[1] )
    {
        *result = m_idLocations [ addr - m_config.m_idLocationsRange[0] ];
        return true;
    }
    else
    {
        return false;
    }
}

inline bool PIC8ProgramMemory::handleSpecialAddressWR ( unsigned int addr,
                                                        unsigned int val )
{
    if ( m_config.m_configWordAddress == addr )
    {
        m_configWord->setWord(val);
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
        return true;
    }
    else if ( m_config.m_idLocationsRange[0] <= addr && addr >= m_config.m_idLocationsRange[1] )
    {
        m_idLocations [ addr - m_config.m_idLocationsRange[0] ] = val;
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
        return true;
    }
    else
    {
        return false;
    }
}

#endif // PIC8PROGRAMMEMORY_H

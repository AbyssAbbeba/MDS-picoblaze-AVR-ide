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
 * @file AdaptableSimProgramMemory.h
 */
// =============================================================================

#ifndef ADAPTABLESIMPROGRAMMEMORY_H
#define ADAPTABLESIMPROGRAMMEMORY_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimProgramMemory
 */
class AdaptableSimProgramMemory : public MCUSimMemory
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum WordSize
        {
            WORD_1B, ///<
            WORD_2B, ///<
            WORD_3B  ///<
        };

        /**
         * @brief
         */
        enum Endian
        {
            END_BIG,   ///<
            END_LITTLE ///<
        };

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
            WordSize m_wordSize;  ///<
            Endian m_endian;      ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AdaptableSimProgramMemory();

        /**
         * @brief
         */
        ~AdaptableSimProgramMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        AdaptableSimProgramMemory * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief
         * @param[in] addr
         * @param[out] data
         * @return
         */
        virtual MCUSim::RetCode directRead ( unsigned int addr,
                                             unsigned int & data ) const override;

        /**
         * @brief
         * @param[in] addr
         * @param[in] data
         * @return
         */
        virtual MCUSim::RetCode directWrite ( unsigned int addr,
                                              unsigned int data ) override;

        /**
         * @brief
         * @param[in] newSize
         */
        virtual void resize ( unsigned int newSize ) override;

        /**
         * @brief
         * @param[in] file
         */
        virtual void loadDataFile ( const DataFile * file ) override;

        /**
         * @brief
         * @param[in] file
         */
        virtual void storeInDataFile ( DataFile * file ) const override;

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
        virtual unsigned int size() const override;

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

inline unsigned int AdaptableSimProgramMemory::size() const
{
    return m_size;
}

inline unsigned int AdaptableSimProgramMemory::readRaw ( unsigned int addr )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_MEM_ERR_RD_NONEXISTENT, addr);
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

inline unsigned int AdaptableSimProgramMemory::read ( unsigned int addr )
{
    return ( 0x3ffff & readRaw(addr) );
}

inline void AdaptableSimProgramMemory::write ( unsigned int addr,
                                            unsigned int val )
{
    if ( addr >= m_size )
    {
        if ( 0 == m_size )
        {
            logEvent(MCUSimEventLogger::FLAG_HI_PRIO, EVENT_MEM_ERR_WR_NONEXISTENT, addr);
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

#endif // ADAPTABLESIMPROGRAMMEMORY_H

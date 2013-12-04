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
 * @ingroup AVR8
 * @file AVR8ProgramMemory.h
 */
// =============================================================================

#ifndef AVR8PROGRAMMEMORY_H
#define AVR8PROGRAMMEMORY_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"
#include "AVR8BootLoader.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8ProgramMemory
 */
class AVR8ProgramMemory : public MCUSimMemory
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            /// @brief
            Config()
            {
                m_undefinedValue = -1;
            }

            ///
            int m_undefinedValue; // -1 means random

            ///
            unsigned int m_size;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8ProgramMemory();

        /**
         * @brief
         */
        ~AVR8ProgramMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] bootLoader
         * @return
         */
        AVR8ProgramMemory * link ( MCUSimEventLogger * eventLogger,
                                   AVR8BootLoader * bootLoader );

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
        /// @name AVR8 simulator subsystems
        //@{
            ///
            AVR8BootLoader * m_bootLoader;
        //@}

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

inline unsigned int AVR8ProgramMemory::size() const
{
    return m_size;
}

inline unsigned int AVR8ProgramMemory::readRaw ( unsigned int addr )
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

    if ( true == m_bootLoader->inUse(addr) )
    {
        logEvent(EVENT_MEM_ERR_RD_ACCESS_DENIED, addr);
        return getUndefVal();
    }

    int result = m_memory[addr];
    if ( result & MFLAG_UNDEFINED )
    {
        logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
    }

    return result;
}

inline unsigned int AVR8ProgramMemory::read ( unsigned int addr )
{
    return (readRaw(addr) & 0x0ffff);
}

inline void AVR8ProgramMemory::write ( unsigned int addr,
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

    m_memory[addr] &= (0xff000000 ^ MFLAG_UNDEFINED);
    m_memory[addr] |= val;

    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

#endif // AVR8PROGRAMMEMORY_H

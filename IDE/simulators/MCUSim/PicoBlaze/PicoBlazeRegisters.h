// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeRegisters.h
 */
// =============================================================================

#ifndef PICOBLAZEREGISTERS_H
#define PICOBLAZEREGISTERS_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeRegisters
 */
class PicoBlazeRegisters : public MCUSimMemory
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
            bool m_banks;         ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PicoBlazeRegisters();

        /**
         * @brief
         */
        ~PicoBlazeRegisters();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PicoBlazeRegisters * link ( MCUSimEventLogger * eventLogger );

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
         * @param[in] mode
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

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
         * @param[in] bank
         */
        void setBank ( int bank );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        virtual unsigned int size() const override
        {
            return m_size;
        }

        /**
         * @brief
         * @return
         */
        int getBank() const
        {
            return m_bank;
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

            /**
             * @brief
             * @param[in] addr
             * @param[in] val
             */
            inline void write ( unsigned int addr,
                                unsigned int val,
                                int targetBank );
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

        /**
         * @brief
         */
        int m_bank;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline unsigned int PicoBlazeRegisters::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( ( unsigned int ) rand() &  0xff );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & 0xff );
    }
}

inline void PicoBlazeRegisters::write ( unsigned int addr,
                                        unsigned int val )
{
    if ( true == m_config.m_banks )
    {
        addr += ( m_bank * ( m_size / 2 ) );
    }

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

    if ( valueOrig != val )
    {
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
    }
}

inline void PicoBlazeRegisters::write ( unsigned int addr,
                                        unsigned int val,
                                        int targetBank )
{
    bool bankOrig = m_bank;
    m_bank = targetBank;
    write(addr, val);
    m_bank = bankOrig;
}

inline unsigned int PicoBlazeRegisters::read ( unsigned int addr )
{
    if ( true == m_config.m_banks )
    {
        addr += ( m_bank * ( m_size / 2 ) );
    }

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

#endif // PICOBLAZEREGISTERS_H

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8DataMemory.h
 */
// =============================================================================

#ifndef AVR8DATAMEMORY_H
#define AVR8DATAMEMORY_H

// Forward declarations
class DataFile;

#include "AVR8RegNames.h"
#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DataMemory
 */
class AVR8DataMemory : public MCUSim::Memory
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            ///
            EVENT_DMEM_STACK_OVERFLOW = EVENT_MEM__MAX__,

            ///
            EVENT_DMEM_STACK_UNDERFLOW,
        };

        /**
         * @brief
         */
        struct Config
        {
            /**
             * @brief
             */
            Config();

            /**
             * @brief
             */
            ~Config();

            int m_undefinedValue;               ///< // -1 means random
            unsigned int m_regFileSize;         ///<
            unsigned int m_sramSize;            ///<
            unsigned int m_ioRegSize;           ///<

            uint32_t * m_ioRegInitValues;       ///< // |8-bit: FLAGS|8-bit: readable|8-bit writable|8-bit: value|
            uint8_t * m_ioRegRandomInit;        ///< // |8-bit: bit mask, log. 1 means random value, 0 means keep initial value|

            unsigned int m_mem2size;            ///<
            unsigned int * m_mem2sizes;         ///<

            uint32_t ** m_ioMem2InitValues;     ///<

            unsigned int m_spWidth;             ///<
            int m_spMax;                        ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8DataMemory();

        /**
         * @brief
         */
        ~AVR8DataMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        AVR8DataMemory * link ( MCUSim::EventLogger * eventLogger );

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
        void reset ( MCUSim::ResetMode mode );

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
            inline unsigned int read ( uint32_t addr );

            /**
             * @brief
             * @param[in] addr
             * @param[in] val
             */
            inline void write ( uint32_t addr,
                                unsigned int val );

            /**
             * @brief
             * @param[in] value
             */
            inline void pushOnStack ( const unsigned int value );

            /**
             * @brief
             * @return
             */
            inline unsigned int popFromStack();
        //@}

        /**
         * @name Rapid memory access methods
         * @brief
         * These methods are significantly faster than regular access methods. The fast access is achieved by omitting
         * security checks which ensure safe access to the simulated memory. Incorrect usage of these methods might lead
         * to mysterious random malfunctions.
         * @warning Do not use these methods unless you REALLY KNOW what you are doing!
         */
        //@{
            /**
             * @brief
             * @warning Incorrect use might severely corrupt the simulator session!
             *
             * @param[in] regAddr Address of the source register.
             * @param[in] bitMask (0x01 = 0th bit, 0x02 = 1st bit, ... 0x80 = 7th bit)
             * @return
             */
            inline bool readBitFast ( unsigned int regAddr,
                                      unsigned int bitMask );

            /**
             * @brief
             * @warning Incorrect use might severely corrupt the simulator session!
             *
             * @param[in] regAddr Address of the target register.
             * @param[in] bitMask (0x01 = 0th bit, 0x02 = 1st bit, ... 0x80 = 7th bit)
             * @return
             */
            inline void setBitFast ( unsigned int regAddr,
                                     unsigned int bitMask );

            /**
             * @brief
             * @warning Incorrect use might severely corrupt the simulator session!
             *
             * @param[in] regAddr Address of the target register.
             * @param[in] bitMask (0x01 = 0th bit, 0x02 = 1st bit, ... 0x80 = 7th bit)
             */
            inline void clearBitFast ( unsigned int regAddr,
                                       unsigned int bitMask );

            /**
             * @brief
             * @warning Incorrect use might severely corrupt the simulator session!
             *
             * @param[in] addr Address of the source register.
             * @return
             */
            inline unsigned int readFast ( unsigned int addr );

            /**
             * @brief
             * @warning Incorrect use might severely corrupt the simulator session!
             *
             * @param[in] addr Address of the target register.
             * @param[in] val 8-bit value to be set in the register (must be 8-bit!)
             */
            inline void writeFast ( unsigned int addr,
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
        uint32_t * m_memory;

        /**
         * @brief
         */
        uint32_t ** m_memory2;

        /**
         * @brief
         */
        unsigned int m_size;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline bool AVR8DataMemory::readBitFast ( unsigned int regAddr,
                                          unsigned int bitMask )
{
    return bool(m_memory[regAddr] & bitMask);
}

inline void AVR8DataMemory::setBitFast ( unsigned int regAddr,
                                         unsigned int bitMask )
{
    m_memory[regAddr] |= bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline void AVR8DataMemory::clearBitFast ( unsigned int regAddr,
                                           unsigned int bitMask )
{
    m_memory[regAddr] &= ~bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline unsigned int AVR8DataMemory::readFast ( unsigned int addr )
{
    return (m_memory[addr] & 0xff);
}

inline void AVR8DataMemory::writeFast ( unsigned int addr,
                                        unsigned int val )
{
    m_memory[addr] &= (0xffffff00 ^ (MFLAG_UNDEFINED | MFLAG_DEFAULT));
    m_memory[addr] |= val;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

inline void AVR8DataMemory::write ( uint32_t addr,
                                    unsigned int val )
{
    unsigned int addrVariant = (addr >> 24);
    addr &= ((1 << 24) - 1);

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

    if ( result & MFLAG_VIRTUAL )
    {
        if ( 0 == addrVariant )
        {
            // Determinate address variant, somehow ... ??
        }

        addr = (result & 0xffff);

        if ( addr >= m_config.m_mem2size )
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return;
        }
        if ( addrVariant >= m_config.m_mem2sizes[addr] )
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return;
        }
        result = m_memory2[addr][addrVariant];
    }

    if ( 0 == (result & 0xffff00) )
    {
        logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
        return;
    }
    if ( 0 == (result & 0xff00) )
    {
        logEvent(EVENT_MEM_WRN_WR_READ_ONLY, addr);
        return;
    }

    if ( result & MFLAG_RESERVED )
    {
        logEvent(EVENT_MEM_WRN_WR_RESERVED_WRITTEN, addr);
    }

    const uint32_t valueOrig = (result & 0xff);
    val &= ((result & 0xff00) >> 8);

    result &= (0xffffff00 ^ (MFLAG_UNDEFINED | MFLAG_DEFAULT));
    result |= val;

    if ( 0 != addrVariant )
    {
        m_memory2[addr][addrVariant] = result;
    }
    else
    {
        m_memory[addr] = result;
    }

    logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, addr);

    if ( valueOrig != val)
    {
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
    }
}

inline void AVR8DataMemory::pushOnStack ( const unsigned int value )
{
    int sp = 0;     // Stack pointer (at most 16b)

    // Load stack pointer
    if ( m_config.m_spWidth > 8 )
    {
        sp = read(AVR8RegNames::SPH);   // Stack Pointer High Register
        sp <<= 8;
    }
    sp |= read(AVR8RegNames::SPL);          // Stack Pointer Low Register

    // Push the value onto stack, post decrement scheme
    write(sp--, value);

    // Check stack pointer for allowed range
    while ( sp < 0 )
    {
        sp += m_config.m_spMax;
        logEvent(EVENT_DMEM_STACK_UNDERFLOW);
    }
    while ( sp > m_config.m_spMax )
    {
        sp -= m_config.m_spMax;
        logEvent(EVENT_DMEM_STACK_OVERFLOW);
    }

    // Store stack pointer
    write(AVR8RegNames::SPL, sp & 0xff);    // Stack Pointer Low Register
    if ( m_config.m_spWidth > 8 )
    {
        sp &= 0xff00;
        sp >>= 8;
        write(AVR8RegNames::SPH, sp);   // Stack Pointer High Register
    }
}

inline unsigned int AVR8DataMemory::popFromStack()
{
    int sp = 0;     // Stack pointer (at most 16b)

    // Load stack pointer
    if ( m_config.m_spWidth > 8 )
    {
        sp = read(AVR8RegNames::SPH);   // Stack Pointer High Register
        sp <<= 8;
    }
    sp |= read(AVR8RegNames::SPL);          // Stack Pointer Low Register

    // Push the value onto stack, post decrement scheme
    unsigned int value = read(++sp);

    // Check stack pointer for allowed range
    while ( sp < 0 )
    {
        sp += m_config.m_spMax;
        logEvent(EVENT_DMEM_STACK_UNDERFLOW);
    }
    while ( sp > m_config.m_spMax )
    {
        sp -= m_config.m_spMax;
        logEvent(EVENT_DMEM_STACK_OVERFLOW);
    }

    // Store stack pointer
    write(AVR8RegNames::SPL, sp & 0xff);    // Stack Pointer Low Register
    if ( m_config.m_spWidth > 8 )
    {
        sp &= 0xff00;
        sp >>= 8;
        write(AVR8RegNames::SPH, sp);   // Stack Pointer High Register
    }

    return value;
}

inline unsigned int AVR8DataMemory::read ( uint32_t addr )
{
    unsigned int addrVariant = (addr >> 24);
    addr &= ((1 << 24) - 1);

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

    if ( result & MFLAG_VIRTUAL )
    {
        if ( 0 == addrVariant )
        {
            // Determinate address variant, somehow ... ??
        }

        addr = (result & 0xffff);

        if ( addr >= m_config.m_mem2size )
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return getUndefVal();
        }
        if ( addrVariant >= m_config.m_mem2sizes[addr] )
        {
            logEvent(EVENT_MEM_ERR_WR_NOT_IMPLEMENTED, addr);
            return getUndefVal();
        }

        result = m_memory2[addr][addrVariant];
    }

    if ( 0 == (result & 0xffff00) )
    {
        logEvent(EVENT_MEM_ERR_RD_NOT_IMPLEMENTED, addr);
    }
    if ( 0 == (result & 0xff0000) )
    {
        logEvent(EVENT_MEM_WRN_RD_WRITE_ONLY, addr);
    }

    if ( result & MFLAG_RESERVED )
    {
        logEvent(EVENT_MEM_WRN_RD_RESERVED_READ, addr);
    }
    if ( result & MFLAG_UNDEFINED )
    {
        logEvent(EVENT_MEM_WRN_RD_UNDEFINED, addr);
    }
    if ( result & MFLAG_DEFAULT )
    {
        logEvent(EVENT_MEM_WRN_RD_DEFAULT, addr);
    }

    logEvent(EVENT_MEM_INF_RD_VAL_READ, addr);

    if ( 0 != (result & 0xff0000) )
    {
        result ^= ((result >> 16) & 0xff) & getUndefVal();
    }

    result &= 0x0ff;
    return result;
}

inline unsigned int AVR8DataMemory::getUndefVal() const
{
    if ( -1 == m_config.m_undefinedValue )
    {
        // Generate random value
        return ( (unsigned int)rand() & ((1 << 8) - 1) );
    }
    else
    {
        // Return predefined value
        return ( m_config.m_undefinedValue & ((1 << 8) - 1) );
    }
}

#endif // AVR8DATAMEMORY_H

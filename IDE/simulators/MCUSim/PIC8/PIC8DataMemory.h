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
 * @file PIC8DataMemory.h
 */
// =============================================================================

#ifndef PIC8DATAMEMORY_H
#define PIC8DATAMEMORY_H

// Forward declarations
class DataFile;

#include "PIC8RegNames.h"
#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8DataMemory
 */
class PIC8DataMemory : public MCUSim::Memory
{
public:
    static const unsigned int NOMINAL_BANK_SIZE = 128;

    PIC8DataMemory();
    ~PIC8DataMemory();

    struct Config
    {
        Config();

        int m_undefinedValue; // -1 means random
        unsigned int m_size;
        unsigned int * m_addrTransTbl;
        unsigned int m_addrTransTblSize;
    };

    Config m_config;

    PIC8DataMemory * link ( MCUSim::EventLogger * eventLogger );

    MCUSim::RetCode directRead ( unsigned int addr, unsigned int & data ) const;
    MCUSim::RetCode directWrite ( unsigned int addr, unsigned int data );
    void resize ( unsigned int newSize );
    unsigned int size() const
    {
        return m_config.m_size;
    }
    void reset ( MCUSim::ResetMode mode );

    void loadDataFile(const DataFile * file);
    void storeInDataFile(DataFile * file) const;

    /// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
    inline bool readBitFast(int regAddr, unsigned int bitMask);
    /// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
    inline void setBitFast(int regAddr, unsigned int bitMask);
    /// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
    inline void clearBitFast(int regAddr, unsigned int bitMask);
    /// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
    inline unsigned int readFast(int addr);
    /// @warning Do not use this method unless you REALLY KNOW WHAT YOU ARE DOING!
    inline void writeFast(int addr, unsigned int val);

    unsigned int read(int addr);
    void write(int addr, unsigned int val);

    unsigned int getUndefVal() const;

    int addrTrans(int addr);
    unsigned int getActiveBank();

protected:
    uint32_t * m_memory;
    int * m_addrTransTab;

    MCUSim::EventLogger * m_eventLogger;

    inline void resetToInitialValues();
    inline void loadConfig();
    inline void mcuReset();
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline bool PIC8DataMemory::readBitFast(unsigned int regAddr, unsigned int bitMask) {
    return bool(m_memory[regAddr] & bitMask);
}

inline void PIC8DataMemory::setBitFast(unsigned int regAddr, unsigned int bitMask) {
    m_memory[regAddr] |= bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline void PIC8DataMemory::clearBitFast(unsigned int regAddr, unsigned int bitMask) {
    m_memory[regAddr] &= ~bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline unsigned int PIC8DataMemory::readFast(unsigned int addr) {
    return (m_memory[addr] & 0xff);
}

inline void PIC8DataMemory::writeFast(unsigned int addr, unsigned int val) {
    m_memory[addr] &= (0xffffff00 ^ (MFLAG_UNDEFINED | MFLAG_DEFAULT));
    m_memory[addr] |= val;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

#endif // PIC8DATAMEMORY_H

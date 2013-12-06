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
 * @ingroup AVR8
 * @file AVR8BootLoader.h
 */
// =============================================================================

#ifndef AVR8BOOTLOADER_H
#define AVR8BOOTLOADER_H

// Forward declarations
class AVR8ProgramMemory;
class AVR8DataMemory;
class AVR8FusesAndLocks;
class AVR8DataEEPROM;
class AVR8InstructionSet;

#include "../MCUSim.h"
#include "AVR8Sim.h"

// Standard headers
#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8BootLoader
 */
class AVR8BootLoader : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            EVENT_BOOT_WR_ACCESS_NOT_ALLOWED,   ///<
            EVENT_BOOT_RD_ACCESS_NOT_ALLOWED,   ///<
            EVENT_BOOT_INVALID_CR_CHAGE,        ///<
            EVENT_BOOT_INVALID_PAGE_ADDR,       ///<
            EVENT_BOOT_WR_BUSY,                 ///<
            EVENT_BOOT_OVERLOAD,                ///< practically that means that the EEPROM write is in progress while attempting to use SPM
            EVENT_BOOT_NO_OPER_SELECTED,        ///<
            EVENT_BOOT_ADDR_OUT_OF_RANGE,       ///<

            EVENT_BOOT_ERR_INVALID_ADDR,        ///<
            EVENT_BOOT_WRN_INVALID_ADDR         ///< (This is only a warning)
        };

        /**
         * @brief
         */
        struct Config
        {
            bool m_enabled;                     ///<
            float m_minProgTime;                ///< // 3.7e-3
            float m_maxProgTime;                ///< // 4.5e-3
            unsigned int m_pageSize;            ///< // 32 WORDS on ATmega8
            int m_bootResetAddress[4];          ///<
            unsigned int m_rwwSectionSize;      ///< // number of PAGES
        };

    ////    Protected Datatypes    ////
    protected:
        /**
         * @brief
         */
        enum SpmMode
        {
            SPMMD_NONE,     ///< None
            SPMMD_BUFFER,   ///< Write to the buffer
            SPMMD_PGERS,    ///< Page Erase
            SPMMD_PGWRT,    ///< Page Write
            SPMMD_BLBSET,   ///< Boot Lock Bit Set
            SPMMD_RWWSRE    ///< Read-While-Write Section Read Enable
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8BootLoader();

        /**
         * @brief
         */
        ~AVR8BootLoader();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] programMemory
         * @param[in,out] dataMemory
         * @param[in,out] fusesAndLocks
         * @param[in,out] dataEEPROM
         * @param[in,out] instructionSet
         * @param[in,out] haltMode
         * @return
         */
        AVR8BootLoader * link ( MCUSimEventLogger * eventLogger,
                                AVR8ProgramMemory   * programMemory,
                                AVR8DataMemory      * dataMemory,
                                AVR8FusesAndLocks   * fusesAndLocks,
                                AVR8DataEEPROM      * dataEEPROM,
                                AVR8InstructionSet  * instructionSet,
                                AVR8Sim::HaltMode   * haltMode );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] timeStep
         * @param[in] clockCycles
         */
        void timeStep ( float timeStep,
                        unsigned int clockCycles = 0 );

        /**
         * @brief
         * @param[in] addr
         * @param[in] val
         * @return
         */
        unsigned int spmWrite ( unsigned int addr,
                                unsigned int val );
        /**
         * @brief
         * @param[in] addr
         * @return
         */
        unsigned int lpmRead ( unsigned int addr );

        /**
         * @brief
         * @return
         */
        unsigned int getBootAddress() const;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool enabled()
        {
            return m_config.m_enabled;
        }

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool inUse ( unsigned int addr ) const;

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
         * @param[in] spmcr
         * @return
         */
        inline SpmMode determinateSpmMode ( unsigned int spmcr ) const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool isInApplicationSection ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool isInRWWSection ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool isReadAllowed ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool isWriteAllowed ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] timeStep
         */
        inline void manageProgTimer ( const float timeStep );

        /**
         * @brief
         * @param[in] clockCycles
         */
        inline void manageContRegTimer ( const unsigned int clockCycles );

        /**
         * @brief
         * @param[in] clockCycles
         */
        inline void normalizeControlReg ( const unsigned int clockCycles );

        /**
         * @brief
         * @param[in] addr
         */
        inline void pageErase ( unsigned int addr );

        /**
         * @brief
         * @param[in] addr
         * @param[in] val
         */
        inline void setLockBits ( unsigned int addr,
                                  unsigned int val );

        /**
         * @brief
         * @param[in] addr
         */
        inline void pageWrite ( unsigned int addr );

        /**
         * @brief
         * @return
         */
        inline float randomTimeInRange() const;

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
        SpmMode m_spmMode;

        /**
         * @brief
         */
        unsigned int m_spmcrLast;

        /**
         * @brief
         */
        int m_cr_timer;

        /**
         * @brief
         */
        uint32_t * m_writeBuffer;

        /**
         * @brief
         */
        float m_progTimer;

        /**
         * @brief
         */
        bool m_writeInProgress;

        /**
         * @brief
         */
        bool m_rwwSectionBusy;

        /// @name AVR8 simulator subsystems
        //@{
            AVR8ProgramMemory * m_programMemory;    ///<
            AVR8DataMemory * m_dataMemory;          ///<
            AVR8FusesAndLocks * m_fusesAndLocks;    ///<
            AVR8DataEEPROM * m_dataEEPROM;          ///<
            AVR8InstructionSet * m_instructionSet;  ///<
            AVR8Sim::HaltMode * m_haltMode;         ///<
        //@}
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline bool AVR8BootLoader::inUse ( unsigned int addr ) const
{
    return ( (true == m_rwwSectionBusy) && (true == isInRWWSection(addr)) );
}

inline bool AVR8BootLoader::isInRWWSection ( unsigned int addr ) const
{
    return ( addr < (m_config.m_pageSize * m_config.m_rwwSectionSize) );
}

#endif // AVR8BOOTLOADER_H

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
 * @file PIC8DataMemory.h
 */
// =============================================================================

#ifndef PIC8DATAMEMORY_H
#define PIC8DATAMEMORY_H

// Forward declarations
class DataFile;
class PIC8ExternalInterrupts;
class PIC8InstructionSet;

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
    ////    Public Static Constants    ////
    public:
        ///
        static const unsigned int NOMINAL_BANK_SIZE = 128;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            Config();

            ///
            ~Config();

            int m_undefinedValue;           ///< -1 means random
            unsigned int m_size;            ///<
            unsigned int * m_randomInit;    ///<
            int * m_addrTransTab;           ///<
            uint32_t * m_initValues;        ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8DataMemory();

        /**
         * @brief
         */
        ~PIC8DataMemory();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8DataMemory * link ( MCUSim::EventLogger    * eventLogger,
                                PIC8ExternalInterrupts * externalInterrupts,
                                PIC8InstructionSet     * instructionSet );

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
            unsigned int read ( int addr );

            /**
             * @brief
             * @param[in] addr
             * @param[in] val
             */
            void write ( int addr,
                         unsigned int val );
        //@}

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
                                      unsigned int bitMask ) const;

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
            inline unsigned int readFast ( unsigned int addr ) const;

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

        /**
         * @brief
         * @param[in] addr
         * @param[in] allowIndirect
         * @return
         */
        inline int addrTrans ( int addr,
                               bool allowIndirect = true ) const;

        /**
         * @brief
         * @return
         */
        inline unsigned int getActiveBank() const;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        inline bool isAddrGenerallyValid ( const unsigned int addr ) const;

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
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8ExternalInterrupts * m_externalInterrupts;

            PIC8InstructionSet * m_instructionSet;
        //@}

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
        MCUSim::EventLogger * m_eventLogger;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline bool PIC8DataMemory::readBitFast ( unsigned int regAddr,
                                          unsigned int bitMask ) const
{
    return (bool) ( m_memory[regAddr] & bitMask );
}

inline void PIC8DataMemory::setBitFast ( unsigned int regAddr,
                                         unsigned int bitMask )
{
    m_memory[regAddr] |= bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline void PIC8DataMemory::clearBitFast ( unsigned int regAddr,
                                           unsigned int bitMask )
{
    m_memory[regAddr] &= ~bitMask;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, regAddr);
}

inline unsigned int PIC8DataMemory::readFast ( unsigned int addr ) const
{
    return ( m_memory[addr] & 0xff );
}

inline void PIC8DataMemory::writeFast ( unsigned int addr,
                                        unsigned int val )
{
    m_memory[addr] &= ( 0xffffff00 ^ ( MFLAG_UNDEFINED | MFLAG_DEFAULT ) );
    m_memory[addr] |= val;
    logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, addr);
}

#endif // PIC8DATAMEMORY_H

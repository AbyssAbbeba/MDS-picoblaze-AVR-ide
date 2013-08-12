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
 * @file AVR8DataEEPROM.h
 */
// =============================================================================

#ifndef AVR8DATAEEPROM_H
#define AVR8DATAEEPROM_H

// Forward declarations
class DataFile;
class AVR8DataMemory;
class AVR8InterruptController;

#include "../MCUSim.h"

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DataEEPROM
 */
class AVR8DataEEPROM : public MCUSimMemory
{
    ////    Private Constants    ////
    private:
        static const int WB_ADDR = 0;   ///<
        static const int WB_DATA = 1;   ///<

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Event
        {
            ///
            EVENT_EEPROM_INVALID_CR_CHAGE = EVENT_MEM__MAX__,

            ///
            EVENT_EEPROM__MAX__
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

            bool m_enabled;              ///<
            unsigned int m_addrRegWidth; ///<
            unsigned int m_size;         ///<
            float m_writeTime;           ///< // 8.448 (in seconds!)
            int m_undefinedValue;        ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AVR8DataEEPROM();

        /**
         * @brief
         */
        ~AVR8DataEEPROM();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] dataMemory
         * @param[in,out] interruptControllers
         * @return
         */
        AVR8DataEEPROM * link ( MCUSimEventLogger     * eventLogger,
                                AVR8DataMemory          * dataMemory,
                                AVR8InterruptController * interruptControllers );

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
         * @param[in] timeStep
         * @param[in] clockCycles
         * @return
         */
        unsigned int timeStep ( float timeStep,
                                unsigned int clockCycles = 0 );    // <-- This has to be called even in a sleep mode

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        unsigned int size() const
        {
            return m_config.m_size;
        }

        /**
         * @brief
         * @return
         */
        bool writeInProgress() const
        {
            return m_writeInProgress;
        }

        /**
         * @brief
         * @return
         */
        bool enabled()
        {
            return m_config.m_enabled;
        }

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @return
         */
        unsigned int getUndefVal() const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] clockCycles
         * @return
         */
        inline unsigned int readEecr ( const unsigned int clockCycles );

        /**
         * @brief
         * @param[in] timeStep
         * @param[in] eecr
         * @return
         */
        inline void writeByte ( float timeStep,
                                unsigned int eecr );

        /**
         * @brief
         * @return
         */
        inline unsigned int readByte();

        /**
         * @brief
         * @return
         */
        inline void loadConfig();

        /**
         * @brief
         * @return
         */
        inline void resetToInitialValues();

        /**
         * @brief
         * @return
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
            /// @brief
            AVR8DataMemory * m_dataMemory;

            /// @brief
            AVR8InterruptController * m_interruptController;
        //@}

        /**
         * @brief
         */
        uint32_t * m_memory;

        /**
         * @brief
         */
        unsigned int m_eearLast;

        /**
         * @brief
         */
        unsigned int m_eecrLast;

        /**
         * @brief
         */
        bool m_writeInProgress;

        /**
         * @brief
         */
        int m_eecr_timer;

        /**
         * @brief
         */
        unsigned int m_writeBuffer[2];

        /**
         * @brief
         */
        float m_writeTimer;
};

#endif // AVR8DATAEEPROM_H

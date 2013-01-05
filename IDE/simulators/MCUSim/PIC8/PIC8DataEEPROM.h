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
 * @ingroup PIC8
 * @file PIC8DataEEPROM.h
 */
// =============================================================================

#ifndef PIC8DATAEEPROM_H
#define PIC8DATAEEPROM_H

// Forward declarations
class DataFile;
class PIC8DataMemory;

#include "PIC8RegNames.h"
#include "../MCUSim.h"

#include <cstdint>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8DataEEPROM
 */
class PIC8DataEEPROM : public MCUSim::Memory
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int NOMINAL_WRITE_PERIOD = 10e-3; // 10ms

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
            EVENT_EEPROM_WRITE_INHIBITED,

            ///
            EVENT_EEPROM_WRITE_STARTED,

            ///
            EVENT_EEPROM_WRITE_CANCELED,

            ///
            EVENT_EEPROM_WRITE_ALLOWED,

            ///
            EVENT_EEPROM_WRITE_FORBIDDEN,

            ///
            EVENT_EEPROM__MAX__
        };

        /**
         * @brief
         */
        enum Reason
        {
            REASON_WREN_NOT_SET = 1,
            REASON_EECON2_55AA_NOT_FOLLOWED,
            REASON_ANOTHER_WRITE_IN_PROGRESS,
            REASON_INVALID_ADDRESS,

            REASON__MAX__
        };

        /**
         * @brief
         */
        struct Config
        {
            Config()
            {
                m_undefinedValue = -1;
            }

            bool m_enabled;              ///<
            unsigned int m_size;         ///<
            float m_writeTime;           ///< (in seconds)
            int m_undefinedValue;        ///< -1 means random
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8DataEEPROM();

        /**
         * @brief
         */
        ~PIC8DataEEPROM();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @return
         */
        PIC8DataEEPROM * link ( MCUSim::EventLogger * eventLogger,
                                PIC8DataMemory      * dataMemory );

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
         * @param[in] timeStep
         * @param[in] clockCycles
         */
        void timeStep ( float timeStep,
                        unsigned int clockCycles = 0 );

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

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        unsigned int read ( unsigned int addr );

        /**
         * @brief
         * @param[in] addr
         * @param[in] val
         */
        void write ( unsigned int addr,
                     unsigned int val );

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

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @return
         */
        inline void readEecon1();

        /**
         * @brief
         * @return
         */
        inline void readEecon2();

        /**
         * @brief
         * @param[in,out] eecon1
         * @return
         */
        inline void writeByte ( unsigned int * eecon1 );

        /**
         * @brief
         * @param[in,out] eecon1
         * @return
         */
        inline void readByte ( unsigned int * eecon1 );

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

    ////    Private Attributes    ////
    private:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            MCUSim::EventLogger * m_eventLogger;

            ///
            PIC8DataMemory * m_dataMemory;
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
        bool m_writeInProgress;

        /**
         * @brief
         */
        int m_writeAllowed;

        /**
         * @brief
         */
        float m_writeTimer;

        /**
         * @brief
         */
        unsigned int m_writeAddress;
};

#endif // PIC8DATAEEPROM_H

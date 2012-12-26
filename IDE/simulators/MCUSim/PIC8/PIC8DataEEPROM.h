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
 * @file PIC8DataEEPROM.h
 */
// =============================================================================

#ifndef PIC8DATAEEPROM_H
#define PIC8DATAEEPROM_H

// Forward declarations
class DataFile;

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
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            int m_undefinedValue;           ///< -1 means random
            unsigned int m_size;            ///<
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
        PIC8DataEEPROM * link ( MCUSim::EventLogger * eventLogger );

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
        MCUSim::EventLogger * m_eventLogger;
};

#endif // PIC8DATAEEPROM_H

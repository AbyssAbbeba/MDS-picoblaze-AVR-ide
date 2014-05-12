// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Avr8UsbProg
 * @file Avr8UsbProgHid.h
 */
// =============================================================================

#ifndef AVR8USBPROGHID_H
#define AVR8USBPROGHID_H

// HID API library header file.
#include "hidapi/hidapi.h"

// Standard header files.
#include <cwchar>

// Qt header files.
#include <QStringList>

class Avr8UsbProgHid
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned short VENDOR_ID   = 0x03eb;

        /// @brief
        static const unsigned short PRODUCT_ID  = 0x2016;

        /// @brief
        static const unsigned int   BUFFER_SIZE = 65;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ShortTag
        {
            ST_
        };

        /**
         * @brief
         */
        enum LongTag
        {
            LT_
        };

        /**
         * @brief
         */
        enum Type
        {
            TYPE_MAIN     = 0, ///<
            TYPE_GLOBAL   = 1, ///<
            TYPE_LOCAL    = 2, ///<
            TYPE_RESERVED = 3  ///<
        };

        /**
         * @brief
         */
        struct ShortItem
        {
            ShortItem ( const std::string & data,
                        const ShortTag bTag,
                        const Type bType = TYPE_MAIN )
                      :
                        m_data ( data ),
                        m_bTag ( bTag ),
                        m_bType ( bType ) {}

            const std::string m_data;
            const ShortTag m_bTag;
            const Type m_bType;
        };

        /**
         * @brief
         */
        struct LongItem
        {
            LongItem ( const std::string & data,
                       const LongTag bTag )
                     :
                       m_data ( data ),
                       m_bTag ( bTag ) {}

            const std::string m_data;
            const LongTag m_bTag;
        };

        /**
         * @brief
         */
        class Exception
        {
            public:
                enum Type
                {
                    INIT_FAILED,  ///<
                    OPEN_FAILED,  ///<
                    WRITE_FAILED, ///<
                    READ_FAILED   ///<
                };

            public:
                Exception ( Type type,
                            const wchar_t * info = NULL );

            public:
                Type m_type;
                std::string m_info;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        Avr8UsbProgHid();

    ////    Public Operations    ////
    public:
        /// @name High level methods.
        //@{
            /**
             * @brief
             * @param[out] result
             */
            void findDevices ( QStringList & result );

            /**
             * Open communication channel between computer and the MCU
             *
             * @param devPath
             * @param speedLevel Connection speed.
             */
            void openDevice ( const QString & devPath,
                              int speedLevel );

            /**
             * Close communication channel between computer and the MCU
             * @return Zero on success
             */
            int closeDevice();
        //@}

        /// @name Methods for working with content of the I/O buffer.
        //@{
            /**
             * @brief
             * @param[out] item
             */
            void readShortItem ( ShortItem & item );

            /**
             * @brief
             * @param[out] item
             */
            void readLongItem ( LongItem & item );

            /**
             * @brief
             * @param[in] item
             */
            void writeShortItem ( const ShortItem & item );

            /**
             * @brief
             * @param[in] item
             */
            void writeLongItem ( const LongItem & item );
        //@}

        /// @name Methods for synchronization between the I/O buffer and the HID device.
        //@{
            /**
             * @brief
             */
            void sendItems();

            /**
             * @brief
             */
            void receiveItems();
        //@}

    ////    Private Attributes    ////
    private:
        /// @brief
        hid_device * m_deviceHandle;

        /// @brief
        unsigned char m_buffer[BUFFER_SIZE];

        /// @brief
        unsigned int m_position;
};

#endif // AVR8USBPROGHID_H

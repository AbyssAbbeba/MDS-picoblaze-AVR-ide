#ifndef AVR8USBPROGHID_H
#define AVR8USBPROGHID_H

#include "hidapi/hidapi.h"

#include <cwchar>

#include <QStringList>

class Avr8UsbProgHid
{
    public:
        static const unsigned short VENDOR_ID  = 0x03eb;
        static const unsigned short PRODUCT_ID = 0x2016;

    public:
        class Exception
        {
            public:
                enum Type
                {
                    INIT_FAILED,
                    OPEN_FAILED,
                    WRITE_FAILED,
                    READ_FAILED
                };

            public:
                Exception ( Type type,
                            const wchar_t * info = NULL );

            public:
                Type m_type;
                std::string m_info;
        };

    public:
        Avr8UsbProgHid();

    public:
        void findProgrammers ( QStringList & result );

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

//         writeShortItem

    private:
        hid_device * m_deviceHandle;
};

#endif // AVR8USBPROGHID_H

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Avr8UsbProg
 * @file Avr8UsbProgHid.cxx
 */
// =============================================================================

#include "Avr8UsbProgHid.h"

// Standard header files.
#include <chrono>
#include <thread>
#include <cstdlib>

//<DEBUG!
#include <iostream>
#include <QDebug>
//DEBUG!>

Avr8UsbProgHid::Avr8UsbProgHid()
{
    m_deviceHandle = NULL;
    m_position = 1;
}

Avr8UsbProgHid::Exception::Exception ( Type type,
                                       const wchar_t * info )
{
    m_type = type;

    if ( NULL != info )
    {
        char buf[256];
        size_t size = wcstombs(buf, info, sizeof(buf));
        m_info = std::string(buf, size);
    }
}

void Avr8UsbProgHid::findDevices ( QStringList & result )
{
    hid_device_info * devs = hid_enumerate(VENDOR_ID, PRODUCT_ID);

    for ( hid_device_info * device = devs;
          NULL != device;
          device = device->next )
    {
        result << device->path;
    }

    hid_free_enumeration(devs);
}

void Avr8UsbProgHid::openDevice(const QString & devPath, int speedLevel)
{
    qDebug() << "openDevice("<<devPath<<", "<<speedLevel<<")";

    if ( 0 != hid_init() )
    {
        throw Exception(Exception::INIT_FAILED);
    }

    m_deviceHandle = hid_open_path(devPath.toAscii().data());
    if ( NULL == m_deviceHandle )
    {
        throw Exception(Exception::OPEN_FAILED);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    int res;


    static const char * testString = "\x40";

    unsigned char buffer[65];

    buffer[0] = 0x00;
    int len = strlen(testString);
    for ( int i = 0; i < len; i++ )
    {
        buffer[i+1] = testString[i];
    }
    for ( int i = len + 1; i < 65; i++ )
    {
        buffer[i] = 0;
    }

    wprintf(L"\nSending report to the device ...");
    res = hid_write(m_deviceHandle, buffer, 65);
    if ( -1 == res )
    {
        wprintf(L"\nError: cannot send data to the device.\n", res);
        wprintf(L"OS message: `%s'.\n", hid_error(m_deviceHandle));
        return;
    }
    else
    {
        wprintf(L" [OK] %d bytes sent to the device.\n", res);
    }

    wprintf(L"\nWaitning for the device to respond...");
    while ( 0 == (res = hid_read_timeout(m_deviceHandle, buffer, 65, 15000)) )
    {
        wprintf(L".");
    }
    wprintf(L"[OK]\n");

    // Print out the returned buffer.
    wprintf(L"Device responded:\n");
    wprintf(L"  []    DEC:   HEX:   ASCII:\n");
    for ( int i = 0; i < res; i++ )
    {
        wprintf(L"%3d:    %3d    0x%02x    '%c'\n", i, buffer[i], buffer[i], buffer[i]);
    }

}

int Avr8UsbProgHid::closeDevice()
{
    std::cout << "closeDevice()";

    // Close the HID device.
    if ( NULL != m_deviceHandle )
    {
        hid_close(m_deviceHandle);
        m_deviceHandle = NULL;
    }
}

void Avr8UsbProgHid::readShortItem ( ShortItem & item )
{
    item.m_bTag = ShortTag ( ( m_buffer[m_position] & 0xf0 ) >> 4 );
    item.m_bType = Type ( ( m_buffer[m_position] & 0xc ) >> 2 );

    int size = ( m_buffer[m_position] & 0x3 );

    item.m_data.clear();
    m_position++;

    for ( int i = 0; i < size; i++ )
    {
        item.m_data.push_back(m_buffer[m_position++]);
    }
}

void Avr8UsbProgHid::readLongItem ( LongItem & item )
{
    m_position++;

    int size = m_buffer[m_position++];

    item.m_bTag = LongTag ( m_buffer[m_position++] );
    item.m_data.clear();

    for ( int i = 0; i < size; i++ )
    {
        item.m_data.push_back(m_buffer[m_position++]);
    }
}

void Avr8UsbProgHid::writeShortItem ( const ShortItem & item )
{
    m_buffer[m_position]  = ( ( item.m_bTag  & 0xf ) << 4 );
    m_buffer[m_position] |= ( ( item.m_bType & 0x3 ) << 2 );
    m_buffer[m_position] |= ( item.m_data.size() & 0x3 );
    m_position++;

    for ( const unsigned char byte : item.m_data )
    {
        m_buffer[m_position++] = byte;
    }
}

void Avr8UsbProgHid::writeLongItem ( const LongItem & item )
{
    m_buffer[m_position++] = 0xfe;
    m_buffer[m_position++] = (unsigned char) item.m_data.size();
    m_buffer[m_position++] = (unsigned char) item.m_bTag;

    for ( const unsigned char byte : item.m_data )
    {
        m_buffer[m_position++] = byte;
    }
}

void Avr8UsbProgHid::sendItems()
{
    m_buffer[0] = 0;
    if ( -1 == hid_write(m_deviceHandle, m_buffer, BUFFER_SIZE) )
    {
        throw Exception(Exception::WRITE_FAILED, hid_error(m_deviceHandle));
    }
    clearBuffer();
}

void Avr8UsbProgHid::receiveItems()
{
    clearBuffer();
    if ( -1 == hid_read_timeout(m_deviceHandle, m_buffer, BUFFER_SIZE, READ_TIMEOUT) )
    {
        throw Exception(Exception::READ_FAILED, hid_error(m_deviceHandle));
    }
}

void Avr8UsbProgHid::clearBuffer()
{
    m_position = 1;
}

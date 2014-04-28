#include "Avr8UsbProgHid.h"

#include <chrono>
#include <thread>
#include <cstdlib>

#include <iostream>
#include <QDebug>

Avr8UsbProgHid::Avr8UsbProgHid()
{
    m_deviceHandle = NULL;
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

void Avr8UsbProgHid::findProgrammers ( QStringList & result )
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


    static const char * testString = "\x01";

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

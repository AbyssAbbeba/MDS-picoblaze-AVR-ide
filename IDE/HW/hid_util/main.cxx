// http://www.signal11.us/oss/hidapi/hidapi/doxygen/html/group__API.html

#ifdef WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "hidapi.h"
#include <iostream>

#define MAX_STR 255

int main ( int argc, char * argv[] )
{
    if ( 2 != argc )
    {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "  " << argv[0] << " \"string_to_send_to_the_device\"" << std::endl;
        return 1;
    }

    int res;
    unsigned char buf[65];
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;


    // Initialize the hidapi library
    wprintf(L"Initializing the hidapi library ...");
    res = hid_init();
    wprintf(L" [OK]\n");

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    wprintf(L"Attempting to open device with VID=0x03eb and PID=0x2016 ...");
    handle = hid_open(0x03eb, 0x2016, NULL);
    if ( NULL == handle )
    {
        wprintf(L" [FAILED]\n");
        wprintf(L"Attempting to open device with VID=0x03eb and PID=0x2015 ...");
        handle = hid_open(0x03eb, 0x2015, NULL);
        if ( NULL == handle )
        {
            wprintf(L" [FAILED]\n");
            return 1;
        }
    }
    wprintf(L" [OK]\n");

    // Read the Manufacturer String
    wprintf(L"Reading the Manufacturer String ... ");
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    wprintf(L"[OK] Manufacturer String: %s\n", wstr);

    // Read the Product String
    wprintf(L"Reading the Product String ... ");
    res = hid_get_product_string(handle, wstr, MAX_STR);
    wprintf(L"[OK] Product String: %s\n", wstr);

    // Read the Serial Number String
    wprintf(L"Reading the Serial Number String ... ");
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    wprintf(L"[OK] Serial Number String: (%d) %s\n", wstr[0], wstr);

    // Read Indexed String 1
    wprintf(L"Reading the Indexed String 1 ... ");
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    wprintf(L"[OK] Indexed String 1: %s\n", wstr);

    buf[0] = 0x0;
    int len = strlen(argv[1]);
    for ( int i = 0; i < len; i++ )
    {
        buf[i+1] = argv[1][i];
    }
    for ( int i = len + 1; i < 65; i++ )
    {
        buf[i] = 0;
    }
    wprintf(L"\nSending report to the device ...");
    res = hid_write(handle, buf, 64);
    if ( -1 == res )
    {
        wprintf(L"\nError: cannot send data to the device.\n", res);
        wprintf(L"OS message: `%s'.\n", hid_error(handle));
        return 1;
    }
    else
    {
        wprintf(L" [OK] %d bytes sent to the device.\n", res);
    }

    // Read requested state
    wprintf(L"\nWaitning for the device to respond...");
    while ( 0 == (res = hid_read(handle, buf, 64)) )
    {
        wprintf(L".");
    }
    wprintf(L"[OK]\n");

    // Print out the returned buffer.
    wprintf(L"Device responded:\n");
    wprintf(L"  []    DEC:   HEX:   ASCII:\n");
    for ( i = 0; i < res; i++ )
    {
        wprintf(L"%3d:    %3d    0x%02x    '%c'\n", i, buf[i], buf[i], buf[i]);
    }

    // Close the HID device. 
    hid_close(handle);

    // Finalize the hidapi library
    res = hid_exit();

    return 0;
}

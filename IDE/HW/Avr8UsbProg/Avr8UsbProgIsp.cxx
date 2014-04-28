#include "Avr8UsbProgIsp.h"

#include "hidapi/hidapi.h"
#include <iostream>

#include <QDebug>

Avr8UsbProgIsp::Avr8UsbProgIsp()
{
    if ( 0 != hid_init() )
    {
        std::cerr << "hid_init() failed." << std::endl;
    }

    tWD_FUSE=0;
    tWD_FLASH=0;
    tWD_EEPROM=0;
    tWD_ERASE=0;
}

void Avr8UsbProgIsp::findProgrammers ( QStringList & result )
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

void Avr8UsbProgIsp::openDevice(const QString & devPath, int speedLevel)
{
    qDebug() << "openDevice("<<devPath<<", "<<speedLevel<<")";
    hid_device * handle = hid_open_path(devPath.toAscii().data());
//     hid_device * handle = hid_open(0x03eb, 0x2016, NULL);
//     hid_set_nonblocking(handle, 1);

    if ( NULL == handle )
    {
        qDebug() << "Unable to open device handle.";
        // TODO: Unable to open device handle.
        return;
    }

    constexpr int MAX_STR = 255;

    int res;
    wchar_t wstr[MAX_STR];

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
    res = hid_write(handle, buffer, 65);
    if ( -1 == res )
    {
        wprintf(L"\nError: cannot send data to the device.\n", res);
        wprintf(L"OS message: `%s'.\n", hid_error(handle));
        return;
    }
    else
    {
        wprintf(L" [OK] %d bytes sent to the device.\n", res);
    }

    wprintf(L"\nWaitning for the device to respond...");
    while ( 0 == (res = hid_read_timeout(handle, buffer, 65, 15000)) )
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

    // Close the HID device.
    hid_close(handle);
}

int Avr8UsbProgIsp::closeDevice()
{
    std::cout << "closeDevice()";
}

void Avr8UsbProgIsp::resetPositivePulse()
{
    std::cout << "resetPositivePulse()";
}

void Avr8UsbProgIsp::setDelays(
    const int _tWD_FUSE,
    const int _tWD_FLASH,
    const int _tWD_EEPROM,
    const int _tWD_ERASE)
{
    tWD_FUSE=_tWD_FUSE;
    tWD_FLASH=_tWD_FLASH;
    tWD_EEPROM=_tWD_EEPROM;
    tWD_ERASE=_tWD_ERASE;
}


/* Programming Enable
 * code: 1010 1100 0101 0011 xxxx xxxx xxxx xxxx
 */
bool Avr8UsbProgIsp::ProgrammingEnable()
{
//      bool result;
//
//      readWrite(0xAC);
//      readWrite(0x53);
//      result=(readWrite() == 0x53);
//      readWrite();
//
//      return result;
return true;
}

/* Chip Erase
 * code: 1010 1100 100x xxxx xxxx xxxx xxxx xxxx
 */
void Avr8UsbProgIsp::chipErase()
{
//      readWrite(0xAC);
//      readWrite(0x80);
//      readWrite();
//      readWrite();
//
//      usleep(tWD_ERASE);
}

/* Read Program Memory */
unsigned char Avr8UsbProgIsp::readProgramMemory(int address)
{
    return readProgramMemoryLowLevel(address & 0x1, address >> 9, (address & 0x1fe) >> 1);
}

/* Read Program Memory
 * code: 0010 H000 0000 00aa bbbb bbbb oooo oooo
 */
unsigned char Avr8UsbProgIsp::readProgramMemoryLowLevel(bool h, unsigned char a, unsigned char b)
{
//      readWrite(0x20 | (h ? 0x08 : 0x00));
//      readWrite(a);
//      readWrite(b);
//      return readWrite();

return 0;
}

/* Load Program Memory Page */
void Avr8UsbProgIsp::loadProgramMemoryPage(int address, unsigned char data)
{
//      loadProgramMemoryPageLowLevel(address & 0x1, address >> 1, data);
}

/* Load Program Memory Page
 * code: 0100 H000 000x xxxx xxxx bbbb iiii iiii
 */
void Avr8UsbProgIsp::loadProgramMemoryPageLowLevel(bool h, unsigned char b, unsigned char i)
{
//      readWrite(0x40 | (h ? 0x08 : 0x00));
//      readWrite();
//      readWrite(b);
//      readWrite(i);
}

/* Write Program Memory Page
 * @param address Byte address, NO WORD ADDRESS !
 */
void Avr8UsbProgIsp::writeProgramMemoryPage(int address)
{
    address>>=1;
    writeProgramMemoryPageLowLevel((unsigned char)(address >> 8), (unsigned char)(address & 0xff));
}

/* Write Program Memory Page
 * code: 0100 1100 0000 00aa bbbb xxxx xxxx xxxx
 */
void Avr8UsbProgIsp::writeProgramMemoryPageLowLevel(unsigned char a, unsigned char b)
{
//      readWrite(0x4C);
//      readWrite(a);
//      readWrite(b);
//      readWrite();
//
//      usleep(tWD_FLASH);
}

/* Read EEPROM Memory
 * code: 1010 0000 000x xxxx xbbb bbbb oooo oooo
 */
unsigned char Avr8UsbProgIsp::readEepromMemory(unsigned int b)
{
//      readWrite(0xA0);
//      readWrite(b >> 8);
//      readWrite(b & 0x0ff);
//      return readWrite();
return 0;
}

/* Write EEPROM Memory
 * code: 1100 0000 000x xxxx xbbb bbbb iiii iiii
 */
void Avr8UsbProgIsp::writeEepromMemory(unsigned int b, unsigned char i)
{
//      readWrite(0xC0);
//      readWrite(b >> 8);
//      readWrite(b & 0xff);
//      readWrite(i);
//
//      usleep(tWD_EEPROM);
}

/* Load EEPROM Memory Page (page access)
 * code: 1100 0001 0000 0000 0000 00bb iiii iiii
 */
void Avr8UsbProgIsp::loadEepromMemoryPage(unsigned int b, unsigned char i)
{
//      readWrite(0xC1);
//      readWrite(b >> 8);
//      readWrite(b & 0xff);
//      readWrite(i);
}

/* Write EEPROM Memory Page (page access)
 * code: 1100 0010 00xx xxxx xbbb bb00 xxxx xxxx
 */
void Avr8UsbProgIsp::writeEepromMemoryPage(unsigned int b)
{
//      readWrite(0xC2);
//      readWrite(b >> 8);
//      readWrite(b & 0xff);
//      readWrite();
//
//      usleep(tWD_EEPROM);
}

/* Read Lock bits
 * code: 0101 1000 0000 0000 xxxx xxxx xxoo oooo
 */
unsigned char Avr8UsbProgIsp::readLockBits()
{
//      readWrite(0x58);
//      readWrite();
//      readWrite();
//      return readWrite();
return 0;
}

/* Write Lock bits
 * code: 1010 1100 111x xxxx xxxx xxxx iiii iiii
 */
void Avr8UsbProgIsp::writelockbits(unsigned char i)
{
//      readWrite(0xAC);
//      readWrite(0xE0);
//      readWrite();
//      readWrite(i);
}

/* Read Signature Byte
 * code: 0011 0000 000x xxxx xxxx xxbb oooo oooo
 */
unsigned char Avr8UsbProgIsp::readSignatureByte(unsigned char b)
{
//      readWrite(0x30);
//      readWrite();
//      readWrite(b);
//      return readWrite();
return 0;
}

/* Write Fuse bits
 * code: 1010 1100 1010 0000 xxxx xxxx iiii iiii
 */
void Avr8UsbProgIsp::writeFuseBits(unsigned char i)
{
//      readWrite(0xAC);
//      readWrite(0xA0);
//      readWrite();
//      readWrite(i);
//
//      usleep(tWD_FUSE);
}

/* Write Fuse High bits
 * code: 1010 1100 1010 1000 xxxx xxxx iiii iiii
 */
void Avr8UsbProgIsp::writeFuseHighBits(unsigned char i)
{
//      readWrite(0xAC);
//      readWrite(0xA8);
//      readWrite();
//      readWrite(i);
//
//      usleep(tWD_FUSE);
}

/* Write Extended Fuse Bits
 * code: 1010 1100 1010 0100 xxxx xxxx xxxx xxxi
 */
void Avr8UsbProgIsp::writeExtendedFuseBits(unsigned char i)
{
//      readWrite(0xAC);
//      readWrite(0xA4);
//      readWrite();
//      readWrite(i);
//
//      usleep(tWD_FUSE);
}

/* Read Fuse bits
 * code: 0101 0000 0000 0000 xxxx xxxx oooo oooo
 */
unsigned char Avr8UsbProgIsp::readFuseBits()
{
//      readWrite(0x50);
//      readWrite();
//      readWrite();
//      return readWrite();
return 0;
}

/* Read Fuse High bits
 * code: 0101 1000 0000 1000 xxxx xxxx oooo oooo
 */
unsigned char Avr8UsbProgIsp::readFuseHighBits()
{
//      readWrite(0x58);
//      readWrite(0x08);
//      readWrite();
//      return readWrite();
return 0;
}

/* Read Extended Fuse Bits
 * code: 0101 0000 0000 1000 xxxx xxxx oooo oooo
 */
unsigned char Avr8UsbProgIsp::readExtendedFuseBits()
{
//      readWrite(0x50);
//      readWrite(0x08);
//      readWrite();
//      return readWrite();
return 0;
}

/* Read Calibration Byte
 * code: 0011 1000 000x xxxx 0000 000b oooo oooo
 */
unsigned char Avr8UsbProgIsp::readCalibrationByte(unsigned char b)
{
//      readWrite(0x38);
//      readWrite();
//      readWrite(b);
//      return readWrite();
return 0;
}

/* Poll RDY/BSY
 * code: 1111 0000 0000 0000 xxxx xxxx xxxx xxxo
 */
bool Avr8UsbProgIsp::pollRDYBSY()
{
//      readWrite(0xF0);
//      readWrite();
//      readWrite();
//      return bool(readWrite());
return false;
}

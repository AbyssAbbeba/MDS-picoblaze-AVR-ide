#include "Avr8UsbProgIsp.h"

Avr8UsbProgIsp::Avr8UsbProgIsp()
{
    tWD_FUSE=0;
    tWD_FLASH=0;
    tWD_EEPROM=0;
    tWD_ERASE=0;
}

void Avr8UsbProgIsp::findProgrammers ( QStringList & result )
{
    result << "xXx-123" << "xXx-456";
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

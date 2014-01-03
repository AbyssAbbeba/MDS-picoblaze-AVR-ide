#ifndef AVR8USBPROGISP_H
#define AVR8USBPROGISP_H

#include <QString>
#include <QStringList>

/**
 * AVR SPI commands, this class allows to program AVR trought SPI via pure
 * command calls. This is the next abstraction layer above programmer HW.
 * This class takes care about making appropriate delays after certain
 * programming instructions.
 */
class Avr8UsbProgIsp
{

public:
	Avr8UsbProgIsp();

        void findProgrammers ( QStringList & result );

	/** Set delays for ceratin programming instructions
	 * @param tWD_FUSE	Minimum wait delay before writing the next fuse
	 * @param tWD_FLASH	Minimum wait delay before writing the next flash location
	 * @param tWD_EEPROM	Minimum wait delay before writing the next EEPROM location
	 * @param tWD_ERASE	Minimum wait delay after chip erase
	 * Note: All delays are in us (micro seconds)
	 */
	void setDelays(
		const int tWD_FUSE,
		const int tWD_FLASH,
		const int tWD_EEPROM,
		const int tWD_ERASE);

	/** Programming Enable
	 * Enable Serial Programming after RESET goes low.
	 * code: 1010 1100 0101 0011 xxxx xxxx xxxx xxxx
	 */
	bool ProgrammingEnable();

	/** Chip Erase
	 * Chip Erase EEPROM and Flash
	 * code: 1010 1100 100x xxxx xxxx xxxx xxxx xxxx
	 */
	void chipErase();

	/** Read Program Memory
	 * This function calls function readProgramMemoryLowLevel but
	 * simplifies its arguments.
	 * @param address Byte address
	 * @return
	 */
	unsigned char readProgramMemory(int address);

	/** Read Program Memory
	 * code: 0010 H000 0000 00aa bbbb bbbb oooo oooo
	 * Read H (high or low) data o from program memory at word address a:b.
	 * @param h
	 * @param a
	 * @param b
	 * @return
	 */
	unsigned char readProgramMemoryLowLevel(bool h, unsigned char b, unsigned char i);

	/** Load Program Memory Page
	 * This function calls function loadProgramMemoryPageLowLevel but
	 * simplifies its arguments.
	 * @param address Byte in page address
	 * @param data Byte value
	 */
	void loadProgramMemoryPage(int address, unsigned char data);

	/** Load Program Memory Page
	 * code: 0100 H000 000x xxxx xxxx bbbb iiii iiii
	 * Write H (high or low) data i to Program Memory page at word address b.
	 * Data low byte must be loaded before Data high byte is applied within the same address.
	 * @param h
	 * @param b
	 * @param i
	 */
	void loadProgramMemoryPageLowLevel(bool h, unsigned char b, unsigned char i);

	/** Write Program Memory Page
	 * This function calls function writeProgramMemoryPageLowLevel but
	 * takes only one argument.
	 * @see writeProgramMemoryPageLowLevel
	 * @param address Memory page address
	 */
	void writeProgramMemoryPage(int address);

	/** Write Program Memory Page
	 * code: 0100 1100 0000 00aa bbbb xxxx xxxx xxxx
	 * Write Program Memory Page at  address a:b.
	 * @param a
	 * @param b
	 */
	void writeProgramMemoryPageLowLevel(unsigned char a, unsigned char b);

	/** Read EEPROM Memory
	 * code: 1010 0000 000x xxxx xbbb bbbb oooo oooo
	 * Read data o from EEPROM memory at address b.
	 * @param b
	 * @return
	 */
	unsigned char readEepromMemory(unsigned int b);

	/** Write EEPROM Memory
	 * code: 1100 0000 000x xxxx xbbb bbbb iiii iiii
	 * Write data i to EEPROM memory at address b.
	 * @param b
	 * @param i
	 */
	void writeEepromMemory(unsigned int b, unsigned char i);

	/** Load EEPROM Memory Page (page access)
	 * code: 1100 0001 0000 0000 0000 00bb iiii iiii
	 * Load data i to EEPROM memory page buffer. After data is loaded, program EEPROM page.
	 * @param b
	 * @param i
	 */
	void loadEepromMemoryPage(unsigned int b, unsigned char i);

	/** Write EEPROM Memory Page (page access)
	 * code: 1100 0010 00xx xxxx xbbb bb00 xxxx xxxx
	 * Write EEPROM page at address b.
	 * @param b
	 */
	void writeEepromMemoryPage(unsigned int b);

	/** Read Lock bits
	 * code: 0101 1000 0000 0000 xxxx xxxx xxoo oooo
	 * Read Lock bits. “0” = programmed, “1” = unprogrammed
	 * @return Lock bits
	 */
	unsigned char readLockBits();

	/** Write Lock bits
	 * code: 1010 1100 111x xxxx xxxx xxxx 11ii iiii
	 * Write Lock bits. Set bits = “0” to program Lock bits
	 * @see readlockbits
	 * @param i
	 */
	void writelockbits(unsigned char i);

	/** Read Signature Byte
	 * code: 0011 0000 000x xxxx xxxx xxbb oooo oooo
	 * Read Signature Byte o at address b.
	 * @param b
	 */
	unsigned char readSignatureByte(unsigned char b);

	/** Write Fuse bits
	 * code: 1010 1100 1010 0000 xxxx xxxx iiii iiii
	 * Set bits = “0” to program, “1” to unprogram.
	 * @param i
	 */
	void writeFuseBits(unsigned char i);

	/** Write Fuse High bits
	 * code: 1010 1100 1010 1000 xxxx xxxx iiii iiii
	 * Set bits = “0” to program, “1” to unprogram.
	 * @param i
	 */
	void writeFuseHighBits(unsigned char i);

	/** Write Extended Fuse Bits
	 * code: 1010 1100 1010 0100 xxxx xxxx xxxx xxxi
	 * Set bits = “0” to program, “1” to unprogram.
	 * @param i
	 */
	void writeExtendedFuseBits(unsigned char i);

	/** Read Fuse bits
	 * code: 0101 0000 0000 0000 xxxx xxxx oooo oooo
	 * Read Fuse bits. “0” = programmed, “1” = unprogrammed.
	 * @return
	 */
	unsigned char readFuseBits();

	/** Read Fuse High bits
	 * code: 0101 1000 0000 1000 xxxx xxxx oooo oooo
	 * Read Fuse High bits. “0” = programmed, “1” = unprogrammed.
	 * @return
	 */
	unsigned char readFuseHighBits();

	/** Read Extended Fuse Bits
	 * code: 0101 0000 0000 1000 xxxx xxxx oooo oooo
	 * Read Extended Fuse bits. “0” = programmed, “1” = unprogrammed.
	 * @return
	 */
	unsigned char readExtendedFuseBits();

	/** Read Calibration Byte
	 * code: 0011 1000 000x xxxx 0000 000b oooo oooo
	 * Read Calibration Byte at address b.
	 * @param b
	 * @return
	 */
	unsigned char readCalibrationByte(unsigned char b);

	/** Poll RDY/BSY
	 * code: 1111 0000 0000 0000 xxxx xxxx xxxx xxxo
	 * If o = “1”, a programming operation is still busy. Wait until this
	 * bit returns to “0” before applying another command.
	 * @return
	 */
	bool pollRDYBSY();

        /**
         * Open communication channel between computer and the MCU
         *
         * @param deviceFile Terminal device file
         * @param speedLevel Connection speed.
         */
        void openDevice(const QString & devSerialNumber, int speedLevel);

        /**
         * Close communication channel between computer and the MCU
         * @return Zero on success
         */
        int closeDevice();

        /** Give the MCU reset positive pulse (remember AVR reset pin is inverted) */
        void resetPositivePulse();

private:
	int tWD_FUSE;	///< Minimum wait delay before writing the next fuse (us)
	int tWD_FLASH;	///< Minimum wait delay before writing the next flash location (us)
	int tWD_EEPROM;	///< Minimum wait delay before writing the next EEPROM location
	int tWD_ERASE;	///< Minimum wait delay after chip erase

};

#endif // AVR8USBPROGISP_H

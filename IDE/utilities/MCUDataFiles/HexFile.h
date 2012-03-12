/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup MCUDataFiles
 * @file HexFile.h
 */

#ifndef HEXFILE_H
#define HEXFILE_H

#include "DataFile.h"
#include <string>

/**
 * @brief Tool for working with Intel® 16 Hex files.
 * @ingroup MCUDataFiles
 * @class HexFile
 *
 * This class provides interface for reading and creating valid HEX-16 files.
 */
class HexFile : public DataFile
{
public:
	/**
	 * @param size Maximum data size
	 */
	HexFile(unsigned int arrsize = 0x10000) : DataFile(arrsize) {};

	/**
	 * @brief Load I16HEX file into the memory array
	 * @param filename
	 */
	void clearAndLoad(const char * filename)
		throw(DataFile::DataFileException);
	
	/// @overload
	void clearAndLoad(const std::string & filename)
		throw(DataFile::DataFileException);

	/**
	 * @brief Save memory array in I16HEX file
	 * @param filename	Target file
	 * @param makeBackup	Make backup file
	 */
	void save(const char * filename, bool makeBackup = true)
		throw(DataFile::DataFileException);

	/// @overload
	void save(const std::string & filename, bool makeBackup = true)
		throw(DataFile::DataFileException);

private:
	/**
	 * @brief Compute I16HEX CRC from the given string
	 * @param data Source string (e.g. "FA5568BA98")
	 * @return CRC (0..255)
	 */
	inline int computeCRC(const char * data) const;
};

#endif // HEXFILE_H

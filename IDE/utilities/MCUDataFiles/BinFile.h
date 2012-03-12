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
 * @file BinFile.h
 */

#ifndef BINFILE_H
#define BINFILE_H

#include "DataFile.h"
#include <string>

/**
 * @brief ...
 * @ingroup MCUDataFiles
 * @class BinFile
 */
class BinFile : public DataFile
{
public:
	/**
	 * @brief ...
	 * @param arrsize
	 */
	BinFile(unsigned int arrsize = 0x10000) : DataFile(arrsize) {};

	/**
	 * @brief Load binary file into the memory array
	 * @param filename Source file
	 */
	void clearAndLoad(const char * filename)
		throw(DataFile::DataFileException);

	/// @overload
	void clearAndLoad(const std::string & filename)
		throw(DataFile::DataFileException);

	/**
	 * @brief Save memory array in binary file
	 * @param filename Target file
	 * @param makeBackup Make backup file
	 */
	void save(const char * filename, bool makeBackup = true)
		throw(DataFile::DataFileException);

	/// @overload
	void save(const std::string & filename, bool makeBackup = true)
		throw(DataFile::DataFileException);
};

#endif // BINFILE_H

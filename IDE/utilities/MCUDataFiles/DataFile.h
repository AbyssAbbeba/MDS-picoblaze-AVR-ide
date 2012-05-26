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
 * @file DataFile.h
 */

#ifndef DATAFILE_H
#define DATAFILE_H

#include <string>
#include <cstdint>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class DataFile
 */
class DataFile
{
public:
	/**
	 * @brief ...
	 * @param arrsize
	 */
	DataFile(unsigned int arrsize = 0x10000);

	~DataFile();

	/**
	 * @brief
	 * TODO: Rewrite DataFileException as class with a std::string error info text.
	 */
	enum DataFileException {
		EXP_IO_ERROR,		///< Unable to work with the specified file
		EXP_BAD_RECORD_LENGTH,	///< Bad record length
		EXP_BAD_CRC,		///< Bad CRC
		EXP_BAD_RECORD_TYPE,	///< Bad record type
		EXP_MEMORY_OVERFLOW,	///< Object cannot contain such large amount of data
		EXP_OUT_OF_RANGE,	///< Requested byte address is out or range
		EXP_BAD_RECORD_FORMAT,	///<
		EXP_BAD_RECORD_COUT	///<
	};

	/**
	 * Load data file into the memory array
	 * @param filename
	 */
	virtual void clearAndLoad(const char * filename)
		throw(DataFile::DataFileException) = 0;

	/// @overload
	virtual void clearAndLoad(const std::string & filename)
		throw(DataFile::DataFileException) = 0;

	/**
	 * Save memory array in data file
	 * @param filename Target file
	 * @param makeBackup Make backup file
	 */
	virtual void save(const char * filename, bool makeBackup = true)
		throw(DataFile::DataFileException) = 0;

	/// @overload
	virtual void save(const std::string & filename, bool makeBackup = true)
		throw(DataFile::DataFileException) = 0;

	/**
	 * Set value for certain cell in the memory array
	 * @param address	Address of the target memory cell
	 * @param value		New value (0..255)
	 */
	void set(unsigned int address, uint8_t value) throw(DataFileException);

	/**
	 * Unset certain cell in the memory array.
	 * @param address Address of the target memory cell
	 */
	void unset(unsigned int address) throw(DataFileException);

	/**
	 * Get value for certain cell in the memory array
	 * @note In case the address is out of range, the method returns -1.
	 * @param address Address of the source memory cell
	 * @return Cell value (-1..255) (-1 means undefined)
	 */
	int get(unsigned int address) const;

	/**
	 * @brief Same as the indexing operator.
	 * @warning In case the index is out of range, the OUT_OF_RANGE exception is thrown.
	 * @param address Cell address
	 * @return Cell value (-1..255) (-1 means undefined)
	 */
	int at(int idx) const throw(DataFileException);

	/**
	 * @brief Get value contained in certain memory cell.
	 * @warning No check for address validity is performed!
	 * @param idx Cell address (index)
	 * @return Cell value (-1..255) (-1 means undefined)
	 */
	int operator[](int idx) const;

	/**
	 * @brief Clear the memory array.
	 */
	void clear();

	/**
	 * @brief Get overall capacity.
	 * @return capacity in bytes
	 */
	unsigned int maxSize() const;

	/**
	 * @brief Get number of nonempty pages of the given size.
	 * @param blockSize Page size (e.g. 64)
	 * @return Number of nonempty pages
	 */
	unsigned int getNumberOfNotEmptyPages(unsigned int pageSize) const;

	/**
	 * @brief Determinate if the specified page is empty.
	 *
	 * @param blockSize	Page size (e.g. 64)
	 * @param blockNumber	Page number
	 * @return True if the block is empty
	 */
	bool isPageEmpty(unsigned int pageSize, unsigned int pageNumber) const;

	/**
	 * @brief Get pointer to whole memory array.
	 * @return The memory array.
	 */
	int16_t * getData();

	/**
	 * @brief
	 * Replace native memory array with another one, the original memory is deleted.
	 *
	 * @param data Memory array from somewhere else. Must be the same size!
	 */
	void setData(int16_t * data);

protected:
	const unsigned int m_arrsize;	///< Size of the memory array
	int16_t * m_memory;		///< Memory array
};

#endif // DATAFILE_H

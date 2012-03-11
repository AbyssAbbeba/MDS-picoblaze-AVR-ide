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
 * @file SrecFile.h
 */

#ifndef SRECFILE_H
#define SRECFILE_H

#include "DataFile.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

/**
 * @brief
 * @ingroup MCUDataFiles
 * @class SrecFile
 */
class SrecFile : public DataFile
{
public:
	/**
	 * @brief ...
	 * @param arrsize
	 */
	SrecFile(unsigned int arrsize = 0x10000);

	struct BlockHeader {
		std::string m_mname;
		std::string m_ver;
		std::string m_rev;
		std::string m_description;
	};

	/**
	* @brief Load Motorola S-Record file into the memory array
	* @param filename
	*/
	void clearAndLoad(const char * filename)
		throw(DataFile::DataFileException);

	/// @overload
	void clearAndLoad(const std::string & filename)
		throw(DataFile::DataFileException);

	/**
	* @brief Save memory array in Motorola S-Record file
	* @param filename	Target file
	* @param makeBackup	Make backup file
	*/
	void save(const char * filename, bool makeBackup = true)
		throw(DataFile::DataFileException);

	/// @overload
	void save(const std::string & filename, bool makeBackup = true)
		throw(DataFile::DataFileException);

	const std::string & getModuleName() const;
	const std::string & getVersion() const;
	const std::string & getRevision() const;
	const std::string & getDescription() const;
	unsigned int getStartingExecutionAddress() const;

	bool setModuleName(const std::string & mname);
	bool setVersion(const std::string & ver);
	bool setRevision(const std::string & rev);
	bool setDescription(const std::string & desc);
	bool setStartingExecutionAddress(uint32_t addr);

private:
	BlockHeader m_blockHeader;
	uint32_t m_startingExecutionAddress;

	/**
	 * @brief Compute Motorola S-Record CRC from the given string
	 * @param data Source string (e.g. "S00F000068656C6C6F20202020200000")
	 * @return CRC (0..255)
	 */
	inline int computeCRC(const std::string & data) const;
	inline int hex2int(const std::string & source) const;
	inline std::vector<unsigned char> readHex(const std::string & source) const;
	inline std::string vec2str(const std::vector<unsigned char> & source) const;

	inline void writeBlockHeaderSRecord (
		std::fstream & file,
		const std::string & mname,
		const std::string & ver,
		const std::string & rev,
		const std::string & description ) const;

	inline void writeRecordCountSRecord(std::fstream & file, int recordCount) const;
	inline bool writeDataSRecord(std::fstream & file, unsigned int address, const std::string & dataField) const;
	inline void writeEndOfBlockSRecord(std::fstream & file, unsigned int sea) const;
};

#endif // SRECFILE_H

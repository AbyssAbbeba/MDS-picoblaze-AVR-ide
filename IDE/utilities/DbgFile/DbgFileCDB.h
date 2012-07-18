/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup DbgFile
 * @file DbgFileCDB.h
 */

#ifndef DBGFILECDB_H
#define DBGFILECDB_H

#include "DbgFile.h"

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFileCDB
 */
class DbgFileCDB : public DbgFile
{
public:
	DbgFileCDB();
	~DbgFileCDB();

	void openFile(const std::string & filename) throw(DbgFileException);

	int getLineByAddr(uint addr) const;
	int getAddrByLine(uint line, const std::string & filename) const;

private:
	int * m_addrToLineMap;
	int ** m_lineToAddrMap;

	inline void loadFile(const std::string & filename);

	inline void clear();

	inline void handleModuleRecord(char * line);
	inline void handleFunctionRecord(char * line);
	inline void handleSymbolRecord(char * line, SymbolRecord * target = NULL);
	inline void handleTypeRecord(char * line);

	inline void handleLinkerSymbolAddressRecord(char * line, bool startAddress);
	inline void handleLinkerRecord(char * line);
	inline void handleLinkerLineRecord(char * line, bool asmRecord);

	inline int getOrInsertFileNumber(const std::string & filename);

	inline char * extractToken(char * & line, char delimiter = '$') const;
	inline const char * checkHexNumber(const char * line) const;
	inline const char * checkNumber(const char * line) const;
	inline int getFileNumber(const std::string & filename) const;

	inline Scope parseScope(char * & line, int & fileNumber, std::string & functionName);
	inline TypeChain parseTypeChain(char * typeChain) const;
	inline TypeChain::DCLType parseDCLType(const char * dclType) const;
	inline AddressSpace parseAddressSpace(const char * addressSpace) const;
	inline void parseMemberSymbol(char * typeMemberDef, std::vector<TypeRecord::TypeMember> & typeMembers);

	inline void generateLineAddressMaps();
};

#endif // DBGFILECDB_H

/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "DbgFileCDB.h"

#include <fstream>
#include <cstring>
#include <cctype>

DbgFileCDB::DbgFileCDB() {
	m_addrToLineMap = NULL;
	m_lineToAddrMap = NULL;
}

DbgFileCDB::~DbgFileCDB() {
	if ( NULL != m_addrToLineMap ) {
		delete[] m_addrToLineMap;
	}
	if ( NULL != m_lineToAddrMap ) {
		for ( uint i = 0; i < m_fileNames.size(); i++ ) {
			if ( NULL != m_lineToAddrMap[i] ) {
				delete[] m_lineToAddrMap[i];
			}
		}
		delete[] m_lineToAddrMap;
	}
}

void DbgFileCDB::openFile(const std::string & filename) throw(DbgFileException) {
	try {
		loadFile(filename);
	} catch ( DbgFileException & e ) {
		clear();
		throw(e);
	}
}

inline void DbgFileCDB::loadFile(const std::string & filename) {
	clear();
	m_sourceFile = filename;

	std::fstream file(filename, std::fstream::in );
	if ( false == file.is_open()) {
		throw DbgFileException(DbgFileException::IO_ERROR, "Unable to open " + filename);
	}

	char line[4096];
	while ( false == file.eof() ) {
		file.getline(line, 4095);
		if ( true == file.bad() ) {
			throw DbgFileException(DbgFileException::IO_ERROR, "Read failed, file: " + filename);
		}

		if ( '\0' == line[0] ) {
			continue; // Skip empty lines
		} else if ( '\0' == line[1] ) {
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
		} else if ( ':' == line[1] ) {
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Colon (':') expected.");
		}

		switch ( line[0] ) {
			case 'M': // Module Record
				handleModuleRecord ( line + 2 );
				break;

			case 'F': // Function Record
				handleFunctionRecord ( line + 2 );
				break;

			case 'S': // Symbol Record
				handleSymbolRecord ( line + 2 );
				break;

			case 'T': // Structure Record
				handleTypeRecord ( line + 2 );
				break;

			case 'L': // Linker Record
				handleLinkerRecord ( line + 2 );
				break;

			default:
				throw DbgFileException(DbgFileException::PARSE_ERROR, "Unknown record type: '" + std::string(1, line[0]) + "', file: " + m_sourceFile);
		}
	}

	file.close();
	generateLineAddressMaps();
}

inline void DbgFileCDB::handleModuleRecord(char * line) {
	ModuleRecord moduleRecord;
	moduleRecord.m_fileNumber = getOrInsertFileNumber(line);
	m_moduleRecords.push_back(moduleRecord);
}

inline void DbgFileCDB::handleFunctionRecord(char * line) {
	FunctionRecord functionRecord;

	functionRecord.m_scope = parseScope(line, functionRecord.m_fileNumber, functionRecord.m_function);

	functionRecord.m_name = extractToken(line);
	functionRecord.m_level = atoi(checkNumber(extractToken(line)));
	functionRecord.m_block = atoi(checkNumber(extractToken(line, '(')));

	char * typeChain = extractToken(line, ',');
	 // Get rid of the trailing ')'
	typeChain [ strlen(typeChain) - 1 ] = '\0';

	functionRecord.m_typeChain = parseTypeChain(typeChain);

	const char * token;

	token = extractToken(line, ',');
	functionRecord.m_addressSpace = parseAddressSpace(token);

	token = extractToken(line, ',');
	if ( 0 == strcmp(token, "0") ) {
		functionRecord.m_onStack = false;
	} else if ( 0 == strcmp(token, "1") ) {
		functionRecord.m_onStack = true;
	} else {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <On Stack> token.");
	}

	functionRecord.m_stackOffset = atoi(checkNumber(extractToken(line, ',')));

	token = extractToken(line, ',');
	if ( 0 == strcmp(token, "0") ) {
		functionRecord.m_isInterrupt = false;
	} else if ( 0 == strcmp(token, "1") ) {
		functionRecord.m_isInterrupt = true;
	} else {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <Is Interrupt> token.");
	}

	functionRecord.m_interruptNum = atoi(checkNumber(extractToken(line, ',')));

	functionRecord.m_registerBank = atoi(checkNumber(line));

	m_functionRecords.push_back(functionRecord);
}

inline DbgFileCDB::Scope DbgFileCDB::parseScope(char * & line, int & fileNumber, std::string & functionName) {
	switch ( line[0] ) {
		case 'G': { // Global scope
			if ( '$' == line[1] ) {
				throw DbgFileException(DbgFileException::PARSE_ERROR, "Dolar ('$') expected.");
			}
			line += 2; // Remove 'G$'
			return SCOPE_GLOBAL;
		}
		case 'F': { // File scope
			line++; // Remove 'F'
			fileNumber = getOrInsertFileNumber(extractToken(line));
			return SCOPE_FILE;
			break;
		}
		case 'L': { // Local scope
			line++; // Remove 'L'
			if ( '$' == line[0] ) { // <-- Function name is optional
				line++; // Remove '$'
			} else {
				functionName = extractToken(line);
			}
			return SCOPE_LOCAL;
			break;
		}
		case 'S': { // No scope, type definition only
			if ( '$' == line[1] ) {
				throw DbgFileException(DbgFileException::PARSE_ERROR, "Dolar ('$') expected.");
			}
			line += 2; // Remove 'S$'
			return SCOPE_NONE;
		}
		default:
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Unknown function scope specifier: '" + std::string(1, line[0]) + "', file: " + m_sourceFile);
	}
}

inline DbgFileCDB::TypeChain::DCLType DbgFileCDB::parseDCLType(const char * dclType) const {
	TypeChain::DCLType result;

	switch ( dclType[0] ) {
		case 'D':
			switch ( dclType[1] ) {
				case 'A': // Array of \<n\> elements
					result.m_type = TypeChain::DCLType::TYPE_DA;
					result.m_n = atoi(checkNumber(dclType + 2));
					break;
				case 'F': // Function
					result.m_type = TypeChain::DCLType::TYPE_DF;
					break;
				case 'G': // Generic pointer
					result.m_type = TypeChain::DCLType::TYPE_DG;
					break;
				case 'C': // Code pointer
					result.m_type = TypeChain::DCLType::TYPE_DC;
					break;
				case 'X': // External ram pointer
					result.m_type = TypeChain::DCLType::TYPE_DX;
					break;
				case 'D': // Internal ram pointer
					result.m_type = TypeChain::DCLType::TYPE_DD;
					break;
				case 'P': // Paged pointer
					result.m_type = TypeChain::DCLType::TYPE_DP;
					break;
				case 'I': // Upper 128 byte pointer
					result.m_type = TypeChain::DCLType::TYPE_DI;
					break;
				default:
					throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <DCLType> specifier");
			}
		case 'S':
			switch ( dclType[1] ) {
				case 'L': // long
					result.m_type = TypeChain::DCLType::TYPE_SL;
					break;
				case 'I': // int
					result.m_type = TypeChain::DCLType::TYPE_SI;
					break;
				case 'C': // char
					result.m_type = TypeChain::DCLType::TYPE_SC;
					break;
				case 'S': // short
					result.m_type = TypeChain::DCLType::TYPE_SS;
					break;
				case 'V': // void
					result.m_type = TypeChain::DCLType::TYPE_SV;
					break;
				case 'F': // float
					result.m_type = TypeChain::DCLType::TYPE_SF;
					break;
				case 'T': // Structure of name \<m_name\>
					result.m_type = TypeChain::DCLType::TYPE_ST;
					result.m_name = std::string(dclType + 2);
					if ( 0 == result.m_name.size() ) {
						throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
					}
					break;
				case 'X': // sbit
					result.m_type = TypeChain::DCLType::TYPE_SX;
					break;
				case 'B': // Bit field of \<n\> bits
					result.m_type = TypeChain::DCLType::TYPE_SB;
					result.m_n = atoi(checkNumber(dclType + 2));
					break;
				default:
					throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <DCLType> specifier");
			}
		default:
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <DCLType> specifier");
	}

	return result;
}

// typeChain = "{2}DF,SV:S"
inline DbgFileCDB::TypeChain DbgFileCDB::parseTypeChain(char * typeChain) const {
	DbgFileCDB::TypeChain result;

	// Get rid of the leading '{'
	typeChain[0] = '\0';
	typeChain++;

	result.m_size = atoi(checkNumber(extractToken(typeChain, '}')));
	char * dclTypes = extractToken(typeChain, ':');
	char * comma;
	while ( NULL != ( comma = strchr(dclTypes, int(',')) ) ) {
		// Replace ',' with NULL, i.e. terminate a substring here
		comma[0] = '\0';

		// append the substring after ',' to the vector
		result.m_DCLType.push_back(parseDCLType(dclTypes));

		// Set line to the string after ','
		dclTypes = ( comma + 1 );
	}
	result.m_DCLType.push_back(parseDCLType(dclTypes)); // append the string after the last comma (',')

	if ( 0 == strcmp(typeChain, "S") ) {
		result.m_signed = true;
	} else if ( 0 == strcmp(typeChain, "U") ) {
		result.m_signed = false;
	} else {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <Sign> specifier in a Type Chain Record.");
	}

	return result;
}

inline DbgFileCDB::AddressSpace DbgFileCDB::parseAddressSpace(const char * addressSpace) const {
	DbgFileCDB::AddressSpace result;

	if ( 0 == strcmp(addressSpace, "A") ) {
		result = ADDRSP_EXTERNAL_STACK;
	} else if ( 0 == strcmp(addressSpace, "B") ) {
		result = ADDRSP_INTERNAL_STACK;
	} else if ( 0 == strcmp(addressSpace, "C") ) {
		result = ADDRSP_CODE;
	} else if ( 0 == strcmp(addressSpace, "D") ) {
		result = ADDRSP_CODE__STATIC_SEGMENT;
	} else if ( 0 == strcmp(addressSpace, "E") ) {
		result = ADDRSP_INTERNAL_RAM__LOWER;
	} else if ( 0 == strcmp(addressSpace, "F") ) {
		result = ADDRSP_EXTERNAL_RAM;
	} else if ( 0 == strcmp(addressSpace, "G") ) {
		result = ADDRSP_INTERNAL_RAM;
	} else if ( 0 == strcmp(addressSpace, "H") ) {
		result = ADDRSP_BIT_ADDRESSABLE;
	} else if ( 0 == strcmp(addressSpace, "I") ) {
		result = ADDRSP_SFR;
	} else if ( 0 == strcmp(addressSpace, "J") ) {
		result = ADDRSP_SBIT;
	} else if ( 0 == strcmp(addressSpace, "R") ) {
		result = ADDRSP_REGISTER;
	} else if ( 0 == strcmp(addressSpace, "Z") ) {
		result = ADDRSP_OTHER;
	} else {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <Address Space> token.");
	}

	return result;
}

inline void DbgFileCDB::handleSymbolRecord(char * line, SymbolRecord * target) {
	SymbolRecord symbolRecord;

	symbolRecord.m_scope = parseScope(line, symbolRecord.m_fileNumber, symbolRecord.m_function);

	symbolRecord.m_name = extractToken(line);
	symbolRecord.m_level = atoi(checkNumber(extractToken(line)));
	symbolRecord.m_block = atoi(checkNumber(extractToken(line, '(')));

	char * typeChain = extractToken(line, ',');
	 // Get rid of the trailing ')'
	typeChain [ strlen(typeChain) - 1 ] = '\0';

	symbolRecord.m_typeChain = parseTypeChain(typeChain);

	const char * token = extractToken(line, ',');
	symbolRecord.m_addressSpace = parseAddressSpace(token);

	token = extractToken(line, ',');
	if ( 0 == strcmp(token, "0") ) {
		symbolRecord.m_onStack = false;
	} else if ( 0 == strcmp(token, "1") ) {
		symbolRecord.m_onStack = true;
	} else {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid <On Stack> token.");
	}

	if ( ']' == line [ strlen(line) - 1 ] ) {
		symbolRecord.m_stackOffset = atoi(checkNumber(extractToken(line, ',')));

		int length = strlen(line);
		if ( ('[' != line[0]) || (length < 3) ) {
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Invalid register list specification.");
		}

		// Remove trainling ']'
		line [ length - 1 ] = '\0';

		// Remove leading '['
		line[0] = '\0';
		line++;

		char * comma;
		while ( NULL != ( comma = strchr(line, int(',')) ) ) {
			// Replace ',' with NULL, i.e. terminate a substring here
			comma[0] = '\0';

			// append the substring after ',' to the vector
			symbolRecord.m_registers.push_back(std::string(line));

			// Set line to the string after ','
			line = ( comma + 1 );
		}
		symbolRecord.m_registers.push_back(std::string(line)); // append the string after the last comma (',')
	} else {
		symbolRecord.m_stackOffset = atoi(checkNumber(line));
	}

	if ( NULL == target ) {
		m_symbolRecords.push_back(symbolRecord);
	} else {
		*target = symbolRecord;
	}
}

inline void DbgFileCDB::handleTypeRecord(char * line) {
	m_typeRecords.push_back(TypeRecord());

	if ( 'F' != line[0] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "'F' expected.");
	}

	line++;
	m_typeRecords.back().m_fileNumber = getOrInsertFileNumber(extractToken(line));
	m_typeRecords.back().m_name = extractToken(line, '[');

	int length = strlen(line) - 1;
	if ( '\0' == line[length] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
	}
	line[length] = '\0'; // Remove trailing ']'

	if ( '\0' == line[0] ) {
		// The structure has no members --> don't continue
		return;
	} else if ( '(' != line[0] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "'(' expected.");
	}

	line++; // Remove leading '('

	char * memberSymbol = line;
	for ( int i = 0, j = 1; j < length; i++, j++ ) {
		if ( ( ')' == line[i] ) && ( '(' == line[j] ) ) {
			line[j] = '\0'; // Remove '(' at the beginning of the type member definition

			parseMemberSymbol(memberSymbol, m_typeRecords.back().m_typeMembers);
			memberSymbol = line + j + 1; // Move memberSymbol pointer behind the '(' at the beginning of the type member definition
		}
	}
	parseMemberSymbol(memberSymbol, m_typeRecords.back().m_typeMembers);
}

inline void DbgFileCDB::parseMemberSymbol(char * typeMemberDef, std::vector<TypeRecord::TypeMember> & typeMembers) {
	char * par = strchr(typeMemberDef, int(')'));
	if ( NULL == par ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "')' expected.");
	}
	par[0] = '\0'; // Remove the trailing ')'

	if ( '{' != typeMemberDef[0] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "'{' expected.");
	}
	typeMemberDef++; // Remove the leading '{'

	// Extract Offset
	typeMembers.push_back(TypeRecord::TypeMember(atoi(checkNumber(extractToken(typeMemberDef, '}')))));

	// Ensure there is 'S:' in front
	if ( ( 'S' != typeMemberDef[0] ) || ( ':' != typeMemberDef[1] ) ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "'S:' expected.");
	}
	// ... and remove the 'S:' from the front
	typeMemberDef += 2;

	// Extract SymbolRecord
	handleSymbolRecord(typeMemberDef, &typeMembers.back().m_memberSymbol);
}

inline void DbgFileCDB::handleLinkerRecord(char * line) {
	if ( '\0' == line[1] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
	} else if ( '$' == line[1] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Dolar ('$') expected.");
	}

	switch ( line[0] ) {
		case 'A': // Linker ASM Line Record
			handleLinkerLineRecord ( (line + 2), true );
			break;

		case 'C': // Linker C-Line Record
			handleLinkerLineRecord ( (line + 2), false );
			break;

		case 'G': // Link Address of Symbol: Symbol has file scope.
		case 'F': // Link Address of Symbol: Symbol has file scope.
		case 'L': // Link Address of Symbol: Symbol has function scope
			handleLinkerSymbolAddressRecord( line, true );
			break;

		case 'X': // Linker Symbol End Address Record
			handleLinkerSymbolAddressRecord( (line + 1) /* <-- remove 'X' */, false );
			break;

		default:
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Unknown record type: '" + std::string(1, line[0]) + "', file: " + m_sourceFile);
	}
}

inline void DbgFileCDB::handleLinkerSymbolAddressRecord(char * line, bool startAddress) {
	int address;
	int fileNumber = -1;
	int level;
	int block;
	Scope scope;
	std::string function;
	std::string name;

	scope = parseScope(line, fileNumber, function);
	name = extractToken(line);
	level = atoi(checkNumber(extractToken(line)));
	block = atoi(checkNumber(extractToken(line, ':')));
	address = int(strtol(checkHexNumber(extractToken(line, ':')), NULL, 16)); // address is in hex

	for (	std::vector<SymbolRecord>::iterator it = m_symbolRecords.begin();
		it != m_symbolRecords.end();
		it++ )
	{
		if (
			( it->m_block != block )
				||
			( it->m_level != level )
				||
			( it->m_scope != scope )
				||
			( it->m_fileNumber != fileNumber )
				||
			( it->m_name != name )
				||
			( it->m_function != function )
		) {
			continue;
		}

		if ( true == startAddress ) {
			it->m_startAddress = address;
		} else {
			it->m_endAddress = address;
		}
	}
}


inline void DbgFileCDB::handleLinkerLineRecord(char * line, bool asmRecord) {
	LineRecord lineRecord;

	// Extract file name
	lineRecord.m_fileNumber = getOrInsertFileNumber(extractToken(line));

	if ( true == asmRecord ) {
		/*
		 * Linker ASM Line Record (filename$line:endAddress)
		 */
		// extract line number
		lineRecord.m_lineNumber = atoi(checkNumber(extractToken(line, ':')));
		// extract end address
		lineRecord.m_address = int(strtol(checkHexNumber(line), NULL, 16)); // address is in hex
	} else {
		/*
		 * Linker C-Line Record (filename$line$level$block:endAddress)
		 */
		// extract line number
		lineRecord.m_lineNumber = atoi(checkNumber(extractToken(line)));
		// extract level number
		lineRecord.m_level = atoi(checkNumber(extractToken(line)));
		// extract block number
		lineRecord.m_block = atoi(checkNumber(extractToken(line, ':')));
		// extract end address
		lineRecord.m_address = int(strtol(checkHexNumber(line), NULL, 16)); // address is in hex
	}

	if ( m_numberOfLines.size() == uint(lineRecord.m_fileNumber) ) {
		m_numberOfLines.push_back(lineRecord.m_lineNumber);

	} else if ( uint(lineRecord.m_lineNumber) > m_numberOfLines[lineRecord.m_fileNumber] ) {
		m_numberOfLines[lineRecord.m_fileNumber] = lineRecord.m_lineNumber;
	}

	if ( lineRecord.m_address > m_lastAddress ) {
		m_lastAddress = lineRecord.m_address;
	}

	m_lineRecords.push_back(lineRecord);
}

inline const char * DbgFileCDB::checkHexNumber(const char * str) const {
	int length = strlen(str);

	if ( 0 == length ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
	}

	for ( int i = 0; i < length; i++ ) {
		if ( 0 == isxdigit(str[i]) ) {
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Decimal digit character expected.");
		}
	}

	return str;
}

inline const char * DbgFileCDB::checkNumber(const char * str) const {
	int length = strlen(str);

	if ( 0 == length ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
	}

	for ( int i = 0; i < length; i++ ) {
		if ( 0 == isdigit(str[i]) ) {
			throw DbgFileException(DbgFileException::PARSE_ERROR, "Decimal digit character expected.");
		}
	}

	return str;
}

inline char * DbgFileCDB::extractToken(char * & line, char delimiter) const {
	char * result = line;
	int idx = 0;
	bool empty = true;

	while ( '\0' != line[idx] ) {
		if ( delimiter == line[idx] ) {
			if ( true == empty ) {
				throw DbgFileException(DbgFileException::PARSE_ERROR, "Empty record field.");
			}

			line[idx] = '\0';
			line += ( ++idx );

			break;
		}

		empty = false;
		idx++;
	}

	if ( '\0' == line[idx] ) {
		throw DbgFileException(DbgFileException::PARSE_ERROR, "Record ends prematurely.");
	}

	return result;
}

inline int DbgFileCDB::getFileNumber(const std::string & filename) const {
	for ( uint i = 0; i < m_fileNames.size(); i++ ) {
		if ( filename == m_fileNames[i] ) {
			return i;
		}
	}

	return -1;
}

inline int DbgFileCDB::getOrInsertFileNumber(const std::string & filename) {
	int fileNumber = getFileNumber(filename);
	if ( -1 != fileNumber ) {
		return fileNumber;
	}

	m_fileNames.push_back(filename);
	return ( m_fileNames.size() - 1 );
}

inline void DbgFileCDB::generateLineAddressMaps() {
	if ( NULL != m_addrToLineMap ) {
		delete[] m_addrToLineMap;
	}
	m_addrToLineMap = new int[m_lastAddress];
	for ( int i = 0; i < m_lastAddress; i++ ) {
		m_addrToLineMap[i] = -1;
	}


	if ( NULL != m_lineToAddrMap ) {
		for ( uint i = 0; i < m_fileNames.size(); i++ ) {
			if ( NULL != m_lineToAddrMap[i] ) {
				delete[] m_lineToAddrMap[i];
			}
		}
		delete[] m_lineToAddrMap;
	}
	m_lineToAddrMap = new int * [ m_fileNames.size() ];
	for ( uint i = 0; i < m_fileNames.size(); i++ ) {
		m_lineToAddrMap[i] = new int [ m_numberOfLines[i] ];
		for ( uint j = 0; j < m_numberOfLines[i]; j++ ) {
			m_lineToAddrMap[i][j] = -1;
		}
	}


	for ( uint i = m_lineRecords.size(); i < m_lineRecords.size(); i++ ) {
		m_addrToLineMap [ m_lineRecords[i].m_address ] = i;
		m_lineToAddrMap [ m_lineRecords[i].m_fileNumber ] [ m_lineRecords[i].m_lineNumber ] = i;
	}
}

inline void DbgFileCDB::clear() {
	m_lastAddress = -1;

	m_sourceFile.clear();

	// Clear vectors
	m_numberOfLines.clear();
	m_fileNames.clear();
	m_lineRecords.clear();
	m_symbolRecords.clear();
	m_functionRecords.clear();
	m_typeRecords.clear();
	m_moduleRecords.clear();
}

int DbgFileCDB::getLineByAddr(uint * addr) const {
	if ( (int)(*addr) > m_lastAddress ) {
		return -1;
	}

	return m_addrToLineMap[addr];
}

int DbgFileCDB::getAddrByLine(uint line, const std::string & filename) const {
	int fileNumber = getFileNumber(filename);
	if ( -1 == fileNumber) {
		return -1;
	}

	if ( line > m_numberOfLines[fileNumber] ) {
		return -1;
	}

	return m_lineToAddrMap[fileNumber][line];
}

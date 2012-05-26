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
 * @file DbgFile.h
 */

#ifndef DBGFILE_H
#define DBGFILE_H

#include <vector>
#include <string>

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFile
 */
class DbgFile
{
public:
	// TODO: add exception type here!
	class DbgFileException {
	public:
		enum Type {
			IO_ERROR,
			PARSE_ERROR
		};

		DbgFileException(Type errorType, const std::string & errorInfo)
			: m_errorType(errorType),
			  m_errorInfo(errorInfo) {}

		const Type m_errorType;
		const std::string m_errorInfo;
	private:
		DbgFileException();
	};


	enum AddressSpace {
		ADDRSP_EXTERNAL_STACK,		///<
		ADDRSP_INTERNAL_STACK,		///<
		ADDRSP_CODE,			///<
		ADDRSP_CODE__STATIC_SEGMENT,	///<
		ADDRSP_INTERNAL_RAM__LOWER,	///<
		ADDRSP_EXTERNAL_RAM,		///<
		ADDRSP_INTERNAL_RAM,		///<
		ADDRSP_BIT_ADDRESSABLE,		///<
		ADDRSP_SFR,			///<
		ADDRSP_SBIT,			///<
		ADDRSP_REGISTER,		///<
		ADDRSP_OTHER			///<
	};

	enum Scope {
		SCOPE_GLOBAL,
		SCOPE_FILE,
		SCOPE_LOCAL,
		SCOPE_NONE
	};

	struct TypeChain {
		struct DCLType {
			enum Type {
				TYPE_DA, ///< Array of \<n\> elements
				TYPE_DF, ///< Function
				TYPE_DG, ///< Generic pointer
				TYPE_DC, ///< Code pointer
				TYPE_DX, ///< External ram pointer
				TYPE_DD, ///< Internal ram pointer
				TYPE_DP, ///< Paged pointer
				TYPE_DI, ///< Upper 128 byte pointer
				TYPE_SL, ///< long
				TYPE_SI, ///< int
				TYPE_SC, ///< char
				TYPE_SS, ///< short
				TYPE_SV, ///< void
				TYPE_SF, ///< float
				TYPE_ST, ///< Structure of name \<m_name\>
				TYPE_SX, ///< sbit
				TYPE_SB  ///< Bit field of \<n\> bits
			} m_type;
			int m_n;
			std::string m_name;
		};
		uint m_size;
		std::vector<DCLType> m_DCLType;
		bool m_signed;
	};

	struct ModuleRecord {
		int m_fileNumber;
	};

	struct LineRecord {
		LineRecord() {
			m_fileNumber = -1;
			m_lineNumber = -1;
			m_level = -1;
			m_block = -1;
			m_address = -1;
		}

		int m_fileNumber;
		int m_lineNumber;
		int m_level;
		int m_block;
		int m_address;
	};

	struct SymbolRecord {
		SymbolRecord() {
			m_fileNumber = -1;
			m_startAddress = -1;
			m_endAddress = -1;
		}

		Scope m_scope;

		std::string m_function; // Valid only for local scope
		std::string m_name;
		int m_fileNumber; // Valid only for file scope
		int m_level;
		int m_block;

		int m_startAddress;
		int m_endAddress;

		TypeChain m_typeChain;
		AddressSpace m_addressSpace;

		bool m_onStack;
		int m_stackOffset;
		std::vector<std::string> m_registers;
	};

	struct FunctionRecord {
		Scope m_scope;

		std::string m_function; // Valid only for local scope
		std::string m_name;
		int m_fileNumber; // Valid only for file scope
		int m_level;
		int m_block;

		TypeChain m_typeChain;
		AddressSpace m_addressSpace;

		int m_stackOffset;
		int m_interruptNum;
		int m_registerBank;
		bool m_isInterrupt;
		bool m_onStack;
	};

	struct TypeRecord {
		struct TypeMember {
			TypeMember(int offset) {
				m_offset = offset;
			}
			int m_offset;
			SymbolRecord m_memberSymbol;
		};
		int m_fileNumber;
		std::string m_name;
		std::vector<TypeMember> m_typeMembers;
	};

	DbgFile() {
		m_lastAddress = -1;
	}
	virtual ~DbgFile() {};

	virtual void openFile(const std::string & filename) throw(DbgFileException) = 0;

	virtual int getLineByAddr(uint addr, std::string & filename) const = 0;
	virtual int getAddrByLine(uint line, const std::string & filename) const = 0;

	const std::vector<LineRecord> & getLineRecords() const {
		return m_lineRecords;
	}
	const std::vector<SymbolRecord> & getSymbolRecords() const {
		return m_symbolRecords;
	}
	const std::vector<FunctionRecord> & getFunctionRecords() const {
		return m_functionRecords;
	}
	const std::vector<ModuleRecord> & getModuleRecords() const {
		return m_moduleRecords;
	}
	const std::vector<TypeRecord> & getTypeRecords() const {
		return m_typeRecords;
	}
	const std::vector<std::string> & getFileNames() const {
		return m_fileNames;
	}

	const std::string getSourceFileName() const {
		return m_sourceFile;
	}

protected:
	std::vector<uint> m_numberOfLines;
	int m_lastAddress;

	std::string m_sourceFile;

	std::vector<LineRecord> m_lineRecords;
	std::vector<SymbolRecord> m_symbolRecords;
	std::vector<FunctionRecord> m_functionRecords;
	std::vector<ModuleRecord> m_moduleRecords;
	std::vector<TypeRecord> m_typeRecords;
	std::vector<std::string> m_fileNames;
};

#endif // DBGFILE_H

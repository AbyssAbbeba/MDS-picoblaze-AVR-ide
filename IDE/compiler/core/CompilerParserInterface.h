/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup Compiler
 * @file CompilerParserInterface.h
 */

#ifndef COMPILERPARSERINTERFACE_H
#define COMPILERPARSERINTERFACE_H

// Standard headers
#include <string>
#include <vector>
#include <cstdio>

// Include basic general compiler declarations and definitions
#include "CompilerBase.h"

// Include compiler components used by parsers and/or lexers
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "StatementTypes.h"

// Used for i18n only
#include <QObject>

// Make sure that the data type of locations is declared (see Bison manual for details)
#if ! defined ( YYLTYPE ) && ! defined ( YYLTYPE_IS_DECLARED )
	typedef struct YYLTYPE {
		int first_line;
		int first_column;
		int last_line;
		int last_column;
	} YYLTYPE;
	#define YYLTYPE_IS_TRIVIAL 1
	#define YYLTYPE_IS_DECLARED 1
#endif // ! YYLTYPE && ! YYLTYPE_IS_DECLARED

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerParserInterface
 */
class CompilerParserInterface {
public:
	CompilerParserInterface() {
		m_strMaxSize = 0;
	}

	/**
	 * @brief
	 * @param location: [In]
	 * @param type: [In]
	 * @param text: [In]
	 */
	virtual void parserMessage(CompilerBase::SourceLocation location, CompilerBase::MessageType type, const std::string & text) = 0;

	/**
	 * @brief
	 * @param location: [In]
	 * @param type: [In]
	 * @param text: [In]
	 */
	virtual void lexerMessage(CompilerBase::SourceLocation location, CompilerBase::MessageType type, const std::string &  text) = 0;

	virtual FILE * fileOpen(const std::string & filename, bool acyclic = true) = 0;
	virtual bool pushFileName(const std::string & filename) = 0;
	virtual void popFileName() = 0;

	virtual int getFileNumber() const = 0;
	virtual int getFileNumber(const std::string & filename) const = 0;

	virtual void syntaxAnalysisComplete(CompilerStatement * codeTree) = 0;

	// ---------------------------------------------------------------------
	// Methods and attributes specific for Bison and Flex
	// ---------------------------------------------------------------------

	/// Size of the string buffer, used for scanning strings and character literals.
	int m_strMaxSize;

	/// Stack to keep track of locations across multiple input buffers
	std::vector<YYLTYPE> m_yyllocStack;

	CompilerBase::SourceLocation toSourceLocation(const YYLTYPE & yylloc) const {
		return toSourceLocation(&yylloc);
	}

	CompilerBase::SourceLocation toSourceLocation(const YYLTYPE * yylloc) const {
		return CompilerBase::SourceLocation (
			getFileNumber(),
			yylloc->first_line,
			yylloc->last_line,
			yylloc->first_column,
			yylloc->last_column );
	}

	void parserMessage(const YYLTYPE & yylloc, CompilerBase::MessageType type, const std::string & text) {
		parserMessage(toSourceLocation(yylloc), type, text);
	}
	void lexerMessage(const YYLTYPE * yylloc, CompilerBase::MessageType type, const std::string & text) {
		lexerMessage(toSourceLocation(yylloc), type, text);
	}

protected:
	void resetCompilerParserInterface() {
		m_yyllocStack.clear();
	}
};

/*
 * In case this file is included in the compiler core, and not in a parser or lexer, we may also
 * need to declare parser/lexer functions, data types, and variables, used by the compiler core.
 */
#ifdef COMPILERCORE_H
	/*
	 * Make sure that the data type of semantic values is declared (see Bison manual).
	 * Actually the compiler core does not work with this data type, it is used only by
	 * the Flex generated lexical analyzer prototypes; in other words, each syntax/lexical
	 * analyzer might use it's own data type of semantic values.
	 */
	#if ! defined ( YYSTYPE ) && ! defined ( YYSTYPE_IS_DECLARED )
		typedef union YYSTYPE {} YYSTYPE;
	#endif

	// Include lexer prototypes (they are used by the core to initialize and destroy a lexer)
	#include "avr8lexer.h"
	#include "pic8lexer.h"
	#include "mcs51lexer.h"

	// Parser prototypes (the core uses them to initiate syntactical analysis)
	int avr8parser_parse  ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
	int pic8parser_parse  ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
	int mcs51parser_parse ( yyscan_t yyscanner, CompilerParserInterface * asmCore );
#endif // COMPILERCORE_H

#endif // COMPILERPARSERINTERFACE_H

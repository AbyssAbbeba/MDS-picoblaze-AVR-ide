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
 * @file CompilerCore.h
 */

#ifndef COMPILERCORE_H
#define COMPILERCORE_H

// Base classes
#include "CompilerBase.h"
#include "CompilerParserInterface.h"
#include "CompilerSemanticInterface.h"

// Standard header files
#include <string>
#include <vector>

// Other header files
#include "CompilerStatement.h"
#include "CompilerExpr.h"

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerCore
 */
class CompilerCore : public CompilerBase, private CompilerParserInterface, private CompilerSemanticInterface {
public:
	CompilerCore();
	~CompilerCore();

protected:
	bool parseSourceFile(TargetArch arch, const std::string & filename);
	virtual void message(const std::string & text, MessageType type = MT_GENERAL) = 0;

private:
	/// @name Interface for syntax and/or lexical analyzer
	//@{
	void parserMessage(SourceLocation location, MessageType type, const std::string & text);
	void lexerMessage(SourceLocation location, MessageType type, const std::string & text);
	FILE * fileOpen(const std::string & filename, bool acyclic = true);
	bool pushFileName(const std::string & filename);
	void popFileName();
	int getFileNumber() const;
	int getFileNumber(const std::string & filename) const;
	void syntaxAnalysisComplete(CompilerStatement * codeTree);
	//@}

	/// @name Interface for semantic analyzer
	//@{
	void setFileName(const std::string & filename);
	//@}

	inline void resetCompilerCore();

	CompilerStatement * m_rootStatement;

	std::vector<std::string> m_fileNameStack;
	std::vector<std::string> m_fileNames;
	std::string m_filename;
	int m_fileNumber;

	bool m_success;
};

#endif // COMPILERCORE_H

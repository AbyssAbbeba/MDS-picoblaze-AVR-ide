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
 * @file CompilerStatement.h
 */

#ifndef COMPILERSTATEMENT_H
#define COMPILERSTATEMENT_H

#include "CompilerBase.h"
#include "CompilerExpr.h"
#include "StatementTypes.h"

#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerStatement
 */
class CompilerStatement {
public:
	StatementTypes::StatementType m_type;
	CompilerBase::SourceLocation m_location;

	int m_userData;
	int m_serialNumber;

	CompilerExpr * m_args;
	CompilerStatement * m_prev;
	CompilerStatement * m_next;
	CompilerStatement * m_branch;

	~CompilerStatement();
	CompilerStatement();
	CompilerStatement(CompilerBase::SourceLocation location, StatementTypes::StatementType type, CompilerExpr * args = NULL);

	void completeDelete();
	static void completeDelete(CompilerStatement * stmt);

	CompilerStatement * createBranch(CompilerStatement * branch);
	CompilerStatement * first();
	CompilerStatement * addLink(CompilerStatement * next);
	CompilerStatement * addArgsLink(CompilerExpr * chainLink);

	std::ostream & print(std::ostream & out, int level = 0, std::string lineString = "1") const;
};

// Tracing operator
std::ostream & operator << (std::ostream & out, const CompilerStatement * stmt);

#endif // COMPILERSTATEMENT_H

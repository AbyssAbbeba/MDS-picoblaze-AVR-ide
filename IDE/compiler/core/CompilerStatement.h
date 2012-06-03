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

#include "StatementTypes.h"
#include "CompilerExpr.h"
#include "CompilerBase.h"

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerStatement
 */
class CompilerStatement {
public:
	int m_next;
	int m_prev;
	int m_userData;
	int m_innerStm;
	int m_serialNumber;
	CompilerBase::LangId m_lang;
	CompilerBase::SourceLocation m_location;
	union StatementType {
		StatementTypes::AsmAVR8 m_asmAVR8;
		StatementTypes::AsmPIC8 m_asmPIC8;
		StatementTypes::AsmMCS51 m_asmMSC51;
	} m_type;
	struct Args {
		int m_argc;
		CompilerExpr * m_argv;
	} m_args;
};

#endif // COMPILERSTATEMENT_H

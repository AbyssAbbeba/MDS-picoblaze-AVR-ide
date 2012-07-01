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
 * @file Compiler.h
 */

#ifndef COMPILER_H
#define COMPILER_H

class CompilerCore;
class CompilerMsgInterface;

#include "CompilerOptions.h"
#include "CompilerBase.h"
#include <iostream>

/**
 * @brief
 * @ingroup Compiler
 * @class Compiler
 */
class Compiler {
public:
	Compiler(CompilerMsgInterface * msgInterface);
	virtual ~Compiler();

	bool compile(CompilerBase::LangId lang, CompilerBase::TargetArch arch, CompilerOptions * opts, const std::string & filename);

private:
	CompilerCore * const m_compilerCore;
};

#endif // COMPILER_H

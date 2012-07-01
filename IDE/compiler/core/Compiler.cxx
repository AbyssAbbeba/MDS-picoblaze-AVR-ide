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

#include "Compiler.h"

#include "CompilerCore.h"
#include "CompilerMsgInterface.h"

Compiler::Compiler(CompilerMsgInterface * msgInterface) : m_compilerCore(new CompilerCore(msgInterface)) {}

Compiler::~Compiler() {
	delete m_compilerCore;
}

bool Compiler::compile(CompilerBase::LangId lang, CompilerBase::TargetArch arch, CompilerOptions * opts, const std::string & filename) {
	return m_compilerCore->compile(lang, arch, opts, filename);
}

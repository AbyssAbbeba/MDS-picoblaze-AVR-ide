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

Compiler::Compiler() {
}

Compiler::~Compiler() {
}

bool Compiler::compile(const std::string & filename) {
	return parseSourceFile(TA_MCS51, filename);
}

void Compiler::message(const std::string & text, CompilerCore::MessageType type) {
	std::cout << "\033[31;1m" << text << "\033[m\n";
}

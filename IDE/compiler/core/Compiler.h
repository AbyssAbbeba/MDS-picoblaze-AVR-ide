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

#include "CompilerCore.h"

#include <iostream>

/**
 * @brief
 * @ingroup Compiler
 * @class Compiler
 */
class Compiler : protected CompilerCore {
public:
	Compiler();
	~Compiler();

	bool compile(const std::string & filename);

protected:
	void message(const std::string & text, MessageType type = MT_GENERAL);
};

#endif // COMPILER_H

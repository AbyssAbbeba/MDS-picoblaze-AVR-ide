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
#include "CompilerMsgInftStdout.h"

#include <iostream>

int main(int argc, char ** argv) {
	CompilerMsgInftStdout msgInterface;
	CompilerOptions opts;
	Compiler compiler(&msgInterface);

	std::string filename = argv[1];

	if ( true == compiler.compile(CompilerBase::LI_ASM, CompilerBase::TA_AVR8/*TA_MCS51*/, &opts, filename) ) {
		std::cout << "SUCCESS\n";
		return 0;
	} else {
		std::cout << "FAILURE\n";
		return 1;
	}
}

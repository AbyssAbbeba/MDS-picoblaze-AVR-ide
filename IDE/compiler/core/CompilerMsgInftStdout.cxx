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

#include "CompilerMsgInftStdout.h"
#include <iostream>

void CompilerMsgInftStdout::message(const std::string & text, CompilerBase::MessageType type) {
	switch ( type ) {
		case CompilerBase::MT_GENERAL:
			std::cout << "\033[34;1m" << text << "\033[m\n";
			break;
		case CompilerBase::MT_ERROR:
			std::cout << "\033[31;1m" << text << "\033[m\n";
			break;
		case CompilerBase::MT_WARNING:
			std::cout << "\033[33;1m" << text << "\033[m\n";
			break;
		case CompilerBase::MT_REMARK:
			std::cout << "\033[32;1m" << text << "\033[m\n";
			break;
	}
}

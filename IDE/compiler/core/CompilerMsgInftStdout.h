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
 * @file CompilerMsgInftStdout.h
 */

#ifndef COMPILERMSGINFTSTDOUT_H
#define COMPILERMSGINFTSTDOUT_H

#include "CompilerMsgInterface.h"

class CompilerMsgInftStdout : public CompilerMsgInterface {
private:
	void message(const std::string & text, CompilerBase::MessageType type = CompilerBase::MT_GENERAL);
};

#endif // COMPILERMSGINFTSTDOUT_H

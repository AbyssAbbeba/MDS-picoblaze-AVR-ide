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
 * @file CompilerMsgInterface.h
 */

#ifndef COMPILERMSGINTERFACE_H
#define COMPILERMSGINTERFACE_H

#include "CompilerBase.h"
#include <iostream>

class CompilerMsgInterface {
public:
	virtual void message(const std::string & text, CompilerBase::MessageType type = CompilerBase::MT_GENERAL) = 0;
};

#endif // COMPILERMSGINTERFACE_H

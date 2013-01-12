// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file CompilerMsgInftStdout.h
 */
// =============================================================================

#ifndef COMPILERMSGINFTSTDOUT_H
#define COMPILERMSGINFTSTDOUT_H

#include "CompilerMsgInterface.h"

class CompilerMsgInftStdout : public CompilerMsgInterface
{
    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        void message ( const std::string & text,
                       CompilerBase::MessageType type = CompilerBase::MT_GENERAL );
};

#endif // COMPILERMSGINFTSTDOUT_H

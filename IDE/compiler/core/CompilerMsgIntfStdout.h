// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerMsgIntfStdout.h
 */
// =============================================================================

#ifndef COMPILERMSGINTFSTDOUT_H
#define COMPILERMSGINTFSTDOUT_H

#include "CompilerMsgInterface.h"

class CompilerMsgIntfStdout : public CompilerMsgInterface
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

#endif // COMPILERMSGINTFSTDOUT_H

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
 * @file CompilerMsgInterface.h
 */
// =============================================================================

#ifndef COMPILERMSGINTERFACE_H
#define COMPILERMSGINTERFACE_H

// Compiler header files
#include "CompilerBase.h"

// Standard header files
#include <string>

class CompilerMsgInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        ~CompilerMsgInterface() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        virtual void message ( const std::string & text,
                               CompilerBase::MessageType type = CompilerBase::MT_GENERAL ) = 0;
};

#endif // COMPILERMSGINTERFACE_H

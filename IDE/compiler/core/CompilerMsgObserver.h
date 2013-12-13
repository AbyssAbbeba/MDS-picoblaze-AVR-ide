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
 * @file CompilerMsgObserver.h
 */
// =============================================================================

#ifndef COMPILERMSGOBSERVER_H
#define COMPILERMSGOBSERVER_H

// Compiler header files.
#include "CompilerBase.h"
#include "CompilerSourceLocation.h"

// Standard header files.
#include <string>

class CompilerMsgObserver
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void message ( const CompilerSourceLocation & location,
                               CompilerBase::MessageType type,
                               const std::string & text ) = 0;

        /**
         * @brief
         * @param[in] limit
         */
        virtual void setMaxNumberOfMessages ( unsigned int limit ) = 0;

        /**
         * @brief
         */
        virtual void reset() {};
};

#endif // COMPILERMSGOBSERVER_H

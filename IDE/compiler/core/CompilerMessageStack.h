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
 * @file CompilerMessageStack.h
 */
// =============================================================================

#ifndef COMPILERMESSAGESTACK_H
#define COMPILERMESSAGESTACK_H

// Compiler compiler header files.
#include "CompilerBase.h"
#include "CompilerSourceLocation.h"

// Standard header files.
#include <utility>
#include <string>
#include <map>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerMessageStack
 */
class CompilerMessageStack
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        typedef std::pair<CompilerSourceLocation,CompilerBase::MessageType> MsgMetaData;

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @return
         */
        bool isUnique ( const CompilerSourceLocation & location,
                        CompilerBase::MessageType type,
                        const std::string & text );

        /**
         * @brief
         * @return
         */
        void reset();

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        std::map<std::string,MsgMetaData> m_data;
};

#endif // COMPILERMESSAGESTACK_H

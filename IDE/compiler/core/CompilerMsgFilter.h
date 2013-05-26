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
 * @file CompilerMsgFilter.h
 */
// =============================================================================

#ifndef COMPILERMSGFILTER_H
#define COMPILERMSGFILTER_H

// Forward declarations.
class CompilerCore;

// Compiler compiler header files.
#include "CompilerBase.h"
#include "CompilerMsgInterface.h"

// Standard header files.
#include <string>

class CompilerMsgFilter : public CompilerMsgInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] compilerCore
         * @param[in,out] msgInterface
         */
        CompilerMsgFilter ( const CompilerCore * compilerCore,
                            CompilerMsgInterface * msgInterface = NULL );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        virtual void message ( const std::string & text,
                               CompilerBase::MessageType type = CompilerBase::MT_GENERAL );

    ////    Private Attributes    ////
    private:
        /// @brief
        const CompilerCore * const m_compilerCore;

        /// @brief
        CompilerMsgInterface * const m_msgInterface;
};

#endif // COMPILERMSGFILTER_H

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
         * @param[in] messageLimit
         */
        CompilerMsgFilter ( const CompilerCore * compilerCore,
                            CompilerMsgInterface * msgInterface = NULL,
                            unsigned int messageLimit = 0 );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        virtual void message ( const std::string & text,
                               CompilerBase::MessageType type = CompilerBase::MT_GENERAL );

        /**
         * @brief
         */
        virtual void reset();

    ////    Private Attributes    ////
    private:
        /// @brief
        const CompilerCore * const m_compilerCore;

        /// @brief
        CompilerMsgInterface * const m_msgInterface;

        /// @brief
        const unsigned int m_messageLimit;

        /// @brief
        unsigned int m_msgCounter;
};

#endif // COMPILERMSGFILTER_H

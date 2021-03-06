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

// Compiler header files.
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
                            CompilerMsgInterface * msgInterface = nullptr,
                            int messageLimit = -1 );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        virtual void message ( const std::string & text,
                               CompilerBase::MessageType type = CompilerBase::MT_GENERAL ) override;

        /**
         * @brief
         */
        virtual void clear() override;

    ////    Public Attributes    ////
    public:
        /// @brief
        int m_messageLimit;

    ////    Private Attributes    ////
    private:
        /// @brief
        const CompilerCore * const m_compilerCore;

        /// @brief
        CompilerMsgInterface * const m_msgInterface;

        /// @brief
        int m_msgCounter;
};

#endif // COMPILERMSGFILTER_H

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
 * @file CompilerSemanticInterface.h
 */
// =============================================================================

#ifndef COMPILERSEMANTICINTERFACE_H
#define COMPILERSEMANTICINTERFACE_H

#include "CompilerCore.h"
#include "CompilerOptions.h"

#include <string>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSemanticInterface
 */
class CompilerSemanticInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        CompilerSemanticInterface ( CompilerCore * compilerCore,
                                    CompilerOptions * opts )
                                  : m_compilerCore ( compilerCore ),
                                    m_opts(opts) {};
        /**
         * @brief
         */
        virtual ~CompilerSemanticInterface() {};

    protected:
        /**
         * @brief
         */
        CompilerSemanticInterface() : m_compilerCore ( NULL ), m_opts ( NULL ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree ) = 0;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        CompilerCore * const m_compilerCore;

        /**
         * @brief
         */
        CompilerOptions * const m_opts;
};

#endif // COMPILERSEMANTICINTERFACE_H

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
    protected:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        CompilerSemanticInterface ( CompilerCore * compilerCore,
                                    CompilerOptions * const opts,
                                    const std::string & filename )
                                  : m_compilerCore(compilerCore),
                                    m_opts(opts),
                                    m_sourceFile(filename) {};
    public:
        /**
         * @brief
         */
        virtual ~CompilerSemanticInterface() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * & codeTree ) = 0;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        const CompilerCore * m_compilerCore;

        /**
         * @brief
         */
        const CompilerOptions * const m_opts;

        /**
         * @brief
         */
        const std::string m_sourceFile;
};

#endif // COMPILERSEMANTICINTERFACE_H

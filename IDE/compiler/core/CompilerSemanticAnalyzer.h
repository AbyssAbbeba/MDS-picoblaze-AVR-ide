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
 * @file CompilerSemanticAnalyzer.h
 */
// =============================================================================

#ifndef COMPILERSEMANTICANALYSER_H
#define COMPILERSEMANTICANALYSER_H

// Common compiler header files.
#include "CompilerSemanticInterface.h"
#include "CompilerCore.h"
#include "CompilerOptions.h"

// Standard header files.
#include <string>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSemanticAnalyzer
 */
class CompilerSemanticAnalyzer
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         * @param[in] filename
         * @return
         */
        CompilerSemanticAnalyzer ( CompilerSemanticInterface * compilerCore,
                                   CompilerOptions * opts )
                                 : m_compilerCore ( compilerCore ),
                                   m_opts ( opts ) {};
        /**
         * @brief
         */
        virtual ~CompilerSemanticAnalyzer() {};

    protected:
        /**
         * @brief
         */
        CompilerSemanticAnalyzer() : m_compilerCore ( nullptr ), m_opts ( nullptr ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void process ( CompilerStatement * codeTree ) = 0;

        /**
         * @brief
         * @param[in] deviceName
         */
        virtual void setDevice ( const std::string & deviceName ) = 0;

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        CompilerSemanticInterface * const m_compilerCore;

        /**
         * @brief
         */
        CompilerOptions * const m_opts;
};

#endif // COMPILERSEMANTICANALYSER_H

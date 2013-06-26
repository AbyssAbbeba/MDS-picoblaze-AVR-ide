// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file Compiler.h
 */
// =============================================================================

#ifndef COMPILER_H
#define COMPILER_H

// Forward declarations.
class CompilerCore;
class CompilerMsgInterface;

// Compiler header files.
#include "CompilerOptions.h"
#include "CompilerBase.h"

// Standard header files.
#include <iostream>

/**
 * @brief
 * @ingroup Compiler
 * @class Compiler
 */
class Compiler
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] msgInterface
         */
        Compiler ( CompilerMsgInterface * msgInterface = NULL,
                   std::string baseIncludeDir = "" );

        /**
         * @brief
         */
        virtual ~Compiler();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @param[in] opts
         * @param[in] genSimData
         * @return
         */
        bool compile ( CompilerBase::LangId lang,
                       CompilerBase::TargetArch arch,
                       CompilerOptions * const opts,
                       bool genSimData = false );

        /**
         * @brief
         * @param[in] directory
         */
        void setBaseIncludeDir ( const std::string & directory );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        CompilerCore * const m_compilerCore;
};

#endif // COMPILER_H

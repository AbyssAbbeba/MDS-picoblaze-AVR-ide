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
        Compiler ( CompilerMsgInterface * msgInterface = NULL );

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
         * @return
         */
        bool compile ( CompilerBase::LangId lang,
                       CompilerBase::TargetArch arch,
                       CompilerOptions * const opts );

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        CompilerCore * const m_compilerCore;
};

#endif // COMPILER_H

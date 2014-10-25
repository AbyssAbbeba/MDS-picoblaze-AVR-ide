// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsDisasmAdaptable
 * @file TestAdaptable.h
 */
// =============================================================================

#ifndef TESTADAPTABLE_H
#define TESTADAPTABLE_H

// Forward declarations.
class DAsm;
class Compiler;
class CompilerOptions;
class CompilerMsgInterface;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

/**
 * @brief
 * @namespace TestAdaptable
 * @ingroup testsDisasmAdaptable
 */
namespace TestAdaptable
{
    ////    Operations    ////
        /**
         * @brief
         * @return
         */
        int init();

        /**
         * @brief
         * @return
         */
        int clean();

        /**
         * @brief
         * @param[in,out] suite
         * @return
         */
        bool addTests ( CU_pSuite suite );

        /**
         * @brief
         */
        void testFunction();

    ////    Attributes    ////
        /**
         * @brief
         */
        DAsm * m_disassembler;

        /**
         * @brief
         */
        Compiler * m_compiler;

        /**
         * @brief
         */
        CompilerOptions * m_options;

        /**
         * @brief
         */
        CompilerMsgInterface * m_msgInt;
};

#endif // TESTADAPTABLE_H

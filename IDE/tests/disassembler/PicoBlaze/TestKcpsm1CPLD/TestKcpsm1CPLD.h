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
 * @ingroup testsDisasmPicoBlaze
 * @file TestKcpsm1CPLD.h
 */
// =============================================================================

#ifndef TESTKCPSM1CPLD_H
#define TESTKCPSM1CPLD_H

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
 * @namespace TestKcpsm1CPLD
 * @ingroup testsDisasmPicoBlaze
 */
namespace TestKcpsm1CPLD
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

#endif // TESTKCPSM1CPLD_H

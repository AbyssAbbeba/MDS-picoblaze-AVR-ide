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
 * @ingroup testsCompilerC
 * @file TestSuitePreprocessor.h
 */
// =============================================================================

#ifndef TESTSUITEPREPROCESSOR_H
#define TESTSUITEPREPROCESSOR_H

// Forward declarations.
class Compiler;
class CompilerOptions;
class CompilerMsgInterface;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include "3rdParty/CUnit/CUnit.h"

/**
 * @brief
 * @namespace TestSuitePreprocessor
 * @ingroup testsCompilerC
 */
namespace TestSuitePreprocessor
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

        /**
         * @brief
         * @param[in] expected
         * @param[in] actual
         * @return
         */
        void compareOutput ( const std::string & expected,
                             const std::string & actual );

    ////    Attributes    ////
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

#endif // TESTSUITEPREPROCESSOR_H

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
 * @ingroup testsAssemblerPicoBlaze
 * @file TestSuiteErr.h
 */
// =============================================================================

#ifndef TESTSUITEERR_H
#define TESTSUITEERR_H

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
 * @namespace TestSuiteErr
 * @ingroup testsAssemblerPicoBlaze
 */
namespace TestSuiteErr
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

        /// @name Result evaluation functions.
        //@{
            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareLst ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareErr ( const std::string & expected,
                              const std::string & actual );
        //@}

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

#endif // TESTSUITEERR_H

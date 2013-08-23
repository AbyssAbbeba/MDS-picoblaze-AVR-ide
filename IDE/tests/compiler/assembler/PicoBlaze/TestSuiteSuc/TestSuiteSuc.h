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
 * @ingroup testsAssemblerPicoBlaze
 * @file TestSuiteSuc.h
 */
// =============================================================================

#ifndef TESTSUITESUC_H
#define TESTSUITESUC_H

// Forward declarations.
class Compiler;
class CompilerOptions;
class CompilerMsgInterface;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestSuiteSuc
 * @ingroup testsAssemblerPicoBlaze
 */
namespace TestSuiteSuc
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
            void compareHex ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareBin ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareDbg ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareSrec ( const std::string & expected,
                               const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareSym ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareMac ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareMem ( const std::string & expected,
                              const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareV ( const std::string & expected,
                            const std::string & actual );

            /**
             * @brief
             * @param[in] expected
             * @param[in] actual
             * @return
             */
            void compareVhd ( const std::string & expected,
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

#endif // TESTSUITESUC_H

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
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestSuiteErr
 * @ingroup testsAssemblerPicoBlaze
 */
namespace TestSuiteErr
{
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

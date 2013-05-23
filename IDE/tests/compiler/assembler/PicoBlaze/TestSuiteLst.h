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
 * @file TestSuiteLst.h
 */
// =============================================================================

#ifndef TESTSUITELST_H
#define TESTSUITELST_H

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestSuiteLst
 * @ingroup testsAssemblerPicoBlaze
 */
namespace TestSuiteLst
{
    /**
     * @brief
     */
    struct TestList
    {
        const char * m_name;
        void (*m_func) (void);
    };

    /**
     * @brief
     */
    static const TestList m_testList[] =
    {
        { NULL, NULL },
        { NULL, NULL }
    };

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
};

#endif // TESTSUITELST_H

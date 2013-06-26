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
 * @ingroup testsMCUSimPicoBlaze
 * @file TestSingleInst.h
 */
// =============================================================================

#ifndef TESTSINGLEINST_H
#define TESTSINGLEINST_H

// The CUnit testing framework.
#include <CUnit/Basic.h>

/**
 * @brief
 * @namespace TestSingleInst
 * @ingroup testsMCUSimPicoBlaze
 */
namespace TestSingleInst
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
};

#endif // TESTSINGLEINST_H

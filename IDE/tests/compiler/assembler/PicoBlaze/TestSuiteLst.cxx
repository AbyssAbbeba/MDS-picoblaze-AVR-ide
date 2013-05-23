// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup testsAssemblerPicoBlaze
 * @file TestSuiteLst.cxx
 */
// =============================================================================

#include "TestSuiteLst.h"

// Standard header files
#include <iostream>

// The CUnit testing framework.
#include <CUnit/Basic.h>


int TestSuiteLst::init()
{
    return 0;
}

int TestSuiteLst::clean()
{
    return 0;
}

bool TestSuiteLst::addTests ( CU_pSuite suite )
{
    for ( int i = 0 ;; i++ )
    {
        if ( NULL == m_testList[i].m_name )
        {
            break;
        }

        if ( NULL == CU_add_test(suite, m_testList[i].m_name, m_testList[i].m_func) )
        {
            return false;
        }
    }
    return true;
}

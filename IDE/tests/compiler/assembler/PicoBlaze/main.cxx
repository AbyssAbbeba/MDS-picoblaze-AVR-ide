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
 * @file main.cxx
 */
// =============================================================================


// Standard header files
#include <iostream>

// The CUnit testing framework.
#include <CUnit/Basic.h>

// Test suites header files.
#include "TestSuiteLst.h"

/**
 * @brief The main function.
 *
 * The main() function for setting up and running the tests. Returns a CUE_SUCCESS on successful running, another CUnit
 * error code on failure.
 *
 * @param[in] argc Number of arguments.
 * @param[in] argv Array of arguments.
 * @return Program exit status.
 */
int main ( int argc, char ** argv )
{
    // Initialize the CUnit test registry.
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Define test suite: testSuiteLst - "Code listing file generation"
    // -----------------------------------------------------------------------------------------------------------------

    // Add the suite to the CUnit test registry.
    CU_pSuite testSuiteLst = CU_add_suite ( "Code listing file generation", TestSuiteLst::init, TestSuiteLst::clean );
    if ( NULL == testSuiteLst )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestSuiteLst::addTests(testSuiteLst) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Run all tests using the CUnit Basic interface.
    // -----------------------------------------------------------------------------------------------------------------
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

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

// Test suites header files.
#include "TestSuiteSuc/TestSuiteSuc.h"
#include "TestSuiteErr/TestSuiteErr.h"

// Standard header files
#include <iostream>

// The CUnit testing framework.
#include <CUnit/Basic.h>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

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
    // Change current directory this the location of the test binary.
    {
        using namespace boost::filesystem;
        current_path(system_complete(path(argv[0]).parent_path()));
    }

    // Initialize the CUnit test registry.
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }


    // -----------------------------------------------------------------------------------------------------------------
    // Register test suite: testSuiteSuc - "Successful compilation & file generation"
    // -----------------------------------------------------------------------------------------------------------------

    // Add the suite to the CUnit test registry.
    CU_pSuite testSuiteLst = CU_add_suite ( "Successful compilation & file generation",
                                            TestSuiteSuc::init,
                                            TestSuiteSuc::clean );
    if ( NULL == testSuiteLst )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestSuiteSuc::addTests(testSuiteLst) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }


    // -----------------------------------------------------------------------------------------------------------------
    // Register test suite: testSuiteErr - "Error cases"
    // -----------------------------------------------------------------------------------------------------------------

    // Add the suite to the CUnit test registry.
    CU_pSuite testSuiteErr = CU_add_suite ( "Error cases",
                                            TestSuiteErr::init,
                                            TestSuiteErr::clean );
    if ( NULL == testSuiteErr )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestSuiteErr::addTests(testSuiteErr) )
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

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsMScript
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "MScript.h"
#include "TestMScriptI/TestMScriptI.h"
#include "TestMScriptII/TestMScriptII.h"
#include "TestMScriptIII/TestMScriptIII.h"

// getopt_long() function
#include <getopt.h>

// Standard header files
#include <iostream>

// The CUnit testing framework.
#include <CUnit/Basic.h>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

/**
 * @brief Initialize the test environment.
 *
 * This function searches for available test cases for the defined test suites, initializes the suites, and does
 * anything else necessary to make this CUnit based testing environment operational.
 *
 * @return Exit code: 0 means success, anything else is supposed to be used as program exit status.
 */
int initTestEnv()
{
    // Initialize the CUnit test registry.
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }


    // -----------------------------------------------------------------------------------------------------------------
    // Register test suites.
    // -----------------------------------------------------------------------------------------------------------------

    // Add MScript-I test suite to the CUnit test registry.
    CU_pSuite testMScriptI = CU_add_suite ( "MScript-I",
                                            TestMScriptI::init,
                                            TestMScriptI::clean );
    if ( NULL == testMScriptI )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestMScriptI::addTests(testMScriptI) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add MScript-II test suite to the CUnit test registry.
    CU_pSuite testMScriptII = CU_add_suite ( "MScript-II",
                                             TestMScriptII::init,
                                             TestMScriptII::clean );
    if ( NULL == testMScriptII )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestMScriptII::addTests(testMScriptII) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add MScript-III test suite to the CUnit test registry.
    CU_pSuite testMScriptIII = CU_add_suite ( "MScript-III",
                                              TestMScriptIII::init,
                                              TestMScriptIII::clean );
    if ( NULL == testMScriptIII )
    {
        // Unable to add the test suite to the registry.
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add unit tests to the suite.
    if ( false == TestMScriptIII::addTests(testMScriptIII) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Successfully finished.
    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}

/**
 * @brief Print help message, a short introduction how to use this program.
 * @param[in] executable Name of executable binary from which this program was launched.
 */
void printHelp ( const char * executable )
{
    std::cout << "Options:" << std::endl;
    std::cout << "    --all, -a" << std::endl;
    std::cout << "        Run all test cases in all suites." << std::endl;
    std::cout << "    --list, -l" << std::endl;
    std::cout << "        List all test cases in all suites." << std::endl;
    std::cout << "    --suite, -s <test_suite_number>" << std::endl;
    std::cout << "        Run specific test suite." << std::endl;
    std::cout << "    --test, -t <test_suite_number>:<test_case_number>" << std::endl;
    std::cout << "        Run specific test in specific suite." << std::endl;
    std::cout << "    --help, -h" << std::endl;
    std::cout << "        Print this message." << std::endl;
    std::cout << std::endl;
}

/**
 * @brief The main function.
 *
 * The main() function for setting up and running the tests. Returns a CUE_SUCCESS on successful running, another CUnit
 * error code on failure.
 *
 * @param[in] argc Number of CLI arguments.
 * @param[in] argv Array of CLI arguments.
 * @return Program exit status.
 */
int main ( int argc, char ** argv )
{
    std::cout << "Integration testing environment for the Moravia Script." << std::endl
              << "(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union." << std::endl
              << "All rights reserved." << std::endl
              << std::endl;

    // -----------------------------------------------------------------------------------------------------------------
    // Initialization.
    // -----------------------------------------------------------------------------------------------------------------

    // Change current directory this the location of the test binary.
    {
        using namespace boost::filesystem;
        current_path(system_complete(path(argv[0]).parent_path()));
    }

    // Initialize the test environment.
    int exitStatus = initTestEnv();
    if ( 0 != exitStatus )
    {
        return exitStatus;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    // If there are no CLI arguments given, run all available tests and exit.
    if ( 1 == argc )
    {
        // Run all tests using the CUnit Basic interface.
        CU_basic_run_tests();
        CU_cleanup_registry();
        return CU_get_error();
    }


    // -----------------------------------------------------------------------------------------------------------------
    // Process the CLI arguments and act accordingly.
    // -----------------------------------------------------------------------------------------------------------------


    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":hals:t:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0,   'h' },
        { "all",         no_argument,       0,   'a' },
        { "list",        no_argument,       0,   'l' },
        { "suite",       required_argument, 0,   's' },
        { "test",        required_argument, 0,   't' },
        { 0,             0,                 0, 0     }
    };

    int opt;
    int idx = 0;
    while ( -1 != ( opt = getopt_long(argc, argv, shortopts, longopts, &idx) ) )
    {
        switch ( opt )
        {
            case 'h':
                printHelp(argv[0]);
                return 0;
            case 'a':
                CU_basic_run_tests();
                break;
            case 't':
            {
                char * testSuiteNumberSrt = new char [ strlen(optarg) + 1 ];
                strcpy(testSuiteNumberSrt, optarg);
                char * testCaseNumberSrt = strchr(testSuiteNumberSrt, ':');
                if ( NULL == testCaseNumberSrt )
                {
                    std::cerr << "Error: option `" << argv[optind-2] << "' requires argument in format "
                              << "`<test_suite_number>:<test_case_number>'." << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }
                testCaseNumberSrt[0] = '\0';
                testCaseNumberSrt++;

                for ( int i = 0; i < strlen(testSuiteNumberSrt); i++ )
                {
                    if ( 0 == isdigit(testSuiteNumberSrt[i]) )
                    {
                        std::cerr << "Error: test suite number expected instead of `" << testSuiteNumberSrt << "'."
                                 << std::endl;
                        delete [] testSuiteNumberSrt;
                        return 1;
                    }
                }
                for ( int i = 0; i < strlen(testCaseNumberSrt); i++ )
                {
                    if ( 0 == isdigit(testCaseNumberSrt[i]) )
                    {
                        std::cerr << "Error: test case number expected instead of `" << testCaseNumberSrt << "'."
                                 << std::endl;
                        delete [] testSuiteNumberSrt;
                        return 1;
                    }
                }

                int suiteNumber = atoi(testSuiteNumberSrt);
                if ( suiteNumber >= CU_get_registry()->uiNumberOfSuites )
                {
                    std::cerr << "Error: there is no test suite with number `" << testSuiteNumberSrt << "'."
                              << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }

                CU_pSuite suite = CU_get_registry()->pSuite;
                for ( int i = 0; i < suiteNumber; i++ )
                {
                    suite = suite->pNext;
                }

                int testNumber = atoi(testCaseNumberSrt);
                if ( testNumber >= suite->uiNumberOfTests )
                {
                    std::cerr << "Error: there is no test case with number `" << testCaseNumberSrt << "'." << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }

                CU_pTest test = suite->pTest;
                for ( int i = 0; i < testNumber; i++ )
                {
                    test = test->pNext;
                }

                CU_basic_run_test(suite, test);
                break;
            }
            case 's':
            {
                for ( int i = 0; i < strlen(optarg); i++ )
                {
                    if ( 0 == isdigit(optarg[i]) )
                    {
                        std::cerr << "Error: test suite number expected after -s option instead of `" << optarg << "'."
                                 << std::endl;
                        return 1;
                    }
                }

                int suiteNumber = atoi(optarg);

                CU_pSuite suite = CU_get_registry()->pSuite;
                if ( suiteNumber >= CU_get_registry()->uiNumberOfSuites )
                {
                    std::cerr << "Error: there is no test suite with number `" << optarg << "'." << std::endl;
                    return 1;
                }
                for ( int i = 0; i < suiteNumber; i++ )
                {
                    suite = suite->pNext;
                }

                CU_basic_run_suite(suite);
                break;
            }
            case 'l':
            {
                CU_pSuite suite = CU_get_registry()->pSuite;
                for ( int i = 0; NULL != suite; i++ )
                {
                    std::cout << "Suite #" << i << ": \"" << suite->pName << "\"" << std::endl;

                    CU_pTest test = suite->pTest;
                    for ( int j = 0; NULL != test; j++ )
                    {
                        std::cout << "    Test #" << j << ": \"" << test->pName << "\"" << std::endl;
                        test = test->pNext;
                    }

                    suite = suite->pNext;
                }

                return 0;
            }

            /* Error states */
            case ':':
                std::cerr << "Error: option `" << argv[optind-1] << "' requires argument." << std::endl;
                return 1;
            case '?':
                std::cerr << "Error: option `" << argv[optind-1] << "' not understood." << std::endl;
                return 1;
            default:
                return 1;
        }
    }

    CU_cleanup_registry();
    return CU_get_error();
}

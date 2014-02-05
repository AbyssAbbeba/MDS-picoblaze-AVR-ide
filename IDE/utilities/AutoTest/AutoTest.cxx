// =============================================================================
/**
 * @brief
 * C++ Implementation: Library for automated testing environment using libCUnit.
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AutoTest
 * @file AutoTest.cxx
 */
// =============================================================================

#include "AutoTest.h"

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"
#include "3rdParty/CUnit/Console.h"
#include "3rdParty/CUnit/Automated.h"

AutoTest::AutoTest ( const std::string & intoMessage )
                   : m_intoMessage ( intoMessage )
{
}

AutoTest::~AutoTest()
{
    CU_cleanup_registry();
}

void AutoTest::addTestSuite ( const std::string & name,
                              int  ( * init ) ( void ),
                              int  ( * clean ) ( void ),
                              bool ( * addTests ) ( CU_pSuite ) )
{
    m_testSuites.push_back ( { name, init, clean , addTests } );
}

int AutoTest::initTestEnv()
{
    // Initialize the CUnit test registry.
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }

    // Register test suites.
    for ( const auto & suite : m_testSuites )
    {
        // Add the suite to the CUnit test registry.
        CU_pSuite testSuite = CU_add_suite ( suite.m_name.c_str(), suite.m_init, suite.m_clean );
        if ( nullptr == testSuite )
        {
            // Unable to add the test suite to the registry.
            CU_cleanup_registry();
            return CU_get_error();
        }

        // Add unit tests to the suite.
        if ( false == (*(suite.m_addTests))(testSuite) )
        {
            // Unable to add the test cases to the suite.
            CU_cleanup_registry();
            return CU_get_error();
        }
    }

    // Successfully finished.
    return 0;
}

void AutoTest::printHelp ( const char * executable )
{
    std::cout << "Usage:" << std::endl;
    std::cout << "    " << executable << " [ OPTION [ OPTION ... ] ]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "    --all, -a" << std::endl;
    std::cout << "        Run all test cases in all suites and print results to the terminal." << std::endl;
    std::cout << "    --interactive, -i" << std::endl;
    std::cout << "        Run test environment in interactive mode." << std::endl;
    std::cout << "    --xml, -x <filename>" << std::endl;
    std::cout << "        Run all test cases in all suites and write results to <filename>-Results.xml, and "
                 "<filename>-Listing.xml files." << std::endl;
    std::cout << "    --list, -l" << std::endl;
    std::cout << "        List all test cases in all suites, this list provides test and suite numbers, names, etc."
              << std::endl;
    std::cout << "    --suite, -s <test_suite_number>" << std::endl;
    std::cout << "        Run one test suite specified by its number." << std::endl;
    std::cout << "    --test, -t <test_suite_number>/<test_case_number>" << std::endl;
    std::cout << "        Run one test case specified by its number." << std::endl;
    std::cout << "    --suite-name, -S <test_suite_name>" << std::endl;
    std::cout << "        Run one test suite specified by its name." << std::endl;
    std::cout << "    --test-name, -T <test_suite_name>/<test_case_name>" << std::endl;
    std::cout << "        Run one test case specified by its name." << std::endl;
    std::cout << "    --dir, -d" << std::endl;
    std::cout << "        Set base directory for the test (by default it's the directory where this binary is located)."
              << std::endl;
    std::cout << "    --help, -h" << std::endl;
    std::cout << "        Print this message." << std::endl;
    std::cout << std::endl;
}

int AutoTest::main ( int argc, char ** argv )
{
    // -----------------------------------------------------------------------------------------------------------------
    // Initialization.
    // -----------------------------------------------------------------------------------------------------------------

    int exitStatus = 0;

    // Print the introduction message.
    std::cout << m_intoMessage << std::endl << std::endl;

    // Change current working directory to the location of the test binary.
    {
        using namespace boost::filesystem;
        current_path(system_complete(path(argv[0]).parent_path()));
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    // If there are no CLI arguments given, run all available tests and exit.
    if ( 1 == argc )
    {
        printHelp(argv[0]);
        return 0;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Process the CLI arguments and act accordingly.
    // -----------------------------------------------------------------------------------------------------------------

    // Disable error messages from getopt_long().
    opterr = 0;

    const char * shortopts = ":halix:s:t:S:T:d:";
    static const struct option longopts[] =
    {
        { "help",        no_argument,       0, 'h' },
        { "all",         no_argument,       0, 'a' },
        { "list",        no_argument,       0, 'l' },
        { "interactive", no_argument,       0, 'i' },
        { "xml",         required_argument, 0, 'x' },
        { "suite",       required_argument, 0, 's' },
        { "test",        required_argument, 0, 't' },
        { "suite-name",  required_argument, 0, 'S' },
        { "test-name",   required_argument, 0, 'T' },
        { "dir",         required_argument, 0, 'd' },
        { 0,             0,                 0,  0  }
    };

    int opt;
    int idx = 0;
    while ( -1 != ( opt = getopt_long(argc, argv, shortopts, longopts, &idx) ) )
    {
        switch ( opt )
        {
            case 'h':
            {
                printHelp(argv[0]);
                return 0;
            }
            case 'i':
            {
                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_console_run_tests();
                break;
            }
            case 'a':
            {
                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_basic_run_tests();
                break;
            }
            case 'd':
            {
                // Change current working directory to the specified base directory of the test.
                {
                    using namespace boost::filesystem;

                    const path dir = optarg;

                    if ( false == is_directory(dir) )
                    {
                        std::cerr << "Directory does not exists: `" << optarg << "'" << std::endl;
                        return 1;
                    }
                    else
                    {
                        current_path(system_complete(dir));
                    }
                }
                break;
            }
            case 'x':
            {
                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_set_output_filename(optarg);
                CU_list_tests_to_file();
                CU_automated_run_tests();
                if ( 0 == CU_get_error() )
                {
                    std::cout << "Results written to: `" << optarg << "-Results.xml'." << std::endl;
                    std::cout << "List of tests written to: `" << optarg << "-Listing.xml'." << std::endl;
                }
                break;
            }
            case 'S':
            {
                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_pSuite suite = CU_get_suite(optarg);
                if ( nullptr == suite )
                {
                    std::cerr << "Error: there is no test suite with name `" << optarg << "'." << std::endl;
                    return 1;
                }

                CU_basic_run_suite(suite);
                break;
            }
            case 'l':
            {
                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_pSuite suite = CU_get_registry()->pSuite;
                for ( int i = 0; nullptr != suite; i++ )
                {
                    std::cout << "Suite #" << i << ": \"" << suite->pName << '"' << std::endl;

                    CU_pTest test = suite->pTest;
                    for ( int j = 0; nullptr != test; j++ )
                    {
                        std::cout << "    Test #" << j << ": \"" << test->pName << '"' << std::endl;
                        test = test->pNext;
                    }

                    suite = suite->pNext;
                }

                return 0;
            }
            case 's':
            {
                for ( size_t i = 0; i < strlen(optarg); i++ )
                {
                    if ( 0 == isdigit(optarg[i]) )
                    {
                        std::cerr << "Error: test suite number expected after -s option instead of `" << optarg << "'."
                                  << std::endl;
                        return 1;
                    }
                }

                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                unsigned int number = 1 + (unsigned int) atoi(optarg);
                CU_pSuite suite = CU_get_suite_at_pos(number);
                if ( nullptr == suite )
                {
                    std::cerr << "Error: there is no test suite with number `" << ( number - 1 ) << "'." << std::endl;
                    return 1;
                }

                CU_basic_run_suite(suite);
                break;
            }
            case 't':
            {
                char * testSuiteNumberSrt = new char [ strlen(optarg) + 1 ];
                strcpy(testSuiteNumberSrt, optarg);
                char * testCaseNumberSrt = strchr(testSuiteNumberSrt, '/');
                if ( nullptr == testCaseNumberSrt )
                {
                    std::cerr << "Error: option `" << argv[optind-2] << "' requires argument in format "
                              << "`<test_suite_number>/<test_case_number>'." << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }
                testCaseNumberSrt[0] = '\0';
                testCaseNumberSrt++;

                for ( size_t i = 0; i < strlen(testSuiteNumberSrt); i++ )
                {
                    if ( 0 == isdigit(testSuiteNumberSrt[i]) )
                    {
                        std::cerr << "Error: test suite number expected instead of `" << testSuiteNumberSrt << "'."
                                 << std::endl;
                        delete [] testSuiteNumberSrt;
                        return 1;
                    }
                }
                for ( size_t i = 0; i < strlen(testCaseNumberSrt); i++ )
                {
                    if ( 0 == isdigit(testCaseNumberSrt[i]) )
                    {
                        std::cerr << "Error: test case number expected instead of `" << testCaseNumberSrt << "'."
                                 << std::endl;
                        delete [] testSuiteNumberSrt;
                        return 1;
                    }
                }

                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                unsigned int number = 1 + (unsigned int) atoi(testSuiteNumberSrt);
                CU_pSuite suite = CU_get_suite_at_pos(number);
                if ( nullptr == suite )
                {
                    std::cerr << "Error: there is no test suite with number `" << ( number - 1 ) << "'."
                              << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }

                number = 1 + (unsigned int) atoi(testCaseNumberSrt);
                CU_pTest test = CU_get_test_at_pos(suite, number);
                if ( nullptr == test )
                {
                    std::cerr << "Error: there is no test case with number `" << ( number - 1 )
                              << "' in the selected suite." << std::endl;
                    delete [] testSuiteNumberSrt;
                    return 1;
                }

                delete [] testSuiteNumberSrt;
                CU_basic_run_test(suite, test);
                break;
            }
            case 'T':
            {
                char * testSuiteName = new char [ strlen(optarg) + 1 ];
                strcpy(testSuiteName, optarg);
                char * testCaseName = strchr(testSuiteName, '/');
                if ( nullptr == testCaseName )
                {
                    std::cerr << "Error: option `" << argv[optind-2] << "' requires argument in format "
                              << "`<test_suite_name>/<test_case_name>'." << std::endl;
                    delete [] testSuiteName;
                    return 1;
                }
                testCaseName[0] = '\0';
                testCaseName++;

                // Initialize the test environment.
                exitStatus = initTestEnv();
                if ( 0 != exitStatus )
                {
                    std::cerr << CU_get_error_msg() << std::endl;
                    return exitStatus;
                }

                CU_pSuite suite = CU_get_suite(testSuiteName);
                if ( nullptr == suite )
                {
                    std::cerr << "Error: there is no test suite with name `" << testSuiteName << "'." << std::endl;
                    delete [] testSuiteName;
                    return 1;
                }

                CU_pTest test = CU_get_test(suite, testCaseName);
                if ( nullptr == test )
                {
                    std::cerr << "Error: there is no test case with name `" << testCaseName << "'." << std::endl;
                    delete [] testSuiteName;
                    return 1;
                }

                delete [] testSuiteName;
                CU_basic_run_test(suite, test);
                break;
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


    exitStatus = CU_get_error();
    if ( 0 != exitStatus )
    {
        std::cerr << CU_get_error_msg() << std::endl;
    }
    else
    {
        exitStatus = ( ( 0 == CU_get_number_of_tests_failed() ) ? 0 : 2 );
    }

    CU_cleanup_registry();
    return exitStatus;
}

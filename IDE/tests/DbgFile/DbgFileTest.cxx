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
 * @ingroup tests
 * @file DbgFileTest.cxx
 */
// =============================================================================

#include "DbgFile/DbgFileAvrCoff.h"

#include <cstdlib>
#include <iostream>
#include <CUnit/Basic.h>

void printDbgFileExceptionType ( const DbgFile::DbgFileException & e )
{
    switch ( e.m_errorType )
    {
        case DbgFile::DbgFileException::IO_ERROR:
            std::cerr << "DbgFileException: IO_ERROR" << std::endl;
            break;
        case DbgFile::DbgFileException::PARSE_ERROR:
            std::cerr << "DbgFileException: PARSE_ERROR" << std::endl;
            break;
        case DbgFile::DbgFileException::COMPATIBILITY_ERROR:
            std::cerr << "DbgFileException: COMPATIBILITY_ERROR" << std::endl;
            break;
    }
    std::cerr << e.m_errorInfo << std::endl;
}

int init_suite0()
{
    return 0;
}

int clean_suite0()
{
    return 0;
}

void test_0()
{
    DbgFileAvrCoff coff;
    try
    {
        coff.openFile("coff_test0.cof");
    }
    catch ( DbgFile::DbgFileException & e )
    {
        printDbgFileExceptionType(e);
        CU_FAIL_FATAL("Exception raised.");
    }
    CU_PASS("File successfully loaded.")
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main ( int argc, char ** argv )
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
    {
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite 1", init_suite0, clean_suite0);
    if ( NULL == pSuite )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ( (NULL == CU_add_test(pSuite, "test #0", test_0)) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

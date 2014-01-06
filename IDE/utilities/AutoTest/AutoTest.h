// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AutoTest
 * @file AutoTest.h
 */
// =============================================================================

#ifndef AUTOTEST_H
#define AUTOTEST_H

// getopt_long() function
#include <getopt.h>

// Standard header files
#include <cctype>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>

// The CUnit testing framework.
#include "3rdParty/CUnit/CUnit.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

/**
 * @brief
 * @ingroup AutoTest
 * @class AutoTest
 */
class AutoTest
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        struct TestSuite
        {
            std::string m_name;
            int  ( * m_init ) ( void );
            int  ( * m_clean ) ( void );
            bool ( * m_addTests ) ( CU_pSuite );
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AutoTest ( const std::string & intoMessage );

        /**
         * @brief
         */
        ~AutoTest();

    ////    Public Operations    ////
    public:
        /**
         * @brief The main function.
         *
         * The main() function for setting up and running the tests.
         *
         * @param[in] argc Number of CLI arguments.
         * @param[in] argv Array of CLI arguments.
         * @return CUE_SUCCESS on successful run, and another CUnit error code on failure.
         */
        int main ( int argc, char ** argv );

        /**
         * @brief
         * @param[in] name
         * @param[in] init
         * @param[in] clean
         * @param[in] addTests
         */
        void addTestSuite ( const std::string & name,
                            int  ( * init ) ( void ),
                            int  ( * clean ) ( void ),
                            bool ( * addTests ) ( CU_pSuite ) );

    ////    Private Operations    ////
    private:
        /**
         * @brief Print help message, a short introduction how to use this program.
         * @param[in] executable Name of executable binary from which this program was launched.
         */
        void printHelp ( const char * executable );

        /**
         * @brief Initialize the test environment.
         *
         * This function searches for available test cases for the defined test suites, initializes the suites, and does
         * anything else necessary to make this CUnit based testing environment operational.
         *
         * @return Exit code: 0 means success, anything else is supposed to be used as program exit status.
         */
        int initTestEnv();

    ////    Public Attributes    ////
    public:
        ///
        const std::string m_intoMessage;

        ///
        std::vector<TestSuite> m_testSuites;
};

#endif // AUTOTEST_H

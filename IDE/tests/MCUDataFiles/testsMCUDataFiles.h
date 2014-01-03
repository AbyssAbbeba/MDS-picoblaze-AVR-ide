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
 * @ingroup testsMCUDataFiles
 * @file testsMCUDataFiles.h
 */
// =============================================================================

#ifndef TESTSMCUDATAFILES_H
#define TESTSMCUDATAFILES_H

// Forward declarations.
class AutoTest;

// Standard header files.
#include <string>

// The CUnit testing framework.
#include <CUnit/CUnit.h>

/**
 * @brief
 * @namespace TestSuiteSuc
 * @ingroup testsAssemblerPicoBlaze
 */
namespace TestSuite
{
    ////    Datatypes    ////
        /**
         * @brief
         */
        typedef void (*fptr)();

        /**
         * @brief
         */
        enum TestSuiteID
        {
            TS_IHEX = 0,
            TS_BIN,
            TS_SREC,
            TS_MEM_2,
            TS_MEM_3,
            TS_V_16,
            TS_V_18,
            TS_VHDL_16,
            TS_VHDL_18,

            TS__MAX__
        };

    ////    Operations    ////
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
        template<TestSuiteID id> bool addTests ( CU_pSuite suite );

        /**
         * @brief
         * @param[in,out] autoTest
         */
        void addSuites ( AutoTest * autoTest );
}

#endif // TESTSMCUDATAFILES_H

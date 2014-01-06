// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsDisasmPicoBlaze
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "TestKcpsm1CPLD/TestKcpsm1CPLD.h"
#include "TestKcpsm1/TestKcpsm1.h"
#include "TestKcpsm2/TestKcpsm2.h"
#include "TestKcpsm3/TestKcpsm3.h"
#include "TestKcpsm6/TestKcpsm6.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS MCU disassembler for PicoBlaze.\n"
                        "(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "KCPSM1CPLD",
                            &TestKcpsm1CPLD::init,
                            &TestKcpsm1CPLD::clean,
                            &TestKcpsm1CPLD::addTests );

    autoTest.addTestSuite ( "KCPSM1",
                            &TestKcpsm1::init,
                            &TestKcpsm1::clean,
                            &TestKcpsm1::addTests );

    autoTest.addTestSuite ( "KCPSM2",
                            &TestKcpsm2::init,
                            &TestKcpsm2::clean,
                            &TestKcpsm2::addTests );

    autoTest.addTestSuite ( "KCPSM3",
                            &TestKcpsm3::init,
                            &TestKcpsm3::clean,
                            &TestKcpsm3::addTests );

    autoTest.addTestSuite ( "KCPSM6",
                            &TestKcpsm6::init,
                            &TestKcpsm6::clean,
                            &TestKcpsm6::addTests );

    return autoTest.main(argc, argv);
}

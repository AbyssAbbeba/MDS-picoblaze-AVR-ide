// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsAssemblerAdaptable
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "TestSuiteSuc/TestSuiteSuc.h"
#include "TestSuiteErr/TestSuiteErr.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS mutitarget macro-assembler for Adaptable Sim.\n"
                        "(C) copyright 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "Successful compilation",
                            &TestSuiteSuc::init,
                            &TestSuiteSuc::clean,
                            &TestSuiteSuc::addTests );

    autoTest.addTestSuite ( "Error cases",
                            &TestSuiteErr::init,
                            &TestSuiteErr::clean,
                            &TestSuiteErr::addTests );

    return autoTest.main(argc, argv);
}

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
 * @ingroup testsCompilerC
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "preprocessor/TestSuitePreprocessor.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS C compiler\n"
                        "(C) copyright 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "Preprocessor",
                            &TestSuitePreprocessor::init,
                            &TestSuitePreprocessor::clean,
                            &TestSuitePreprocessor::addTests );

    return autoTest.main(argc, argv);
}

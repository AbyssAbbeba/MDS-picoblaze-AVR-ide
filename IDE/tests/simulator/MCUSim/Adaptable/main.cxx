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
 * @ingroup testsMCUSimAdaptable
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "TestAdaptable/TestAdaptable.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS MCU simulator for Adaptable.\n"
                        "(C) copyright 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "Adaptable",
                            &TestAdaptable::init,
                            &TestAdaptable::clean,
                            &TestAdaptable::addTests );

    return autoTest.main(argc, argv);
}

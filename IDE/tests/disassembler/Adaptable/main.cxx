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
 * @ingroup testsDisasmAdaptable
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "TestAdaptable.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS MCU disassembler for Adaptable Sim.\n"
                        "(C) copyright 2014 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "Adaptable",
                            &TestAdaptable::init,
                            &TestAdaptable::clean,
                            &TestAdaptable::addTests );

    return autoTest.main(argc, argv);
}

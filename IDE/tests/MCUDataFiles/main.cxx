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
 * @ingroup testsMCUDataFiles
 * @file main.cxx
 */
// =============================================================================

#include "testsMCUDataFiles.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS's MCU machine code containers library.\n"
                        "(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    TestSuite::addSuites(&autoTest);
    return autoTest.main(argc, argv);
}

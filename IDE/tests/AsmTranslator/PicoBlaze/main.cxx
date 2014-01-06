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
 * @ingroup testsAsmTranslator
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "Mediatronix/TestMedAsmTrans.h"
#include "openPICIDE/TestOpenPICIDEAsmTrans.h"
#include "Xilinx/TestXilAsmTrans.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the MDS assembler translator tool.\n"
                        "(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "Mediatronix",
                            &TestMedAsmTrans::init,
                            &TestMedAsmTrans::clean,
                            &TestMedAsmTrans::addTests );

    autoTest.addTestSuite ( "openPICIDE",
                            &TestOpenPICIDEAsmTrans::init,
                            &TestOpenPICIDEAsmTrans::clean,
                            &TestOpenPICIDEAsmTrans::addTests );

    autoTest.addTestSuite ( "Xilinx",
                            &TestXilAsmTrans::init,
                            &TestXilAsmTrans::clean,
                            &TestXilAsmTrans::addTests );

    return autoTest.main(argc, argv);
}

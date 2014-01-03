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
 * @ingroup testsMScript
 * @file main.cxx
 */
// =============================================================================

// Test suites header files.
#include "TestMScriptI/TestMScriptI.h"
#include "TestMScriptII/TestMScriptII.h"
#include "TestMScriptIII/TestMScriptIII.h"

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

int main ( int argc, char ** argv )
{
    AutoTest autoTest ( "Integration testing environment for the Moravia Script.\n"
                        "(C) copyright 2013 Moravia Microsystems, s.r.o., Brno, CZ, European Union.\n"
                        "All rights reserved." );

    autoTest.addTestSuite ( "MScript-I",
                            TestMScriptI::init,
                            TestMScriptI::clean,
                            TestMScriptI::addTests );

    autoTest.addTestSuite ( "MScript-II",
                            TestMScriptII::init,
                            TestMScriptII::clean,
                            TestMScriptII::addTests );

    autoTest.addTestSuite ( "MScript-III",
                            TestMScriptIII::init,
                            TestMScriptIII::clean,
                            TestMScriptIII::addTests );

    return autoTest.main(argc, argv);
}

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
 * @ingroup testsMCUSimPIC8
 * @file TestGenericSubsys.cxx
 */
// =============================================================================

#include "TestGenericSubsys.h"

// Tool for working with Intel® 16 Hex files.
#include "HexFile.h"

// PIC8 simulator.
#include "PIC8Sim.h"
#include "PIC8ProgramMemory.h"

// MCUSim test script interpreter.
#include "MCUSimTestScript.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Standard header files.
#include <iostream>

int TestGenericSubsys::init()
{
    m_pic8Sim     = new PIC8Sim;
    m_programFile = new HexFile;
    m_testScript  = new MCUSimTestScript(m_pic8Sim);

    m_pic8Sim->reset(MCUSim::RSTMD_NEW_CONFIG);

    return 0;
}

int TestGenericSubsys::clean()
{
    delete m_programFile;
    delete m_pic8Sim;
    delete m_testScript;

    return 0;
}

bool TestGenericSubsys::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestGenericSubsys") / "testcases" );
          directory_iterator() != dir;
          dir++ )
    {
        if ( false == is_regular_file(*dir) )
        {
            continue;
        }

        std::string extension = dir->path().extension().string();
        std::string testName = dir->path().filename().string();

        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if ( ".asm" == extension )
        {
            testName.resize(testName.size() - 3);
            testCaseFiles.push_back(testName);
        }
    }

    std::sort(testCaseFiles.begin(), testCaseFiles.end());

    for ( const auto & testCaseFile : testCaseFiles )
    {
        if ( nullptr == CU_add_test(suite, testCaseFile.c_str(), &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestGenericSubsys::testFunction()
{
    using namespace boost::filesystem;

    create_directory ( path("TestGenericSubsys") / "results" );

    const std::string testName = CU_get_current_test()->pName;

    std::string inFile  = ( path("TestGenericSubsys") / "testcases" / (testName + "."   ) ).string();
    std::string outFile = ( path("TestGenericSubsys") / "results"   / (testName + ".out") ).string();
    std::string hexFile = ( path("TestGenericSubsys") / "results"   / (testName + ".hex") ).string();

    bool useAsmFile;
    if ( true == exists(inFile + "in") )
    {
        inFile += "in";
        useAsmFile = false;
    }
    else
    {
        inFile += "asm";
        useAsmFile = true;
    }

    m_pic8Sim->getLog()->clear();
    m_pic8Sim->reset(MCUSim::RSTMD_INITIAL_VALUES);
    m_pic8Sim->reset(MCUSim::RSTMD_MCU_RESET);

    try
    {
        m_programFile->clearAndLoad(hexFile);
    }
    catch ( const DataFileException & e )
    {
        CU_FAIL("Instance of DataFileException thrown:");
        std::cerr << e.toString() << std::endl;
        return;
    }

    dynamic_cast<PIC8ProgramMemory*>(m_pic8Sim->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(m_programFile);

    switch ( m_testScript->runScript(inFile, outFile, useAsmFile) )
    {
        case MCUSimTestScript::ES_NO_COMMANDS:
            CU_FAIL("Test script does not contain any commands.");
            break;
        case MCUSimTestScript::ES_NO_ASSERTIONS:
            CU_FAIL("Test script does not contain any assertions.");
            break;
        case MCUSimTestScript::ES_OK:
            // Success.
            break;
        case MCUSimTestScript::ES_ABORTED:
            CU_FAIL("Test script execution encountered a fatal error and was aborted.");
            break;
        case MCUSimTestScript::ES_ASSERTION_FAILED:
            CU_FAIL("Some of the test script assertions have failed.");
            break;
    }
}

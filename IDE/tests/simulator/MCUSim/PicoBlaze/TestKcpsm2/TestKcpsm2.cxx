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
 * @ingroup testsMCUSimPicoBlaze
 * @file TestKcpsm2.cxx
 */
// =============================================================================

#include "TestKcpsm2.h"

// Tool for working with Intel® 16 Hex files.
#include "HexFile.h"

// PicoBlaze simulator.
#include "PicoBlazeSim.h"
#include "PicoBlazeConfig.h"
#include "PicoBlazeProgramMemory.h"

// MCUSim test script interpreter.
#include "MCUSimTestScript.h"
#include "../PicoBlazeStrategy.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Standard header files.
#include <iostream>

int TestKcpsm2::init()
{
    m_picoBlazeSim = new PicoBlazeSim;
    m_programFile  = new HexFile;
    m_strategy     = new PicoBlazeStrategy;
    m_testScript   = new MCUSimTestScript(m_picoBlazeSim, m_strategy);

    dynamic_cast<PicoBlazeConfig&>(m_picoBlazeSim->getConfig()).configure(MCUSim::FAMILY_KCPSM2);
    m_picoBlazeSim->reset(MCUSim::RSTMD_NEW_CONFIG);

    return 0;
}

int TestKcpsm2::clean()
{
    delete m_programFile;
    delete m_picoBlazeSim;
    delete m_strategy;
    delete m_testScript;

    return 0;
}

bool TestKcpsm2::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestKcpsm2") / "testcases" );
          directory_iterator() != dir;
          dir++ )
    {
        if ( false == is_regular_file(*dir) )
        {
            continue;
        }

        std::string extension = dir->path().extension().string();
        std::string testName  = dir->path().filename().string();

        if ( ".in" == extension || ".asm" == extension )
        {
            bool found = false;

            testName.resize ( testName.size() - extension.size() );

            for ( const auto & testCaseFile : testCaseFiles )
            {
                if ( testCaseFile == testName )
                {
                    found = true;
                    break;
                }
            }

            if ( false == found )
            {
                testCaseFiles.push_back(testName);
            }
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

void TestKcpsm2::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("TestKcpsm2") / "results" );

    std::string inFile  = ( path("TestKcpsm2") / "testcases" / (testName + "."   ) ).string();
    std::string outFile = ( path("TestKcpsm2") / "results"   / (testName + ".out") ).string();
    std::string hexFile = ( path("TestKcpsm2") / "results"   / (testName + ".hex") ).string();

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

    m_picoBlazeSim->reset(MCUSim::RSTMD_INITIAL_VALUES);
    m_picoBlazeSim->reset(MCUSim::RSTMD_MCU_RESET);

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

    MCUSimSubsys * programMemSubsys = m_picoBlazeSim->getSubsys(MCUSimSubsys::ID_MEM_CODE);
    PicoBlazeProgramMemory * programMem = dynamic_cast<PicoBlazeProgramMemory*>(programMemSubsys);
    programMem->loadDataFile(m_programFile);

    CU_ASSERT_TRUE ( m_testScript->runScript(inFile, outFile, useAsmFile) );
}

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
 * @file TestKcpsm1CPLD.cxx
 */
// =============================================================================

#include "TestKcpsm1CPLD.h"

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
#include <boost/filesystem.hpp>

// Standard header files.
#include <iostream>

int TestKcpsm1CPLD::init()
{
    m_picoBlazeSim = new PicoBlazeSim;
    m_programFile  = new HexFile;
    m_strategy     = new PicoBlazeStrategy;
    m_testScript   = new MCUSimTestScript(m_picoBlazeSim, m_strategy);

    dynamic_cast<PicoBlazeConfig&>(m_picoBlazeSim->getConfig()).configure(MCUSim::FAMILY_KCPSM1CPLD);
    m_picoBlazeSim->reset(MCUSim::RSTMD_NEW_CONFIG);

    return 0;
}

int TestKcpsm1CPLD::clean()
{
    delete m_programFile;
    delete m_picoBlazeSim;
    delete m_strategy;
    delete m_testScript;

    return 0;
}

bool TestKcpsm1CPLD::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestKcpsm1CPLD") / "testcases" );
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

            for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
                  it != testCaseFiles.cend();
                  it++ )
            {
                if ( 0 == it->compare(testName) )
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

    for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
          it != testCaseFiles.cend();
          it++ )
    {
        if ( NULL == CU_add_test(suite, it->c_str(), &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestKcpsm1CPLD::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    std::string inFile  = ( path("TestKcpsm1CPLD") / "testcases" / (testName + "."   ) ).string();
    std::string outFile = ( path("TestKcpsm1CPLD") / "results"   / (testName + ".out") ).string();
    std::string hexFile = ( path("TestKcpsm1CPLD") / "results"   / (testName + ".hex") ).string();

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
    catch ( DataFileException & e )
    {
        CU_FAIL_FATAL("Instance of DataFileException thrown:");
        std::cerr << e.toString() << std::endl;
    }

    MCUSimSubsys * programMemSubsys = m_picoBlazeSim->getSubsys(MCUSimSubsys::ID_MEM_CODE);
    PicoBlazeProgramMemory * programMem = dynamic_cast<PicoBlazeProgramMemory*>(programMemSubsys);
    programMem->loadDataFile(m_programFile);

    CU_ASSERT_TRUE ( m_testScript->runScript(inFile, outFile, useAsmFile) );
}

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
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

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Standard header files.
#include <iostream>

int TestKcpsm2::init()
{
    m_picoBlazeSim = new PicoBlazeSim;
    m_programFile  = new HexFile;
    m_testScript   = new MCUSimTestScript(m_picoBlazeSim);

    dynamic_cast<PicoBlazeConfig&>(m_picoBlazeSim->getConfig()).configure(MCUSim::FAMILY_KCPSM2);
    m_picoBlazeSim->reset(MCUSim::RSTMD_NEW_CONFIG);

    return 0;
}

int TestKcpsm2::clean()
{
    delete m_programFile;
    delete m_picoBlazeSim;
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
        std::string testName = dir->path().filename().string();

        if ( ".in" == extension )
        {
            testName.resize(testName.size() - 3);
            testCaseFiles.push_back(testName);
        }
    }

    std::sort(testCaseFiles.begin(), testCaseFiles.end());

    for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
          it != testCaseFiles.cend();
          it++ )
    {
        char * testCaseName = new char [ it->size() + 1 ];
        strcpy(testCaseName, it->c_str());

        if ( NULL == CU_add_test(suite, testCaseName, &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestKcpsm2::testFunction()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;

    const std::string inFile  = ( path("TestKcpsm2") / "testcases" / (testName + ".in" ) ).string();
    const std::string outFile = ( path("TestKcpsm2") / "results"   / (testName + ".out") ).string();
    const std::string hexFile = ( path("TestKcpsm2") / "results"   / (testName + ".hex") ).string();

    m_picoBlazeSim->reset(MCUSim::RSTMD_INITIAL_VALUES);
    m_picoBlazeSim->reset(MCUSim::RSTMD_MCU_RESET);

    try
    {
        m_programFile->clearAndLoad(hexFile);
    }
    catch ( DataFile::Exception & e )
    {
        CU_FAIL_FATAL("Instance of DataFile::Exception thrown:");
        std::cerr << e.toString() << std::endl;
    }

    dynamic_cast<PicoBlazeProgramMemory*>(m_picoBlazeSim->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(m_programFile);

    CU_ASSERT_TRUE ( m_testScript->runScript(inFile, outFile) );
}

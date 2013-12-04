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
#include <boost/filesystem.hpp>

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

void TestGenericSubsys::testFunction()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;

    const std::string inFile  = ( path("TestGenericSubsys") / "testcases" / (testName + ".asm" ) ).string();
    const std::string outFile = ( path("TestGenericSubsys") / "results"   / (testName + ".out") ).string();
    const std::string hexFile = ( path("TestGenericSubsys") / "results"   / (testName + ".hex") ).string();

    m_pic8Sim->reset(MCUSim::RSTMD_INITIAL_VALUES);
    m_pic8Sim->reset(MCUSim::RSTMD_MCU_RESET);

    try
    {
        m_programFile->clearAndLoad(hexFile);
    }
    catch ( DataFileException & e )
    {
        CU_FAIL_FATAL("Instance of DataFileException thrown:");
        std::cerr << e.toString() << std::endl;
    }

    dynamic_cast<PIC8ProgramMemory*>(m_pic8Sim->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(m_programFile);

    CU_ASSERT_TRUE ( m_testScript->runScript(inFile, outFile) );
}

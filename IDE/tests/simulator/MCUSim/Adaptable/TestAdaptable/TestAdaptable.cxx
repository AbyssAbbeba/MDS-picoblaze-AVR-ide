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
 * @file TestAdaptable.cxx
 */
// =============================================================================

#include "TestAdaptable.h"

// Tool for working with Intel 16 Hex files.
#include "HexFile.h"

// Adaptable simulator.
#include "AdaptableSim.h"
#include "AdaptableSimConfig.h"
#include "AdaptableSimProgramMemory.h"

// Simulator configuration management.
#include "McuDeviceSpecAdaptable.h"

// MCUSim test script interpreter.
#include "MCUSimTestScript.h"
#include "AdaptableStrategy.h"

// Library for working with processor definition files used by Adaptable Simulator.
#include "AdjSimProcDef/AdjSimProcDef.h"
#include "AdjSimProcDef/AdjSimProcDefParser.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Standard header files.
#include <memory>
#include <iostream>

int TestAdaptable::init()
{
    m_adaptableSim = new AdaptableSim;
    m_programFile  = new HexFile;
    m_strategy     = new AdaptableStrategy;
    m_testScript   = new MCUSimTestScript(m_adaptableSim, m_strategy);

    return 0;
}

int TestAdaptable::clean()
{
    delete m_programFile;
    delete m_adaptableSim;
    delete m_strategy;
    delete m_testScript;

    return 0;
}

bool TestAdaptable::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator basedir ( path("TestAdaptable") / "testcases" );
          directory_iterator() != basedir;
          basedir++ )
    {
        if ( false == is_directory(*basedir) )
        {
            continue;
        }

        for ( directory_iterator dir(basedir->path()); directory_iterator() != dir; dir++ )
        {
            if ( false == is_regular_file(*dir) )
            {
                continue;
            }

            std::string extension = dir->path().extension().string();
            std::string testName  = ( basedir->path().filename() / dir->path().filename() ).string();

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

void TestAdaptable::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("TestAdaptable") / "results" );
    create_directory ( path("TestAdaptable") / "results" / path(testName).parent_path() );

    std::string inFile  = ( path("TestAdaptable") / "testcases" / (testName + "."   ) ).string();
    std::string outFile = ( path("TestAdaptable") / "results"   / (testName + ".out") ).string();
    std::string hexFile = ( path("TestAdaptable") / "results"   / (testName + ".hex") ).string();

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

    // Configure simulator.
    {
        std::string devSpecFile = system_complete ( path("TestAdaptable") / "testcases" / ( testName + ".procdef" ) )
                                                  . string();

        std::ifstream file ( devSpecFile, (std::ios_base::in | std::ios_base::binary) );
        if ( false == file.is_open() )
        {
            CU_FAIL("Unable to open processor definition file: " + devSpecFile);
            return;
        }
        static const long long int MAX_SIZE = 102400;
        std::unique_ptr<char[]> data ( new char [ MAX_SIZE ] );
        size_t len = (size_t) file.readsome (data.get(), MAX_SIZE);
        if ( true == file.bad() )
        {
            CU_FAIL("Unable to read processor definition file: " + devSpecFile);
            return;
        }

        std::unique_ptr<AdjSimProcDefParser> parser (new AdjSimProcDefParser(std::string(data.get(), len)));
        if ( false == parser.get()->isValid() )
        {
            CU_FAIL("Unable to parse processor definition file: " + devSpecFile);
            return;
        }

        McuDeviceSpecAdaptable simConfigManager(parser.get()->data());
        simConfigManager.setupSimulator(dynamic_cast<AdaptableSimConfig&>(m_adaptableSim->getConfig()));
    }

    // Reset simulator.
    m_adaptableSim->reset(MCUSim::RSTMD_NEW_CONFIG);
    m_adaptableSim->reset(MCUSim::RSTMD_INITIAL_VALUES);
    m_adaptableSim->reset(MCUSim::RSTMD_MCU_RESET);
    m_adaptableSim->getLog()->clear();

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

    MCUSimSubsys * programMemSubsys = m_adaptableSim->getSubsys(MCUSimSubsys::ID_MEM_CODE);
    AdaptableSimProgramMemory * programMem = dynamic_cast<AdaptableSimProgramMemory*>(programMemSubsys);
    programMem->loadDataFile(m_programFile);

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

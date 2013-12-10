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
 * @file TestMScriptII.cxx
 */
// =============================================================================

#include "TestMScriptII.h"

// Standard header files.
#include <cstdio>
#include <fstream>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// MScript language interpreter header files.
#include "MScript.h"

// Testing header files.
#include "MScriptTestStrategy.h"

#define TEST_SUITE_DIR "TestMScriptII"

int TestMScriptII::init()
{
    m_testStrategy = new MScriptTestStrategy();
    m_core = new MScript(m_testStrategy);

    return 0;
}

int TestMScriptII::clean()
{
    delete m_core;
    delete m_testStrategy;

    return 0;
}

bool TestMScriptII::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path(TEST_SUITE_DIR) / "testcases" );
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

        if ( ".mscript" == extension )
        {
            testName.resize(testName.size() - 8);
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

void TestMScriptII::testFunction()
{
    using namespace boost::filesystem;

    bool success = true;
    std::string testName       = CU_get_current_test()->pName;
    std::string logFileName    = ( path(TEST_SUITE_DIR) / "results"   / (testName + ".log") ).string();
    std::string scriptFileName = ( path(TEST_SUITE_DIR) / "testcases" / (testName + ".mscript") ).string();

    FILE * scriptFile = fopen(scriptFileName.c_str(), "r");
    CU_ASSERT_FATAL( NULL != scriptFile );
    CU_ASSERT_FATAL( true == m_core->loadScript(scriptFile, scriptFileName) );

    try
    {
        m_core->executeRun();
    }
    catch ( const MScriptBase::MScriptRunTimeError & e )
    {
        CU_FAIL("An instance of MScriptBase::MScriptRunTimeError exception thrown.");
        success = false;
    }

    std::ofstream logFile(logFileName);
    for ( auto i : m_core->getMessages() )
    {
        logFile << i << std::endl;
    }
    m_core->clearMessages();

    CU_ASSERT_FATAL( true == success );
}

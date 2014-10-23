// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup testsAssemblerAdaptable
 * @file TestSuiteErr.cxx
 */
// =============================================================================

#include "TestSuiteErr.h"

// Standard header files.
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Compiler header files.
#include "Compiler.h"
#include "core/CompilerMsgIntfFile.h"

int TestSuiteErr::init()
{
    using namespace boost::filesystem;

    m_msgInt = new CompilerMsgIntfFile();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL | CompilerOptions::V_ERRORS |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestSuiteErr::clean()
{
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestSuiteErr::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestSuiteErr") / "testcases" );
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
            testName.resize(testName.size() - 4);
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

void TestSuiteErr::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("TestSuiteErr") / "results" );

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back ( system_complete ( path("TestSuiteErr") / "testcases" / ( testName + ".asm" ) )
                                                         . string() );

    const std::string resultsCommonPath = system_complete( path("TestSuiteErr") / "results" / testName ).string();
    m_options->m_symbolTable  = resultsCommonPath + ".sym";
    m_options->m_macroTable   = resultsCommonPath + ".mac";
    m_options->m_codeTree     = resultsCommonPath + ".crt";;
    m_options->m_lstFile      = resultsCommonPath + ".lst";

    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(resultsCommonPath + ".err");
    if ( true == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_ADAPTABLE, m_options) )
    {
        CU_FAIL("Compilation succeeded (while it should have failed).");

        return;
    }


    const std::string expectedCommonPath = system_complete( path("TestSuiteErr") / "expected" / testName ).string();
    compareLst ( expectedCommonPath + ".lst.exp", m_options->m_lstFile );
    compareErr ( expectedCommonPath + ".err.exp", resultsCommonPath + ".err" );
}

void TestSuiteErr::compareErr ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> errExpFileVec;

    std::ifstream errExpFile(expected);
    while ( false == errExpFile.eof() )
    {
        if ( false == errExpFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + expected ).c_str());
            return;
        }

        std::string line;
        std::getline(errExpFile, line);
        errExpFileVec.push_back(line);
    }
    errExpFile.close();

    size_t expFileNoOfLines = errExpFileVec.size();
    size_t errLineNumber = 0;

    std::ifstream errFile(actual);
    while ( false == errFile.eof() )
    {
        if ( false == errFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + actual ).c_str());
            return;
        }

        std::string errFileLine;
        std::getline(errFile, errFileLine);

        if ( errLineNumber >= expFileNoOfLines )
        {
            CU_FAIL("errLineNumber < expFileNoOfLines");
            return;
        }

        if ( errFileLine != errExpFileVec[errLineNumber] )
        {
            CU_FAIL("Result and expectation inconsistence found in compilation error log.");
            break;
        }

        errLineNumber++;
    }

    size_t errFileNoOfLines = errLineNumber;
    CU_ASSERT_EQUAL(errFileNoOfLines, expFileNoOfLines);
}

void TestSuiteErr::compareLst ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> lstExpFileVec;

    std::ifstream lstExpFile(expected);
    while ( false == lstExpFile.eof() )
    {
        if ( false == lstExpFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + expected ).c_str());
            return;
        }

        std::string line;
        std::getline(lstExpFile, line);

        // Skip compiler messages.
        if ( line.size() < 2 || ':' != line[1] )
        {
            lstExpFileVec.push_back(line);
        }
    }
    lstExpFile.close();

    size_t expFileNoOfLines = lstExpFileVec.size();
    size_t lstLineNumber = 0;

    std::ifstream lstFile(actual);
    while ( false == lstFile.eof() )
    {
        if ( false == lstFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + actual ).c_str());
            return;
        }

        std::string lstFileLine;
        std::getline(lstFile, lstFileLine);

        // Skip compiler messages.
        if ( lstFileLine.size() > 1 && ':' == lstFileLine[1] )
        {
            continue;
        }

        if ( lstLineNumber >= expFileNoOfLines )
        {
            CU_FAIL("lstLineNumber < expFileNoOfLines");
            return;
        }

        if ( lstFileLine != lstExpFileVec[lstLineNumber] )
        {
            CU_FAIL("Result and expectation inconsistence found in code listing.");
            break;
        }

        lstLineNumber++;
    }

    size_t lstFileNoOfLines = lstLineNumber;
    CU_ASSERT_EQUAL(lstFileNoOfLines, expFileNoOfLines);
}

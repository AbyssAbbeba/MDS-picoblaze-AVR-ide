// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup testsAssemblerPicoBlaze
 * @file TestSuiteErr.cxx
 */
// =============================================================================

#include "TestSuiteErr.h"

// Standard header files.
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Compiler header files.
#include "core/Compiler.h"
#include "core/CompilerMsgIntfFile.h"

int TestSuiteErr::init()
{
    m_msgInt = new CompilerMsgIntfFile();
    m_compiler = new Compiler(m_msgInt);
    m_options = new CompilerOptions();

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

    for ( directory_iterator dir ( path("TestSuiteErr") /= path("testcases") );
          directory_iterator() != dir;
          dir++ )
    {
        if ( false == is_regular_file(*dir) )
        {
            continue;
        }

        std::string extension = dir->path().extension().string();
        std::string testName = dir->path().filename().string();

        if ( ".asm" == extension )
        {
            testName.resize(testName.size() - 4);
            testCaseFiles.push_back(testName);
        }
    }

    std::sort(testCaseFiles.begin(), testCaseFiles.end());

    for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
          it != testCaseFiles.cend();
          it++ )
    {
        char * testCaseName = new char [ it->size() ];
        strcpy(testCaseName, it->c_str());

        if ( NULL == CU_add_test(suite, testCaseName, &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestSuiteErr::testFunction()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;
    m_options->m_sourceFile = ( path("TestSuiteErr") /= path("testcases") /= path(testName + ".asm") ).generic_string();

    std::string resultsCommonPath = ( path("TestSuiteErr") /= path("results") /= path(testName) ).generic_string();
    m_options->m_symbolTable  = resultsCommonPath + ".sym";
    m_options->m_macroTable   = resultsCommonPath + ".mac";
    m_options->m_codeTree     = resultsCommonPath + ".crt";;
    m_options->m_lstFile      = resultsCommonPath + ".lst";

    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(resultsCommonPath + ".err");
    CU_ASSERT_FATAL ( false == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_KCPSM3, m_options) );
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->closeFile();

    std::string expectedCommonPath = ( path("TestSuiteErr") /= path("expected") /= path(testName) ).generic_string();
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
        CU_ASSERT_TRUE_FATAL(errExpFile.good());

        std::string line;
        std::getline(errExpFile, line);
        errExpFileVec.push_back(line);
    }
    errExpFile.close();

    size_t expFileNoOfLines = errExpFileVec.size();
    int errLineNumber = 0;

    std::ifstream errFile(actual);
    while ( false == errFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(errFile.good());

        std::string errFileLine;
        std::getline(errFile, errFileLine);

        CU_ASSERT(errLineNumber < expFileNoOfLines);

        if ( errFileLine != errExpFileVec[errLineNumber] )
        {
            CU_FAIL("Result and expectation inconsistence found in compilation error log.");
        }

        errLineNumber++;
    }

    int errFileNoOfLines = errLineNumber;
    CU_ASSERT_EQUAL(errFileNoOfLines, expFileNoOfLines);
}

void TestSuiteErr::compareLst ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> lstExpFileVec;

    std::ifstream lstExpFile(expected);
    while ( false == lstExpFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(lstExpFile.good());

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
    int lstLineNumber = 0;

    std::ifstream lstFile(actual);
    while ( false == lstFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(lstFile.good());

        std::string lstFileLine;
        std::getline(lstFile, lstFileLine);

        // Skip compiler messages.
        if ( lstFileLine.size() > 1 && ':' == lstFileLine[1] )
        {
            continue;
        }

        CU_ASSERT(lstLineNumber < expFileNoOfLines);

        if ( lstFileLine != lstExpFileVec[lstLineNumber] )
        {
            CU_FAIL("Result and expectation inconsistence found in code listing.");
        }

        lstLineNumber++;
    }

    int lstFileNoOfLines = lstLineNumber;
    CU_ASSERT_EQUAL(lstFileNoOfLines, expFileNoOfLines);
}

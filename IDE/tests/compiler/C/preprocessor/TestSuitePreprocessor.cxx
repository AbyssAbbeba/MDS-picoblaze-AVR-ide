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
 * @ingroup testsCompilerC
 * @file TestSuitePreprocessor.cxx
 */
// =============================================================================

#include "TestSuitePreprocessor.h"

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
#include "compiler/Compiler.h"
#include "compiler/core/CompilerMsgIntfFile.h"

int TestSuitePreprocessor::init()
{
    using namespace boost::filesystem;

    m_msgInt = new CompilerMsgIntfFile();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_syntaxCheckOnly = true;

    return 0;
}

int TestSuitePreprocessor::clean()
{
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestSuitePreprocessor::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("preprocessor") / "testcases" );
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

        if ( ".c" == extension )
        {
            testName.resize(testName.size() - 2);
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

void TestSuitePreprocessor::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("preprocessor") / "results" );

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back((path("preprocessor")/"testcases"/(testName + ".c")).string());

    const std::string resultsCommonPath = ( path("..") / "results" / testName ).string();
    m_options->m_cunit = resultsCommonPath + ".out";

    const std::string errFile = (path("preprocessor") / "results" / ( testName + ".err" )).string();
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);
    m_compiler->compile(CompilerBase::LI_C, CompilerBase::TA_PICOBLAZE, m_options);

    const std::string expectedCommonPath = system_complete(path("preprocessor") / "expected" / testName ).string();
    compareOutput ( expectedCommonPath + ".out.exp", m_options->m_cunit );
}

void TestSuitePreprocessor::compareOutput ( const std::string & expected,
                                            const std::string & actual )
{
    using namespace boost::filesystem;

    std::vector<std::string> outExpFileVec;

    std::ifstream outExpFile(expected);
    while ( false == outExpFile.eof() )
    {
        if ( false == outExpFile.good() )
        {
            CU_FAIL("Cannot read file: " + expected);
            return;
        }

        std::string line;
        std::getline(outExpFile, line);
        while ( ( false == line.empty() ) && ( ( ' ' == line.back() ) || ( '\t' == line.back() ) ) )
        {
            line.pop_back();
        }
        outExpFileVec.push_back(line);
    }
    outExpFile.close();

    size_t expFileNoOfLines = outExpFileVec.size();
    size_t outLineNumber = 0;

    std::ifstream outFile(actual);
    while ( false == outFile.eof() )
    {
        if ( false == outFile.good() )
        {
            CU_FAIL("Cannot read file: " + actual);
            return;
        }

        std::string line;
        std::getline(outFile, line);

        while ( ( false == line.empty() ) && ( ( ' ' == line.back() ) || ( '\t' == line.back() ) ) )
        {
            line.pop_back();
        }

        if ( outLineNumber >= expFileNoOfLines )
        {
            CU_FAIL_MSG ( actual,
                          outLineNumber + 1,
                          "Output is longer than expected." );
            return;
        }

        if ( line != outExpFileVec[outLineNumber] )
        {
            CU_FAIL_MSG ( actual,
                          outLineNumber + 1,
                          "\nexpected: '" + outExpFileVec[outLineNumber] + "'"
                          "\n" "actual:   '" + line + "'" );

            break;
        }

        outLineNumber++;
    }

    if ( outLineNumber < expFileNoOfLines )
    {
        CU_FAIL_MSG ( actual,
                      outLineNumber + 1,
                      "Output is shorter than expected." );
    }
}

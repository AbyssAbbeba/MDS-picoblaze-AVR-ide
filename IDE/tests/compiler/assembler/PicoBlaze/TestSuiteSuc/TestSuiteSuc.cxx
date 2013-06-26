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
 * @ingroup testsAssemblerPicoBlaze
 * @file TestSuiteSuc.cxx
 */
// =============================================================================

#include "TestSuiteSuc.h"

// Standard header files.
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

// MCUDataFiles
#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"
#include "DbgFileNative.h"

// Moravia Microsystems, s.r.o. proprietary Debugger File
#include "DbgFileNative.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Compiler header files.
#include "core/Compiler.h"
#include "core/CompilerMsgIntfStdout.h"

int TestSuiteSuc::init()
{
    using namespace boost::filesystem;

    m_msgInt = new CompilerMsgIntfStdout();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_verbosity = CompilerOptions::V_GENERAL;

    return 0;
}

int TestSuiteSuc::clean()
{
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestSuiteSuc::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestSuiteSuc") / "testcases" );
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
        char * testCaseName = new char [ it->size() + 1 ];
        strcpy(testCaseName, it->c_str());

        if ( NULL == CU_add_test(suite, testCaseName, &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestSuiteSuc::testFunction()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;

    m_options->m_sourceFile = ( path("TestSuiteSuc") / "testcases" / (testName + ".asm") ).string();

    std::string resultsCommonPath = ( path("..") / "results" / testName ).string();
    m_options->m_symbolTable  = resultsCommonPath + ".sym";
    m_options->m_macroTable   = resultsCommonPath + ".mac";
    m_options->m_mdsDebugFile = resultsCommonPath + ".dbg";
    m_options->m_codeTree     = resultsCommonPath + ".crt";
    m_options->m_hexFile      = resultsCommonPath + ".hex";
    m_options->m_binFile      = resultsCommonPath + ".bin";
    m_options->m_srecFile     = resultsCommonPath + ".srec";
    m_options->m_lstFile      = resultsCommonPath + ".lst";
    m_options->m_verilogFile  = resultsCommonPath + ".v";
    m_options->m_vhdlFile     = resultsCommonPath + ".vhd";

    CU_ASSERT_FATAL ( true == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_KCPSM3, m_options) );

    std::string expectedCommonPath = ( path("..") / "expected" / testName ).string();
    compareLst ( expectedCommonPath + ".lst.exp", m_options->m_lstFile     );
    compareSym ( expectedCommonPath + ".sym.exp", m_options->m_symbolTable );
    compareMac ( expectedCommonPath + ".mac.exp", m_options->m_macroTable  );
    compareV   ( expectedCommonPath + ".v.exp",   m_options->m_verilogFile );
    compareVhd ( expectedCommonPath + ".vhd.exp", m_options->m_vhdlFile    );

    try
    {
        compareHex ( expectedCommonPath + ".hex.exp",  m_options->m_hexFile  );
        compareBin ( expectedCommonPath + ".bin.exp",  m_options->m_binFile  );
        compareSrec( expectedCommonPath + ".srec.exp", m_options->m_srecFile );
    }
    catch ( DataFile::Exception & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL_FATAL("An instance of DataFile::Exception thrown!");
    }

    try
    {
        compareDbg ( expectedCommonPath + ".dbg.exp",  m_options->m_mdsDebugFile );
    }
    catch ( DbgFile::Exception & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL_FATAL("An instance of DbgFile::Exception thrown!");
    }
}

void TestSuiteSuc::compareHex ( const std::string & expected,
                                const std::string & actual )
{
    CU_ASSERT ( HexFile(expected) == HexFile(actual) );
}

void TestSuiteSuc::compareBin ( const std::string & expected,
                                const std::string & actual )
{
    CU_ASSERT ( BinFile(expected) == BinFile(actual) );
}

void TestSuiteSuc::compareSrec ( const std::string & expected,
                                 const std::string & actual )
{
    CU_ASSERT ( SrecFile(expected) == SrecFile(actual) );
}

void TestSuiteSuc::compareDbg ( const std::string & expected,
                                const std::string & actual )
{
    CU_ASSERT ( DbgFileNative(expected) == DbgFileNative(actual) );
}

void TestSuiteSuc::compareV ( const std::string & /*expected*/,
                              const std::string & /*actual*/ )
{
    // TODO: implemet this once support for Verilog code generation is complete.
}

void TestSuiteSuc::compareVhd ( const std::string & /*expected*/,
                                const std::string & /*actual*/ )
{
    // TODO: implemet this once support for VHDL code generation is complete.
}

void TestSuiteSuc::compareSym ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> symExpFileVec[2];

    int numberOfTable = 0;
    std::ifstream symExpFile(expected);
    while ( false == symExpFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(symExpFile.good());

        std::string line;
        std::getline(symExpFile, line);

        if ( true == line.empty() )
        {
            continue;
        }
        if ( "REMOVED SYMBOLS:" == line )
        {
            numberOfTable = 1;
            continue;
        }
        symExpFileVec[numberOfTable].push_back(line);
    }
    symExpFile.close();

    numberOfTable = 0;
    std::ifstream symFile(actual);
    while ( false == symFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(symFile.good());

        std::string line;
        std::getline(symFile, line);

        if ( true == line.empty() )
        {
            continue;
        }
        if ( "REMOVED SYMBOLS:" == line )
        {
            numberOfTable = 1;
            continue;
        }

        bool entryFoundInSymExpFile = false;
        for ( std::vector<std::string>::iterator it = symExpFileVec[numberOfTable].begin();
              it != symExpFileVec[numberOfTable].end();
              it++ )
        {
            if ( line == *it )
            {
                entryFoundInSymExpFile = true;
                symExpFileVec[numberOfTable].erase(it);
                break;
            }
        }

        CU_ASSERT_TRUE(entryFoundInSymExpFile);
    }

    if ( 0 != ( symExpFileVec[0].size() + symExpFileVec[1].size() ) )
    {
        CU_FAIL("Symbol table contains more entries than expected.");
    }
}

void TestSuiteSuc::compareMac ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> macExpFileVec;

    std::ifstream macExpFile(expected);
    while ( false == macExpFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(macExpFile.good());

        std::string line;
        std::getline(macExpFile, line);
        macExpFileVec.push_back(line);

    }
    macExpFile.close();

    std::ifstream macFile(actual);
    while ( false == macFile.eof() )
    {
        CU_ASSERT_TRUE_FATAL(macFile.good());

        std::string line;
        std::getline(macFile, line);

        bool entryFoundInMacExpFile = false;
        for ( std::vector<std::string>::iterator it = macExpFileVec.begin();
              it != macExpFileVec.end();
              it++ )
        {
            if ( line == *it )
            {
                entryFoundInMacExpFile = true;
                macExpFileVec.erase(it);
                break;
            }
        }

        CU_ASSERT_TRUE(entryFoundInMacExpFile);
    }

    if ( 0 != macExpFileVec.size() )
    {
        CU_FAIL("Macro table contains more entries than expected.");
    }
}

void TestSuiteSuc::compareLst ( const std::string & expected,
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

        CU_ASSERT_FATAL(lstLineNumber < expFileNoOfLines);

        if ( lstFileLine != lstExpFileVec[lstLineNumber] )
        {
            CU_FAIL("Result and expectation inconsistence found in code listing.");
        }

        lstLineNumber++;
    }

    int lstFileNoOfLines = lstLineNumber;
    CU_ASSERT_EQUAL(lstFileNoOfLines, expFileNoOfLines);
}

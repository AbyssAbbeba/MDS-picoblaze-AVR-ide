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
 * @ingroup testsAssemblerAdaptable
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
#include "MCUDataFiles.h"

// Moravia Microsystems, s.r.o. proprietary Debugger File
#include "DbgFileNative.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// Compiler header files.
#include "Compiler.h"
#include "core/CompilerMsgIntfFile.h"

int TestSuiteSuc::init()
{
    using namespace boost::filesystem;

    m_msgInt = new CompilerMsgIntfFile();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / ".." / "compiler" / "include" ).string() );

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

void TestSuiteSuc::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("TestSuiteSuc") / "results" );

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back ( ( path("TestSuiteSuc") / "testcases" / (testName + ".asm") ).string() );

    const std::string resultsCommonPath = ( path("..") / "results" / testName ).string();
    m_options->m_symbolTable    = resultsCommonPath + ".sym";
    m_options->m_macroTable     = resultsCommonPath + ".mac";
    m_options->m_mdsDebugFile   = resultsCommonPath + ".dbg";
    m_options->m_codeTree       = resultsCommonPath + ".crt";
    m_options->m_hexFile        = resultsCommonPath + ".hex";
    m_options->m_binFile        = resultsCommonPath + ".bin";
    m_options->m_srecFile       = resultsCommonPath + ".srec";
    m_options->m_lstFile        = resultsCommonPath + ".lst";
    m_options->m_verilogFile    = resultsCommonPath + ".v";
    m_options->m_vhdlFile       = resultsCommonPath + ".vhd";
    m_options->m_memFile        = resultsCommonPath + ".mem";
    m_options->m_rawHexDumpFile = resultsCommonPath + ".rawhex";

    const std::string errFile = (path("TestSuiteSuc") / "results" / ( testName + ".err" )).string();
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);
    bool result = m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_ADAPTABLE, m_options);


    if ( false == result )
    {
        CU_FAIL("Compilation failed.");
        return;
    }

    const std::string expectedCommonPath = system_complete( path("TestSuiteSuc") / "expected" / testName ).string();
    compareLst ( expectedCommonPath + ".lst.exp", m_options->m_lstFile     );
    compareSym ( expectedCommonPath + ".sym.exp", m_options->m_symbolTable );
    compareMac ( expectedCommonPath + ".mac.exp", m_options->m_macroTable  );

    try
    {
        compareHex  ( expectedCommonPath +  ".hex.exp", m_options->m_hexFile     );
        compareBin  ( expectedCommonPath +  ".bin.exp", m_options->m_binFile     );
        compareSrec ( expectedCommonPath + ".srec.exp", m_options->m_srecFile    );
        compareMem  ( expectedCommonPath +  ".mem.exp", m_options->m_memFile     );
        compareV    ( expectedCommonPath +    ".v.exp", m_options->m_verilogFile );
        compareVhd  ( expectedCommonPath +  ".vhd.exp", m_options->m_vhdlFile    );
    }
    catch ( const DataFileException & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL("An instance of DataFileException thrown!");
        return;
    }

    try
    {
        compareDbg ( expectedCommonPath + ".dbg.exp",  m_options->m_mdsDebugFile );
    }
    catch ( const DbgFile::Exception & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL("An instance of DbgFile::Exception thrown!");
        return;
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

void TestSuiteSuc::compareMem ( const std::string & expected,
                                const std::string & actual )
{
    CU_ASSERT ( XilMemFile(expected, 3) == XilMemFile(actual, 3) );
}

void TestSuiteSuc::compareV ( const std::string & expected,
                              const std::string & actual )
{
    XilVerilogFile expectedVerilog ( expected, "", "", XilHDLFile::SIZE_18b );
    XilVerilogFile actualVerilog   ( actual,   "", "", XilHDLFile::SIZE_18b );
    CU_ASSERT ( expectedVerilog == actualVerilog );
}

void TestSuiteSuc::compareVhd ( const std::string & expected,
                                const std::string & actual )
{
    XilVHDLFile expectedVHDL ( expected, "", "", XilHDLFile::SIZE_18b );
    XilVHDLFile actualVHDL   ( actual,   "", "", XilHDLFile::SIZE_18b );
    CU_ASSERT ( expectedVHDL == actualVHDL );
}

void TestSuiteSuc::compareSym ( const std::string & expected,
                                const std::string & actual )
{
    std::vector<std::string> symExpFileVec[2];

    int numberOfTable = 0;
    std::ifstream symExpFile(expected);
    while ( false == symExpFile.eof() )
    {
        if ( false == symExpFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + expected ).c_str());
            return;
        }

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
    unsigned int lineNumber = 0;
    std::ifstream symFile(actual);
    while ( false == symFile.eof() )
    {
        if ( false == symFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + actual ).c_str());
            return;
        }

        lineNumber++;
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
        for ( auto it = symExpFileVec[numberOfTable].begin();
              it != symExpFileVec[numberOfTable].end();
              ++it )
        {
            if ( line == *it )
            {
                entryFoundInSymExpFile = true;
                symExpFileVec[numberOfTable].erase(it);
                break;
            }
        }

        if ( false == entryFoundInSymExpFile )
        {
            CU_FAIL_MSG ( actual,
                          lineNumber,
                          "unexpected entry: '" + line + '\'' );
            break;
        }
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
        if ( false == macExpFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + expected ).c_str());
            return;
        }

        std::string line;
        std::getline(macExpFile, line);
        macExpFileVec.push_back(line);

    }
    macExpFile.close();

    unsigned int lineNumber = 0;
    std::ifstream macFile(actual);
    while ( false == macFile.eof() )
    {
        if ( false == macFile.good() )
        {
            CU_FAIL((std::string("Cannot read file: ") + actual ).c_str());
            return;
        }

        lineNumber++;
        std::string line;
        std::getline(macFile, line);

        bool entryFoundInMacExpFile = false;
        for ( auto it = macExpFileVec.begin();
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

        if ( false == entryFoundInMacExpFile )
        {
            CU_FAIL_MSG ( actual,
                          lineNumber,
                          "unexpected entry: '" + line + '\'' );
            break;
        }
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
            CU_FAIL_MSG ( actual,
                          lstLineNumber + 1,
                          "Output is longer than expected." );
            return;
        }

        if ( lstFileLine != lstExpFileVec[lstLineNumber] )
        {
            CU_FAIL_MSG ( actual,
                          lstLineNumber + 1,
                          "\nexpected: '" + lstExpFileVec[lstLineNumber] + "'"
                          "\n" "actual:   '" + lstFileLine + "'" );
            break;
        }

        lstLineNumber++;
    }

    if ( lstLineNumber < expFileNoOfLines )
    {
        CU_FAIL_MSG ( actual,
                      lstLineNumber + 1,
                      "Output is shorter than expected." );
    }
}

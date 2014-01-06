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
 * @ingroup testsMCUDataFiles
 * @file testsMCUDataFiles.cxx
 */
// =============================================================================

#include "testsMCUDataFiles.h"

//
#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"
#include "XilMemFile.h"
#include "XilVHDLFile.h"
#include "XilVerilogFile.h"

// Standard header files.
#include <string>
#include <cctype>
#include <cstring>
#include <iostream>
#include <algorithm>

// The CUnit testing framework.
#include "3rdParty/CUnit/Basic.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Library for automated testing environment using libCUnit.
#include "AutoTest.h"

#define ADD_TEST_SUITE(id)                                              \
    autoTest->addTestSuite ( testSuiteID2cstring(TestSuiteID(id)),      \
                             &TestSuite::init,                          \
                             &TestSuite::clean,                         \
                             &TestSuite::addTests<TestSuiteID(id)> );

bool fileCompare ( const std::string & fileName1,
                   const std::string & fileName2 )
{
    std::ifstream file1(fileName1, std::ios_base::binary);
    std::ifstream file2(fileName2, std::ios_base::binary);

    if ( false == file1.is_open() || false == file2.is_open() )
    {
        return false;
    }

    while ( false == file1.eof() && false == file2.eof() )
    {
        if ( true == file1.bad() || true == file2.bad() )
        {
            return false;
        }

        if ( file1.get() != file2.get() )
        {
            return false;
        }
    }

    if ( true != file1.eof() || true != file2.eof() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

template<class Container>
    void testLoadAndSave ( const std::string & origFile,
                           const std::string & genFile1,
                           const std::string & genFile2,
                           bool invertable )
{
    Container file0;
    Container file1;
    Container file2;

    testLoadAndSave(file0, file1, file2, origFile, genFile1, genFile2, invertable);
}

void testLoadAndSave ( DataFile & file0,
                       DataFile & file1,
                       DataFile & file2,
                       const std::string & origFile,
                       const std::string & genFile1,
                       const std::string & genFile2,
                       bool invertable )
{
    try
    {
        file0.clearAndLoad(origFile);
        file0.save(genFile1);

        file1.clearAndLoad(genFile1);
        file1.save(genFile2);

        file2.clearAndLoad(genFile2);
    }
    catch ( DataFileException & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL("DataFileException thrown.");
        return;
    }

    if ( true == invertable )
    {
        CU_ASSERT_TRUE( file0 == file1 );
    }
    CU_ASSERT_TRUE( file1 == file2 );
    CU_ASSERT_TRUE( fileCompare(genFile1, genFile2) );
}

void testHex()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "hex" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<HexFile>( testcase, result + "0", result + "1", true );
}

void testBin()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "bin" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<BinFile>( testcase, result + "0", result + "1", false );
}

void testSrec()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "srec" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<SrecFile>( testcase, result + "0", result + "1", true );
}

template <int BYTES_PER_RECORD> void testMem()
{
    using namespace boost::filesystem;

    static const std::string number = ( ( 2 == BYTES_PER_RECORD ) ? "2" : "3" );

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / ( number + ".mem" ) / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    XilMemFile file0(BYTES_PER_RECORD);
    XilMemFile file1(BYTES_PER_RECORD);
    XilMemFile file2(BYTES_PER_RECORD);

    testLoadAndSave( file0, file1, file2, testcase, result + number + ".0", result + number + ".1", false );
}

template <XilHDLFile::OPCodeSize OP_CODE_SIZE> void testVerilog()
{
    using namespace boost::filesystem;

    static const std::string number = ( ( XilHDLFile::SIZE_16b == OP_CODE_SIZE ) ? "16" : "18" );
    static const std::string templateFile = ( number + ".VerilogTemplate.v" );

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / ( number + ".v" ) / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    XilVerilogFile file0(testName, templateFile, OP_CODE_SIZE);
    XilVerilogFile file1(testName, templateFile, OP_CODE_SIZE);
    XilVerilogFile file2(testName, templateFile, OP_CODE_SIZE);

    testLoadAndSave( file0, file1, file2, testcase, result + number + ".0", result + number + ".1", false );
}

template <XilHDLFile::OPCodeSize OP_CODE_SIZE> void testVHDL()
{
    using namespace boost::filesystem;

    static const std::string number = ( ( XilHDLFile::SIZE_16b == OP_CODE_SIZE ) ? "16" : "18" );
    static const std::string templateFile = ( number + ".VHDLTemplate.vhd" );

    const std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / ( number + ".vhd" ) / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    XilVHDLFile file0(testName, templateFile, OP_CODE_SIZE);
    XilVHDLFile file1(testName, templateFile, OP_CODE_SIZE);
    XilVHDLFile file2(testName, templateFile, OP_CODE_SIZE);

    testLoadAndSave( file0, file1, file2, testcase, result + number + ".0", result + number + ".1", false );
}

TestSuite::fptr testSuiteID2funcPtr ( TestSuite::TestSuiteID id )
{
    switch ( id )
    {
        case TestSuite::TS_IHEX:    return &testHex;
        case TestSuite::TS_BIN:     return &testBin;
        case TestSuite::TS_SREC:    return &testSrec;
        case TestSuite::TS_MEM_2:   return &testMem<2>;
        case TestSuite::TS_MEM_3:   return &testMem<3>;
        case TestSuite::TS_V_16:    return &testVerilog<XilHDLFile::SIZE_16b>;
        case TestSuite::TS_V_18:    return &testVerilog<XilHDLFile::SIZE_18b>;
        case TestSuite::TS_VHDL_16: return &testVHDL<XilHDLFile::SIZE_16b>;
        case TestSuite::TS_VHDL_18: return &testVHDL<XilHDLFile::SIZE_18b>;
        case TestSuite::TS__MAX__:  return NULL;
    }
    return NULL;
}

const char * testSuiteID2cstring ( TestSuite::TestSuiteID id )
{
    switch ( id )
    {
        case TestSuite::TS_IHEX:    return "Intel HEX";
        case TestSuite::TS_BIN:     return "raw binary";
        case TestSuite::TS_SREC:    return "Motorola S-Record";
        case TestSuite::TS_MEM_2:   return "Xilinx MEM (2B)";
        case TestSuite::TS_MEM_3:   return "Xilinx MEM (3B)";
        case TestSuite::TS_V_16:    return "Verilog (16b)";
        case TestSuite::TS_V_18:    return "Verilog (18b)";
        case TestSuite::TS_VHDL_16: return "VHDL (18b)";
        case TestSuite::TS_VHDL_18: return "VHDL (16b)";
        case TestSuite::TS__MAX__:  return "<invalid>";
    }
    return NULL;
}

const char * testSuiteID2ext ( TestSuite::TestSuiteID id )
{
    switch ( id )
    {
        case TestSuite::TS_IHEX:    return ".hex";
        case TestSuite::TS_BIN:     return ".bin";
        case TestSuite::TS_SREC:    return ".srec";
        case TestSuite::TS_MEM_2:   return ".mem";
        case TestSuite::TS_MEM_3:   return ".mem";
        case TestSuite::TS_V_16:    return ".v";
        case TestSuite::TS_V_18:    return ".v";
        case TestSuite::TS_VHDL_16: return ".vhd";
        case TestSuite::TS_VHDL_18: return ".vhd";
        case TestSuite::TS__MAX__:  return NULL;
    }
    return NULL;
}

const char * testSuiteID2dir ( TestSuite::TestSuiteID id )
{
    switch ( id )
    {
        case TestSuite::TS_IHEX:    return "hex";
        case TestSuite::TS_BIN:     return "bin";
        case TestSuite::TS_SREC:    return "srec";
        case TestSuite::TS_MEM_2:   return "2.mem";
        case TestSuite::TS_MEM_3:   return "3.mem";
        case TestSuite::TS_V_16:    return "16.v";
        case TestSuite::TS_V_18:    return "18.v";
        case TestSuite::TS_VHDL_16: return "16.vhd";
        case TestSuite::TS_VHDL_18: return "18.vhd";
        case TestSuite::TS__MAX__:  return NULL;
    }
    return NULL;
}

int TestSuite::init()
{
    return 0;
}

int TestSuite::clean()
{
    return 0;
}

void TestSuite::addSuites ( AutoTest * autoTest )
{
    ADD_TEST_SUITE(TS_IHEX);
    ADD_TEST_SUITE(TS_BIN);
    ADD_TEST_SUITE(TS_SREC);
    ADD_TEST_SUITE(TS_MEM_2);
    ADD_TEST_SUITE(TS_MEM_3);
    ADD_TEST_SUITE(TS_V_16);
    ADD_TEST_SUITE(TS_V_18);
    ADD_TEST_SUITE(TS_VHDL_16);
    ADD_TEST_SUITE(TS_VHDL_18);
}

template<TestSuite::TestSuiteID id> bool TestSuite::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("testcases") / testSuiteID2dir(id) );
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

        if ( testSuiteID2ext(id) == extension )
        {
            testCaseFiles.push_back(testName);
        }
    }

    std::sort(testCaseFiles.begin(), testCaseFiles.end());

    for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
          it != testCaseFiles.cend();
          it++ )
    {
        if ( NULL == CU_add_test(suite, it->c_str(), testSuiteID2funcPtr(id)) )
        {
            return false;
        }
    }

    return true;
}

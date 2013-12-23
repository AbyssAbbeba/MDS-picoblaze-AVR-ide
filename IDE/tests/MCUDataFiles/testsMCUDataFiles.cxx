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
#include <CUnit/Basic.h>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

typedef void (*fptr)();

enum TestSuiteID
{
    TS_IHEX = 0,
    TS_BIN,
    TS_SREC,
    TS_MEM_2,
    TS_MEM_3,
    TS_V_16,
    TS_V_18,
    TS_VHDL_16,
    TS_VHDL_18,

    TS__MAX__
};

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
        CU_FAIL_FATAL("DataFileException thrown.");
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

    std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "hex" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<HexFile>( testcase, result + "0", result + "1", true );
}

void testBin()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "bin" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<BinFile>( testcase, result + "0", result + "1", false );
}

void testSrec()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / "srec" / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    testLoadAndSave<SrecFile>( testcase, result + "0", result + "1", true );
}

template <int BYTES_PER_RECORD> void testMem()
{
    using namespace boost::filesystem;

    static const std::string number = ( ( 2 == BYTES_PER_RECORD ) ? "2" : "3" );

    std::string testName = CU_get_current_test()->pName;
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

    std::string testName = CU_get_current_test()->pName;
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

    std::string testName = CU_get_current_test()->pName;
    std::string testcase = system_complete( path("testcases") / ( number + ".vhd" ) / testName ).string();
    std::string result   = system_complete( path("results") / testName ).string();

    XilVHDLFile file0(testName, templateFile, OP_CODE_SIZE);
    XilVHDLFile file1(testName, templateFile, OP_CODE_SIZE);
    XilVHDLFile file2(testName, templateFile, OP_CODE_SIZE);

    testLoadAndSave( file0, file1, file2, testcase, result + number + ".0", result + number + ".1", false );
}

fptr testSuiteID2funcPtr ( int id )
{
    switch ( TestSuiteID(id) )
    {
        case TS_IHEX:    return &testHex;
        case TS_BIN:     return &testBin;
        case TS_SREC:    return &testSrec;
        case TS_MEM_2:   return &testMem<2>;
        case TS_MEM_3:   return &testMem<3>;
        case TS_V_16:    return &testVerilog<XilHDLFile::SIZE_16b>;
        case TS_V_18:    return &testVerilog<XilHDLFile::SIZE_18b>;
        case TS_VHDL_16: return &testVHDL<XilHDLFile::SIZE_16b>;
        case TS_VHDL_18: return &testVHDL<XilHDLFile::SIZE_18b>;
        case TS__MAX__:  return NULL;
    }
    return NULL;
}

const char * testSuiteID2cstring ( int id )
{
    switch ( TestSuiteID(id) )
    {
        case TS_IHEX:    return "Intel® HEX";
        case TS_BIN:     return "raw binary";
        case TS_SREC:    return "Motorola® S-Record";
        case TS_MEM_2:   return "Xilinx® MEM (2B)";
        case TS_MEM_3:   return "Xilinx® MEM (3B)";
        case TS_V_16:    return "Verilog (16b)";
        case TS_V_18:    return "Verilog (18b)";
        case TS_VHDL_16: return "VHDL (18b)";
        case TS_VHDL_18: return "VHDL (16b)";
        case TS__MAX__:  return "<invalid>";
    }
    return NULL;
}

const char * testSuiteID2ext ( int id )
{
    switch ( TestSuiteID(id) )
    {
        case TS_IHEX:    return ".hex";
        case TS_BIN:     return ".bin";
        case TS_SREC:    return ".srec";
        case TS_MEM_2:   return ".mem";
        case TS_MEM_3:   return ".mem";
        case TS_V_16:    return ".v";
        case TS_V_18:    return ".v";
        case TS_VHDL_16: return ".vhd";
        case TS_VHDL_18: return ".vhd";
        case TS__MAX__:  return NULL;
    }
    return NULL;
}

const char * testSuiteID2dir ( int id )
{
    switch ( TestSuiteID(id) )
    {
        case TS_IHEX:    return "hex";
        case TS_BIN:     return "bin";
        case TS_SREC:    return "srec";
        case TS_MEM_2:   return "2.mem";
        case TS_MEM_3:   return "3.mem";
        case TS_V_16:    return "16.v";
        case TS_V_18:    return "18.v";
        case TS_VHDL_16: return "16.vhd";
        case TS_VHDL_18: return "18.vhd";
        case TS__MAX__:  return NULL;
    }
    return NULL;
}

int init()
{
    return 0;
}

int clean()
{
    return 0;
}

bool addTests()
{
    using namespace boost::filesystem;

    for ( int i = 0; i < TS__MAX__; i++ )
    {
        std::vector<std::string> testCaseFiles;

        for ( directory_iterator dir ( path("testcases") / testSuiteID2dir(i) );
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

            if ( testSuiteID2ext(i) == extension )
            {
                testCaseFiles.push_back(testName);
            }
        }

        CU_pSuite testSuite = CU_add_suite ( testSuiteID2cstring(i), &init, &clean );
        if ( NULL == testSuite )
        {
            // Unable to add the test suite to the registry.
            CU_cleanup_registry();
            return false;
        }

        std::sort(testCaseFiles.begin(), testCaseFiles.end());

        for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
              it != testCaseFiles.cend();
              it++ )
        {
            char * testCaseName = new char [ it->size() + 1 ];
            strcpy(testCaseName, it->c_str());

            if ( NULL == CU_add_test(testSuite, testCaseName, testSuiteID2funcPtr(i)) )
            {
                return false;
            }
        }
    }

    return true;
}

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
 * @ingroup testsDisasmPicoBlaze
 * @file TestKcpsm3.cxx
 */
// =============================================================================

#include "TestKcpsm3.h"

// Standard header files.
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Tool for working with Intel® 16 Hex files.
#include "MCUDataFiles/HexFile.h"

// Compiler header files.
#include "compiler/core/Compiler.h"
#include "compiler/core/CompilerMsgIntfStdout.h"

// Disassembler header files.
#include "disassembler/PicoBlaze/DAsmPicoBlazeKcpsm3.h"

int TestKcpsm3::init()
{
    using namespace boost::filesystem;

    m_disassembler = new DAsmPicoBlazeKcpsm3();

    m_msgInt = new CompilerMsgIntfStdout();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_device = "kcpsm3";
    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL  | CompilerOptions::V_ERRORS |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestKcpsm3::clean()
{
    delete m_disassembler;
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestKcpsm3::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestKcpsm3") / "testcases" );
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

void TestKcpsm3::testFunction()
{
    using namespace boost::filesystem;

    std::string testName = CU_get_current_test()->pName;

    /*
     * Compile the original source code, and generate <testcase>.hex file from it.
     */
    m_options->m_sourceFile = system_complete ( path("TestKcpsm3") / "testcases" / (testName + ".asm") ).string();

    std::string resultsPath = system_complete( path("TestKcpsm3") / "results" / testName ).string();
    m_options->m_codeTree   = resultsPath + ".crt";
    m_options->m_hexFile    = resultsPath + ".hex";
    m_options->m_lstFile    = resultsPath + ".lst";

    CU_ASSERT_FATAL ( true == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options) );
    HexFile originalHex(m_options->m_hexFile);

    /*
     * Disassemble code .hex generated from the original code, and generate <testcase>.asm_new file from it.
     */
    m_disassembler->clear();

    CU_ASSERT_FATAL ( true == m_disassembler->disassemble(originalHex) );

    std::ofstream disassembledCode( m_options->m_sourceFile + "_new", std::ios_base::out | std::ios_base::trunc );
    disassembledCode << *m_disassembler;
    disassembledCode.close();

    const std::vector<std::string> & messages = m_disassembler->getMessages();
    for ( std::vector<std::string>::const_iterator it = messages.cbegin();
          it != messages.cend();
          it++ )
    {
        std::cerr << *it << std::endl;
    }

    /*
     * Compile the code generated by disassembler, and generate <testcase>.hex_new file from it.
     */
    m_options->m_sourceFile += "_new";
    m_options->m_codeTree   += "_new";
    m_options->m_hexFile    += "_new";
    m_options->m_lstFile    += "_new";

    CU_ASSERT_FATAL ( true == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options) );
    HexFile generatedHex(m_options->m_hexFile);

    /*
     * Compare the two generated .hex files, i.e. <testcase>.hex and <testcase>.hex_new, they should be the same.
     */
    try
    {
        CU_ASSERT ( originalHex == generatedHex );
    }
    catch ( DataFileException & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL_FATAL("An instance of DataFileException thrown!");
    }
}
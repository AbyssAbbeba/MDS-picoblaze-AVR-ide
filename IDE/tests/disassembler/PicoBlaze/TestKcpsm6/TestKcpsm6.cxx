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
 * @ingroup testsDisasmPicoBlaze
 * @file TestKcpsm6.cxx
 */
// =============================================================================

#include "TestKcpsm6.h"

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
#include "disassembler/PicoBlaze/DAsmPicoBlazeKcpsm6.h"

int TestKcpsm6::init()
{
    using namespace boost::filesystem;

    m_disassembler = new DAsmPicoBlazeKcpsm6();

    m_msgInt = new CompilerMsgIntfStdout();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_device = "kcpsm6";
    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL  | CompilerOptions::V_ERRORS  |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestKcpsm6::clean()
{
    delete m_disassembler;
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestKcpsm6::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("TestKcpsm6") / "testcases" );
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

    for ( std::vector<std::string>::const_iterator it = testCaseFiles.cbegin();
          it != testCaseFiles.cend();
          it++ )
    {
        if ( NULL == CU_add_test(suite, it->c_str(), &testFunction) )
        {
            return false;
        }
    }

    return true;
}

void TestKcpsm6::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    /*
     * Compile the original source code, and generate <testcase>.hex file from it.
     */
    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back( system_complete ( path("TestKcpsm6") / "testcases" / ( testName + ".asm" ) )
                                                        . string() );

    std::string resultsPath = system_complete( path("TestKcpsm6") / "results" / testName ).string();
    m_options->m_codeTree   = resultsPath + ".crt";
    m_options->m_hexFile    = resultsPath + ".hex";
    m_options->m_lstFile    = resultsPath + ".lst";

    if ( false == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options) )
    {
        CU_FAIL("Compilation failed.");
        return;
    }
    HexFile originalHex(m_options->m_hexFile);

    /*
     * Disassemble code .hex generated from the original code, and generate <testcase>.asm_new file from it.
     */
    m_disassembler->clear();

    bool disasmExitCode = m_disassembler->disassemble(originalHex);

    std::ofstream disassembledCode ( (resultsPath + ".asm_new"), std::ios_base::out | std::ios_base::trunc );
    disassembledCode << *m_disassembler;
    disassembledCode.close();

    for ( auto msg : m_disassembler->getMessages() )
    {
        std::cout << msg << std::endl;
    }

    if ( false == disasmExitCode )
    {
        CU_FAIL("Disassembly failed.");
        return;
    }

    /*
     * Compile the code generated by disassembler, and generate <testcase>.hex_new file from it.
     */
    m_options->m_sourceFiles[0] = ( resultsPath + ".asm_new" );
    m_options->m_codeTree += "_new";
    m_options->m_hexFile  += "_new";
    m_options->m_lstFile  += "_new";

    if ( false == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options) )
    {
        CU_FAIL("Recompilation of the disassembled code failed.");
        return;
    }
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
        CU_FAIL("An instance of DataFileException thrown!");
        return;
    }
}

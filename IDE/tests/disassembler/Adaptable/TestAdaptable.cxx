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
 * @ingroup testsDisasmAdaptable
 * @file TestAdaptable.cxx
 */
// =============================================================================

#include "TestAdaptable.h"

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

// Tool for working with Intel® 16 Hex files.
#include "MCUDataFiles/HexFile.h"

// Compiler header files.
#include "compiler/Compiler.h"
#include "compiler/core/CompilerMsgIntfFile.h"

// Disassembler header files.
#include "disassembler/Adaptable/DAsmAdaptable.h"

// Library for working with processor definition files used by Adaptable Simulator.
#include "AdjSimProcDef/AdjSimProcDefParser.h"

int TestAdaptable::init()
{
    using namespace boost::filesystem;

    m_disassembler = nullptr;

    m_msgInt = new CompilerMsgIntfFile();
    m_options = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );

    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL  | CompilerOptions::V_ERRORS  |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestAdaptable::clean()
{
    if ( nullptr != m_disassembler )
    {
        delete m_disassembler;
    }
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    return 0;
}

bool TestAdaptable::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir("testcases");
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

void TestAdaptable::testFunction()
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( "results" );

    // Setup disassembler.
    if ( nullptr != m_disassembler )
    {
        delete m_disassembler;
    }
    std::string devSpecFile = system_complete(path("testcases") / ( testName + ".procdef" )).string();
    AdjSimProcDefParser * parser = new AdjSimProcDefParser(devSpecFile);
    if ( false == parser->isValid() )
    {
        CU_FAIL("Unable to parse processor definition file.");
        delete parser;
        return;
    }
    m_disassembler = new DAsmAdaptable(parser->data());
    delete parser;

    /*
     * Compile the original source code, and generate <testcase>.hex file from it.
     */
    m_options->m_device = devSpecFile;
    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back( system_complete ( path("testcases") / ( testName + ".asm" ) )
                                                        . string() );

    std::string resultsPath = system_complete( path("results") / testName ).string();
    m_options->m_codeTree   = resultsPath + ".crt";
    m_options->m_hexFile    = resultsPath + ".hex";
    m_options->m_lstFile    = resultsPath + ".lst";

    const std::string errFile = ( resultsPath + ".err" );
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);
    bool result = m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options);

    if ( false == result )
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

    const std::vector<std::string> & messages = m_disassembler->getMessages();
    for ( const auto & msg : messages )
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

    if ( false == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_ADAPTABLE, m_options) )
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
    catch ( const DataFileException & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL("An instance of DataFileException thrown!");
        return;
    }
}
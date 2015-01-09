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
#include <memory>
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

    {
        std::ifstream file ( devSpecFile, (std::ios_base::in | std::ios_base::binary) );
        if ( false == file.is_open() )
        {
            CU_FAIL("Unable to open processor definition file: " + devSpecFile);
            return;
        }
        static const long long int MAX_SIZE = 102400;
        std::unique_ptr<char[]> data ( new char [ MAX_SIZE ] );
        size_t len = (size_t) file.readsome (data.get(), MAX_SIZE);
        if ( true == file.bad() )
        {
            CU_FAIL("Unable to read processor definition file: " + devSpecFile);
            return;
        }

        std::unique_ptr<AdjSimProcDefParser> parser (new AdjSimProcDefParser(std::string(data.get(), len)));
        if ( false == parser.get()->isValid() )
        {
            CU_FAIL("Unable to parse processor definition file: " + devSpecFile);
            return;
        }
        m_disassembler = new DAsmAdaptable(parser.get()->data());
    }

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
    bool result = m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_ADAPTABLE, m_options);

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

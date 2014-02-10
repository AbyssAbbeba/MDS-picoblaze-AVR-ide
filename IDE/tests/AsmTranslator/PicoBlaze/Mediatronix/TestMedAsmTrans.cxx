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
 * @ingroup testsAsmTranslator
 * @file TestMedAsmTrans.cxx
 */
// =============================================================================

#include "TestMedAsmTrans.h"

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
#include "Compiler.h"
#include "core/CompilerMsgIntfFile.h"

// AsmTranslator header files.
#include "AsmTranslator.h"

bool TestMedAsmTrans::fileCompare ( const std::string & fileName1,
                                    const std::string & fileName2 )
{
    std::ifstream file1(fileName1, std::ios_base::binary);
    std::ifstream file2(fileName2, std::ios_base::binary);

    if ( false == file1.is_open() || false == file2.is_open() )
    {
        return false;
    }

    std::string line;
    while ( false == file1.eof() && false == file2.eof() )
    {
        if ( true == file1.bad() || true == file2.bad() )
        {
            return false;
        }

        line = file1.get();
        if ( std::string::npos == line.find("INIT") )
        {
            continue;
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

int TestMedAsmTrans::init()
{
    using namespace boost::filesystem;

    m_msgInt   = new CompilerMsgIntfFile();
    m_options  = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );
    m_translator = new AsmTranslator();

    m_translator->m_config.m_letterCase[AsmTranslatorConfig::F_INSTRUCTION] = AsmTranslatorConfig::LC_UPPERCASE;
    m_translator->m_config.m_letterCase[AsmTranslatorConfig::F_DIRECTIVE]   = AsmTranslatorConfig::LC_UPPERCASE;

    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL  | CompilerOptions::V_ERRORS |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestMedAsmTrans::clean()
{
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    delete m_translator;
    return 0;
}

bool TestMedAsmTrans::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("Mediatronix") / "testcases" );
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

        if ( ".psm" == extension )
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

void TestMedAsmTrans::testFunction()
{
    AsmTranslatorConfig::Indentation indentation[3] =
    {
        AsmTranslatorConfig::IND_KEEP,
        AsmTranslatorConfig::IND_TABS,
        AsmTranslatorConfig::IND_SPACES
    };
    std::string suffixes[3] = { ".ind_keep", ".ind_tabs", ".ind_spaces" };

    for ( int i = 0; i < 3; i++ )
    {
        m_translator->m_config.m_indentation = indentation[i];

        m_translator->m_config.m_shortInstructions = false;
        test ( suffixes[i] + ".full_inst" );

        m_translator->m_config.m_shortInstructions = true;
        test ( suffixes[i] + ".short_inst" );
    }
}

void TestMedAsmTrans::test ( const std::string & suffix )
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("Mediatronix") / "results" );

    m_translator->clear();
    bool result = m_translator->translate ( AsmTranslator::V_KCPSM_MEDIATRONIX,
                                            ( path("Mediatronix") / "results"   / (testName + suffix+".asm") ).string(),
                                            ( path("Mediatronix") / "testcases" / (testName + ".psm") ).string(),
                                            true );

    {
        std::ofstream logFile( ( path("Mediatronix") / "results" / (testName + suffix + ".log") ).string() );
        for ( const auto & i : m_translator->getMessages() )
        {
            if ( 0 == i.first )
            {
                logFile << (testName + ".psm") << ": " << i.second << std::endl;
            }
            else
            {
                logFile << (testName + ".psm") << ":" << i.first << ": " << i.second << std::endl;
            }
        }
    }

    if ( false == result )
    {
        CU_FAIL("Translation failed!");
        return;
    }

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back ( ( path("Mediatronix") / "results" / (testName + suffix + ".asm") ).string() );
    m_options->m_vhdlFile   = testName + suffix + ".vhd";
    m_options->m_lstFile    = testName + suffix + ".lst";

    switch ( testName.front() )
    {
        case '0':
            m_options->m_device = "kcpsm1cpld";
            break;
        case '1':
            m_options->m_device = "kcpsm1";
            break;
        case '2':
            m_options->m_device = "kcpsm2";
            break;
        case '3':
            m_options->m_device = "kcpsm3";
            break;
        case '6':
            m_options->m_device = "kcpsm6";
            break;
        default:
            CU_FAIL("Environment setup error: unknown device.");
            return;
    }

    m_options->m_verilogTemplate = (current_path() / (m_options->m_device + ".vhd")).string();
    m_options->m_vhdlTemplate    = (current_path() / (m_options->m_device + ".v"  )).string();

    const std::string errFile = (path("Mediatronix") / "results" / (testName + suffix + ".err")).string();
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);
    result = m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options);


    if ( false == result )
    {
        CU_FAIL("Compilation failed!");
        return;
    }

    std::string expectedCommonPath = system_complete( path("Mediatronix") / "expected" / testName ).string();

    if ( false == fileCompare ( ( expectedCommonPath + ".vhd.exp" ), m_options->m_vhdlFile ) )
    {
        CU_FAIL("VHDL output differs from the expected one.");
    }
}

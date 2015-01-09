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
 * @file TestXilAsmTrans.cxx
 */
// =============================================================================

#include "TestXilAsmTrans.h"

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

void TestXilAsmTrans::fileCompare ( const std::string & fileName1,
                                    const std::string & fileName2 )
{
    std::ifstream file1(fileName1, std::ios_base::binary);
    std::ifstream file2(fileName2, std::ios_base::binary);

    if ( false == file1.is_open() || false == file2.is_open() )
    {
        CU_FAIL("Unable to open VHD file!");
        return;
    }

    bool comparisonMade = false;
    int lineNumber = 0;
    std::string line1, line2;
    while ( false == file1.eof() && false == file2.eof() )
    {
        if ( true == file1.bad() || true == file2.bad() )
        {
            CU_FAIL("Unable to read VHD file!");
            return;
        }

        do
        {
            lineNumber++;
            std::getline(file1, line1);
            if ( ( line1.size() > 0 ) && ( '\r' == line1.back() ) )
            {
                line1.pop_back();
            }
        }
        while ( ( std::string::npos == line1.find("INIT") ) && ( false == file1.eof() ) );

        do
        {
            std::getline(file2, line2);
            if ( ( line2.size() > 0 ) && ( '\r' == line2.back() ) )
            {
                line2.pop_back();
            }
        }
        while ( ( std::string::npos == line2.find("INIT") ) && ( false == file2.eof() ) );

        size_t pos;
        std::string * line = &line1;
        for ( int i = 0; i < 2; i++ )
        {
            pos = line->find("X\"");
            if ( std::string::npos != pos )
            {
                line->assign(line->substr(pos + 2));
                pos = line->find('"');
                if ( std::string::npos != pos )
                {
                    line->assign(line->substr(0, pos));
                }
            }
            line = &line2;
        }

        if ( ( true == line1.empty() ) || ( true == line2.empty() ) )
        {
            continue;
        }

        comparisonMade = true;

        if ( line1 != line2 )
        {
            CU_FAIL_MSG ( fileName1,
                          lineNumber + 1,
                          "\nexpected: '" + line1 + "'"
                          "\n" "actual:   '" + line2 + "'" );
            return;
        }
    }

    if ( false == comparisonMade )
    {
        CU_FAIL("No VHD comparison made!");
    }
}

int TestXilAsmTrans::init()
{
    using namespace boost::filesystem;

    m_translator = new AsmTranslator();

    m_msgInt   = new CompilerMsgIntfFile();
    m_options  = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );

    m_translator->m_config.m_letterCase[AsmTranslatorConfig::F_INSTRUCTION] = AsmTranslatorConfig::LC_UPPERCASE;
    m_translator->m_config.m_letterCase[AsmTranslatorConfig::F_DIRECTIVE]   = AsmTranslatorConfig::LC_UPPERCASE;

    m_options->m_verbosity = CompilerOptions::Verbosity ( CompilerOptions::V_GENERAL  | CompilerOptions::V_ERRORS |
                                                          CompilerOptions::V_WARNINGS | CompilerOptions::V_REMARKS );

    return 0;
}

int TestXilAsmTrans::clean()
{
    delete m_compiler;
    delete m_options;
    delete m_msgInt;
    delete m_translator;
    return 0;
}

bool TestXilAsmTrans::addTests ( CU_pSuite suite )
{
    using namespace boost::filesystem;

    std::vector<std::string> testCaseFiles;

    for ( directory_iterator dir ( path("Xilinx") / "testcases" );
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

void TestXilAsmTrans::testFunction()
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

void TestXilAsmTrans::translate ( const std::string & testName,
                                  const std::string & suffix,
                                  bool include )
{
    using namespace boost::filesystem;

    path inFile = path("Xilinx") / "testcases";
    if ( true == include )
    {
        inFile /= "include";
    }

    inFile /= ( testName + ".psm" );

    m_translator->clear();
    bool result = m_translator->translate ( AsmTranslator::V_KCPSM_XILINX,
                                            ( path("Xilinx") / "results"   / (testName + suffix + ".asm") ).string(),
                                            inFile.string(),
                                            true );

    std::string errorLog;
    std::ofstream logFile( ( path("Xilinx") / "results" / (testName + suffix + ".log") ).string() );
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

        errorLog += i.second;
        errorLog += '\n';
    }

    if ( false == result )
    {
        CU_FAIL("Translation failed for: " + inFile.string() + '\n' + errorLog);
        return;
    }

    const std::vector<std::string> includedFiles = m_translator->getIncludedFiles();
    for ( const auto & file : includedFiles )
    {
        translate(file.substr(0, file.size() - 4), "", true);
    }
}

void TestXilAsmTrans::test ( const std::string & suffix )
{
    using namespace boost::filesystem;

    const std::string testName = CU_get_current_test()->pName;

    create_directory ( path("Xilinx") / "results" );

    translate(testName, suffix);

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back ( ( path("Xilinx") / "results" / (testName + suffix + ".asm") ).string() );
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

    const path vhdTemplate = system_complete( path("Xilinx") / "testcases" / (testName + ".vhd") );
    if ( true == is_regular_file(vhdTemplate) )
    {
        m_options->m_vhdlTemplate = vhdTemplate.string();
    }
    else
    {
        m_options->m_vhdlTemplate = (current_path() / (m_options->m_device + ".vhd")).string();
    }

    const std::string errFile = (path("Xilinx") / "results" / (testName + suffix + ".err")).string();
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);

    if ( false == m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options) )
    {
        std::string errorLog;
        std::ifstream errLogFile(errFile);
        while ( ( false == errLogFile.eof() ) && ( false == errLogFile.bad() ) )
        {
            std::string line;
            std::getline(errLogFile, line);
            size_t pos = line.find("Error: ");
            if ( std::string::npos != pos )
            {
                line = line.substr(pos);
                errorLog += line;
                errorLog += '\n';
            }
        }
        CU_FAIL("Compilation failed:\n" + errorLog);
        return;
    }

    std::string expectedCommonPath = system_complete( path("Xilinx") / "expected" / testName ).string();

    fileCompare ( ( expectedCommonPath + ".vhd.exp" ), m_options->m_vhdlFile );
}

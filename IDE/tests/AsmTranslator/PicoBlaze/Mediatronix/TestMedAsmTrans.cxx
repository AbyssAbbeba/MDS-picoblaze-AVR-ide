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

// MCUDataFiles
#include "XilVHDLFile.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// Compiler header files.
#include "core/Compiler.h"
#include "core/CompilerMsgIntfFile.h"

// AsmTranslator header files.
#include "AsmTranslator.h"

int TestMedAsmTrans::init()
{
    using namespace boost::filesystem;

    m_msgInt   = new CompilerMsgIntfFile();
    m_options  = new CompilerOptions();
    m_compiler = new Compiler ( m_msgInt,
                                system_complete( path("..") / ".." / ".." / "compiler" / "include" ).string() );
    m_translator = new AsmTranslator();

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

void TestMedAsmTrans::testFunction()
{
    using namespace boost::filesystem;

    XilHDLFile::OPCodeSize opCodeSize;
    const std::string testName = CU_get_current_test()->pName;

    m_translator->clear();
    bool result = m_translator->translate ( AsmTranslator::V_KCPSM_MEDIATRONIX,
                                            ( path("Mediatronix") / "results"   / (testName + ".asm") ).string(),
                                            ( path("Mediatronix") / "testcases" / (testName + ".psm") ).string(),
                                            true );

    {
        std::ofstream logFile( ( path("Mediatronix") / "results" / (testName + ".log") ).string() );
        for ( auto line : m_translator->getMessages() )
        {
            logFile << line << std::endl;
        }
    }

    if ( false == result )
    {
        CU_FAIL("Translation failed!");
        return;
    }

    m_options->m_sourceFiles.clear();
    m_options->m_sourceFiles.push_back ( ( path("Mediatronix") / "results" / (testName + ".asm") ).string() );
    m_options->m_vhdlFile   = testName + ".vhd";
    m_options->m_lstFile    = testName + ".lst";

    switch ( testName.front() )
    {
        case '0':
            m_options->m_device = "kcpsm1cpld";
            opCodeSize = XilHDLFile::SIZE_16b;
            break;
        case '1':
            m_options->m_device = "kcpsm1";
            opCodeSize = XilHDLFile::SIZE_16b;
            break;
        case '2':
            m_options->m_device = "kcpsm2";
            opCodeSize = XilHDLFile::SIZE_18b;
            break;
        case '3':
            m_options->m_device = "kcpsm3";
            opCodeSize = XilHDLFile::SIZE_18b;
            break;
        case '6':
            m_options->m_device = "kcpsm6";
            opCodeSize = XilHDLFile::SIZE_18b;
            break;
        default:
            CU_FAIL("Environment setup error: unknown device.");
            return;
    }

    const std::string errFile = (path("Mediatronix") / "results" / (testName + ".err")).string();
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->openFile(errFile);
    result = m_compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, m_options);
    dynamic_cast<CompilerMsgIntfFile*>(m_msgInt)->closeFile();

    if ( false == result )
    {
        CU_FAIL("Compilation failed!");
        return;
    }

    std::string expectedCommonPath = system_complete( path("Mediatronix") / "expected" / testName ).string();

    try
    {
        XilVHDLFile expectedVHDL ( (expectedCommonPath + ".vhd.exp"), "", "", opCodeSize );
        XilVHDLFile actualVHDL   ( m_options->m_vhdlFile,   "", "", opCodeSize );
        CU_ASSERT ( expectedVHDL == actualVHDL );
    }
    catch ( const DataFileException & e )
    {
        std::cerr << std::endl << e.toString() << std::endl;
        CU_FAIL("An instance of DataFileException thrown!");
    }
}

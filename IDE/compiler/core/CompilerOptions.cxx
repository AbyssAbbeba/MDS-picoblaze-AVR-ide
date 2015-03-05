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
 * @ingroup Compiler
 * @file CompilerOptions.cxx
 */
// =============================================================================

#include "CompilerOptions.h"

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// OS compatibility.
#include "../../utilities/os/os.h"

// Standard headers.
#include <fstream>
#include <cstdio>

CompilerOptions::ProcessorLimits::ProcessorLimits()
{
    m_iCodeMemSize  = -1;
    m_xCodeMemSize  = -1;
    m_iDataMemSize  = -1;
    m_eDataMemSize  = -1;
    m_xDataMemSize  = -1;
    m_regFileSize   = -1;
    m_nvDataMemSize = -1;
}

CompilerOptions::CompilerOptions()
{
    m_maxMacroExp     = 1024;
    m_maxInclusion    = 1024;
    m_hexMaxRecLength = 16;
    m_messageLimit    = 1024;

    m_syntaxCheckOnly = false;
    m_makeBackupFiles = true;
    m_briefMsgOutput  = false;
    m_strict = false;

    m_enableDigraphs = true;
    m_enableTrigraphs = true;
    m_enableNamedOperators = true;

    m_optimization = O_BASIC;

    m_verbosity = (Verbosity)( V_GENERAL | V_ERRORS | V_WARNINGS | V_REMARKS );
}

void CompilerOptions::clearOutputFiles()
{
    const std::string * const files[] =
    {
        &m_symbolTable, &m_macroTable,  &m_mdsDebugFile,
        &m_codeTree,    &m_lstFile,     &m_hexFile,
        &m_binFile,     &m_srecFile,    &m_verilogFile,
        &m_vhdlFile,    &m_prcTarget,   &m_memFile,
        &m_cunit,       &m_second,      &m_rawHexDumpFile,
        &m_stringTable,
        nullptr
    };

    for ( int i = 0; nullptr != files[i]; i++ )
    {
        if ( true == files[i]->empty() )
        {
            continue;
        }

        if ( true == m_makeBackupFiles )
        {
            rename(files[i]->c_str(), (*files[i] + '~').c_str());
        }

        std::ofstream file(*(files[i]), std::fstream::trunc );
    }
}

void CompilerOptions::normalizeFilePaths()
{
    using namespace boost::filesystem;

    std::string * const files[] =
    {
        &m_symbolTable, &m_macroTable,  &m_mdsDebugFile,
        &m_codeTree,    &m_lstFile,     &m_vhdlTemplate,
        &m_binFile,     &m_srecFile,    &m_verilogFile,
        &m_vhdlFile,    &m_prcTarget,   &m_verilogTemplate,
        &m_memFile,     &m_hexFile,     &m_rawHexDumpFile,
        &m_second,      &m_cunit,       &m_stringTable,
        nullptr
    };

    m_sourceFiles[0] = canonical(path(makeHomeSafe(m_sourceFiles[0])).make_preferred()).string();
    const path basePath = canonical(path(makeHomeSafe(m_sourceFiles[0])).parent_path().make_preferred());

    for ( std::string & srcFile : m_sourceFiles )
    {
        srcFile = canonical(path(makeHomeSafe(srcFile)).make_preferred(), basePath).string();
    }

    for ( int i = 0; nullptr != files[i]; i++ )
    {
        if ( true == files[i]->empty() )
        {
            continue;
        }

        *files[i] = absolute(path(makeHomeSafe(*files[i])).make_preferred(), basePath).string();
    }
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions::ProcessorLimits & limits )
{
    out << "== CompilerOptions::ProcessorLimits ==" << std::endl;
    out << "  m_iCodeMemSize = "  << limits.m_iCodeMemSize  << std::endl;
    out << "  m_xCodeMemSize = "  << limits.m_xCodeMemSize  << std::endl;
    out << "  m_iDataMemSize = "  << limits.m_iDataMemSize  << std::endl;
    out << "  m_eDataMemSize = "  << limits.m_eDataMemSize  << std::endl;
    out << "  m_xDataMemSize = "  << limits.m_xDataMemSize  << std::endl;
    out << "  m_regFileSize = "   << limits.m_regFileSize   << std::endl;
    out << "  m_nvDataMemSize = " << limits.m_nvDataMemSize << std::endl;

    out << "  m_disabledInstructions: ";
    if ( true == limits.m_disabledInstructions.empty() )
    {
        out << "    (none)" << std::endl;
    }
    else
    {
        out << std::endl;
        int idx = 0;
        for ( const auto instruction : limits.m_disabledInstructions )
        {
            out << "    [" << idx++ << "] = " << (int)instruction << std::endl;
        }
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions & opts )
{
    out << std::boolalpha;

    out << "== CompilerOptions ==" << std::endl;

    out << "  === File names ==="       << std::endl;
    out << "    m_symbolTable = \""     << opts.m_symbolTable     << '"' << std::endl;
    out << "    m_macroTable = \""      << opts.m_macroTable      << '"' << std::endl;
    out << "    m_stringTable = \""     << opts.m_stringTable     << '"' << std::endl;
    out << "    m_mdsDebugFile = \""    << opts.m_mdsDebugFile    << '"' << std::endl;
    out << "    m_codeTree = \""        << opts.m_codeTree        << '"' << std::endl;
    out << "    m_lstFile = \""         << opts.m_lstFile         << '"' << std::endl;
    out << "    m_hexFile = \""         << opts.m_hexFile         << '"' << std::endl;
    out << "    m_binFile = \""         << opts.m_binFile         << '"' << std::endl;
    out << "    m_srecFile = \""        << opts.m_srecFile        << '"' << std::endl;
    out << "    m_verilogFile = \""     << opts.m_verilogFile     << '"' << std::endl;
    out << "    m_vhdlFile = \""        << opts.m_vhdlFile        << '"' << std::endl;
    out << "    m_memFile = \""         << opts.m_memFile         << '"' << std::endl;
    out << "    m_verilogTemplate = \"" << opts.m_verilogTemplate << '"' << std::endl;
    out << "    m_vhdlTemplate = \""    << opts.m_vhdlTemplate    << '"' << std::endl;
    out << "    m_prcTarget = \""       << opts.m_prcTarget       << '"' << std::endl;
    out << "    m_cunit = \""           << opts.m_cunit           << '"' << std::endl;
    out << "    m_second = \""          << opts.m_second          << '"' << std::endl;
    out << "    m_sourceFiles:\""       << std::endl;
    for ( const auto & file : opts.m_sourceFiles )
    {
        out << "      - \"" << file << '"' << std::endl;
    }

    out << "  === Other compilation and code generation options ===" << std::endl;
    out << "    m_maxMacroExp = "       << opts.m_maxMacroExp << std::endl;
    out << "    m_maxInclusion = "      << opts.m_maxInclusion << std::endl;
    out << "    m_hexMaxRecLength = "   << opts.m_hexMaxRecLength << std::endl;
    out << "    m_syntaxCheckOnly = "   << opts.m_syntaxCheckOnly << std::endl;
    out << "    m_makeBackupFiles = "   << opts.m_makeBackupFiles << std::endl;
    out << "    m_device = " << opts.m_device << std::endl;
    out << "    m_includePath: " << std::endl;
    for ( const auto & path : opts.m_includePath )
    {
        out << "      - \"" << path << '"' << std::endl;
    }

    out << opts.m_processorlimits;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions * opts )
{
    out << *opts;
    return out;
}

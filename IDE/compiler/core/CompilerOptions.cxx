// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file CompilerOptions.cxx
 */
// =============================================================================

#include "CompilerOptions.h"

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
    m_maxMacroExp = -1;
    m_maxInclusion = -1;
    m_hexMaxRecLength = 255;
    m_syntaxCheckOnly = false;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions::ProcessorLimits & limits )
{
    out << "== CompilerOptions::ProcessorLimits ==" << std::endl;
    out << "  m_iCodeMemSize = " << limits.m_iCodeMemSize << std::endl;
    out << "  m_xCodeMemSize = " << limits.m_xCodeMemSize << std::endl;
    out << "  m_iDataMemSize = " << limits.m_iDataMemSize << std::endl;
    out << "  m_eDataMemSize = " << limits.m_eDataMemSize << std::endl;
    out << "  m_xDataMemSize = " << limits.m_xDataMemSize << std::endl;
    out << "  m_regFileSize = " << limits.m_regFileSize << std::endl;
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
        for ( std::vector<char>::const_iterator it = limits.m_disabledInstructions.cbegin();
            it != limits.m_disabledInstructions.cend();
            it++ )
        {
            out << "    [" << idx++ << "] = " << (int)*it << std::endl;
        }
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions & opts )
{
    out << "== CompilerOptions ==" << std::endl;

    out << "  === File names ===" << std::endl;
    out << "    m_sourceFile = \"" << opts.m_sourceFile << "\"" << std::endl;
    out << "    m_symbolTable = \"" << opts.m_symbolTable << "\"" << std::endl;
    out << "    m_macroTable = \"" << opts.m_macroTable << "\"" << std::endl;
    out << "    m_mdsDebugFile = \"" << opts.m_mdsDebugFile << "\"" << std::endl;
    out << "    m_codeTree = \"" << opts.m_codeTree << "\"" << std::endl;
    out << "    m_lstFile = \"" << opts.m_lstFile << "\"" << std::endl;
    out << "    m_hexFile = \"" << opts.m_hexFile << "\"" << std::endl;
    out << "    m_binFile = \"" << opts.m_binFile << "\"" << std::endl;
    out << "    m_srecFile = \"" << opts.m_srecFile << "\"" << std::endl;

    out << "  === Other compilation and code generation options ===" << std::endl;
    out << "    m_maxMacroExp = " << opts.m_maxMacroExp << std::endl;
    out << "    m_maxInclusion = " << opts.m_maxInclusion << std::endl;
    out << "    m_hexMaxRecLength = " << opts.m_hexMaxRecLength << std::endl;
    out << "    m_syntaxCheckOnly = " << ( true == opts.m_syntaxCheckOnly ? "true" : "false" ) << std::endl;
    out << "    m_makeBackupFiles = " << ( true == opts.m_makeBackupFiles ? "true" : "false" ) << std::endl;
    out << opts.m_processorlimits;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerOptions * opts )
{
    out << *opts;
    return out;
}

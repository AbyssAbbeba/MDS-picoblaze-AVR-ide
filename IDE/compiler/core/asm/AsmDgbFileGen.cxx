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
 * @ingroup Assembler
 * @file AsmDgbFileGen.cxx
 */
// =============================================================================

#include "AsmDgbFileGen.h"

// Standard headers.
#include <fstream>
#include <cstdint>

/**
 * @brief
 * @param[in] arg
 */
#define BREAK_TO_4_CHARS(arg) \
    ( (char) ( ( (uint32_t)(arg) & 0xff000000 ) >> 24 ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x00ff0000 ) >> 16 ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x0000ff00 ) >> 8  ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x000000ff ) ) )

AsmDgbFileGen::AsmDgbFileGen()
{
}

AsmDgbFileGen::~AsmDgbFileGen()
{
}

void AsmDgbFileGen::setCode ( const CompilerBase::SourceLocation & location,
                              int code,
                              int address )
{
    m_data.push_back(DbgRecord(location, code, address));
}

void AsmDgbFileGen::output ( CompilerSemanticInterface * compilerCore,
                             const CompilerOptions * opts )
{
    if ( opts->m_mdsDebugFile.empty() )
    {
        return;
    }

    if ( true == opts->m_makeBackupFiles )
    {
        rename(opts->m_mdsDebugFile.c_str(), (opts->m_mdsDebugFile + "~").c_str());
    }

    std::ofstream file ( opts->m_mdsDebugFile, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to open ").toStdString() + "\"" + opts->m_mdsDebugFile  + "\"" );
        return;
    }

    file << FileHeaders::AsmNativeDgbFile << "\n";
    const std::vector<std::string> & sourceFiles = compilerCore->listSourceFiles();
    for ( std::vector<std::string>::const_iterator it = sourceFiles.cbegin();
          it != sourceFiles.cend();
          it++ )
    {
        file << *it << "\n";
    }
    file << "\n";
    for ( std::vector<DbgRecord>::const_iterator i = m_data.cbegin();
          i != m_data.cend();
          i ++ )
    {
        if ( (size_t)(i->m_location.m_fileNumber) < sourceFiles.size() )
        {
            file << BREAK_TO_4_CHARS(i->m_address)
                 << BREAK_TO_4_CHARS(i->m_location.m_fileNumber)
                 << BREAK_TO_4_CHARS(i->m_location.m_lineStart);
        }
    }

    if ( true == file.bad() )
    {
        compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to write to ").toStdString() + "\"" + opts->m_mdsDebugFile  + "\"" );
        return;
    }
}

void AsmDgbFileGen::clear()
{
    m_data.clear();
}

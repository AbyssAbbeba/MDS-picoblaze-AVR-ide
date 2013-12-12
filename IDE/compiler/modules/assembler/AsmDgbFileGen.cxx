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
 * @ingroup Assembler
 * @file AsmDgbFileGen.cxx
 */
// =============================================================================

#include "AsmDgbFileGen.h"

// Support for debug files.
#include "DbgFileNative.h"

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

void AsmDgbFileGen::setCode ( const CompilerSourceLocation & location,
                              int code,
                              int address )
{
    m_data.push_back(DbgRecord(location, code, address));
}

inline void AsmDgbFileGen::outputToFile ( CompilerSemanticInterface * compilerCore,
                                          const std::string & filename )
{
    std::ofstream file ( filename, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                          QObject::tr("unable to open ").toStdString() + "\"" + filename  + "\"" );
        return;
    }

    file << CompilerFileHeaders::AsmNativeDgbFile << "\n";
    const std::vector<std::pair<std::string,FILE*>> & sourceFiles = compilerCore->listSourceFiles();
    for ( std::vector<std::pair<std::string,FILE*>>::const_iterator it = sourceFiles.cbegin();
          it != sourceFiles.cend();
          it++ )
    {
        file << it->first << "\n";
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
                                          QObject::tr("unable to write to ").toStdString() + "\"" + filename + "\"" );
        return;
    }
}

inline void AsmDgbFileGen::outputToContainer ( CompilerSemanticInterface * compilerCore,
                                               DbgFileNative * target )
{
    size_t numberOfFiles = compilerCore->listSourceFiles().size();

    target->directSetupPrepare();
    target->directSetupFiles ( compilerCore->listSourceFiles() );

    for ( std::vector<DbgRecord>::const_iterator i = m_data.cbegin();
          i != m_data.cend();
          i ++ )
    {
        if ( (size_t)(i->m_location.m_fileNumber) < numberOfFiles )
        {
            target->directSetupRelation ( (unsigned int) i->m_address,
                                          (unsigned int) i->m_location.m_fileNumber,
                                          (unsigned int) i->m_location.m_lineStart );
        }
    }

    target->directSetupFinalize();
}

void AsmDgbFileGen::output ( CompilerSemanticInterface * compilerCore,
                             const CompilerOptions * opts )
{
    if ( true == compilerCore->m_simulatorData.m_genSimData )
    {
        DbgFileNative * dbgFile = new DbgFileNative();
        outputToContainer(compilerCore, dbgFile);
        compilerCore->m_simulatorData.m_simDbg = dbgFile;
    }

    if ( false == opts->m_mdsDebugFile.empty() )
    {
        outputToFile(compilerCore, opts->m_mdsDebugFile);
    }
}

void AsmDgbFileGen::clear()
{
    m_data.clear();
}

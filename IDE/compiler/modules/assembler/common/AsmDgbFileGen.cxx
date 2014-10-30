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
#include "DbgFile/DbgFileNative.h"

#include "os/os.h"

// Common compiler header files.
#include "CompilerLocationTracker.h"

// Standard headers.
#include <fstream>
#include <cstdint>
#include <utility>

/**
 * @brief
 * @param[in] arg
 */
#define BREAK_TO_4_CHARS(arg)                                \
    ( (char) ( ( (uint32_t)(arg) & 0xff000000 ) >> 24 ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x00ff0000 ) >> 16 ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x0000ff00 ) >> 8  ) ) << \
    ( (char) ( ( (uint32_t)(arg) & 0x000000ff )       ) )

/**
 * @brief
 * @param[in] arg
 */
#define BREAK_TO_2_CHARS(arg)                            \
    ( (char) ( ( (uint16_t)(arg) & 0xff00 ) >> 8  ) ) << \
    ( (char) ( ( (uint16_t)(arg) & 0x00ff )       ) )

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
    std::ofstream file ( filename, ( std::fstream::out | std::fstream::trunc | std::fstream::binary ) );

    if ( false == file.is_open() )
    {
        compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("unable to open: ").toStdString() + "`" + filename  + "'" );
        return;
    }

    file << CompilerFileHeaders::AsmNativeDgbFile << '\n';

    const std::vector<std::pair<std::string,FILE*>> & sourceFiles = compilerCore->listSourceFiles();
    for ( const auto & srcFile : sourceFiles )
    {
        file << boost::filesystem::make_relative(compilerCore->getBasePath(), srcFile.first).string() << '\n';
    }
    file << '\n';

    bool first = true;
    for ( const auto & dbgRecord : m_data )
    {
        std::vector<CompilerSourceLocation> locationTrace;
        compilerCore->locationTrack().traverse ( dbgRecord.m_location, &locationTrace );

        for ( const auto & location : locationTrace )
        {
            if ( location.m_fileNumber < (int) sourceFiles.size() )
            {
                if ( true == first )
                {
                    first = false;
                    file << BREAK_TO_4_CHARS(dbgRecord.m_address);
                }
                file << '\1'
                     << BREAK_TO_2_CHARS(location.m_fileNumber)
                     << BREAK_TO_4_CHARS(location.m_lineStart);
            }
        }
        if ( false == first )
        {
            file << '\0';
            first = true;
        }
    }

    if ( true == file.bad() )
    {
        compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("unable to write to: ").toStdString() + "`" + filename + "'" );
        return;
    }
}

inline void AsmDgbFileGen::outputToContainer ( CompilerSemanticInterface * compilerCore,
                                               DbgFileNative * target )
{
    int numberOfFiles = (int) compilerCore->listSourceFiles().size();

    target->directSetupPrepare();
    target->directSetupFiles ( compilerCore->listSourceFiles() );

    for ( const auto & dbgRecord : m_data )
    {
        std::vector<CompilerSourceLocation> locationTrace;
        std::vector<std::pair<unsigned int, unsigned int>> locationTrace2;

        compilerCore->locationTrack().traverse ( dbgRecord.m_location, &locationTrace );

        for ( const auto & location : locationTrace )
        {
            if ( location.m_fileNumber < numberOfFiles )
            {
                locationTrace2.push_back ( { (unsigned int) location.m_fileNumber,
                                             (unsigned int) location.m_lineStart } );
            }
        }

        if ( false == locationTrace2.empty() )
        {
            target->directSetupRelation ( (unsigned int) dbgRecord.m_address, locationTrace2 );
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

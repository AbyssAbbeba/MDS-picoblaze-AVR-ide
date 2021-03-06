// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014, 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Assembler
 * @file AsmMachineCodeGen.cxx
 */
// =============================================================================

#include "AsmMachineCodeGen.h"

// Standard headers.
#include <iostream>

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerSemanticInterface.h"

// Header files of libMCUDataFiles.
#include "MCUDataFiles/BinFile.h"
#include "MCUDataFiles/HexFile.h"
#include "MCUDataFiles/DataFile.h"
#include "MCUDataFiles/SrecFile.h"

AsmMachineCodeGen::AsmMachineCodeGen ()
{
    clear();
    reserve ( INITIAL_MAX_SIZE - 1 );
}

AsmMachineCodeGen::~AsmMachineCodeGen()
{
}

void AsmMachineCodeGen::clear()
{
    m_address = 0;
    m_maxSize = 0;
    m_size = 0;
    m_code.clear();
    m_used.clear();
}

void AsmMachineCodeGen::setOrigin ( unsigned int address )
{
    m_address = address;
}

void AsmMachineCodeGen::incrementAddr ( unsigned int increment )
{
    m_address += increment;
}

unsigned int AsmMachineCodeGen::setCode ( uint32_t code )
{
    if ( true == reserve ( m_address ) )
    {
        m_code [ m_address ] = code;
        m_used [ m_address ] = 1;

        if ( m_address >= m_size )
        {
            m_size = ( 1 + m_address );
        }
    }
    m_address++;

    return ( m_address - 1 );
}

void AsmMachineCodeGen::setCode ( unsigned int address,
                                  uint32_t code )
{
    if ( true == reserve ( address ) )
    {
        m_code [ address ] = code;
        m_used [ address ] = 1;

        if ( address >= m_size )
        {
            m_size = ( 1 + address );
        }
    }
}

inline bool AsmMachineCodeGen::reserve ( unsigned int maxAddr )
{
    if ( m_maxSize > maxAddr )
    {
        return true;
    }

    if ( maxAddr >= SIZE_HINT )
    {
        return false;
    }

    maxAddr++;

    // Align maxAddr to a multiply of INITIAL_MAX_SIZE.
    if ( 0 != ( maxAddr % INITIAL_MAX_SIZE ) )
    {
        maxAddr += ( INITIAL_MAX_SIZE - ( maxAddr % INITIAL_MAX_SIZE ) );
    }

    m_used.resize ( maxAddr );
    m_code.resize ( maxAddr );

    for ( unsigned int i = m_maxSize; i < maxAddr; i++ )
    {
        m_used[i] = 0;
    }
    m_maxSize = maxAddr;

    return true;
}

void AsmMachineCodeGen::output ( AsmMachineCodeGen::WordSize wordSize,
                                 AsmMachineCodeGen::Endianness byteOrder,
                                 DataFile * target ) const
{
    int addrIncrement;

    if ( E_LITTLE_ENDIAN == byteOrder )
    {
        addrIncrement = 1;
    }
    else
    {
        addrIncrement = -1;
    }

    for ( unsigned int i = 0; i < m_maxSize; i++ )
    {
        if ( 0 == m_used[i] )
        {
            continue;
        }

        unsigned int addr = i * (int)wordSize;
        unsigned int shift = 0;
        uint32_t mask = 0xff;

        if ( E_BIG_ENDIAN == byteOrder )
        {
            addr += ( (int)wordSize - 1 );
        }

        for ( int j = 0; j < (int)wordSize; j++ )
        {
            target->set(addr, (uint8_t) ( ( m_code[i] & mask ) >> shift ));

            addr += addrIncrement;
            shift += 8;
            mask <<= 8;
        }
    }
}

inline unsigned int AsmMachineCodeGen::sizeB ( AsmMachineCodeGen::WordSize wordSize ) const
{
    return ( m_size * (unsigned int) wordSize );
}

void AsmMachineCodeGen::output ( AsmMachineCodeGen::WordSize wordSize,
                                 AsmMachineCodeGen::Endianness byteOrder,
                                 CompilerSemanticInterface * compilerCore,
                                 const CompilerOptions * opts )
{
    if ( size() > 0x10000 )
    {
        compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("The resulting machine code is too big to be stored in a "
                                                      "file.").toStdString() );
        return;
    }

    if ( true == compilerCore->m_simulatorData.m_genSimData )
    {
        BinFile * dataFile = new BinFile(sizeB(wordSize));
        output(wordSize, byteOrder, dataFile);
        compilerCore->m_simulatorData.m_simData = dataFile;
    }

    if ( false == opts->m_hexFile.empty() )
    {
        HexFile dataFile(sizeB(wordSize), opts->m_hexMaxRecLength);
        saveMachineCode(wordSize, byteOrder, &dataFile, opts->m_hexFile, compilerCore);
    }

    if ( false == opts->m_binFile.empty() )
    {
        BinFile dataFile(sizeB(wordSize));
        saveMachineCode(wordSize, byteOrder, &dataFile, opts->m_binFile, compilerCore);
    }

    if ( false == opts->m_srecFile.empty() )
    {
        SrecFile dataFile(sizeB(wordSize));
        saveMachineCode(wordSize, byteOrder, &dataFile, opts->m_srecFile, compilerCore);
    }
}

inline void AsmMachineCodeGen::saveMachineCode ( AsmMachineCodeGen::WordSize wordSize,
                                                 AsmMachineCodeGen::Endianness byteOrder,
                                                 DataFile * dataFile,
                                                 const std::string & fileName,
                                                 CompilerSemanticInterface * compilerCore )
{
    try
    {
        output(wordSize, byteOrder, dataFile);
        dataFile->save(fileName, false);
    }
    catch ( const DataFileException & e )
    {
        std::cerr << e.toString() << std::endl;
        compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("unable to save file ").toStdString() + '"' + fileName  + '"' );
    }
}

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
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
#include "../CompilerSemanticInterface.h"
#include "../CompilerOptions.h"

// Header files of libMCUDataFiles.
#include "DataFile.h"
#include "BinFile.h"
#include "HexFile.h"
#include "SrecFile.h"

AsmMachineCodeGen::AsmMachineCodeGen ( WordSize wordSize )
                                     : m_wordSize ( wordSize )
{
    clear();
    reserve ( INITIAL_MAX_SIZE );
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
        m_code[address] = code;
        m_used[address] = 1;

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

    m_used.reserve ( maxAddr );
    m_code.reserve ( maxAddr );

    for ( unsigned int i = m_maxSize; i < maxAddr; i++ )
    {
        m_used[i] = 0;
    }
    m_maxSize = maxAddr;

    return true;
}

void AsmMachineCodeGen::output ( Endianness byteOrder,
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

        unsigned int addr = i * (int)m_wordSize;
        unsigned int shift = 0;
        uint32_t mask = 0xff;

        if ( E_BIG_ENDIAN == byteOrder )
        {
            addr += ( (int)m_wordSize - 1 );
        }

        for ( int j = 0; j < (int)m_wordSize; j++ )
        {
            target->set(addr, (uint8_t) ( ( m_code[i] & mask ) >> shift ));

            addr += addrIncrement;
            shift += 8;
            mask <<= 8;
        }
    }
}

void AsmMachineCodeGen::output ( Endianness byteOrder,
                                 CompilerSemanticInterface * compilerCore,
                                 const CompilerOptions * opts )
{
    if ( size() > 0x10000 )
    {
        compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                          QObject::tr("The resulting machine code is too big to be stored in a "
                                                      "file.").toStdString() );
        return;
    }

    if ( true == compilerCore->m_simulatorData.m_genSimData )
    {
        BinFile * dataFile = new BinFile(sizeB());
        saveMachineCode(byteOrder, dataFile, opts->m_binFile, compilerCore, opts);
        compilerCore->m_simulatorData.m_simData = dataFile;
    }

    if ( false == opts->m_hexFile.empty() )
    {
        HexFile dataFile(sizeB(), opts->m_hexMaxRecLength);
        saveMachineCode(byteOrder, &dataFile, opts->m_hexFile, compilerCore, opts);
    }

    if ( false == opts->m_binFile.empty() )
    {
        BinFile dataFile(sizeB());
        saveMachineCode(byteOrder, &dataFile, opts->m_binFile, compilerCore, opts);
    }

    if ( false == opts->m_srecFile.empty() )
    {
        SrecFile dataFile(sizeB());
        saveMachineCode(byteOrder, &dataFile, opts->m_srecFile, compilerCore, opts);
    }
}

inline void AsmMachineCodeGen::saveMachineCode ( Endianness byteOrder,
                                                 DataFile * dataFile,
                                                 const std::string & fileName,
                                                 CompilerSemanticInterface * compilerCore,
                                                 const CompilerOptions * opts )
{
    try
    {
        output(byteOrder, dataFile);
        dataFile->save(fileName, false);
    }
    catch ( const DataFile::Exception & e )
    {
        std::cerr << e.toString() << std::endl;
        compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                          QObject::tr("unable to save file ").toStdString() + "\"" + fileName  + "\"" );
    }
}

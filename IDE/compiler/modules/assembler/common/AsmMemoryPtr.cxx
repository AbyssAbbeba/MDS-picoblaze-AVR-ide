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
 * @file AsmMemoryPtr.cxx
 */
// =============================================================================

#include "AsmMemoryPtr.h"

// Common compiler header files.
#include "CompilerSemanticInterface.h"

// Standard header files.
#include <cstddef>

AsmMemoryPtr::AsmMemoryPtr ( CompilerSemanticInterface * compilerCore )
                           : m_compilerCore ( compilerCore )
{
}

void AsmMemoryPtr::clear()
{
    m_reg  = 0;
    m_data = 0;
    m_code = 0;

    for ( int i = 0; i < MS__MAX__; i++ )
    {
        m_reserved[i].clear();
    }
}

void AsmMemoryPtr::reserve ( AsmMemoryPtr::MemorySpace where,
                             unsigned int address )
{
    unsigned int addrA = ( address / sizeof(unsigned int) );
    unsigned int addrB = ( address % sizeof(unsigned int) );
    size_t origSize = m_reserved[where].size();

    if ( ( size_t ) addrA >= origSize )
    {
        m_reserved[where].resize ( addrA + 1 );
        for ( size_t i = origSize; i <= addrA; i++ )
        {
            m_reserved[where][i] = 0;
        }
    }

    m_reserved[where][addrA] |= ( 1 << addrB );
}

bool AsmMemoryPtr::isReserved ( AsmMemoryPtr::MemorySpace where,
                                unsigned int address ) const
{
    unsigned int addrA = ( address / sizeof(unsigned int) );
    unsigned int addrB = ( address % sizeof(unsigned int) );

    if ( ( size_t ) addrA >= m_reserved[where].size() )
    {
        return false;
    }

    return ( bool ) ( m_reserved[where][addrA] & ( 1 << addrB ) );
}

bool AsmMemoryPtr::tryReserve ( const CompilerSourceLocation & location,
                                AsmMemoryPtr::MemorySpace where,
                                unsigned int address )
{
    unsigned int hardLimit = 0;
    const char * memName = "<error-invalid>";

    switch ( where )
    {
        case MS_REG:
            hardLimit = m_hardLimits.m_reg;
            memName = "register file";
            break;
        case MS_DATA:
            hardLimit = m_hardLimits.m_data;
            memName = "scratch pad";
            break;
        case MS_CODE:
            hardLimit = m_hardLimits.m_code;
            memName = "program";
            break;
        case MS__MAX__:
            break;
    }

    if ( true == isReserved(where, address) )
    {
        m_compilerCore->semanticMessage ( location,
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "reusing already reserved space in %1 memory at address: 0x%2" )
                                                      . arg ( memName )
                                                      . arg ( address, 0, 16 )
                                                      . toStdString() );
        return false;
    }
    else if ( address >= hardLimit )
    {
        m_compilerCore->semanticMessage ( location,
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "attempting to use unavailable space in %1 memory at address: "
                                                        "0x%2" )
                                                      . arg ( memName )
                                                      . arg ( address, 0, 16 )
                                                      . toStdString() );
        return false;
    }

    reserve(where, address);
    return true;
}

unsigned int AsmMemoryPtr::getUsage ( MemorySpace where ) const
{
    unsigned int result = 0;

    for ( auto bitField : m_reserved[where] )
    {
        for ( unsigned int i = 0; i < sizeof(unsigned int); i++ )
        {
            if ( bitField & ( 1 << i ) )
            {
                result++;
            }
        }
    }

    return result;
}

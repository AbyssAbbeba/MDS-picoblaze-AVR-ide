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
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3MemoryPtr.cxx
 */
// =============================================================================

#include "AsmKcpsm3MemoryPtr.h"

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"

// Standard header files.
#include <cstddef>

AsmKcpsm3MemoryPtr::AsmKcpsm3MemoryPtr ( CompilerSemanticInterface * compilerCore )
                                       : m_compilerCore ( compilerCore )
{
}

void AsmKcpsm3MemoryPtr::clear()
{
    m_reg  = 0;
    m_data = 0;
    m_code = 0;

    for ( int i = 0; i < MS__MAX__; i++ )
    {
        m_reserved[i].clear();
    }
}

void AsmKcpsm3MemoryPtr::reserve ( AsmKcpsm3MemoryPtr::MemorySpace where,
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

bool AsmKcpsm3MemoryPtr::isReserved ( AsmKcpsm3MemoryPtr::MemorySpace where,
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

bool AsmKcpsm3MemoryPtr::tryReserve ( const CompilerSourceLocation & location,
                                      AsmKcpsm3MemoryPtr::MemorySpace where,
                                      unsigned int address )
{
    unsigned int hardLimit = 0;
    const char * memName = "<error-invalid>";

    switch ( where )
    {
        case MS_REG:
            hardLimit = HARD_LIM_REG;
            memName = "register file";
            break;
        case MS_DATA:
            hardLimit = HARD_LIM_DATA;
            memName = "scratch pad";
            break;
        case MS_CODE:
            hardLimit = HARD_LIM_CODE;
            memName = "program";
            break;
        case MS__MAX__:
            break;
    }

    if ( true == isReserved(where, address) )
    {
        m_compilerCore->compilerMessage ( location,
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "reusing already reserved space in %1 memory at address: %2" )
                                                      .arg(memName)
                                                      .arg(address)
                                                      .toStdString() );
        return false;
    }
    else if ( address > hardLimit )
    {
        m_compilerCore->compilerMessage ( location,
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("Attempting to use unavailable space in %1 memory at address: %2")
                                                     .arg(memName)
                                                     .arg(address)
                                                     .toStdString() );
        return false;
    }

    reserve(where, address);
    return true;
}

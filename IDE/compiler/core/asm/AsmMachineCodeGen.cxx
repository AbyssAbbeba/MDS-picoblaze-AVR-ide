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
 * @file AsmMachineCodeGen.cxx
 */
// =============================================================================

#include "AsmMachineCodeGen.h"

AsmMachineCodeGen::AsmMachineCodeGen()
{
    m_address = 0;
    m_maxSize = 0;
    reserve ( INITIAL_MAX_SIZE );
}

AsmMachineCodeGen::~AsmMachineCodeGen()
{
}

void AsmMachineCodeGen::setOrigin ( uint32_t address )
{
    m_address = address;
}

uint32_t AsmMachineCodeGen::setCode ( uint32_t code )
{
    reserve ( m_address );

    m_code [ m_address ] = code;
    m_used [ m_address ] = 1;

    m_address++;

    return ( m_address - 1 );
}

void AsmMachineCodeGen::setCode ( uint32_t address,
                                  uint32_t code )
{
    reserve ( address );

    m_code[address] = code;
    m_used[address] = 1;
}

inline void AsmMachineCodeGen::reserve ( uint32_t maxAddr )
{
    maxAddr++;

    m_used.reserve ( maxAddr );
    m_code.reserve ( maxAddr );

    if ( m_maxSize < maxAddr )
    {
        for ( uint32_t i = m_maxSize; i < maxAddr; i++ )
        {
            m_used[i] = 0;
        }
        m_maxSize = maxAddr;
    }
}

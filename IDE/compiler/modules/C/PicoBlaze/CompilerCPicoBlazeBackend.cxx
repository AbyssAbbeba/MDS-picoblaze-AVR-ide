// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerCPicoBlaze
 * @file CompilerCPicoBlazeBackend.cxx
 */
// =============================================================================

#include "CompilerCPicoBlazeBackend.h"

CompilerCPicoBlazeBackend::CompilerCPicoBlazeBackend ( CompilerSemanticInterface * compilerCore,
                                                       CompilerOptions * opts )
                                                     :
                                                       CompilerCBackend ( compilerCore, opts )
{
    m_dataTypeSize.m_bool = 1;
    m_dataTypeSize.m_char = 1;
    m_dataTypeSize.m_shortInt = 1;
    m_dataTypeSize.m_int = 2;
    m_dataTypeSize.m_longInt = 4;
    m_dataTypeSize.m_longLongInt = 8;

    m_dataTypeSize.m_float = 4;
    m_dataTypeSize.m_double = 8;
    m_dataTypeSize.m_longDouble = 10;
    m_dataTypeSize.m_complexFloat = 0;
    m_dataTypeSize.m_complexDouble = 0;
    m_dataTypeSize.m_complexLongDouble = 0;

    m_dataTypeSize.m_pointer = 2;
}

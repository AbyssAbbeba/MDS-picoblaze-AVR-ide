// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCDeclaration.cxx
 */
// =============================================================================

#include "CompilerCDeclaration.h"

CompilerCDeclaration::CompilerCDeclaration()
{
    m_array    = nullptr;
    m_id       = nullptr;
    m_typename = nullptr;
    m_value    = nullptr;
    m_owner    = nullptr;
    m_members  = nullptr;
}

CompilerCDeclaration::~CompilerCDeclaration()
{
    if ( nullptr != m_array )
    {
        delete m_array;
    }

    if ( nullptr != m_id )
    {
        delete m_id;
    }

    if ( nullptr != m_typename )
    {
        delete m_typename;
    }

    if ( nullptr != m_value )
    {
        m_value->completeDelete();
    }

    if ( nullptr != m_owner )
    {
        delete m_owner;
    }

    if ( nullptr != m_members )
    {
        delete m_members;
    }
}

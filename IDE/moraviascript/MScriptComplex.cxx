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
 * @ingroup MoraviaScript
 * @file MScriptComplex.cxx
 */
// =============================================================================

#include "MScriptComplex.h"

bool MScriptComplex::operator == ( const MScriptComplex & n ) const
{
    return ( ( n.m_r == m_r ) && ( n.m_i == m_i ) );
}

bool MScriptComplex::operator != ( const MScriptComplex & n ) const
{
    return ( ( n.m_r != m_r ) || ( n.m_i != m_i ) );
}

bool MScriptComplex::operator && ( const MScriptComplex & n ) const
{
    return ( ( ( 0.0 != n.m_r ) || ( 0.0 != n.m_i ) ) && ( ( 0.0 != m_r ) || ( 0.0 != m_i ) ) );
}

bool MScriptComplex::operator || ( const MScriptComplex & n ) const
{
    return ( ( 0.0 != n.m_r ) || ( 0.0 != n.m_i ) || ( 0.0 != m_r ) || ( 0.0 != m_i ) );
}

MScriptComplex MScriptComplex::operator + ( const MScriptComplex & n ) const
{
    MScriptComplex r;
    r.m_r = ( n.m_r + m_r );
    r.m_i = ( n.m_i + m_i );
    return r;
}

MScriptComplex MScriptComplex::operator - ( const MScriptComplex & n ) const
{
    MScriptComplex r;
    r.m_r = ( n.m_r - m_r );
    r.m_i = ( n.m_i - m_i );
    return r;
}

MScriptComplex MScriptComplex::operator * ( const MScriptComplex & n ) const
{
    MScriptComplex r;
    r.m_r = ( ( m_r * n.m_r ) - ( m_i * n.m_i ) );
    r.m_i = ( ( m_i * n.m_r ) + ( m_r * n.m_i ) );
    return r;
}

MScriptComplex MScriptComplex::operator / ( const MScriptComplex & n ) const
{
    MScriptComplex r;
    const double den = ( n.m_r * n.m_r + n.m_i * n.m_i );
    r.m_r = ( ( m_r * n.m_r + m_i * n.m_i ) / den );
    r.m_i = ( ( m_i * n.m_r - m_r * n.m_i ) / den );
    return r;
}

MScriptComplex MScriptComplex::power ( const MScriptComplex & n ) const
{
    
}

bool MScriptComplex::divisableBy ( const MScriptComplex & n ) const
{
    return ( 0 != ( n.m_r * n.m_r + n.m_i * n.m_i ) );
}

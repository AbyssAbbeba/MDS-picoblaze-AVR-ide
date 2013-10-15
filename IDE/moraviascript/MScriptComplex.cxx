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

bool MScriptComplex::operator < ( const MScriptComplex & n ) const
{

}

bool MScriptComplex::operator > ( const MScriptComplex & n ) const
{

}

bool MScriptComplex::operator <= ( const MScriptComplex & n ) const
{

}

bool MScriptComplex::operator >= ( const MScriptComplex & n ) const
{

}

bool MScriptComplex::operator && ( const MScriptComplex & n ) const
{

}

bool MScriptComplex::operator || ( const MScriptComplex & n ) const
{

}

MScriptComplex MScriptComplex::operator + ( const MScriptComplex & n ) const
{

}

MScriptComplex MScriptComplex::operator - ( const MScriptComplex & n ) const
{

}

MScriptComplex MScriptComplex::operator * ( const MScriptComplex & n ) const
{

}

MScriptComplex MScriptComplex::operator / ( const MScriptComplex & n ) const
{

}

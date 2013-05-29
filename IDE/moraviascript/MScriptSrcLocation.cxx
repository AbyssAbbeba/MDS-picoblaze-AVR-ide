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
 * @ingroup MoraviaScript
 * @file MScriptSrcLocation.cxx
 */
// =============================================================================

#include "MScriptSrcLocation.h"

MScriptSrcLocation::MScriptSrcLocation()
{
    m_line[0] = 0;
    m_line[1] = 0;
    m_column[0] = 0;
    m_column[1] = 0;
}

MScriptSrcLocation::MScriptSrcLocation ( const YYLTYPE * yylloc )
{
    m_line[0]   = yylloc->first_line;
    m_line[1]   = yylloc->last_line;
    m_column[0] = yylloc->first_column;
    m_column[1] = yylloc->last_column;
}

MScriptSrcLocation::MScriptSrcLocation ( const YYLTYPE & yylloc )
{
    m_line[0]   = yylloc.first_line;
    m_line[1]   = yylloc.last_line;
    m_column[0] = yylloc.first_column;
    m_column[1] = yylloc.last_column;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptSrcLocation & location )
{
    out << "{" << m_line[0] << "." << m_column[0] << "-" << m_line[1] << "." << m_column[1] << "}";
    return out;
}

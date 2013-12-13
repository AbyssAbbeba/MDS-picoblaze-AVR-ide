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
 * @ingroup Compiler
 * @file CompilerSourceLocation.cxx
 */
// =============================================================================

#include "CompilerSourceLocation.h"

CompilerSourceLocation::CompilerSourceLocation ( int fileNumber,
                                                 int lineStart,
                                                 int lineEnd,
                                                 int colStart,
                                                 int colEnd )
{
    m_fileNumber = fileNumber;
    m_lineStart  = lineStart;
    m_lineEnd    = lineEnd;
    m_colStart   = colStart;
    m_colEnd     = colEnd;
}

CompilerSourceLocation::CompilerSourceLocation()
{
    m_fileNumber = -1;
    m_lineStart  = -1;
    m_lineEnd    = -1;
    m_colStart   = -1;
    m_colEnd     = -1;
}

bool CompilerSourceLocation::isSet() const
{
    if ( -1 == m_fileNumber )
    {
        return false;
    }
    if ( -1 == m_lineStart )
    {
        return false;
    }
    if ( -1 == m_lineEnd )
    {
        return false;
    }
    if ( -1 == m_colStart )
    {
        return false;
    }
    if ( -1 == m_colEnd )
    {
        return false;
    }

    return true;
}

void CompilerSourceLocation::serialize ( CompilerSerializer & output ) const
{
    output.write ( (uint16_t) m_fileNumber );
    output.write ( (uint32_t) m_lineStart );
    output.write ( (uint32_t) m_lineEnd );
    output.write ( (uint16_t) m_colStart );
    output.write ( (uint16_t) m_colEnd );
}

void CompilerSourceLocation::deserialize ( CompilerSerializer & input )
{
    m_fileNumber = input.translateFileNumber ( (int) input.read_ui16() );
    m_lineStart  = (int) input.read_ui32();
    m_lineEnd    = (int) input.read_ui32();
    m_colStart   = (int16_t) input.read_ui16();
    m_colEnd     = (int16_t) input.read_ui16();
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerSourceLocation & location )
{
    if ( -1 != location.m_fileNumber )
    {
        out << location.m_fileNumber;
    }
    out << ":";
    if ( -1 != location.m_lineStart )
    {
        out << location.m_lineStart;
    }
    out << ".";
    if ( -1 != location.m_colStart )
    {
        out << location.m_colStart;
    }
    out << "-";
    if ( -1 != location.m_lineEnd )
    {
        out << location.m_lineEnd;
    }
    out << ".";
    if ( -1 != location.m_colEnd )
    {
        out << location.m_colEnd;
    }

    return out;
}

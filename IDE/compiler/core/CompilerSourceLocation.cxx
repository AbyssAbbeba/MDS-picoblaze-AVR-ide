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
    m_origin     = -1;
}

CompilerSourceLocation::CompilerSourceLocation()
{
    m_fileNumber = -1;
    m_lineStart  = -1;
    m_lineEnd    = -1;
    m_colStart   = -1;
    m_colEnd     = -1;
    m_origin     = -1;
}

bool CompilerSourceLocation::isSet() const
{
    if ( -1 != m_origin )
    {
        return true;
    }
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

bool CompilerSourceLocation::equal ( const CompilerSourceLocation & obj,
                                     bool omitOrigin ) const
{
    if ( ( false == omitOrigin ) && ( obj.m_origin != m_origin ) )
    {
        return false;
    }

    return ( ( obj.m_fileNumber == m_fileNumber )
               &&
             ( obj.m_lineStart  == m_lineStart  )
               &&
             ( obj.m_lineEnd    == m_lineEnd    )
               &&
             ( obj.m_colStart   == m_colStart   )
               &&
             ( obj.m_colEnd     == m_colEnd     ) );
}

void CompilerSourceLocation::serialize ( CompilerSerializer & output ) const
{
    output.write ( (uint16_t) m_fileNumber );
    output.write ( (uint32_t) m_lineStart );
    output.write ( (uint32_t) m_lineEnd );
    output.write ( (uint16_t) m_colStart );
    output.write ( (uint16_t) m_colEnd );
    output.write ( (uint32_t) m_origin );
}

void CompilerSourceLocation::deserialize ( CompilerSerializer & input )
{
    m_fileNumber = input.translateFileNumber ( (int) input.read_ui16() );
    m_lineStart  = (int) input.read_ui32();
    m_lineEnd    = (int) input.read_ui32();
    m_colStart   = (int) input.read_ui16();
    m_colEnd     = (int) input.read_ui16();
    m_origin     = input.translateLOrigin ( (int) input.read_ui32() );
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
    if ( -1 != location.m_origin )
    {
        out << "[" << location.m_origin << "]";
    }

    return out;
}

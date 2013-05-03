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
 * @ingroup Compiler
 * @file CompilerBase.cxx
 */
// =============================================================================

#include "CompilerBase.h"

CompilerBase::SourceLocation::SourceLocation ( int fileNumber,
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

CompilerBase::SourceLocation::SourceLocation()
{
    m_fileNumber = -1;
    m_lineStart  = -1;
    m_lineEnd    = -1;
    m_colStart   = -1;
    m_colEnd     = -1;
}

bool CompilerBase::SourceLocation::isSet() const
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

std::ostream & operator << ( std::ostream & out,
                             const CompilerBase::SourceLocation & location )
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

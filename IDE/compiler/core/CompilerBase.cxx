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

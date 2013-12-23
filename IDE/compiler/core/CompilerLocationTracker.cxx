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
 * @file CompilerLocationTracker.cxx
 */
// =============================================================================

#include "CompilerLocationTracker.h"

int CompilerLocationTracker::add ( const CompilerSourceLocation & location,
                                   int next )
{
    m_locations.push_back(std::make_pair(location, next));
    return (int) (m_locations.size() - 1);
}

const CompilerSourceLocation & CompilerLocationTracker::getLocation ( int origin )
{
    return m_locations[origin].first;
}

int CompilerLocationTracker::getNext ( int origin )
{
    return m_locations[origin].second;
}

void CompilerLocationTracker::clear()
{
    m_locations.clear();
}

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

int CompilerLocationTracker::add ( const CompilerSourceLocation & location )
{
    m_locations.push_back(location);
    return (int) (m_locations.size() - 1);
}

const CompilerSourceLocation & CompilerLocationTracker::getLocation ( int origin ) const
{
    return m_locations[origin];
}

void CompilerLocationTracker::clear()
{
    m_locations.clear();
}

int CompilerLocationTracker::getInitShift()
{
    return (int) m_locations.size();
}

void CompilerLocationTracker::serialize ( CompilerSerializer & output ) const
{
    output.write ( (uint32_t) m_locations.size() );
    for ( size_t i = 0; i < m_locations.size(); i++ )
    {
        output << m_locations[i];
    }
}

void CompilerLocationTracker::deserialize ( CompilerSerializer & input )
{
    CompilerSourceLocation location;
    size_t size = input.read_ui32();

    for ( size_t i = 0; i < size; i++ )
    {
        input >> location;
        m_locations.push_back(location);
    }
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerLocationTracker & tracker )
{
    int i = 0;
    for ( const auto & location : tracker.m_locations )
    {
        out << i << ": " << location << std::endl;
        i++;
    }

    return out;
}

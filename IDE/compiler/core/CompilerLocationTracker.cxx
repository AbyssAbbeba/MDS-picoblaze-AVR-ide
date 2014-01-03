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

const CompilerSourceLocation & CompilerLocationTracker::getLocation ( int origin ) const
{
    return m_locations[origin].first;
}

int CompilerLocationTracker::getNext ( int origin ) const
{
    return m_locations[origin].second;
}

bool CompilerLocationTracker::differs ( const CompilerSourceLocation & a,
                                        const CompilerSourceLocation & b ) const
{
    size_t size;
    std::vector<const CompilerSourceLocation *> foundLocations;

    traverse(a, &foundLocations);
    size = foundLocations.size();
    traverse(b, &foundLocations);

    return ( foundLocations.size() != size );
}

void CompilerLocationTracker::traverse ( const CompilerSourceLocation & source,
                                         std::vector<const CompilerSourceLocation *> * target ) const
{
    if ( -1 == source.m_origin )
    {
        for ( std::vector<const CompilerSourceLocation *>::const_iterator it = target->cbegin();
              target->cend() != it;
              ++it )
        {
            if ( (*it)->equal(source) )
            {
                return;
            }
        }

        target->push_back(&source);
    }
    else
    {
        traverse(getLocation(source.m_origin), target);
        int next = getNext(source.m_origin);
        if ( -1 != next )
        {
            traverse(getLocation(next), target);
        }
    }
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
        output << m_locations[i].first;
        output.write ( (uint32_t) m_locations[i].second );
    }
}

void CompilerLocationTracker::deserialize ( CompilerSerializer & input )
{
    CompilerSourceLocation location;
    size_t size = input.read_ui32();

    for ( size_t i = 0; i < size; i++ )
    {
        input >> location;
        m_locations.push_back ( std::make_pair ( location, input.translateLOrigin(input.read_ui32()) ) );
    }
}

std::ostream & operator << ( std::ostream & out,
                             const CompilerLocationTracker & tracker )
{
    for ( std::vector<std::pair<CompilerSourceLocation,int> >::const_iterator it = tracker.m_locations.cbegin();
          it != tracker.m_locations.cend();
          it++ )
    {
        out << it->first << " --> " << it->second << std::endl;
    }
    return out;
}

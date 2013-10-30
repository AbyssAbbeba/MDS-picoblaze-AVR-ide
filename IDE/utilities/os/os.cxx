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
 * @ingroup oscomp
 * @file os.cxx
 */
// =============================================================================

#include "os.h"

// Standard header files.
#include <cstring>

namespace boost
{
    #ifdef __linux__
      namespace filesystem3
    #else // __linux__
      namespace filesystem
    #endif // __linux__
    {
        template < >
            path & path::append< typename path::iterator > ( typename path::iterator begin,
                                                             typename path::iterator end,
                                                             const codecvt_type & cvt )
            {
                for( ; begin != end ; ++begin )
                {
                    *this /= *begin;
                }

                return *this;
            }
    }
}

namespace boost
{
    namespace filesystem
    {
        // Return path when appended to a_From will resolve to same as a_To.
        path make_relative ( path a_From,
                             path a_To )
        {
            a_From = absolute( a_From );
            a_To = absolute( a_To );
            path ret;
            path::const_iterator itrFrom( a_From.begin() ), itrTo( a_To.begin() );

            // Find common base.
            for ( path::const_iterator toEnd( a_To.end() ), fromEnd( a_From.end() );
                  itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo;
                  ++itrFrom, ++itrTo );

            // Navigate backwards in directory to reach previously found base.
            for ( path::const_iterator fromEnd ( a_From.end() );
                  itrFrom != fromEnd;
                  ++itrFrom )
            {
                if ( (*itrFrom) != "." )
                {
                    ret /= "..";
                }
            }

            // Now navigate down the directory branch.
            ret.append ( itrTo, a_To.end() );
            return ret;
        }
    }
}

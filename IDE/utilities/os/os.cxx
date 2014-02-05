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
 * @ingroup oscomp
 * @file os.cxx
 */
// =============================================================================

#include "os.h"

// Standard header files.
#include <cstring>

// Linux specific header files.
#ifdef __linux__
#  include <unistd.h>
#  include <sys/types.h>
#  include <pwd.h>
#endif // __linux__

namespace boost
{
    #if BOOST_VERSION <= 104800
        namespace filesystem3
    #else
        namespace filesystem
    #endif
    {
        template < >
            path & path::append< typename path::iterator > ( typename path::iterator begin,
                                                             typename path::iterator end,
                                                             const codecvt_type & /*cvt*/ )
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
            path ret;
            a_From = absolute( a_From );
            a_To = absolute( a_To );
            auto itrFrom = a_From.begin(), itrTo = a_To.begin();

            // Find common base.
            for ( auto toEnd = a_To.end(), fromEnd = a_From.end();
                  itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo;
                  ++itrFrom, ++itrTo );

            // Navigate backwards in directory to reach previously found base.
            for ( auto fromEnd = a_From.end();
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

        std::string makeHomeSafe ( const std::string & input )
        {
            #ifdef __linux__
                std::string result = input;
                if ( '~' == result[0] )
                {
                    result.replace(0, 1, getpwuid(getuid())->pw_dir);
                }
                return result;
            #else // __linux__
                return input;
            #endif // __linux__
        }
    }
}

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup oscomp
 * @file os.h
 */
// =============================================================================

#ifndef OS_H
#define OS_H

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

namespace boost
{
    namespace filesystem
    {
        /**
         * @brief
         * @param[in] a
         * @param[in] b
         * @return
         */
        path make_relative ( path a, path b );
    }
}

// Code specific for other operating systems than GNU/Linux.
#ifndef __linux__
    /**
     * @brief
     * @param[in] s
     * @return
     */
    inline char * strdup ( const char * s )
    {
        char * d = malloc ( 1 + strlen ( s ) );
        if ( NULL == d )
        {
            return NULL;
        }
        return strcpy ( d, s);
    }
#endif // __linux__

#endif // OS_H

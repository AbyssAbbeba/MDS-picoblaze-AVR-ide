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
#include "boost/filesystem.hpp"

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

        std::string makeHomeSafe ( const std::string & input );
    }
}

#endif // OS_H

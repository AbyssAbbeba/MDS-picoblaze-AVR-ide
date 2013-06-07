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

#ifdef STRDUP_DECLARED
    char * strdup ( const char * s )
    {
        char * d = malloc ( 1 + strlen ( s ) );
        if ( NULL == d )
        {
            return NULL;
        }
        return strcpy ( d, s);
    }
#endif // STRDUP_DECLARED

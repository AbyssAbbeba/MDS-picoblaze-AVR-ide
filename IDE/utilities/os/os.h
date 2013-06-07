// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup oscomp
 * @file os.h
 */
// =============================================================================

#ifndef OS_H
#define OS_H

    // Code specific for other operating systems than GNU/Linux.
    #ifndef __linux__

        #ifndef STRDUP_DECLARED
        #define STRDUP_DECLARED
            char * strdup ( const char * s );
        #endif // STRDUP_DECLARED

    #endif // __linux__

#endif // OS_H

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup oscomp
 * @file getline.h
 */
// =============================================================================

#ifndef GETLINE_H
#define GETLINE_H

#ifndef __linux__

#include <cassert>

constexpr size_t MIN_CHUNK = 64;

inline int getstr ( char ** lineptr,
                    size_t * n,
                    FILE * stream,
                    const char terminator,
                    int offset )
{
    int nchars_avail;
    char * read_pos;
    int ret;

    if ( nullptr == lineptr || nullptr == n || nullptr == stream )
    {
        errno = EINVAL;
        return -1;
    }

    if ( nullptr == *lineptr )
    {
        *n = MIN_CHUNK;
        *lineptr = (char*) malloc (*n);

        if (!*lineptr)
        {
            errno = ENOMEM;
            return -1;
        }
    }

    nchars_avail = *n - offset;
    read_pos = *lineptr + offset;

    while ( true )
    {
        int save_errno;
        register int c = getc (stream);

        save_errno = errno;

        assert ( (*lineptr + *n) == (read_pos + nchars_avail) );
        if ( nchars_avail < 2 )
        {
            if (* n > MIN_CHUNK )
            {
                *n *= 2;
            }
            else
            {
                *n += MIN_CHUNK;
            }

            nchars_avail = *n + *lineptr - read_pos;
            *lineptr = (char*) realloc (*lineptr, *n);

            if ( nullptr == *lineptr )
            {
                errno = ENOMEM;
                return -1;
            }

            read_pos = *n - nchars_avail + *lineptr;
            assert((*lineptr + *n) == (read_pos + nchars_avail));
        }

        if ( 0 != ferror (stream) )
        {
            errno = save_errno;
            return -1;
        }

        if ( EOF == c )
        {
            if ( read_pos == *lineptr )
            {
                return -1;
            }
            else
            {
                break;
            }
        }

        *read_pos++ = c;
        nchars_avail--;

        if ( terminator == c )
        {
            break;
        }
    }

    *read_pos = '\0';

    ret = read_pos - ( *lineptr + offset );
    return ret;
}

inline int getline ( char **lineptr,
                     size_t *n,
                     FILE *stream )
{
    return getstr ( lineptr, n, stream, '\n', 0 );
}

#endif // __linux__
#endif // GETLINE_H

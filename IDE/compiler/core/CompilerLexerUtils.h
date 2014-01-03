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
 * @ingroup Compiler
 * @file CompilerLexerUtils.h
 */
// =============================================================================

#ifndef COMPILERLEXERUTILS_H
#define COMPILERLEXERUTILS_H

#include "CompilerParserInterface.h"

// Standard header files.
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

/**
 * @brief
 * @namespace CompilerLexerUtils
 * @ingroup Compiler
 */
namespace CompilerLexerUtils
{
    ////    Operations    ////

    /**
     * @brief Convert a C like escape sequence to its binary form.
     * @param[in,out] compiler
     * @param[in] location
     * @param[in] ch The escape sequence to convert.
     * @return Binary value represented by the escape sequence, e.g. for `\n' it's 0x0A.
     */
    uint32_t escapeSequence ( CompilerParserInterface * compiler,
                              const YYLTYPE * location,
                              const char * seq,
                              int * size );

    /**
     * @brief Convert a string to an integer and check its size
     * @param[in,out] compiler
     * @param[in] location
     * @param[in] str
     * @param[in] max
     * @param[in] base
     * @return
     */
    int convertStrToNumber ( CompilerParserInterface * compiler,
                             const YYLTYPE * location,
                             const char * str,
                             unsigned int max,
                             int base );
};

// Code specific for other operating systems than GNU/Linux.
#ifndef __linux__
    /**
     * @brief
     * @param[in] s
     * @return
     */
    inline char * strdup ( const char * s )
    {
        char * d = malloc ( 1 + strlen ( s ) )
        if ( NULL == d )
        {
            return NULL;
        }
        return strcpy ( d, s );
    }
#endif // __linux__

#endif // COMPILERLEXERUTILS_H

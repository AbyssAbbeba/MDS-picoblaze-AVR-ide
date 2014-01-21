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
 * @ingroup MoraviaScript
 * @file MScriptLexerAuxFunc.h
 */
// =============================================================================

#ifndef MSCRIPTLEXERAUXFUNC_H
#define MSCRIPTLEXERAUXFUNC_H

// MScript language interpreter header files.
#include "MScriptParserInterface.h"
#include "MScriptSrcLocation.h"

// Standard header files.
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptLexerAuxFunc
 */
namespace MScriptLexerAuxFunc
{
    ////    Operations    ////

    /**
     * @brief Convert a C like escape sequence to its binary form.
     * @param[in,out] core
     * @param[in] location
     * @param[in] ch The escape sequence to convert.
     * @return Binary value represented by the escape sequence, e.g. for `\n' it's 0x0A.
     */
    uint32_t escapeSequence ( MScriptParserInterface * core,
                              const MScriptSrcLocation location,
                              const char * seq,
                              int * size );

    /**
     * @brief Convert a string to integer and check its size.
     * @param[in,out] core
     * @param[in] location
     * @param[in] str
     * @param[in] max
     * @param[in] base
     * @return
     */
    long long str2int ( MScriptParserInterface * core,
                        const MScriptSrcLocation location,
                        const char * str,
                        int base );

    /**
     * @brief Convert a string to floating point number.
     * @param[in,out] core
     * @param[in] location
     * @param[in] str
     * @return
     */
    double str2float ( MScriptParserInterface * core,
                       const MScriptSrcLocation location,
                       const char * str );
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
        char * d = (char*) malloc ( 1 + strlen ( s ) );
        if ( nullptr == d )
        {
            return nullptr;
        }
        return strcpy ( d, s );
    }
#endif // __linux__

#endif // MSCRIPTLEXERAUXFUNC_H

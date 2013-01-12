// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file LexerUtils.h
 */
// =============================================================================

#ifndef LEXERUTILS_H
#define LEXERUTILS_H

#include "CompilerParserInterface.h"

#include <cstdint>

/**
 * @brief
 * @namespace LexerUtils
 * @ingroup Compiler
 */
namespace LexerUtils
{
    ////    Operations    ////

    /**
     * @brief Convert a two-character escape sequence to its binary form
     * @param[in,out] compiler
     * @param[in] location
     * @param[in] ch The escape sequence to convert, e.g. for `\n' it's `n'
     * @return Binary value represented by the escape sequence, e.g. for `\n' it's 0x0A
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

#endif // LEXERUTILS_H

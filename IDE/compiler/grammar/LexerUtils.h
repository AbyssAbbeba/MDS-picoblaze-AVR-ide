/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup Compiler
 * @file LexerUtils.h
 */

#ifndef LEXERUTILS_H
#define LEXERUTILS_H

#include "CompilerParserInterface.h"

#include <cstdint>

/**
 * @brief
 * @namespace LexerUtils
 * @ingroup Compiler
 */
namespace LexerUtils {
	/**
	 * @brief Convert a two-character escape sequence to its binary form
	 * @param compiler: [InOut]
	 * @param location: [In]
	 * @param ch: [In] The escape sequence to convert, e.g. for `\n' it's `n'
	 * @return Binary value represented by the escape sequence, e.g. for `\n' it's 0x0A
	 */
	uint32_t escapeSequence(CompilerParserInterface * compiler, const YYLTYPE * location, const char * seq, int * size);

	/**
	 * @brief Convert a string to an integer and check its size
	 * @param compiler: [InOut]
	 * @param location: [In]
	 * @param str: [In]
	 * @param max: [In]
	 * @param base: [In]
	 * @return
	 */
	int convertStrToNumber(CompilerParserInterface * compiler, const YYLTYPE * location, const char * str, uint max, int base);
};

#endif // LEXERUTILS_H

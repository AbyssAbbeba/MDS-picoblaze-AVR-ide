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
 * @ingroup MoraviaScript
 * @file MScriptLexerAuxFunc.h
 */
// =============================================================================

#ifndef MSCRIPTLEXERAUXFUNC_H
#define MSCRIPTLEXERAUXFUNC_H

// MScript language interpreter header files.
#include "MScriptParserInterface.h"
#include "MScriptSrcLocation.h"

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptLexerAuxFunc
 */
namespace MScriptLexerAuxFunc
{
    /**
     * @brief Convert a string to integer and check its size.
     * @param[in,out] core
     * @param[in] location
     * @param[in] str
     * @param[in] max
     * @param[in] base
     * @return
     */
    int str2int ( MScriptParserInterface * core,
                  const MScriptSrcLocation location,
                  const char * str,
                  unsigned int max,
                  int base );

    /**
     * @brief Convert a string to floating point number.
     * @param[in,out] core
     * @param[in] location
     * @param[in] str
     * @return
     */
    int str2float ( MScriptParserInterface * core,
                    const MScriptSrcLocation location,
                    const char * str );
};

#endif // MSCRIPTLEXERAUXFUNC_H

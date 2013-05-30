// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MoraviaScript
 * @file MScriptLexerAuxFunc.cxx
 */
// =============================================================================

#include "MScriptLexerAuxFunc.h"

// Standard header files.
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>

/*
 * Code specific for other operating systems than GNU/Linux.
 */
#ifndef __linux__
    char * strdup ( const char * s )
    {
        char * d = malloc ( 1 + strlen ( s ) );
        if ( NULL == d )
        {
            return NULL;
        }
        return strcpy ( d, s);
    }
#endif // __linux__

int MScriptLexerAuxFunc::str2int ( MScriptParserInterface * core,
                                   const MScriptSrcLocation location,
                                   const char * str,
                                   unsigned int max,
                                   int base )
{
    unsigned long number;
    if ( ( strlen(str) > max )
            ||
         ( ( 0x1ULL << 32 ) <= ( number = strtoul(str, NULL, base) ) ) )
    {
//         core->lexerMessage ( location,
//                              CompilerBase::MT_ERROR,
//                              QObject::tr("Too big number: ").toStdString() + "`" + std::string(str) + "'" );

        return 1; // Some "neutral dummy value"
    }

    return int(number);
}

int MScriptLexerAuxFunc::str2float ( MScriptParserInterface * core,
                                     const MScriptSrcLocation location,
                                     const char * str )
{
    
}

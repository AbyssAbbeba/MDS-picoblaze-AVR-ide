// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptLexerAuxFunc.cxx
 */
// =============================================================================

#include "MScriptLexerAuxFunc.h"

// MScript language interpreter header files.
#include "MScriptBase.h"

// Standard header files.
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Used for i18n only.
#include <QObject>

long long  MScriptLexerAuxFunc::str2int ( MScriptParserInterface * core,
                                          const MScriptSrcLocation location,
                                          const char * str,
                                          int base )
{
    long long result = strtoll(str, nullptr, base);

    if ( ( LLONG_MIN == result ) || ( LLONG_MAX == result ) )
    {
        core->lexerMessage ( location,
                             MScriptBase::MT_ERROR,
                             QObject::tr("number is too big: ").toStdString() + "`" + std::string(str) + "'" );

        return 1; // Some "neutral dummy value"
    }

    return result;
}

double MScriptLexerAuxFunc::str2float ( MScriptParserInterface * core,
                                        const MScriptSrcLocation location,
                                        const char * str )
{
    double result = strtod ( str, nullptr );

    if ( ( -HUGE_VAL == result ) || ( HUGE_VAL == result ) )
    {
        core->lexerMessage ( location,
                             MScriptBase::MT_ERROR,
                             QObject::tr("number is too big: ").toStdString() + "`" + std::string(str) + "'" );

        return 1.0; // Some "neutral dummy value"
    }

    return result;
}

uint32_t MScriptLexerAuxFunc::escapeSequence ( MScriptParserInterface * core,
                                               const MScriptSrcLocation location,
                                               const char * seq,
                                               int * size )
{
    uint32_t value; // Converted escape sequence, i.e. its binary value
    bool unicode = false;

    *size = 1;
    switch ( seq[1] )
    {
        // Simple escape sequences, return converted value right away
        case 'a' : return '\a'; // alarm
        case 'b' : return '\b'; // backspace
        case '\'': return '\''; // single quote
        case '"' : return '\"'; // double quote
        case '?' : return '\?'; // question mark
        case '\\': return '\\'; // backslash
        case 'f' : return '\f'; // form feed
        case 'n' : return '\n'; // line feed
        case 'r' : return '\r'; // carriage return
        case 't' : return '\t'; // horizontal tab
        case 'v' : return '\v'; // vertical tab

        // Octal, 3 digits
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
            *size = 1;
            if ( 0 == sscanf(seq + 1, "%o", &value) )
            {
                *size = 0;
            }
            break;
        // Hexadecimal, max. 2 digits
        case 'x':
            *size = 1;
            if ( 0 == sscanf(seq + 2, "%x", &value) )
            {
                *size = 0;
            }
            break;
        // Hexadecimal, max. 4 digits
        case 'u':
            *size = 2;
            unicode = true;
            if ( 0 == sscanf(seq + 2, "%x", &value) )
            {
                *size = 0;
            }
            break;
        // Hexadecimal, max. 8 digits
        case 'U':
            *size = 4;
            unicode = true;
            if ( 0 == sscanf(seq + 2, "%x", &value) )
            {
                *size = 0;
            }
            break;
        default:
            core->lexerMessage ( location,
                                 MScriptBase::MT_ERROR,
                                 QObject::tr("unrecognized escape sequence: ").toStdString() + "`" + seq + "'" );
            *size = 0;
            return 0; // Some "neutral" dummy value
    }

    // Convert unicode value (\uXXXX) to UTF-8
    if ( true == unicode )
    {
        uint32_t result[4];

        for ( int i = 0; i < 4; i++ )
        {
            result[i] = 0;
        }

        if ( value < 0x80)
        {
            result[0] = value;
            *size = 1;
        }
        else if ( value < 0x800 )
        {
            result[0] = ( ( value >> 6    ) | 0xC0 );
            result[1] = ( ( value  & 0x3F ) | 0x80 );
            *size = 2;
        }
        else if ( value < 0x10000 )
        {
            result[0] =   ( ( value >> 12   ) | 0xE0 );
            result[1] = ( ( ( value >> 6    ) & 0x3F ) | 0x80 );
            result[2] =   ( ( value  & 0x3F ) | 0x80 );
            *size = 3;
        }
        else if ( value < 0x110000 )
        {
            result[0] =   ( ( value >> 18   ) | 0xF0 );
            result[1] = ( ( ( value >> 12   ) & 0x3F ) | 0x80 );
            result[2] = ( ( ( value >> 6    ) & 0x3F ) | 0x80 );
            result[3] =   ( ( value  & 0x3F ) | 0x80 );
            *size = 4;
        }
        else
        {
            core->lexerMessage ( location,
                                 MScriptBase::MT_ERROR,
                                 QObject::tr("invalid unicode character: ").toStdString() + "`" + seq + "'" );
            *size = 1;
        }

        value = ( ( result[3] << 24 ) | ( result[2] << 16 ) | ( result[1] << 8 ) | result[0] );
    }

    return value;
}

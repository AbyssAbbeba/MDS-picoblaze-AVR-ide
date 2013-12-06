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
 * @ingroup DAsmCore
 * @file DAsmCore.cxx
 */
// =============================================================================

#include "DAsmCore.h"

// Standard header files.
#include <cctype>
#include <cstring>
#include <cstdio>

void DAsmCore::indent ( std::string & line,
                        unsigned int column )
{
    if ( Config::IND_SPACES == m_config.m_indentation )
    {
        line += ' ';

        for ( unsigned int i = line.size(); i < column; i++ )
        {
            line += ' ';
        }
    }
    else
    {
        line += '\t';

        unsigned int lineSize = 0;
        for ( unsigned int i = 0; i < line.size(); i++ )
        {
            if ( '\t' == line[i] )
            {
                lineSize += ( m_config.m_tabSize - ( lineSize % m_config.m_tabSize ) );
            }
            else
            {
                lineSize++;
            }
        }

        lineSize /= m_config.m_tabSize;
        column   /= m_config.m_tabSize;

        for ( unsigned int i = lineSize; i < column; i++ )
        {
            line += '\t';
        }
    }
}

void DAsmCore::appendStr ( std::string & line,
                           const std::string & str )
{
    std::string result;

    if ( Config::LC_LOWERCASE == m_config.m_letterCase )
    {
        for ( size_t i = 0; i < str.size(); i++ )
        {
            result += (char) tolower ( (int) str[i] );
        }
    }
    else
    {
        for ( size_t i = 0; i < str.size(); i++ )
        {
            result += (char) toupper ( (int) str[i] );
        }
    }

    line += result;
}

std::string DAsmCore::letterCase ( const char * str )
{
    std::string result;
    size_t len = strlen(str);

    if ( Config::LC_LOWERCASE == m_config.m_letterCase )
    {
        for ( size_t i = 0; i < len; i++ )
        {
            result += (char) tolower ( (int) str[i] );
        }
    }
    else
    {
        for ( size_t i = 0; i < len; i++ )
        {
            result += (char) toupper ( (int) str[i] );
        }
    }

    return result;
}

std::string DAsmCore::num2str ( unsigned int number )
{
    if ( Config::R_BIN == m_config.m_radix )
    {
        std::string result = "0b";

        bool fsdFound = false;
        for ( int i = sizeof(number) - 1; i >= 0; i-- )
        {
            if ( 0 == ( number & ( 1U << i ) ) )
            {
                if ( true == fsdFound )
                {
                    result += '0';
                }
            }
            else
            {
                fsdFound = true;
                result += '1';
            }
        }
        if ( false == fsdFound )
        {
            result += '0';
        }

        return result;
    }
    else
    {
        char buffer[16];
        const char * format = "%d";

        switch ( m_config.m_radix )
        {
            case Config::R_OCT: format = "0%o";  break;
            case Config::R_DEC: format = "%d";   break;
            case Config::R_HEX: format = "0x%x"; break;
            default:
                break;
        }
        sprintf(buffer, format, number);
        return buffer;
    }
}

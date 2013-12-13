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
 * @ingroup MCUDataFiles
 * @file XilVHDLFile.cxx
 */
// =============================================================================

#include "XilVHDLFile.h"

#include <cctype>
#include <cstring>

int XilVHDLFile::extractHexField ( const std::string & line,
                                   std::string * hexField,
                                   const char * markFragment )
{
    int address = -1;

    size_t position = line.find(markFragment);
    if ( std::string::npos == position )
    {
        return -1;
    }

    for ( size_t i = position + strlen(markFragment); i < line.size(); i++ )
    {
        if ( 0 == isxdigit(line[i]) )
        {
            if ( 0 != isalpha(line[i]) )
            {
                address = -1;
            }
            break;
        }

        if ( -1 == address )
        {
            address = 0;
        }

        address <<= 4;
        address |= hexDigit2uint(line[i]);
    }

    if ( -1 == address )
    {
        return -1;
    }

    if (
            ( std::string::npos != line.find("=>", position) )
                ||
            (
                ( std::string::npos != line.find("of", position) )
                    &&
                ( std::string::npos != line.rfind("attribute", position) )
            )
       )
    {
        position = line.find('"');
        if ( std::string::npos == position )
        {
            throw DataFileException(DataFileException::EXP_NOT_UNDERSTOOD, "missing `\"' in `" + line + "'");
        }

        position++;

        size_t endPosition = line.find('"', position);
        if ( std::string::npos == endPosition )
        {
            throw DataFileException(DataFileException::EXP_NOT_UNDERSTOOD, "missing 2nd `\"' in `" + line + "'");
        }

        *hexField = line.substr(position, endPosition - position);
        if ( false == checkHex(*hexField) )
        {
            throw DataFileException(DataFileException::EXP_NOT_UNDERSTOOD, "`" + (*hexField) + "' is not a hex. field");
        }
    }

    return address;
}

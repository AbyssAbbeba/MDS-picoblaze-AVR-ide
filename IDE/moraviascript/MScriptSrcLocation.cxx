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
 * @file MScriptSrcLocation.cxx
 */
// =============================================================================

#include "MScriptSrcLocation.h"

// MScript language interpreter header files.
#include "MScriptBase.h"

MScriptSrcLocation::MScriptSrcLocation()
{
    m_file      = -1;
    m_line[0]   = -1;
    m_line[1]   = -1;
    m_column[0] = -1;
    m_column[1] = -1;
}

MScriptSrcLocation::MScriptSrcLocation ( const YYLTYPE * yylloc )
{
    m_file      = 0;
    m_line[0]   = yylloc->first_line;
    m_line[1]   = yylloc->last_line;
    m_column[0] = yylloc->first_column;
    m_column[1] = yylloc->last_column;
}

MScriptSrcLocation::MScriptSrcLocation ( const YYLTYPE & yylloc )
{
    m_file      = 0;
    m_line[0]   = yylloc.first_line;
    m_line[1]   = yylloc.last_line;
    m_column[0] = yylloc.first_column;
    m_column[1] = yylloc.last_column;
}

bool MScriptSrcLocation::operator == ( const MScriptSrcLocation & obj ) const
{
    if ( obj.m_file != m_file )
    {
        return false;
    }
    else if ( obj.m_line[0] != m_line[0] )
    {
        return false;
    }
    else if ( obj.m_line[1] != m_line[1] )
    {
        return false;
    }
    else if ( obj.m_column[0] != m_column[0] )
    {
        return false;
    }
    else if ( obj.m_column[1] != m_column[1] )
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string MScriptSrcLocation::toString ( const MScriptBase * core )  const
{
    char result[50];
    sprintf ( result,
              "%d.%d-%d.%d",
              m_line[0],
              m_column[0],
              m_line[1],
              m_column[1] );

    return ( core->fileNumber2str(m_file) + result );
}

void MScriptSrcLocation::serialize ( MScriptSerializer & output ) const
{
    output.write ( (uint16_t) m_file );
    output.write ( (uint32_t) m_line[0] );
    output.write ( (uint32_t) m_line[1] );
    output.write ( (uint16_t) m_column[0] );
    output.write ( (uint16_t) m_column[1] );
}

void MScriptSrcLocation::deserialize ( MScriptSerializer & input )
{
    m_file      = input.translateFileNumber ( (int) input.read_ui16() );
    m_line[0]   = (int) input.read_ui32();
    m_line[1]   = (int) input.read_ui32();
    m_column[0] = (int) input.read_ui16();
    m_column[1] = (int) input.read_ui16();
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptSrcLocation & location )
{
    out << location.m_file << ":"
        << location.m_line[0] << "." << location.m_column[0] << "-"
        << location.m_line[1] << "." << location.m_column[1];

    return out;
}

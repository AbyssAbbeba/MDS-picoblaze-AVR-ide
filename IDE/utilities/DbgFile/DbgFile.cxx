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
 * @ingroup DbgFile
 * @file DbgFile.cxx
 */
// =============================================================================

#include "DbgFile.h"

void DbgFile::getLinesByAddr ( unsigned int addr,
                               std::vector<std::pair<const std::string *, unsigned int>> & lines ) const
{
    std::vector<unsigned int> recordNumbers;

    lines.clear();
    getLineByAddr(addr, recordNumbers);

    if ( true == recordNumbers.empty() )
    {
        return;
    }

    for ( auto lineRecordIdx : recordNumbers )
    {
        int fileNumber = m_lineRecords[lineRecordIdx].m_fileNumber;

        if ( -1 == fileNumber )
        {
            continue;
        }

        lines.push_back ( std::make_pair ( & m_fileNames [ fileNumber ],
                                           m_lineRecords [ lineRecordIdx ].m_lineNumber ) );
    }
}

std::string DbgFile::Exception::toString() const
{
    std::string result;
    switch ( m_errorType )
    {
        case IO_ERROR:            result = "IO_ERROR";            break;
        case PARSE_ERROR:         result = "PARSE_ERROR";         break;
        case COMPATIBILITY_ERROR: result = "COMPATIBILITY_ERROR"; break;
    }
    return result + ": " + m_errorInfo;
}

bool DbgFile::operator == ( const DbgFile & obj )
{
    if ( obj.m_lastAddress != m_lastAddress )
    {
        return false;
    }
    else if ( obj.m_numberOfLines.size() != m_numberOfLines.size() )
    {
        return false;
    }
    else if ( obj.m_lineRecords.size() != m_lineRecords.size() )
    {
        return false;
    }
    else if ( obj.m_symbolRecords.size() != m_symbolRecords.size() )
    {
        return false;
    }
    else if ( obj.m_functionRecords.size() != m_functionRecords.size() )
    {
        return false;
    }
    else if ( obj.m_moduleRecords.size() != m_moduleRecords.size() )
    {
        return false;
    }
    else if ( obj.m_typeRecords.size() != m_typeRecords.size() )
    {
        return false;
    }
    else if ( obj.m_fileNames.size() != m_fileNames.size() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

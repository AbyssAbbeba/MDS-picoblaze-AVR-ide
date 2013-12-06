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

int DbgFile::getLineByAddr ( unsigned int addr,
                             std::string & fileName ) const
{
    int lineRecordIdx = getLineByAddr(addr);
    if ( -1 == lineRecordIdx )
    {
        return -1;
    }

    int fileNumber = m_lineRecords[lineRecordIdx].m_fileNumber;
    if ( -1 == fileNumber )
    {
        fileName = "";
    }
    else
    {
        fileName = m_fileNames[fileNumber];
    }
    return m_lineRecords[lineRecordIdx].m_lineNumber;
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
    if ( obj.m_numberOfLines.size() != m_numberOfLines.size() )
    {
        return false;
    }
    if ( obj.m_lineRecords.size() != m_lineRecords.size() )
    {
        return false;
    }
    if ( obj.m_symbolRecords.size() != m_symbolRecords.size() )
    {
        return false;
    }
    if ( obj.m_functionRecords.size() != m_functionRecords.size() )
    {
        return false;
    }
    if ( obj.m_moduleRecords.size() != m_moduleRecords.size() )
    {
        return false;
    }
    if ( obj.m_typeRecords.size() != m_typeRecords.size() )
    {
        return false;
    }
    if ( obj.m_fileNames.size() != m_fileNames.size() )
    {
        return false;
    }

    for ( int i = 0; i < m_lastAddress; i++ )
    {
        std::string fileNames[2];

        if ( getLineByAddr(i, fileNames[0]) != obj.getLineByAddr(i, fileNames[1]) )
        {
            return false;
        }
        if ( fileNames[0] != fileNames[1] )
        {
            return false;
        }
    }

    return true;
}

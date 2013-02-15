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

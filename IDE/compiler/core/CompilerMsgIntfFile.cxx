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
 * @ingroup Compiler
 * @file CompilerMsgIntfFile.cxx
 */
// =============================================================================

#include "CompilerMsgIntfFile.h"

void CompilerMsgIntfFile::openFile ( const std::string & filename )
{
    if ( true == m_ofstream.is_open() )
    {
        m_ofstream.close();
    }
    m_ofstream.open(filename, std::fstream::trunc);
}

void CompilerMsgIntfFile::closeFile()
{
    m_ofstream.close();
}

bool CompilerMsgIntfFile::isFileOpened()
{
    return m_ofstream.is_open();
}

void CompilerMsgIntfFile::message ( const std::string & text,
                                    CompilerBase::MessageType type )
{
    m_ofstream << type << ": " << text << std::endl;
}

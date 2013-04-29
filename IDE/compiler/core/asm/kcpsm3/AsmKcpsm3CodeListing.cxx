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
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3CodeListing.cxx
 */
// =============================================================================

#include "AsmKcpsm3CodeListing.h"

#include <fstream>

AsmKcpsm3CodeListing::LstLine::LstLine()
{
    m_address    = -1;
    m_code       = -1;
    m_macro      = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_fileNumber = -1;
    m_lineNumber = -1;
}

AsmKcpsm3CodeListing::LstLine::LstLine ( const char * line,
                                         int lineNumber,
                                         int fileNumber )
{
    m_address    = -1;
    m_code       = -1;
    m_macro      = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_fileNumber = fileNumber;
    m_lineNumber = lineNumber;
    m_line       = line;
}

void AsmKcpsm3CodeListing::loadSourceFiles()
{
    std::fstream file;
    int fileNumber = 0;
    int lineNumber;
    char line [ MAX_LINE_LENGTH ];

    for ( std::vector<std::string>::const_iterator it = m_compilerCore->listSourceFiles().begin();
          it != m_compilerCore->listSourceFiles().end();
          it++ )
    {
        fileNumber++;
        lineNumber = 0;
        file.open ( *it, std::fstream::in );

        if ( false == file.is_open())
        {
            m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                                QObject::tr("Unable to open ").toStdString() + "\"" + *it  + "\"" );
            return;
        }

        // Iterate over lines in the file
        while ( false == file.eof() )
        {
            lineNumber++;
            file.getline ( line, MAX_LINE_LENGTH );
            m_listing.push_back ( LstLine ( line, lineNumber, fileNumber ) );
        }

        file.close();
    }
}

void AsmKcpsm3CodeListing::output ( const std::string & filename )
{
    char line [ 16 ];
    int lineNumber = 0;
    std::ofstream file ( filename, std::fstream::out | std::fstream::trunc );

    if ( false == file.is_open())
    {
        // TODO: Report error here as proper compiler error message.
        return;
    }

    for ( std::vector<LstLine>::const_iterator i = m_listing.begin();
          i != m_listing.end();
          i++ )
    {
        if ( true == file.bad() )
        {
            // TODO: Report error here as proper compiler error message.
            return;
        }

        lineNumber++;

        if ( -1 != i->m_value )
        {
            sprintf ( line, "  %04X   ", i->m_value );
            file << line;
        }
        else
        {
            if ( -1 != i->m_address )
            {
                sprintf ( line, "%04X ", i->m_address );
                file << line;
            }
            else
            {
                file << "     ";
            }

            if ( -1 != i->m_code )
            {
                sprintf ( line, "%03X ", i->m_code );
                file << line;
            }
            else
            {
                file << "    ";
            }
        }

        if ( -1 != i->m_inclusion )
        {
            sprintf ( line, "=%2d ", i->m_inclusion );
            file << line;
        }
        else
        {
            file << "    ";
        }

        sprintf ( line, "%6d ", lineNumber);
        file << line;

        if ( -1 != i->m_macro )
        {
            sprintf ( line, "+%2d ", i->m_macro );
            file << line;
        }
        else
        {
            file << "    ";
        }

        file << i->m_line << std::endl;
    }
}

void AsmKcpsm3CodeListing::setOPcode ( int opCode,
                                       CompilerBase::SourceLocation location,
                                       unsigned int address )
{
}

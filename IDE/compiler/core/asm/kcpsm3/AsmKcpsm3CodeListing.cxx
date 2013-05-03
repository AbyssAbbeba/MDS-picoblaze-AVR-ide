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

// Standard headers.
#include <fstream>

AsmKcpsm3CodeListing::LstLine::LstLine()
{
    m_address    = -1;
    m_code       = -1;
    m_macro      = -1;
    m_inclusion  = -1;
    m_value      = -1;
}

AsmKcpsm3CodeListing::LstLine::LstLine ( const char * line )
{
    m_address    = -1;
    m_code       = -1;
    m_macro      = -1;
    m_inclusion  = -1;
    m_value      = -1;
    m_line       = line;
}

AsmKcpsm3CodeListing::AsmKcpsm3CodeListing ( CompilerSemanticInterface * compilerCore,
                                             CompilerOptions * opts )
                                           : m_compilerCore ( compilerCore ),
                                             m_opts ( opts )
{
    m_numberOfFiles = 0;
    m_numberOfMacros = 0;
};

AsmKcpsm3CodeListing::~AsmKcpsm3CodeListing()
{
}

void AsmKcpsm3CodeListing::loadSourceFiles()
{
    std::fstream file;
    int fileNumber = -1;
    char line [ MAX_LINE_LENGTH ];

    m_numberOfFiles = m_compilerCore->listSourceFiles().size();
    m_listing.resize(m_numberOfFiles);

    for ( std::vector<std::string>::const_iterator it = m_compilerCore->listSourceFiles().begin();
          it != m_compilerCore->listSourceFiles().end();
          it++ )
    {
        fileNumber++;
        file.open ( *it, std::fstream::in );

        if ( false == file.is_open())
        {
            m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                                QObject::tr("unable to open ").toStdString() + "\"" + *it  + "\"" );
            return;
        }

        // Iterate over lines in the file
        while ( false == file.eof() )
        {
            file.getline ( line, MAX_LINE_LENGTH );
            m_listing[fileNumber].push_back ( LstLine ( line ) );
        }

        file.close();
    }
}

void AsmKcpsm3CodeListing::printCodeListing ( std::ostream & out,
                                              unsigned int & lineNumber,
                                              unsigned int fileNumber,
                                              unsigned int inclusionLevel,
                                              unsigned int macroLevel ) const
{
    char line [ 16 ];

    for ( std::vector<LstLine>::const_iterator i = m_listing[fileNumber].cbegin();
          i != m_listing[fileNumber].cend();
          i++ )
    {
        lineNumber++;

        if ( -1 != i->m_value )
        {
            sprintf ( line, "  %04X     ", i->m_value );
            out << line;
        }
        else
        {
            if ( -1 != i->m_address )
            {
                sprintf ( line, "%04X ", i->m_address );
                out << line;
            }
            else
            {
                out << "     ";
            }

            if ( -1 != i->m_code )
            {
                sprintf ( line, "%05X ", i->m_code );
                out << line;
            }
            else
            {
                out << "      ";
            }
        }

        if ( 0 != inclusionLevel )
        {
            sprintf ( line, "=%d ", inclusionLevel );
            out << line;
            if ( inclusionLevel < 10 )
            {
                out << " ";
            }
        }
        else
        {
            out << "    ";
        }

        sprintf ( line, "%6d ", lineNumber);
        out << line;

        if ( 0 != macroLevel )
        {
            sprintf ( line, "+%d ", macroLevel );
            out << line;
            if ( macroLevel < 10 )
            {
                out << " ";
            }
        }
        else
        {
            out << "    ";
        }

        out << i->m_line << std::endl;

        if ( -1 != i->m_macro )
        {
            printCodeListing ( out, lineNumber, i->m_macro, inclusionLevel, ( 1 + macroLevel ) );
        }
        if ( -1 != i->m_inclusion )
        {
            printCodeListing ( out, lineNumber, i->m_inclusion, ( 1 + inclusionLevel ), macroLevel );
        }
    }
}

void AsmKcpsm3CodeListing::output()
{
    if ( m_opts->m_lstFile.empty() )
    {
        return;
    }

    if ( true == m_opts->m_makeBackupFiles )
    {
        rename(m_opts->m_lstFile.c_str(), (m_opts->m_lstFile + "~").c_str());
    }

    std::ofstream file ( m_opts->m_lstFile, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to open ").toStdString() + "\"" + m_opts -> m_lstFile  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("unable to write to ").toStdString() + "\"" + m_opts -> m_lstFile  + "\"" );
        return;
    }
}

inline bool AsmKcpsm3CodeListing::checkLocation ( CompilerBase::SourceLocation location )
{
    if ( -1 != location.m_fileNumber && (size_t)location.m_fileNumber < (m_numberOfFiles + m_numberOfMacros) )
    {
        if ( 0 < location.m_lineStart && (size_t)location.m_lineStart <= m_listing[location.m_fileNumber].size() )
        {
            // Location is valid.
            return true;
        }
    }

    // Location is NOT valid.
    m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                        QObject::tr("some of the source code files were aparently changed during compilation").toStdString() );
    return false;
}

void AsmKcpsm3CodeListing::setInclusion ( CompilerBase::SourceLocation location,
                                          int fileNumber )
{
    if ( ( true == checkLocation ( location ) ) && ( -1 != fileNumber ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_inclusion = fileNumber;
    }
}

void AsmKcpsm3CodeListing::setCode ( CompilerBase::SourceLocation location,
                                     int code,
                                     int address )
{
    if ( true == checkLocation ( location ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_code = code;
        m_listing[location.m_fileNumber][location.m_lineStart].m_address = address;
    }
}

void AsmKcpsm3CodeListing::setValue ( CompilerBase::SourceLocation location,
                                      int value )
{
    if ( true == checkLocation ( location ) )
    {
        location.m_lineStart--;
        m_listing[location.m_fileNumber][location.m_lineStart].m_value = value;
    }
}

void AsmKcpsm3CodeListing::expandMacro ( CompilerBase::SourceLocation location,
                                         const CompilerStatement * definition,
                                         CompilerStatement * expansion )
{
    if ( false == checkLocation ( location ) )
    {
        return;
    }

    m_numberOfMacros++;

    location.m_lineStart--;
    m_listing[location.m_fileNumber][location.m_lineStart].m_macro = (m_numberOfFiles + m_numberOfMacros - 1);

    unsigned int lineNumber = 0;
    rewriteMacroLoc(lineNumber, expansion);
    m_listing.resize(m_numberOfFiles + m_numberOfMacros);
    copyMacroBody(definition);
}

void AsmKcpsm3CodeListing::copyMacroBody ( const CompilerStatement * macro )
{
    if ( NULL == macro )
    {
        return;
    }

    LstLine line = m_listing[macro->location().m_fileNumber][macro->location().m_lineStart - 1];
    m_listing[m_numberOfFiles + m_numberOfMacros - 1].push_back(line);

    copyMacroBody(macro->m_branch);
    copyMacroBody(macro->m_next);
}

void AsmKcpsm3CodeListing::rewriteMacroLoc ( unsigned int & lineNumber,
                                             CompilerStatement * macro )
{
    if ( NULL == macro )
    {
        return;
    }

    lineNumber++;

    macro->m_userData++;
    macro->m_location.m_fileNumber = ( m_numberOfFiles + m_numberOfMacros - 1 );
    macro->m_location.m_lineStart = lineNumber;
    macro->m_location.m_lineEnd = lineNumber;

    rewriteMacroLoc(lineNumber, macro->m_branch);
    rewriteMacroLoc(lineNumber, macro->m_next);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3CodeListing * codeListing )
{
    unsigned int lineNumber = 0;
    codeListing->printCodeListing(out, lineNumber);
    return out;
}

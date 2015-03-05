// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Assembler
 * @file AsmStringTable.cxx
 */
// =============================================================================

#include "AsmStringTable.h"

// Standard headers.
#include <ctime>
#include <cstdio>
#include <cstring>
#include <fstream>

const std::map<std::string, AsmStringTable::PredefinedStringID> AsmStringTable::PREDEFINED_STRINGS =
{
    { "__DATE__", PRE_DEF_DATE },
    { "__TIME__", PRE_DEF_TIME },
    { "__FILE__", PRE_DEF_FILE },
    { "__LINE__", PRE_DEF_LINE }
};

void AsmStringTable::add ( const std::string & name,
                           const std::string & value,
                           const CompilerSourceLocation * location )
{
    if ( true == find(name) )
    {
        if ( nullptr == location )
        {
            m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "string already defined: " ).toStdString()
                                                            + '"' + name + '"' );
        }
        else
        {
            m_compilerCore -> semanticMessage ( *location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "string already defined: " ).toStdString()
                                                            + '"' + name + '"' );
        }
    }

    if ( nullptr == location )
    {
        m_table.insert(std::pair<std::string,String>(name, {value, CompilerSourceLocation()}));
    }
    else
    {
        m_table.insert(std::pair<std::string,String>(name, {value, *location}));
    }
}

bool AsmStringTable::find ( const std::string & name ) const
{
    if ( PREDEFINED_STRINGS.cend() != PREDEFINED_STRINGS.find(name) )
    {
        return true;
    }

    return ( m_table.cend() != m_table.find(name) );
}

bool AsmStringTable::get ( const std::string & name,
                           std::string & value,
                           const CompilerSourceLocation * location )
{
    const auto predef = PREDEFINED_STRINGS.find(name);
    if ( PREDEFINED_STRINGS.cend() != predef )
    {
        switch ( predef->second )
        {
            case PRE_DEF_DATE: // __DATE__
            case PRE_DEF_TIME: // __TIME__
            {
                time_t rawtime;
                struct tm * timeinfo;

                time(&rawtime);
                timeinfo = localtime(&rawtime);

                char buffer[16];
                strftime(buffer, 16, ( ( PRE_DEF_DATE == predef->second ) ? "%b %e %Y" : "%T" ), timeinfo );
                value = buffer;

                break;
            }

            case PRE_DEF_FILE: // __FILE__
            case PRE_DEF_LINE: // __LINE__
            {
                if ( nullptr == location )
                {
                    value.clear();
                }
                else
                {
                    if ( PRE_DEF_LINE == predef->second )
                    {
                        char buffer[16];
                        sprintf(buffer, "%d", location->m_lineStart);
                        value = buffer;
                    }
                    else
                    {
                        value = m_compilerCore->getFileName(location->m_fileNumber);
                    }
                }
                break;
            }
        }

        return true;
    }

    const auto it = m_table.find(name);

    if ( m_table.cend() != it )
    {
        value = it->second.m_value;
        return true;
    }

    return false;
}

const CompilerSourceLocation * AsmStringTable::getLocation ( const std::string & name ) const
{
    const auto it = m_table.find(name);

    if ( m_table.cend() != it )
    {
        return &( it->second.m_location );
    }

    return nullptr;
}

void AsmStringTable::output()
{
    if ( true == m_opts->m_stringTable.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_stringTable, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to open: " ).toStdString()
                                                        + '"' + m_opts->m_stringTable  + '"' );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "unable to write to: " ).toStdString()
                                                        + '"' + m_opts->m_stringTable  + '"' );
        return;
    }
}

void AsmStringTable::clear()
{
    m_table.clear();
}

inline void AsmStringTable::printSymLocation ( std::ostream & out,
                                                const CompilerSourceLocation & location ) const
{
    if ( false == location.isSet() )
    {
        return;
    }

    out << m_compilerCore->locationToStr(location);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmStringTable * stringTable )
{
    for ( const auto & string : stringTable->m_table )
    {
        out << string.first;
        for ( unsigned int i = string.first.size(); i < 35; i++ )
        {
            if ( i % 2 )
            {
                out << ' ';
            }
            else
            {
                out << '.';
            }
        }
        out << ' ';
        stringTable->printSymLocation(out, string.second.m_location);
        out << ' ' << '"' << string.second.m_value << '"';
        out << std::endl;
    }

    return out;
}

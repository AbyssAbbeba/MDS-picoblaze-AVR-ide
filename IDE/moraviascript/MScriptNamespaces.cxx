// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptNamespaces.cxx
 */
// =============================================================================

#include "MScriptNamespaces.h"

// MScript language interpreter header files.
#include "MScriptInterpretInterface.h"

MScriptNamespaces::MScriptNamespaces ( MScriptInterpretInterface * interpret )
                                     : m_interpret ( interpret )
{
    m_ns = NULL;
    clear();
}

MScriptNamespaces::~MScriptNamespaces()
{
    delete m_ns;
}

MScriptNamespaces::NsDesc::~NsDesc()
{
    clear();
}

void MScriptNamespaces::NsDesc::clear()
{
    for ( std::vector<NsDesc*>::iterator it = m_contains.begin();
          it != m_contains.end();
          it++ )
    {
        delete *it;
    }
}

bool MScriptNamespaces::NsDesc::constains ( const NsDesc * ns ) const
{
    if ( ns == this )
    {
        return true;
    }

    for ( std::vector<NsDesc*>::const_iterator it = m_contains.cbegin();
          it != m_contains.cend();
          it++ )
    {
        if ( true == (*it)->constains(ns) )
        {
            return true;
        }
    }

    return false;
}

void MScriptNamespaces::leave()
{
    m_current.pop_back();
}

void MScriptNamespaces::define ( const MScriptSrcLocation & location,
                                 const std::string & ns )
{
//     current()
// 
//     m_ns
}

void MScriptNamespaces::enter ( const MScriptSrcLocation & location,
                                MScriptNamespaces::NsDesc * ns )
{
    return m_current.push_back(ns);
}

void MScriptNamespaces::defineEnter ( const MScriptSrcLocation & location,
                                      const std::string & ns )
{
    
}

MScriptNamespaces::NsDesc * MScriptNamespaces::current() const
{
    return m_current.back();
}

void MScriptNamespaces::clear()
{
    if ( NULL != m_ns )
    {
        delete m_ns;
    }
    m_ns = new NsDesc ( "::", NULL, MScriptSrcLocation() );

    m_current.clear();
    m_current.push_back(m_ns);
}


void MScriptNamespaces::NsDesc::print ( std::ostream & out,
                                        int level,
                                        std::string lineString ) const
{
    char addr[19];
    sprintf ( addr,
              "0x%08x%08x",
              (unsigned int) ( ( 0xffffffff00000000 & reinterpret_cast<long long unsigned int>(this) ) >> 32),
              (unsigned int) (   0x00000000ffffffff & reinterpret_cast<long long unsigned int>(this) ) );
    out << addr << " ";

    if ( NULL == this )
    {
        out << "<ERROR:NULL!>" << std::endl;
        return;
    }

    for ( int i = 0; i < level; i++ )
    {
        if ( '0' == lineString[i] )
        {
            out << "    ";
        }
        else
        {
            out << "  │ ";
        }
    }

    if ( NULL == m_parent )
    {
        if ( true == m_contains.empty() )
        {
            lineString[level] = '0';
        }
        out << "  █─ ";
    }
    else
    {
        if ( true == m_contains.empty() )
        {
            out << "  └─ ";
            lineString[level] = '0';
        }
        else
        {
            out << "  ├─ ";
        }
    }

    out << m_name;

    out << " {";
    out << m_location;
    out << "}";
    out << std::endl;

    level++;
    lineString += "1";
    for ( std::vector<NsDesc*>::const_iterator i = m_contains.cbegin();
          i != m_contains.cend();
          i++ )
    {
        (*i)->print(out, level, lineString);
    }
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces & namespaces )
{
    namespaces.m_ns->print(out);
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces::NsDesc * nsDesc )
{
    std::vector<const std::string*> path;

    for ( const MScriptNamespaces::NsDesc * ns = nsDesc;
          ns != NULL;
          ns = ns->m_parent )
    {
        path.push_back ( &(ns->m_name) );
    }

    for ( std::vector<const std::string*>::const_reverse_iterator i = path.crbegin();
          i != path.crend();
          i++ )
    {
        out << "::" << **i;
    }

    return out;
}

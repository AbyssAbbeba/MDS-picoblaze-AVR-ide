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
        if ( true == it->constains(ns) )
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
}

void MScriptNamespaces::enter ( const MScriptSrcLocation & location,
                                const std::string & ns )
{
}
void MScriptNamespaces::defineEnter ( const MScriptSrcLocation & location,
                                      const std::string & ns )
{
    
}

const MScriptNamespaces::NsDesc * MScriptNamespaces::current() const
{
    return m_current.back();
}

void MScriptNamespaces::clear()
{
    m_current.clear();
    if ( NULL != m_ns )
    {
        delete m_ns;
    }

    m_ns = new NsDesc ( "::", NULL );
    m_current.push_back(m_ns);
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces & namespaces )
{
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces::NsDesc * nsDesc )
{
    return out;
}

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
 * @file MScriptNamespaces.cxx
 */
// =============================================================================

#include "MScriptNamespaces.h"

// MScript language interpreter header files.
#include "MScriptInterpretInterface.h"

// Used for i18n only.
#include <QObject>

MScriptNamespaces::MScriptNamespaces ( MScriptInterpretInterface * interpret )
                                     : m_interpret ( interpret )
{
    m_rootNs = nullptr;
    clear();
}

MScriptNamespaces::~MScriptNamespaces()
{
    delete m_rootNs;
}

MScriptNamespaces::NsDesc::~NsDesc()
{
    clear();
}

void MScriptNamespaces::NsDesc::clear()
{
    for ( auto ns : m_contains )
    {
        delete ns;
    }
}

void MScriptNamespaces::leave()
{
    m_current.pop_back();
}

MScriptNamespaces::NsDesc * MScriptNamespaces::analyseId ( const std::string & id,
                                                           std::string & bareId,
                                                           const MScriptSrcLocation * location ) const
{
    NsDesc * result;
    size_t pos = id.find("::");

    if ( std::string::npos == pos )
    {
        bareId = id;
        return current();
    }
    else if ( 0 == pos )
    {
        pos = 2;
        result = m_rootNs;
    }
    else
    {
        pos = 0;
        result = current();
    }

    NsDesc * resultOrig = result;
    size_t lastPos = pos;
    while ( std::string::npos != ( pos = id.find("::", lastPos) ) )
    {
        result = result->getChildByName ( id.substr(lastPos, pos - lastPos) );

        if ( nullptr == result )
        {
            if ( nullptr != location )
            {
                m_interpret->interpreterMessage ( *location,
                                                  MScriptBase::MT_ERROR,
                                                  QObject::tr ( "namespace `%1' does not exists in namespace `%2'" )
                                                              . arg ( id.substr(lastPos, pos - lastPos).c_str() )
                                                              . arg ( resultOrig->toString().c_str() )
                                                              . toStdString() );
            }
            return nullptr;
        }

        resultOrig = result;
        lastPos = ( pos + 2 );
    }

    bareId = id.substr(lastPos);
    return result;
}

MScriptNamespaces::NsDesc * MScriptNamespaces::str2ns ( const std::string & ns,
                                                        bool define )
{
    std::vector<std::string> nsVector;

    // 
    size_t pos;
    size_t lastPos = 0;
    while ( std::string::npos != ( pos = ns.find("::", lastPos) ) )
    {
        nsVector.push_back ( ns.substr(lastPos, pos - lastPos) );
        lastPos = pos + 2;
    }
    nsVector.push_back ( ns.substr(lastPos) );

    //
    NsDesc * nsDesc = m_rootNs;
    for ( const auto & nspace : nsVector )
    {
        NsDesc * child = nsDesc->getChildByName(nspace);

        if ( nullptr == child )
        {
            if ( false == define )
            {
                return nullptr;
            }

            // Define new namespace.
            child = new NsDesc ( nspace, nsDesc, MScriptSrcLocation() );
            nsDesc->m_contains.push_back(child);
        }

        nsDesc = child;
    }

    //
    return nsDesc;
}

int MScriptNamespaces::NsDesc::inheritsFrom ( const MScriptNamespaces::NsDesc * ns ) const
{
    int distance = 0;

    for ( const NsDesc * nsDesc = this;
          nullptr != nsDesc;
          nsDesc = nsDesc->m_parent )
    {
        if ( ns == nsDesc )
        {
            return distance;
        }
        distance++;
    }

    return -1;
}

MScriptNamespaces::NsDesc * MScriptNamespaces::define ( const MScriptSrcLocation & location,
                                                        const std::string & ns )
{
    std::string bareId;
    NsDesc * nsDesc = analyseId(ns, bareId, &location);

    if ( nullptr == nsDesc )
    {
        return nullptr;
    }

    if ( nullptr != nsDesc->getChildByName ( bareId ) )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "namespace `%1' already constans namespace `%2'" )
                                                      . arg ( nsDesc->toString().c_str() )
                                                      . arg ( bareId.c_str() )
                                                      . toStdString() );
        return nullptr;
    }

    nsDesc->m_contains.push_back(new NsDesc ( bareId, nsDesc, location ));
    return nsDesc->m_contains.back();
}

void MScriptNamespaces::defineEnter ( const MScriptSrcLocation & location,
                                      const std::string & ns )
{
    enter(define(location, ns));
}

void MScriptNamespaces::enter ( MScriptNamespaces::NsDesc * ns )
{
    if ( nullptr != ns )
    {
        m_current.push_back(ns);
    }
}

MScriptNamespaces::NsDesc * MScriptNamespaces::current() const
{
    return m_current.back();
}

void MScriptNamespaces::clear()
{
    if ( nullptr != m_rootNs )
    {
        delete m_rootNs;
    }
    m_rootNs = new NsDesc ( "::", nullptr, MScriptSrcLocation() );

    m_current.clear();
    m_current.push_back(m_rootNs);
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

    if ( nullptr == this )
    {
        out << "<ERROR:nullptr!>" << std::endl;
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

    if ( nullptr == m_parent )
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
    for ( const auto child : m_contains )
    {
        child->print(out, level, lineString);
    }
}

std::string MScriptNamespaces::NsDesc::toString() const
{
    std::string result;
    std::vector<const std::string*> path;

    for ( const MScriptNamespaces::NsDesc * ns = this;
          ns != nullptr;
          ns = ns->m_parent )
    {
        path.push_back ( &(ns->m_name) );
    }

    for ( std::vector<const std::string*>::const_reverse_iterator i = path.crbegin();
          i != path.crend();
          i++ )
    {
        result.append("::");
        result.append(**i);
    }

    return result;
}

MScriptNamespaces::NsDesc * MScriptNamespaces::NsDesc::getChildByName ( const std::string & name ) const
{
    for ( auto child : m_contains )
    {
        if ( name == child->m_name )
        {
            return child;
        }
    }

    return nullptr;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces & namespaces )
{
    namespaces.m_rootNs->print(out);
    return out;
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptNamespaces::NsDesc * nsDesc )
{
    out << nsDesc->toString() << "::";
    return out;
}

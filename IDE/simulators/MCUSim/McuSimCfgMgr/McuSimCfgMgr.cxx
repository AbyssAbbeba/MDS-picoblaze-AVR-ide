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
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgr.cxx
 */
// =============================================================================

#include "McuSimCfgMgr.h"

#include "McuDeviceSpec.h"
#include "McuDeviceSpecAVR8.h"
#include "McuDeviceSpecPIC8.h"
#include "AVR8/AVR8Config.h"
#include "PIC8/PIC8Config.h"
#include "McuSimCfgMgrAVR8.h"
#include "McuSimCfgMgrPIC8.h"

#include <QFile>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>
#include <QRegExp>

McuSimCfgMgr * McuSimCfgMgr::m_instance = NULL;

McuSimCfgMgr::McuSimCfgMgr()
{
    m_mcuSimCfgMgrAVR8 = new McuSimCfgMgrAVR8 ( &m_devices,
                                                &m_currentXMLElement,
                                                &m_expectedXMLElements,
                                                &m_expectCharacters,
                                                &m_auxInt0,
                                                &m_auxInt1 );

    m_mcuSimCfgMgrPIC8 = new McuSimCfgMgrPIC8 ( &m_devices,
                                                &m_currentXMLElement,
                                                &m_expectedXMLElements,
                                                &m_expectCharacters,
                                                &m_auxInt0,
                                                &m_auxInt1 );
}

McuSimCfgMgr::~McuSimCfgMgr()
{
    delete m_mcuSimCfgMgrAVR8;
    delete m_mcuSimCfgMgrPIC8;
    clear();
}

McuSimCfgMgr * McuSimCfgMgr::getInstance()
{
    if ( NULL == m_instance )
    {
        m_instance = new McuSimCfgMgr();
    }
    return m_instance;
}

inline void McuSimCfgMgr::clear()
{
    foreach ( McuDeviceSpec * item, m_devices )
    {
        delete item;
    }
    m_devices.clear();
}

bool McuSimCfgMgr::openConfigFile ( const char * filename )
{
    bool result = true;

    QFile file(filename);
    if ( false == file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "McuSimCfgMgr IO error";
        result = false;
    }

    QXmlSimpleReader reader;
    reader.setContentHandler(this);

    m_expectCharacters = false;
    m_expectedXMLElements.clear();
    m_expectedXMLElements << "mcuspecfile";

    m_currentXMLElement.clear();
    m_currentXMLElement.push("");

    if ( false == reader.parse(QXmlInputSource(&file)) )
    {
        qDebug() << "McuSimCfgMgr XML parsing failed.";
        result = false;
    }

    file.close();

    if ( false == result )
    {
        clear();
    }
    return result;
}

bool McuSimCfgMgr::characters ( const QString & characters )
{
    QString ch = characters.simplified();
    if ( ( false == m_expectCharacters ) && ( false == ch.isEmpty() ) )
    {
        qDebug() << "Unexpected characters '" + ch + "'";
        return false;
    }

    if ( ( true == m_currentXMLElement.isEmpty() ) || ( false == m_expectCharacters ) )
    {
        return true;
    }

    if ( false == m_devices.isEmpty() )
    {
        switch ( m_devices.last()->m_arch )
        {
            case MCUSim::Arch::ARCH_AVR8:
                return m_mcuSimCfgMgrAVR8->characters(ch);
            case MCUSim::Arch::ARCH_PIC8:
                return m_mcuSimCfgMgrPIC8->characters(ch);
            default:
                qDebug() << "Unknown architecture";
                return false;
        }
    }

    return true;
}

bool McuSimCfgMgr::endElement
(
    const QString & /*namespaceURI*/,
    const QString & /*localName*/,
    const QString & /*qName*/)
{
    m_currentXMLElement.pop();
    return true;
}

bool McuSimCfgMgr::startElement ( const QString & namespaceURI,
                                  const QString & localName,
                                  const QString & qName,
                                  const QXmlAttributes & atts )
{
    if ( false == m_expectedXMLElements.contains(localName) )
    {
        qDebug() << "Unexpected element: " << localName;
        return false;
    }

    m_expectedXMLElements.clear();
    m_currentXMLElement.push(localName);
    m_expectCharacters = false;

    if ( "mcuspecfile" == localName )
    {
        m_expectedXMLElements << "device";
    }
    else if ( "device" == localName )
    {
        if ( "avr8:device" == qName )
        {
            m_expectedXMLElements << "adc";
            m_devices.append(new McuDeviceSpecAVR8());
            return m_mcuSimCfgMgrAVR8->attributes(localName, atts);
        }
        else if ( "pic8:device" == qName )
        {
            m_expectedXMLElements << "instructionset";
            m_devices.append(new McuDeviceSpecPIC8());
            return m_mcuSimCfgMgrPIC8->attributes(localName, atts);
        }
        else
        {
            qDebug() << "Unknown architecture: \"" << qName << "\"";
        }
    }
    else if ( false == m_devices.isEmpty() )
    {
        switch ( m_devices.last()->m_arch )
        {
            case MCUSim::Arch::ARCH_AVR8:
                return m_mcuSimCfgMgrAVR8->startElement(namespaceURI, localName, qName, atts);
            case MCUSim::Arch::ARCH_PIC8:
                return m_mcuSimCfgMgrPIC8->startElement(namespaceURI, localName, qName, atts);
            default:
                qDebug() << "Unknown architecture";
                return false;
        }
    }
    return true;
}

bool McuSimCfgMgr::setupSimulator ( const char * mcuName,
                                    MCUSim::Config & mcuConfig ) const
{
    const QString name = mcuName;
    const int size = m_devices.size();

    int idx = -1;
    for ( int i = 0; i < size; i++ )
    {
        if ( name == m_devices[i]->m_name )
        {
            idx = i;
            break;
        }
    }

    if ( -1 == idx )
    {
        return false;
    }

    if ( mcuConfig.getArch() != m_devices[idx]->m_arch )
    {
        return false;
    }

    switch ( m_devices[idx]->m_arch )
    {
        case MCUSim::Arch::ARCH_AVR8:
            return dynamic_cast<McuDeviceSpecAVR8*>(m_devices[idx])->setupSimulator(dynamic_cast<AVR8Config&>(mcuConfig));
        case MCUSim::Arch::ARCH_PIC8:
            return dynamic_cast<McuDeviceSpecPIC8*>(m_devices[idx])->setupSimulator(dynamic_cast<PIC8Config&>(mcuConfig));
        case MCUSim::Arch::ARCH_PICOBLAZE:
            return true; // In this simulator implemetation, PicoBlaze requires no configuration.
        default:
            qDebug() << "Unknown architecture";
            return false;
    }

    return true;
}

const McuDeviceSpec * McuSimCfgMgr::getDeviceSpec ( const char * mcuName ) const
{
    const QString name = mcuName;
    const int size = m_devices.size();

    int idx = -1;
    for ( int i = 0; i < size; i++ )
    {
        if ( name == m_devices[i]->m_name )
        {
            idx = i;
            break;
        }
    }

    if ( -1 == idx )
    {
        return NULL;
    }

    return m_devices[idx];
}

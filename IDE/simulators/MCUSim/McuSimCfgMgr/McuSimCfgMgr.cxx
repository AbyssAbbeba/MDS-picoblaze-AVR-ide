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
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgr.cxx
 */
// =============================================================================

// Feature set configuration.
#include "../../../mds.h"

#include "McuSimCfgMgr.h"

#include "McuDeviceSpec.h"
#include "McuDeviceSpecAVR8.h"
#include "McuDeviceSpecPIC8.h"
#include "AVR8/AVR8Config.h"
#include "PIC8/PIC8Config.h"
#include "PicoBlaze/PicoBlazeConfig.h"
#include "McuSimCfgMgrAVR8.h"
#include "McuSimCfgMgrPIC8.h"
#include "AdjSimProcDef/AdjSimProcDef.h"

#ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    #include "McuDeviceSpecAdaptable.h"
#endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

#include <QFile>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>
#include <QRegExp>

McuSimCfgMgr * McuSimCfgMgr::m_instance = nullptr;

McuSimCfgMgr::McuSimCfgMgr()
{
  #ifdef MDS_FEATURE_AVR8
    m_mcuSimCfgMgrAVR8 = new McuSimCfgMgrAVR8 ( &m_devices,
                                                &m_currentXMLElement,
                                                &m_expectedXMLElements,
                                                &m_expectCharacters,
                                                &m_auxInt0,
                                                &m_auxInt1 );
  #else // MDS_FEATURE_AVR8
    m_mcuSimCfgMgrAVR8 = nullptr;
  #endif // MDS_FEATURE_AVR8

  #ifdef MDS_FEATURE_PIC8
    m_mcuSimCfgMgrPIC8 = new McuSimCfgMgrPIC8 ( &m_devices,
                                                &m_currentXMLElement,
                                                &m_expectedXMLElements,
                                                &m_expectCharacters,
                                                &m_auxInt0,
                                                &m_auxInt1 );
  #else // MDS_FEATURE_PIC8
    m_mcuSimCfgMgrPIC8 = nullptr;
  #endif // MDS_FEATURE_PIC8
}

McuSimCfgMgr::~McuSimCfgMgr()
{
  #ifdef MDS_FEATURE_AVR8
    delete m_mcuSimCfgMgrAVR8;
  #endif // MDS_FEATURE_AVR8

  #ifdef MDS_FEATURE_PIC8
    delete m_mcuSimCfgMgrPIC8;
  #endif // MDS_FEATURE_PIC8

    clear();
}

McuSimCfgMgr * McuSimCfgMgr::getInstance()
{
    if ( nullptr == m_instance )
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
        return false;
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
          #ifdef MDS_FEATURE_AVR8
            case MCUSim::Arch::ARCH_AVR8:
                return m_mcuSimCfgMgrAVR8->characters(ch);
          #endif // MDS_FEATURE_AVR8

          #ifdef MDS_FEATURE_PIC8
            case MCUSim::Arch::ARCH_PIC8:
                return m_mcuSimCfgMgrPIC8->characters(ch);
          #endif // MDS_FEATURE_PIC8

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

bool McuSimCfgMgr::startElement ( const QString &
                                  #if defined(MDS_FEATURE_AVR8) || defined(MDS_FEATURE_PIC8)
                                    namespaceURI
                                  #endif
                                  ,
                                  const QString & localName,
                                  const QString & qName,
                                  const QXmlAttributes &
                                  #if defined(MDS_FEATURE_AVR8) || defined(MDS_FEATURE_PIC8)
                                    atts
                                  #endif
                                )
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
          #ifdef MDS_FEATURE_AVR8
            m_expectedXMLElements << "adc";
            m_devices.append(new McuDeviceSpecAVR8());
            return m_mcuSimCfgMgrAVR8->attributes(localName, atts);
          #endif // MDS_FEATURE_AVR8
        }
        else if ( "pic8:device" == qName )
        {
          #ifdef MDS_FEATURE_PIC8
            m_expectedXMLElements << "instructionset";
            m_devices.append(new McuDeviceSpecPIC8());
            return m_mcuSimCfgMgrPIC8->attributes(localName, atts);
          #endif // MDS_FEATURE_PIC8
        }
        else
        {
            qDebug() << "Unknown architecture: \"" << qName << '"';
        }
    }
    else if ( false == m_devices.isEmpty() )
    {
        switch ( m_devices.last()->m_arch )
        {
          #ifdef MDS_FEATURE_AVR8
            case MCUSim::Arch::ARCH_AVR8:
                return m_mcuSimCfgMgrAVR8->startElement(namespaceURI, localName, qName, atts);
          #endif // MDS_FEATURE_AVR8

          #ifdef MDS_FEATURE_PIC8
            case MCUSim::Arch::ARCH_PIC8:
                return m_mcuSimCfgMgrPIC8->startElement(namespaceURI, localName, qName, atts);
          #endif // MDS_FEATURE_PIC8

            default:
                qDebug() << "Unknown architecture";
                return false;
        }
    }
    return true;
}

bool McuSimCfgMgr::setupSimulator ( const char * mcuName,
                                    MCUSimConfig & mcuConfig,
                                    const AdjSimProcDef *
                                    #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
                                    procDef
                                    #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
                                   ) const
{
    if ( false ) {}
  #ifdef MDS_FEATURE_PICOBLAZE
    else if ( MCUSim::ARCH_PICOBLAZE == mcuConfig.getArch() )
    {
        MCUSimBase::Family dev;

        if ( 0 == strcmp("kcpsm6", mcuName) )
        {
            dev = MCUSim::FAMILY_KCPSM6;
        }
        else if ( 0 == strcmp("kcpsm3", mcuName) )
        {
            dev = MCUSim::FAMILY_KCPSM3;
        }
        else if ( 0 == strcmp("kcpsm2", mcuName) )
        {
            dev = MCUSim::FAMILY_KCPSM2;
        }
        else if ( 0 == strcmp("kcpsm1", mcuName) )
        {
            dev = MCUSim::FAMILY_KCPSM1;
        }
        else if ( 0 == strcmp("kcpsm1cpld", mcuName) )
        {
            dev = MCUSim::FAMILY_KCPSM1CPLD;
        }
        else
        {
            return false;
        }

        dynamic_cast<PicoBlazeConfig&>(mcuConfig).configure(dev);
        return true;
    }
  #endif // MDS_FEATURE_PICOBLAZE
  #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    else if ( MCUSim::ARCH_ADAPTABLE == mcuConfig.getArch() )
    {
        McuDeviceSpecAdaptable(*procDef).setupSimulator(dynamic_cast<AdaptableSimConfig&>(mcuConfig));
        return true;
    }
  #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

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
      #ifdef MDS_FEATURE_AVR8
        case MCUSim::Arch::ARCH_AVR8:
            return dynamic_cast<McuDeviceSpecAVR8*>(m_devices[idx])->setupSimulator(dynamic_cast<AVR8Config&>(mcuConfig));
      #endif // MDS_FEATURE_AVR8

      #ifdef MDS_FEATURE_PIC8
        case MCUSim::Arch::ARCH_PIC8:
            return dynamic_cast<McuDeviceSpecPIC8*>(m_devices[idx])->setupSimulator(dynamic_cast<PIC8Config&>(mcuConfig));
      #endif // MDS_FEATURE_PIC8
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
        return nullptr;
    }

    return m_devices[idx];
}

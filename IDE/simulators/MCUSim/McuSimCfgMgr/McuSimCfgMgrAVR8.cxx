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
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgrAVR8.cxx
 */
// =============================================================================

#include "McuSimCfgMgrAVR8.h"

#include "McuSimCfgMgrMacros.h"
#include "McuDeviceSpecAVR8.h"
#include <QDebug>

McuSimCfgMgrAVR8::McuSimCfgMgrAVR8 ( QVector<McuDeviceSpec*> * devices,
                                     QStack<QString> * currentXMLElement,
                                     QStringList * expectedXMLElements,
                                     bool * expectCharacters,
                                     int * auxInt0,
                                     int * auxInt1 )
                                   :
                                     m_devices ( *devices ),
                                     m_currentXMLElement ( *currentXMLElement ),
                                     m_expectedXMLElements ( *expectedXMLElements ),
                                     m_expectCharacters ( *expectCharacters ),
                                     m_auxInt0 ( *auxInt0 ),
                                     m_auxInt1 ( *auxInt1 )
{
}

bool McuSimCfgMgrAVR8::characters ( const QString & ch )
{
    McuDeviceSpecAVR8 * device = dynamic_cast<McuDeviceSpecAVR8*>(m_devices.last());
    QString tag = m_currentXMLElement.top();

    if ( "availableinstructions" == tag )
    {
        static const QStringList specificInstructions = QStringList("STS_K_RR_16")
            << "ADIW"               << "CALL"               << "JMP"                << "BREAK"
            << "EICALL"             << "EIJMP"              << "ICALL"              << "IJMP"
            << "DES"                << "ELPM_Rd_Zplus"      << "ELPM_Rd_Z"          << "ELPM"
            << "MUL"                << "MULS"               << "MULSU"              << "FMUL"
            << "FMULS"              << "FMULSU"             << "LD_Rd_X"            << "LD_Rd_Xplus"
            << "LD_Rd_minusX"       << "LDD_Rd_Y"           << "LD_Rd_Yplus"        << "LD_Rd_minusY"
            << "LDD_Rd_Yq"          << "LDD_Rd_Z"           << "LD_Rd_Zplus"        << "LD_Rd_minusZ"
            << "LDD_Rd_Zq"          << "ST_Rr_X"            << "ST_Rr_Xplus"        << "ST_Rr_minusX"
            << "STD_Rr_Y"           << "ST_Rr_Yplus"        << "ST_Rr_minusY"       << "STD_Rr_Yq"
            << "STD_Rr_Z"           << "ST_Rr_Zplus"        << "ST_Rr_minusZ"       << "STD_Rr_Zq"
            << "LAS"                << "LAC"                << "LAT"                << "SPM"
            << "SPM_Zplus"          << "POP"                << "PUSH"               << "MOVW"
            << "SBIW"               << "RCALL"              << "LDS_Rd_k_16b"       << "LDS_Rd_k"
            << "LPM"                << "LPM_Rd_Z"           << "LPM_Rd_Zplus"       << "STS_k_Rr_16bb"
            << "STS_k_Rr";

        for ( int i = 0; i < AVR8InsNames::SPECI__MAX__; i++ )
        {
            device->m_instructionSet.m_availableInstructions[i] = false;
        }

        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }

            int instruction = specificInstructions.indexOf(item);
            if ( -1 == instruction )
            {
                qDebug() << "Unknown instruction: " << item;
                return false;
            }

            device->m_instructionSet.m_availableInstructions[instruction] = true;
        }
    }
    else if ( "possibleinterrupts" == tag )
    {
        for ( int i = 0; i < AVR8InterruptController::INTVEC__MAX__; i++ )
        {
            device->m_interruptController.m_possibleInterrupts[i] = false;
        }

        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }
            AVR8InterruptController::InterruptVector interrupt;

            if ( "RESET" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_RESET;
            }
            else if ( "INT0" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_INT0;
            }
            else if ( "INT1" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_INT1;
            }
            else if ( "TIMER2_COMP" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER2_COMP;
            }
            else if ( "TIMER2_OVF" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER2_OVF;
            }
            else if ( "TIMER1_CAPT" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER1_CAPT;
            }
            else if ( "TIMER1_COMPA" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER1_COMPA;
            }
            else if ( "TIMER1_COMPB" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER1_COMPB;
            }
            else if ( "TIMER1_OVF" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER1_OVF;
            }
            else if ( "TIMER0_OVF" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TIMER0_OVF;
            }
            else if ( "SPI_STC" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_SPI_STC;
            }
            else if ( "USART_RXC" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_USART_RXC;
            }
            else if ( "USART_UDRE" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_USART_UDRE;
            }
            else if ( "USART_TXC" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_USART_TXC;
            }
            else if ( "ADC" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_ADC;
            }
            else if ( "EE_RDY" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_EE_RDY;
            }
            else if ( "ANA_COMP" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_ANA_COMP;
            }
            else if ( "TWI" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_TWI;
            }
            else if ( "SPM_RDY" == item )
            {
                interrupt = AVR8InterruptController::INTVEC_SPM_RDY;
            }
            else
            {
                qDebug() << "Unindentified interrupt ID: " << item;
                return false;
            }

            device->m_interruptController.m_possibleInterrupts[interrupt] = true;
        }
    }
    else if ( "hasport" == tag )
    {
        static const QStringList ports = QStringList("A") << "B" << "C" << "D";

        for ( unsigned int i = 0; i < AVR8IO::NUMBER_OF_PORTS; i++ )
        {
            device->m_io.m_hasPort[i] = false;
        }

        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }

            int port = ports.indexOf(item);
            if ( -1 == port )
            {
                qDebug() << "Unknown port: " << item;
                return false;
            }

            device->m_io.m_hasPort[port] = true;
        }
    }
    else if ( "availablepins" == tag )
    {
        static const QStringList pinNames = QStringList("PA0")
            << "PA1" << "PA2" << "PA3" << "PA4" << "PA5" << "PA6" << "PA7"
            << "PB0" << "PB1" << "PB2" << "PB3" << "PB4" << "PB5" << "PB6" << "PB7"
            << "PC0" << "PC1" << "PC2" << "PC3" << "PC4" << "PC5" << "PC6" << "PC7"
            << "PD0" << "PD1" << "PD2" << "PD3" << "PD4" << "PD5" << "PD6" << "PD7";

        for ( unsigned int i = 0; i < AVR8IO::NUMBER_OF_PINS; i++ )
        {
            device->m_io.m_availablePins[i] = false;
        }

        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }

            int pin = pinNames.indexOf(item);
            if ( -1 == pin )
            {
                qDebug() << "Unknown port: " << item;
                return false;
            }

            device->m_io.m_availablePins[pin] = true;
        }
    }

    return true;
}

bool McuSimCfgMgrAVR8::startElement ( const QString & /*namespaceURI*/,
                                      const QString & localName,
                                      const QString & /*qName*/,
                                      const QXmlAttributes & atts )
{
    if ( "adc" == localName )
    {
        m_expectedXMLElements << "analogcomparator";
    }
    else if ( "analogcomparator" == localName )
    {
        m_expectedXMLElements << "bootloader";
    }
    else if ( "bootloader" == localName )
    {
        m_expectedXMLElements << "clockcontrol";
    }
    else if ( "clockcontrol" == localName )
    {
        m_expectedXMLElements << "dataeeprom";
    }
    else if ( "dataeeprom" == localName )
    {
        m_expectedXMLElements << "datamemory";
    }
    else if ( "datamemory" == localName )
    {
        m_expectedXMLElements << "ioreginitvalues";
    }
    else if ( "ioreginitvalues" == localName )
    {
        m_expectedXMLElements<< "ioreginitvalue" << "iomem2initvalues";
    }
    else if ( "ioreginitvalue" == localName )
    {
        m_expectedXMLElements << "ioreginitvalue" << "bit" << "iomem2initvalues";
    }
    else if ( "bit" == localName )
    {
        m_expectedXMLElements << "ioreginitvalue" << "bit" << "iomem2initvalues";
    }
    else if ( "iomem2initvalues" == localName )
    {
        m_auxInt0 = -1;
        m_expectedXMLElements << "externalinterrupts" << "initvalues";
    }
    else if ( "initvalues" == localName )
    {
        m_auxInt0++;
        m_auxInt1 = -1;
        m_expectedXMLElements << "externalinterrupts" << "initvalues" << "initvalue";
    }
    else if ( "initvalue" == localName )
    {
        m_auxInt1++;
        m_expectedXMLElements << "externalinterrupts" << "initvalues" << "initvalue";
    }
    else if ( "externalinterrupts" == localName )
    {
        m_expectedXMLElements << "fusesandlocks";
    }
    else if ( "fusesandlocks" == localName )
    {
        m_expectedXMLElements << "instructionset";
    }
    else if ( "instructionset" == localName )
    {
        m_expectedXMLElements << "availableinstructions";
    }
    else if ( "availableinstructions" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "interruptcontroller";
    }
    else if ( "interruptcontroller" == localName )
    {
        m_expectedXMLElements << "possibleinterrupts";
    }
    else if ( "possibleinterrupts" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "io";
    }
    else if ( "io" == localName )
    {
        m_expectedXMLElements << "specfuncmap";
    }
    else if ( "specfuncmap" == localName )
    {
        m_expectedXMLElements << "hasport" << "pin";
    }
    else if ( "pin" == localName )
    {
        m_expectedXMLElements << "hasport" << "pin";
    }
    else if ( "hasport" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "availablepins";
    }
    else if ( "availablepins" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "isp";
    }
    else if ( "isp" == localName )
    {
        m_expectedXMLElements << "parallelprog";
    }
    else if ( "parallelprog" == localName )
    {
        m_expectedXMLElements << "programmemory";
    }
    else if ( "programmemory" == localName )
    {
        m_expectedXMLElements << "spi";
    }
    else if ( "spi" == localName )
    {
        m_expectedXMLElements << "systemcontrol";
    }
    else if ( "systemcontrol" == localName )
    {
        m_expectedXMLElements << "timercounter0";
    }
    else if ( "timercounter0" == localName )
    {
        m_expectedXMLElements << "timercounter1";
    }
    else if ( "timercounter1" == localName )
    {
        m_expectedXMLElements << "timercounter2";
    }
    else if ( "timercounter2" == localName )
    {
        m_expectedXMLElements << "watchdogtimer";
    }
    else if ( "watchdogtimer" == localName )
    {
        m_expectedXMLElements << "twi";
    }
    else if ( "twi" == localName )
    {
        m_expectedXMLElements << "usart";
    }
    else if ( "usart" == localName )
    {
        m_expectedXMLElements << "device";
    }
    else
    {
        qDebug() << "Unknown element: " << localName;
        return false;
    }

    return attributes(localName, atts);
}

inline bool McuSimCfgMgrAVR8::attributes ( const QString & localName,
                                           const QXmlAttributes & atts )
{
    int numberOf = atts.length();
    McuDeviceSpecAVR8 * device = dynamic_cast<McuDeviceSpecAVR8*>(m_devices.last());

    if ( "device" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "family" == atts.localName(i) )
            {
                if ( "megaAVR" == atts.value(i) )
                {
                    device->m_family = MCUSim::Family::FAMILY_MEGAAVR;
                }
                else if ( "tinyAVR" == atts.value(i) )
                {
                    device->m_family = MCUSim::Family::FAMILY_TINYAVR;
                }
                else if ( "XMEGA" == atts.value(i) )
                {
                    device->m_family = MCUSim::Family::FAMILY_XMEGA;
                }
                else if ( "AVR32" == atts.value(i) )
                {
                    device->m_family = MCUSim::Family::FAMILY_AVR32;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else if ( "name" == atts.localName(i) )
            {
                device->m_name = atts.value(i);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "adc" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_adc.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_adc.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "analogcomparator" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_analogComparator.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_analogComparator.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "bootloader" == localName )
    {
        CHECK_NO_OF_ATTRS(9);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_bootLoader.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_bootLoader.m_enabled = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "minProgTime" == atts.localName(i) )
            {
                device->m_bootLoader.m_minProgTime = atts.value(i).toFloat(&ok);
            }
            else if ( "maxProgTime" == atts.localName(i) )
            {
                device->m_bootLoader.m_maxProgTime = atts.value(i).toFloat(&ok);
            }
            else if ( "pageSize" == atts.localName(i) )
            {
                device->m_bootLoader.m_pageSize = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "bootResetAddress0" == atts.localName(i) )
            {
                device->m_bootLoader.m_bootResetAddress[0] = atts.value(i).toInt(&ok, 0);
            }
            else if ( "bootResetAddress1" == atts.localName(i) )
            {
                device->m_bootLoader.m_bootResetAddress[1] = atts.value(i).toInt(&ok, 0);
            }
            else if ( "bootResetAddress2" == atts.localName(i) )
            {
                device->m_bootLoader.m_bootResetAddress[2] = atts.value(i).toInt(&ok, 0);
            }
            else if ( "bootResetAddress3" == atts.localName(i) )
            {
                device->m_bootLoader.m_bootResetAddress[3] = atts.value(i).toInt(&ok, 0);
            }
            else if ( "rwwSectionSize" == atts.localName(i) )
            {
                device->m_bootLoader.m_rwwSectionSize = atts.value(i).toUInt(&ok, 0);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "dataeeprom" == localName )
    {
        CHECK_NO_OF_ATTRS(3);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_dataEEPROM.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_dataEEPROM.m_enabled = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "size" == atts.localName(i) )
            {
                device->m_dataEEPROM.m_size = atts.value(i).toUInt(&ok, 0);
                if ( device->m_dataEEPROM.m_size <= 256)
                {
                    device->m_dataEEPROM.m_addrRegWidth = 8;
                }
                else
                {
                    device->m_dataEEPROM.m_addrRegWidth = 16;
                }
            }
            else if ( "writeTime" == atts.localName(i) )
            {
                device->m_dataEEPROM.m_writeTime = atts.value(i).toFloat(&ok);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "datamemory" == localName )
    {
        CHECK_NO_OF_ATTRS(5);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "regFileSize" == atts.localName(i) )
            {
                device->m_dataMemory.m_regFileSize = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "sramSize" == atts.localName(i) )
            {
                device->m_dataMemory.m_sramSize = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "ioRegSize" == atts.localName(i) )
            {
                device->m_dataMemory.m_ioRegSize = atts.value(i).toUInt(&ok, 0);
                if ( true == ok )
                {
                    device->m_dataMemory.m_ioRegInitValues = new uint32_t[device->m_dataMemory.m_ioRegSize];
                    device->m_dataMemory.m_ioRegRandomInit = new uint8_t[device->m_dataMemory.m_ioRegSize];
                    device->m_dataMemory.m_ioRegDesc = new McuDeviceSpecAVR8::DataMemory::IORegDesc[device->m_dataMemory.m_ioRegSize];
                }
            }
            else if ( "mem2size" == atts.localName(i) )
            {
                device->m_dataMemory.m_mem2size = atts.value(i).toUInt(&ok, 0);
                if ( true == ok )
                {
                    device->m_dataMemory.m_ioMem2InitValues = new uint32_t * [device->m_dataMemory.m_mem2size];
                    for ( unsigned int i = 0; i < device->m_dataMemory.m_mem2size; i++ )
                    {
                        device->m_dataMemory.m_ioMem2InitValues[i] = NULL;
                    }
                    device->m_dataMemory.m_mem2sizes = new unsigned int[device->m_dataMemory.m_mem2size];
                }
            }
            else if ( "spWidth" == atts.localName(i) )
            {
                device->m_dataMemory.m_spWidth = atts.value(i).toUInt(&ok, 0);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "ioreginitvalue" == localName )
    {
        CHECK_NO_OF_ATTRS(9);

        QString regName;
        QString regDesc;
        unsigned int addr = 0;
        uint32_t initValue = 0;
        uint8_t randMask = 0;

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "addr" == atts.localName(i) )
            {
                addr = ( 0xff & atts.value(i).toUInt(&ok, 0) );
            }
            else if ( "value" == atts.localName(i) )
            {
                initValue |= ( 0xff & atts.value(i).toUInt(&ok, 0) );
            }
            else if ( "writemask" == atts.localName(i) )
            {
                initValue |= ( (0xff & atts.value(i).toUInt(&ok, 0)) << 8 );
            }
            else if ( "readmask" == atts.localName(i) )
            {
                initValue |= ( (0xff & atts.value(i).toUInt(&ok, 0)) << 16);
            }
            else if ( "randommask" == atts.localName(i) )
            {
                randMask = ( 0xff & atts.value(i).toUInt(&ok, 0) );
            }
            else if ( "reserved" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    initValue |= MCUSimMemory::MFLAG_RESERVED;
                }
                else if ( "false" != atts.value(i) )
                {
                    ok = false;
                }
            }
            else if ( "virtual" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    initValue |= MCUSimMemory::MFLAG_VIRTUAL;
                }
                else if ( "false" != atts.value(i) )
                {
                    ok = false;
                }
            }
            else if ( "name" == atts.localName(i) )
            {
                regName = atts.value(i);
            }
            else if ( "desc" == atts.localName(i) )
            {
                regDesc = atts.value(i);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }

            if ( addr > device->m_dataMemory.m_ioRegSize )
            {
                qDebug() << "Address is too high: " << addr << ", tag: " << localName;
            }
        }

        m_auxInt0 = addr;
        device->m_dataMemory.m_ioRegInitValues[addr] = initValue;
        device->m_dataMemory.m_ioRegRandomInit[addr] = randMask;
        device->m_dataMemory.m_ioRegDesc[addr].m_name = regName.toStdString();
        device->m_dataMemory.m_ioRegDesc[addr].m_desc = regDesc.toStdString();

    }
    else if ( "bit" == localName )
    {
        CHECK_NO_OF_ATTRS(4);

        int bitNumber = -1;
        QString name;
        QString stip;
        QString ttip;

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "no" == atts.localName(i) )
            {
                bool ok = true;
                bitNumber = ( atts.value(i).toInt(&ok, 0) );
                if ( (false == ok) || (bitNumber < 0) || (bitNumber > 7) )
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else if ( "name" == atts.localName(i) )
            {
                name = atts.value(i);
            }
            else if ( "stip" == atts.localName(i) )
            {
                stip = atts.value(i);
            }
            else if ( "ttip" == atts.localName(i) )
            {
                ttip = atts.value(i);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( (bitNumber < 0) || (bitNumber > 7) )
            {
                INVALID_ATTR_VALUE();
            }
        }

        device->m_dataMemory.m_ioRegDesc[m_auxInt0].m_bit[bitNumber].m_name = name.toStdString();
        device->m_dataMemory.m_ioRegDesc[m_auxInt0].m_bit[bitNumber].m_ttip = stip.toStdString();
        device->m_dataMemory.m_ioRegDesc[m_auxInt0].m_bit[bitNumber].m_stip = stip.toStdString();

    }
    else if ( "iomem2initvalues" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "mem2sizes" == atts.localName(i) )
            {
                QStringList list = atts.value(i).split(QRegExp("\\s+"));
                if ( (unsigned int)(list.size()) != device->m_dataMemory.m_mem2size )
                {
                    qDebug() << "Invalid number of elemets, "
                        << list.size() << " given, "
                        << device->m_dataMemory.m_mem2size << " expected, tag: "
                        << localName << ", attr: " << atts.localName(i);
                    return false;
                }

                for ( unsigned int i = 0; i < device->m_dataMemory.m_mem2size; i ++ )
                {
                    bool ok = true;

                    device->m_dataMemory.m_mem2sizes[i] = list[i].toUInt(&ok, 0);

                    if ( false == ok )
                    {
                        INVALID_ATTR_VALUE();
                    }
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "initvalue" == localName )
    {
        CHECK_NO_OF_ATTRS(3);

        if ( 0 == device->m_dataMemory.m_mem2size )
        {
            qDebug() << "There should be no <initvalue ... />, mem2size is 0.";
            return false;
        }

        if ( (unsigned int)(m_auxInt0) > device->m_dataMemory.m_mem2size )
        {
            qDebug() << "Too many <initvalues>...</initvalues>";
            return false;
        }

        if ( 0 == m_auxInt0 )
        {
        }
        else if ( (unsigned int)(m_auxInt1) > device->m_dataMemory.m_mem2sizes[m_auxInt0] )
        {
            qDebug() << "Too many <initvalue ... />";
            return false;
        }

        uint32_t initValue = 0;
        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "value" == atts.localName(i) )
            {
                initValue |= ( (0xff & atts.value(i).toUInt(&ok, 0)) );
            }
            else if ( "writemask" == atts.localName(i) )
            {
                initValue |= ( (0xff & atts.value(i).toUInt(&ok, 0)) << 8 );
            }
            else if ( "readmask" == atts.localName(i) )
            {
                initValue |= ( (0xff & atts.value(i).toUInt(&ok, 0)) << 16 );
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }

        if ( 0 == m_auxInt1 )
        {
            device->m_dataMemory.m_ioMem2InitValues[m_auxInt0] = new uint32_t[device->m_dataMemory.m_mem2sizes[m_auxInt0]];
        }
        device->m_dataMemory.m_ioMem2InitValues[m_auxInt0][m_auxInt1] = initValue;
    }
    else if ( "externalinterrupts" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_externalInterrupts.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_externalInterrupts.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "fusesandlocks" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "defaultFuses" == atts.localName(i) )
            {
                QStringList list = atts.value(i).split(QRegExp("\\s+"));

                device->m_fusesAndLocks.m_defaultFuses = 0;
                foreach ( const QString & item, list )
                {
                    if ( 0 == item.length() )
                    {
                        continue;
                    }

                    if ( "CKSEL0" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_CKSEL0;
                    }
                    else if ( "CKSEL1" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_CKSEL1;
                    }
                    else if ( "CKSEL2" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_CKSEL2;
                    }
                    else if ( "CKSEL3" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_CKSEL3;
                    }
                    else if ( "SUT0" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_SUT0;
                    }
                    else if ( "SUT1" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_SUT1;
                    }
                    else if ( "BODEN" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_BODEN;
                    }
                    else if ( "BODLEVEL" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_BODLEVEL;
                    }
                    else if ( "BOOTRST" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_BOOTRST;
                    }
                    else if ( "BOOTSZ0" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_BOOTSZ0;
                    }
                    else if ( "BOOTSZ1" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_BOOTSZ1;
                    }
                    else if ( "EESAVE" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_EESAVE;
                    }
                    else if ( "CKOPT" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_CKOPT;
                    }
                    else if ( "SPIEN" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_SPIEN;
                    }
                    else if ( "WDTON" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_WDTON;
                    }
                    else if ( "RSTDISBL" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_RSTDISBL;
                    }
                    else if ( "JTAGEN" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_JTAGEN;
                    }
                    else if ( "OCDEN" == item )
                    {
                        device->m_fusesAndLocks.m_defaultFuses |= AVR8FusesAndLocks::FUSE_OCDEN;
                    }
                    else
                    {
                        INVALID_ATTR_VALUE();
                    }
                }
            }
            else if ( "defaultLockBits" == atts.localName(i) )
            {
                QStringList list = atts.value(i).split(QRegExp("\\s+"));

                device->m_fusesAndLocks.m_defaultLockBits = 0;
                foreach ( const QString & item, list )
                {
                    if ( 0 == item.length() )
                    {
                        continue;
                    }

                    if ( "LB1" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_LB1;
                    }
                    else if ( "LB2" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_LB2;
                    }
                    else if ( "BLB01" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_BLB01;
                    }
                    else if ( "BLB02" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_BLB02;
                    }
                    else if ( "BLB11" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_BLB11;
                    }
                    else if ( "BLB12" == item )
                    {
                        device->m_fusesAndLocks.m_defaultLockBits |= AVR8FusesAndLocks::LB_BLB12;
                    }
                    else
                    {
                        INVALID_ATTR_VALUE();
                    }
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "instructionset" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "pcWidth" == atts.localName(i) )
            {
                if ( "PCWIDTH_16" == atts.value(i) )
                {
                    device->m_instructionSet.m_pcWidth = AVR8InstructionSet::PCWIDTH_16;
                }
                else if ( "PCWIDTH_22" == atts.value(i) )
                {
                    device->m_instructionSet.m_pcWidth = AVR8InstructionSet::PCWIDTH_22;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "io" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "pullUpresistance" == atts.localName(i) )
            {
                device->m_io.m_pullUpresistance = atts.value(i).toFloat(&ok);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "pin" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        static const QStringList pinNames = QStringList("PA0")
            << "PA1" << "PA2" << "PA3" << "PA4" << "PA5" << "PA6" << "PA7"
            << "PB0" << "PB1" << "PB2" << "PB3" << "PB4" << "PB5" << "PB6" << "PB7"
            << "PC0" << "PC1" << "PC2" << "PC3" << "PC4" << "PC5" << "PC6" << "PC7"
            << "PD0" << "PD1" << "PD2" << "PD3" << "PD4" << "PD5" << "PD6" << "PD7";

        static const QStringList pinSFuncs = QStringList("T0")
            << "INT0" << "INT1" << "RESET" << "RXD" << "TXD" << "XCK"
            << "XTAL1" << "TOSC1" << "XTAL2" << "TOSC2" << "T1" << "AIN0" << "AIN1"
            << "ICP1" << "ADC5" << "SCL" << "ADC4" << "SDA" << "ADC3" << "ADC2"
            << "ADC1" << "ADC0" << "SCK" << "MISO" << "MOSI" << "OC2" << "SS"
            << "OC1B" << "OC1A";

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "name" == atts.localName(i) )
            {
                m_auxInt0 = pinNames.indexOf(atts.value(i));
                if ( -1 == m_auxInt0 )
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else if ( "sfunc" == atts.localName(i) )
            {
                m_auxInt1 = pinSFuncs.indexOf(atts.value(i));
                if ( -1 == m_auxInt1 )
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
        device->m_io.m_specFuncMap[(AVR8PinNames::SPF)m_auxInt1] = (AVR8PinNames::PIN)m_auxInt0;
    }
    else if ( "isp" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_isp.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_isp.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "parallelprog" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_parallelProg.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_parallelProg.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "programmemory" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "size" == atts.localName(i) )
            {
                device->m_programMemory.m_size = atts.value(i).toUInt(&ok, 0);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "spi" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_spi.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_spi.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "systemcontrol" == localName )
    {
        CHECK_NO_OF_ATTRS(12);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "resetTresholdLow" == atts.localName(i) )
            {
                device->m_systemControl.m_resetTresholdLow = atts.value(i).toFloat(&ok);
            }
            else if ( "resetTresholdHigh" == atts.localName(i) )
            {
                device->m_systemControl.m_resetTresholdHigh = atts.value(i).toFloat(&ok);
            }
            else if ( "minResetPulseWidth" == atts.localName(i) )
            {
                device->m_systemControl.m_minResetPulseWidth = atts.value(i).toFloat(&ok);
            }
            else if ( "powerOnResetTresholdFalling" == atts.localName(i) )
            {
                device->m_systemControl.m_powerOnResetTresholdFalling = atts.value(i).toFloat(&ok);
            }
            else if ( "powerOnResetTresholdRaising" == atts.localName(i) )
            {
                device->m_systemControl.m_powerOnResetTresholdRaising = atts.value(i).toFloat(&ok);
            }
            else if ( "tBOD" == atts.localName(i) )
            {
                device->m_systemControl.m_tBOD = atts.value(i).toFloat(&ok);
            }
            else if ( "brownOutTreshold0" == atts.localName(i) )
            {
                device->m_systemControl.m_brownOutTreshold[0] = atts.value(i).toFloat(&ok);
            }
            else if ( "brownOutTreshold1" == atts.localName(i) )
            {
                device->m_systemControl.m_brownOutTreshold[1] = atts.value(i).toFloat(&ok);
            }
            else if ( "hasPowerOnReset" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_systemControl.m_hasPowerOnReset = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_systemControl.m_hasPowerOnReset = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "hasBrownOutReset" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_systemControl.m_hasBrownOutReset = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_systemControl.m_hasBrownOutReset = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "hasExternalReset" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_systemControl.m_hasExternalReset = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_systemControl.m_hasExternalReset = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "hasWatchDog" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_systemControl.m_hasWatchDog = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_systemControl.m_hasWatchDog = false;
                }
                else
                {
                    ok = false;
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }
        }
    }
    else if ( "timercounter0" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_timerCounter0.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_timerCounter0.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "timercounter1" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_timerCounter1.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_timerCounter1.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "timercounter2" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_timerCounter2.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_timerCounter2.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "twi" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_twi.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_twi.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else if ( "usart" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_usart.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_usart.m_enabled = false;
                }
                else
                {
                    INVALID_ATTR_VALUE();
                }
            }
            else
            {
                INVALID_ATTRIBUTE();
            }
        }
    }
    else
    {
        CHECK_NO_OF_ATTRS(0);
    }

    return true;
}

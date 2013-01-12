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
 * @file McuSimCfgMgrPIC8.cxx
 */
// =============================================================================

#include "McuSimCfgMgrPIC8.h"

#include "McuSimCfgMgrMacros.h"
#include "McuDeviceSpecPIC8.h"

#include <QDebug>

McuSimCfgMgrPIC8::McuSimCfgMgrPIC8 ( QVector<McuDeviceSpec*> * devices,
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

inline PIC8PinNames::PIN McuSimCfgMgrPIC8::sfunc2pinEnum ( const QString & str,
                                                           bool * ok )
{
    using namespace PIC8PinNames;

    if ( NULL != ok )
    {
        *ok = true;
    }

    if ( "RA0" == str )
    {
        return PIN_RA0;
    }
    else if ( "RA1" == str )
    {
        return PIN_RA1;
    }
    else if ( "RA2" == str )
    {
        return PIN_RA2;
    }
    else if ( "RA3" == str )
    {
        return PIN_RA3;
    }
    else if ( "RA4" == str )
    {
        return PIN_RA4;
    }
    else if ( "RA5" == str )
    {
        return PIN_RA5;
    }
    else if ( "RA6" == str )
    {
        return PIN_RA6;
    }
    else if ( "RA7" == str )
    {
        return PIN_RA7;
    }
    else if ( "RB0" == str )
    {
        return PIN_RB0;
    }
    else if ( "RB1" == str )
    {
        return PIN_RB1;
    }
    else if ( "RB2" == str )
    {
        return PIN_RB2;
    }
    else if ( "RB3" == str )
    {
        return PIN_RB3;
    }
    else if ( "RB4" == str )
    {
        return PIN_RB4;
    }
    else if ( "RB5" == str )
    {
        return PIN_RB5;
    }
    else if ( "RB6" == str )
    {
        return PIN_RB6;
    }
    else if ( "RB7" == str )
    {
        return PIN_RB7;
    }
    else if ( "MCLR" == str )
    {
        return PIN_MCLR;
    }
    else if ( "OSC1" == str )
    {
        return PIN_OSC1;
    }
    else if ( "OSC2" == str )
    {
        return PIN_OSC2;
    }
    else if ( "VSS" == str )
    {
        return PIN_VSS;
    }
    else if ( "VDD" == str )
    {
        return PIN_VDD;
    }
    else
    {
        if ( NULL != ok )
        {
            *ok = false;
        }
        return PIN_NC;
    }
}

inline int McuSimCfgMgrPIC8::name2spfEnum ( const QString & str,
                                            bool * ok )
{
    using namespace PIC8PinNames;

    if ( NULL != ok )
    {
        *ok = true;
    }

    if ( "CLKIN" == str )
    {
        return SPF_CLKIN;
    }
    else if ( "CLKOUT" == str )
    {
        return SPF_CLKOUT;
    }
    else if ( "T0CKI" == str )
    {
        return SPF_T0CKI;
    }
    else if ( "INT" == str )
    {
        return SPF_INT;
    }
    else if ( "MCLR" == str )
    {
        return SPF_MCLR;
    }
    else if ( "CLOCK" == str )
    {
        return SPF_CLOCK;
    }
    else if ( "DATA" == str )
    {
        return SPF_DATA;
    }
    else
    {
        if ( NULL != ok )
        {
            *ok = false;
        }
        return -1;
    }
}

inline int McuSimCfgMgrPIC8::string2interruptNumber ( const QString & str,
                                                      bool * ok )
{
    if ( NULL != ok )
    {
        *ok = true;
    }

    if ( "RESET" == str )
    {
        return PIC8InterruptController::INTVEC_RESET;
    }
    else if ( "INT0" == str )
    {
        return PIC8InterruptController::INTVEC_INT0;
    }
    else if ( "TMR0" == str )
    {
        return PIC8InterruptController::INTVEC_TMR0;
    }
    else if ( "PORTB" == str )
    {
        return PIC8InterruptController::INTVEC_PORTB;
    }
    else if ( "EEPROM" == str )
    {
        return PIC8InterruptController::INTVEC_EEPROM;
    }

    if ( NULL != ok )
    {
        *ok = false;
    }
    return -1;
}

bool McuSimCfgMgrPIC8::startElement ( const QString & /*namespaceURI*/,
                                      const QString & localName,
                                      const QString & /*qName*/,
                                      const QXmlAttributes & atts )
{
    if ( "instructionset" == localName )
    {
        m_expectedXMLElements << "programmemory";
    }
    else if ( "programmemory" == localName )
    {
        m_expectedXMLElements << "idlocationsrange";
    }
    else if ( "idlocationsrange" == localName )
    {
        m_expectedXMLElements << "datamemory";
    }
    else if ( "datamemory" == localName )
    {
        m_expectedXMLElements << "register" << "configword";
    }
    else if ( "register" == localName )
    {
        m_expectedXMLElements << "register" << "bit" << "configword";
    }
    else if ( "bit" == localName )
    {
        m_expectedXMLElements << "register" << "bit" << "configword";
    }
    else if ( "configword" == localName )
    {
        m_expectedXMLElements<< "io";
    }
    else if ( "io" == localName )
    {
        m_expectedXMLElements << "specfuncmap";
    }
    else if ( "specfuncmap" == localName )
    {
        m_expectedXMLElements << "pin" << "hasport";
    }
    else if ( "pin" == localName )
    {
        m_expectedXMLElements << "pin" << "hasport";
    }
    else if ( "hasport" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "availablepins";
    }
    else if ( "availablepins" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "clockcontrol";
    }
    else if ( "clockcontrol" == localName )
    {
        m_expectedXMLElements << "stack";
    }
    else if ( "stack" == localName )
    {
        m_expectedXMLElements << "interruptcontroller";
    }
    else if ( "interruptcontroller" == localName )
    {
        m_expectedXMLElements << "possibleinterrupts";
    }
    else if ( "possibleinterrupts" == localName )
    {
        m_expectCharacters = true;
        m_expectedXMLElements << "dataeeprom";
    }
    else if ( "dataeeprom" == localName )
    {
        m_expectedXMLElements << "watchdogtimer";
    }
    else if ( "watchdogtimer" == localName )
    {
        m_expectedXMLElements << "timercounter0";
    }
    else if ( "timercounter0" == localName )
    {
        m_expectedXMLElements << "timer0wdtprescaller";
    }
    else if ( "timer0wdtprescaller" == localName )
    {
        m_expectedXMLElements << "externalinterrupts";
    }
    else if ( "externalinterrupts" == localName )
    {
        m_expectedXMLElements << "isp";
    }
    else if ( "isp" == localName )
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

bool McuSimCfgMgrPIC8::attributes ( const QString & localName,
                                    const QXmlAttributes & atts )
{
    int numberOf = atts.length();
    McuDeviceSpecPIC8 * device = dynamic_cast<McuDeviceSpecPIC8*>(m_devices.last());

    if ( "device" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        for ( int i = 0; i < numberOf; i++ )
        {
            if ( "family" == atts.localName(i) )
            {
                if ( "PIC16F" == atts.value(i) )
                {
                    device->m_family = MCUSim::Family::FAMILY_PIC16F;
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
    else if ( "instructionset" == localName )
    {
        CHECK_NO_OF_ATTRS(0);
    }
    else if ( "programmemory" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "size" == atts.localName(i) )
            {
                device->m_programMemory.m_size = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "configwordaddress" == atts.localName(i) )
            {
                device->m_programMemory.m_configWordAddress = atts.value(i).toUInt(&ok, 0);
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
    else if ( "idlocationsrange" == localName )
    {
        CHECK_NO_OF_ATTRS(2);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "endpoint0" == atts.localName(i) )
            {
                device->m_programMemory.m_idLocationsRange[0] = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "endpoint1" == atts.localName(i) )
            {
                device->m_programMemory.m_idLocationsRange[1] = atts.value(i).toUInt(&ok, 0);
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
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "size" == atts.localName(i) )
            {
                unsigned int size = atts.value(i).toUInt(&ok, 0);

                device->m_dataMemory.m_size = size;

                device->m_dataMemory.m_addrTransTab = new int [ size ];
                device->m_dataMemory.m_randomInit = new unsigned int [ size ];
                device->m_dataMemory.m_initValues = new uint32_t [ size ];
                device->m_dataMemory.m_regDescription = new McuDeviceSpecPIC8::DataMemory::RegDescription [ size ];

                for ( unsigned int i = 0; i < size; i++ )
                {
                    device->m_dataMemory.m_addrTransTab[i] = -1;
                    device->m_dataMemory.m_randomInit[i] = 0;
                    device->m_dataMemory.m_initValues[i] = 0;
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
    else if ( "register" == localName )
    {
        CHECK_NO_OF_ATTRS(9);

        std::string regName;
        std::string regDesc;
        int absoluteAddr = -1;
        uint32_t initValue = 0;
        unsigned int randomInit = 0;

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "addresses" == atts.localName(i) )
            {
                QStringList addresses = atts.value(i).split(",");

                int size = addresses.size();
                device->m_dataMemory.m_regDescription[absoluteAddr].m_numberOfAddresses = size;
                device->m_dataMemory.m_regDescription[absoluteAddr].m_addresses = new unsigned int [ size ];

                for ( int j = 0; j < size; j++ )
                {
                    int addr = addresses[j].toUInt(&ok, 0);

                    if ( 0 == j )
                    {
                        absoluteAddr = addr;
                        if ( -1 == absoluteAddr || (unsigned int)absoluteAddr >= device->m_dataMemory.m_size )
                        {
                            ok = false;
                            break;
                        }
                    }

                    if ( (unsigned int)addr >= device->m_dataMemory.m_size )
                    {
                        ok = false;
                        break;
                    }

                    device->m_dataMemory.m_addrTransTab[addr] = absoluteAddr;
                    device->m_dataMemory.m_regDescription[absoluteAddr].m_addresses[j] = (unsigned int)addr;
                }
            }
            else if ( "value" == atts.localName(i) )
            {
                initValue |= ( 0xff & atts.value(i).toUInt(&ok, 0) );
            }
            else if ( "readmask" == atts.localName(i) )
            {
                initValue |= ( 0xff & atts.value(i).toUInt(&ok, 0) ) << 16;
            }
            else if ( "writemask" == atts.localName(i) )
            {
                initValue |= ( 0xff & atts.value(i).toUInt(&ok, 0) ) << 8;
            }
            else if ( "randommask" == atts.localName(i) )
            {
                randomInit = ( 0xff & atts.value(i).toUInt(&ok, 0) );
            }
            else if ( "reserved" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    initValue |= MCUSim::Memory::MFLAG_RESERVED;
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
                    initValue |= MCUSim::Memory::MFLAG_VIRTUAL;
                }
                else if ( "false" != atts.value(i) )
                {
                    ok = false;
                }
            }
            else if ( "name" == atts.localName(i) )
            {
                regName = atts.value(i).toStdString();
            }
            else if ( "desc" == atts.localName(i) )
            {
                regDesc = atts.value(i).toStdString();
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

        if ( -1 == absoluteAddr || (unsigned int)absoluteAddr >= device->m_dataMemory.m_size )
        {
            qDebug() << "ERROR: -1 == absoluteAddr || absoluteAddr >= device->m_dataMemory.m_size";
            return false;
        }

        m_auxInt0 = absoluteAddr;
        device->m_dataMemory.m_initValues[absoluteAddr] = initValue;
        device->m_dataMemory.m_randomInit[absoluteAddr] = randomInit;
        device->m_dataMemory.m_regDescription[absoluteAddr].m_name = regName;
        device->m_dataMemory.m_regDescription[absoluteAddr].m_desc = regDesc;
    }
    else if ( "bit" == localName )
    {
        CHECK_NO_OF_ATTRS(4);

        int no = -1;
        std::string name;
        std::string stip;
        std::string ttip;

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "no" == atts.localName(i) )
            {
                no = ( int ) ( atts.value(i).toUInt(&ok, 0) );
                if ( -1 == no || no > 7 )
                {
                    ok = false;
                }
            }
            else if ( "name" == atts.localName(i) )
            {
                name = atts.value(i).toStdString();
            }
            else if ( "stip" == atts.localName(i) )
            {
                stip = atts.value(i).toStdString();
            }
            else if ( "ttip" == atts.localName(i) )
            {
                ttip = atts.value(i).toStdString();
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

        if ( -1 == no || no > 7 )
        {
            qDebug() << "-1 == no || no > 7";
            return false;
        }

        device->m_dataMemory.m_regDescription[m_auxInt0].m_bit[no].m_name = name;
        device->m_dataMemory.m_regDescription[m_auxInt0].m_bit[no].m_ttip = ttip;
        device->m_dataMemory.m_regDescription[m_auxInt0].m_bit[no].m_stip = stip;
    }
    else if ( "configword" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "defaultcfgword" == atts.localName(i) )
            {
                device->m_configWord.m_defaultCfgWord = ( 0xff & atts.value(i).toUInt(&ok, 0) );
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
    else if ( "io" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_io.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_io.m_enabled = false;
                }
                else
                {
                    ok = false;
                }
            }
            else if ( "pullUpresistance" == atts.localName(i) )
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
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            int func = -1;
            PIC8PinNames::PIN pin = PIC8PinNames::PIN_NC;
            bool ok = true;

            if ( "sfunc" == atts.localName(i) )
            {
                pin = sfunc2pinEnum(atts.value(i), &ok);
            }
            else if ( "name" == atts.localName(i) )
            {
                func = name2spfEnum(atts.value(i), &ok);
            }
            else
            {
                INVALID_ATTRIBUTE();
            }

            if ( false == ok )
            {
                INVALID_ATTR_VALUE();
            }

            if ( PIC8PinNames::PIN_NC == pin || -1 == func )
            {
                qDebug() << "-1 == pin || -1 == func";
                return false;
            }

            device->m_io.m_specFuncMap[func] = pin;
        }
    }
    else if ( "stack" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "size" == atts.localName(i) )
            {
                device->m_stack.m_size = atts.value(i).toUInt(&ok, 0);
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
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "size" == atts.localName(i) )
            {
                device->m_dataEEPROM.m_size = atts.value(i).toUInt(&ok, 0);
            }
            else if ( "writetime" == atts.localName(i) )
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
    else if ( "watchdogtimer" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_watchDogTimer.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_watchDogTimer.m_enabled = false;
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
            bool ok = true;

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
    else if ( "timer0wdtprescaller" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

            if ( "enabled" == atts.localName(i) )
            {
                if ( "true" == atts.value(i) )
                {
                    device->m_timer0WdtPrescaller.m_enabled = true;
                }
                else if ( "false" == atts.value(i) )
                {
                    device->m_timer0WdtPrescaller.m_enabled = false;
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
    else if ( "externalinterrupts" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

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
    else if ( "isp" == localName )
    {
        CHECK_NO_OF_ATTRS(1);

        for ( int i = 0; i < numberOf; i++ )
        {
            bool ok = true;

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
                    ok = false;
                }
            }
            else if ( "delaybetweencmdanddata" == atts.localName(i) )
            {
                device->m_isp.m_delayBetweenCmdAndData = atts.value(i).toFloat(&ok);
            }
            else if ( "progtime" == atts.localName(i) )
            {
                device->m_isp.m_progTime = atts.value(i).toFloat(&ok);
            }
            else if ( "commandset" == atts.localName(i) )
            {
                if ( "PIC16C55X" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16C55X;
                }
                else if ( "PIC16F62X" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16F62X;
                }
                else if ( "PIC16C6XX_7XX_9XX" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16C6XX_7XX_9XX;
                }
                else if ( "PIC16F87X" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16F87X;
                }
                else if ( "PIC16F84A" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16F84A;
                }
                else if ( "PIC16F8X" == atts.value(i) )
                {
                    device->m_isp.m_commandSet = PIC8ISP::COMMANDSET_PIC16F8X;
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
    else
    {
        CHECK_NO_OF_ATTRS(0);
    }

    return true;
}

bool McuSimCfgMgrPIC8::characters ( const QString & ch )
{
    McuDeviceSpecPIC8 * device = dynamic_cast<McuDeviceSpecPIC8*>(m_devices.last());
    QString tag = m_currentXMLElement.top();

    if ( "hasport" == tag )
    {
        static const QStringList ports = QStringList("A") << "B" << "C" << "D";

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
        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }

            bool ok = true;
            int pin = sfunc2pinEnum(item, &ok);
            if ( true == ok )
            {
                device->m_io.m_availablePins[pin] = true;
            }
        }
    }
    else if ( "possibleinterrupts" == tag )
    {
        QStringList list = ch.split(QRegExp("\\s+"));
        foreach ( const QString & item, list )
        {
            if ( 0 == item.length() )
            {
                continue;
            }

            bool ok = true;
            int interruptNumber = string2interruptNumber(item, &ok);
            if ( true == ok )
            {
                device->m_interruptController.m_possibleInterrupts[interruptNumber] = true;
            }
        }
    }

    return true;
}

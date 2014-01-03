#include "Avr8UsbProgDeviceDB.h"

#include <QFile>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>


Avr8UsbProgDeviceDB::Avr8UsbProgDeviceDB()
{
        if(!initiateBD())
                throw XML_PARSE_ERROR;
}

bool Avr8UsbProgDeviceDB::initiateBD()
{
        bool result=true;

        QFile file(":/Avr8UsbProg/devices.xml");
        if(!file.open(QIODevice::ReadOnly)) {
                qDebug() << "Avr8UsbProgDeviceDB XML IO error";
                result=false;
        }

        QXmlSimpleReader reader;
        reader.setContentHandler(this);

        errorMessage="";

        expectCharacters=false;
        expectedXMLElements.clear();
        expectedXMLElements << "devices";

        currentXMLElement.clear();
        currentXMLElement.push("");

        if(!reader.parse(QXmlInputSource(&file))) {
                qDebug() << "Avr8UsbProgDeviceDB XML parsing failed: " << errorString();
                result=false;
        }

        file.close();
        return result;
}

int Avr8UsbProgDeviceDB::size() const
{
        return devices.size();
}

const Avr8UsbProgDeviceDB::Device & Avr8UsbProgDeviceDB::at(int idx) const
{
        return *(devices[idx]);
}

const Avr8UsbProgDeviceDB::Device & Avr8UsbProgDeviceDB::operator[](int idx) const
{
        return *(devices[idx]);
}

bool Avr8UsbProgDeviceDB::characters(const QString & _ch)
{
        QString ch=_ch.simplified();
        if(!expectCharacters && !ch.isEmpty()) {
                errorMessage="Unexpected characters '" + ch + "'";
                return false;
        }

        if(currentXMLElement.isEmpty()) {
                return true;
        }

        QString tag=currentXMLElement.top();
        QString parentTag=currentXMLElement.at(currentXMLElement.size()-2);

        if(tag == "web") {
                devices.last()->web=ch;
        } else if(tag == "byte0") {
                devices.last()->signature[0].byte=ch.toInt(0, 16);
        } else if(tag == "byte1") {
                devices.last()->signature[1].byte=ch.toInt(0, 16);
        } else if(tag == "byte2") {
                devices.last()->signature[2].byte=ch.toInt(0, 16);

        } else if(tag == "size") {
                if(parentTag == "flash") {
                        devices.last()->flash.size=ch.toInt();
                } else {
                        devices.last()->eeprom.size=ch.toInt();
                }
        } else if(tag == "page") {
                if(parentTag == "flash") {
                        devices.last()->flash.page=ch.toInt();
                } else {
                        devices.last()->eeprom.page=ch.toInt();
                }
        } else if(tag == "delay") {
                if(parentTag == "flash") {
                        devices.last()->flash.delay=int(ch.toFloat() * 1000);
                } else {
                        devices.last()->eeprom.delay=int(ch.toFloat() * 1000);
                }
        } else if(tag == "bit") {
                if(parentTag == "flb") {
                        devices.last()->fuses.flb.bitDescription[bitNumber]=ch;
                } else if(parentTag == "fhb") {
                        devices.last()->fuses.fhb.bitDescription[bitNumber]=ch;
                } else if(parentTag == "feb") {
                        devices.last()->fuses.feb.bitDescription[bitNumber]=ch;
                } else if(parentTag == "lockbits") {
                        devices.last()->lockBits.bitDescription[bitNumber]=ch;
                }
        }

        return true;
}

bool Avr8UsbProgDeviceDB::endElement(
        const QString & /*namespaceURI*/,
        const QString & /*localName*/,
        const QString & /*qName*/)
{
        currentXMLElement.pop();
        return true;
}

bool Avr8UsbProgDeviceDB::startElement(
        const QString & /*namespaceURI*/,
        const QString & localName,
        const QString & /*qName*/,
        const QXmlAttributes & atts)
{
        if(!expectedXMLElements.contains(localName)) {
                errorMessage=QString("Unexpected element: %1").arg(localName);
                return false;
        }

        currentXMLElement.push(localName);
        expectCharacters=false;

        if(localName == "devices") {
                expectedXMLElements.clear();
                expectedXMLElements << "mcu";

        } else if(localName == "mcu") {
                expectedXMLElements.clear();
                expectedXMLElements << "web";
                devices.append(new Device());

        } else if(localName == "web") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "signature";

        } else if(localName == "signature") {
                expectedXMLElements.clear();
                expectedXMLElements << "byte0";

        } else if(localName == "byte0") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "byte1";

        } else if(localName == "byte1") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "byte2";

        } else if(localName == "byte2") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "flash";

        } else if(localName == "flash") {
                expectedXMLElements.clear();
                expectedXMLElements << "size";

        } else if(localName == "size") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "page";

        } else if(localName == "page") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "delay";

        } else if(localName == "delay") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "eeprom";
                expectedXMLElements << "fuses";

        } else if(localName == "eeprom") {
                expectedXMLElements.clear();
                expectedXMLElements << "size";

        } else if(localName == "fuses") {
                expectedXMLElements.clear();
                expectedXMLElements << "feb";

        } else if(localName == "flb") {
                expectedXMLElements.clear();
                expectedXMLElements << "bit";

        } else if(localName == "fhb") {
                expectedXMLElements.clear();
                expectedXMLElements << "bit";

        } else if(localName == "feb") {
                expectedXMLElements.clear();
                expectedXMLElements << "bit";

        } else if(localName == "lockbits") {
                expectedXMLElements.clear();
                expectedXMLElements << "bit";

        } else if(localName == "bit") {
                expectCharacters=true;
                expectedXMLElements.clear();
                expectedXMLElements << "bit";
                expectedXMLElements << "fhb";
                expectedXMLElements << "flb";
                expectedXMLElements << "lockbits";
                expectedXMLElements << "mcu";

        } else {
                errorMessage=QString("Unknown element: %1").arg(localName);
                return false;
        }

        return parseAttributes(localName, atts);
}

bool Avr8UsbProgDeviceDB::parseAttributes(const QString &localName, const QXmlAttributes &atts)
{
        int length=atts.length();

        if(localName == "devices") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "mcu") {
                if(length != 3) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                for(int i=0; i<length; i++) {
                        if(atts.localName(i) == "name") {
                                devices.last()->name=atts.value(i);

                        } else if(atts.localName(i) == "erasedelay") {
                                devices.last()->eraseDelay=int(atts.value(i).toFloat() * 1000);

                        } else if(atts.localName(i) == "numberofcalibrationbytes") {
                                devices.last()->numberOfCalibrationBytes=atts.value(i).toInt();

                        } else {
                                errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                                return false;
                        }
                }
        } else if(localName == "signature") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "byte0") {
                if(length != 1) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                if(atts.localName(0) == "address") {
                        int value=atts.value(0).toInt(0, 16);
                        if(value < 0 || value > 0xffff) {
                                errorMessage=QString("Invalid attribute value, tag: %1, attribute: %1, value: %1").arg(localName).arg(atts.localName(0)).arg(atts.value(0));
                                return false;
                        }
                        devices.last()->signature[0].address=(unsigned char)value;
                } else {
                        errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                        return false;
                }

        } else if(localName == "byte1") {
                if(length != 1) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                if(atts.localName(0) == "address") {
                        int value=atts.value(0).toInt();
                        if(value < 0 || value > 0xffff) {
                                errorMessage=QString("Invalid attribute value, tag: %1, attribute: %1, value: %1").arg(localName).arg(atts.localName(0)).arg(atts.value(0));
                                return false;
                        }
                        devices.last()->signature[1].address=(unsigned char)value;
                } else {
                        errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                        return false;
                }
        } else if(localName == "byte2") {
                if(length != 1) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                if(atts.localName(0) == "address") {
                        int value=atts.value(0).toInt();
                        if(value < 0 || value > 0xffff) {
                                errorMessage=QString("Invalid attribute value, tag: %1, attribute: %1, value: %1").arg(localName).arg(atts.localName(0)).arg(atts.value(0));
                                return false;
                        }
                        devices.last()->signature[2].address=(unsigned char)value;
                } else {
                        errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                        return false;
                }
        } else if(localName == "flash") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "eeprom") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "size") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "page") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "delay") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "fuses") {
                if(length != 1) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                if(atts.localName(0) == "delay") {
                        devices.last()->fuses.delay=int(atts.value(0).toFloat() * 1000);

                } else {
                        errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                        return false;
                }

        } else if(localName == "flb") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "fhb") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "feb") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "lockbits") {
                if(length) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }
        } else if(localName == "bit") {
                if(length < 2 || length > 3) {
                        errorMessage="Invalid number of attributes";
                        return false;
                }

                QString parentElement=currentXMLElement.at(currentXMLElement.size()-2);
                QString bitName;
                bool defaultState = false;

                for(int i=0; i<length; i++) {
                        if(atts.localName(i) == "name") {
                                bitName=atts.value(i).toUpper();

                        } else if(atts.localName(i) == "default") {
                                if(atts.value(i) == "0")
                                        defaultState=false;
                                else
                                        defaultState=true;

                        } else if(atts.localName(i) == "i") {
                                bitNumber=atts.value(i).toInt();
                                if(bitNumber < 0 || bitNumber > 7) {
                                        errorMessage=QString("Invalid attribute value, tag: %1, attribute: %1, value: %1").arg(localName).arg(atts.localName(i)).arg(atts.value(i));
                                        return false;
                                }
                        } else {
                                errorMessage=QString("Invalid attribute, tag: %1").arg(localName);
                                return false;
                        }
                }

                if(parentElement == "lockbits") {
                        devices.last()->lockBits.bitName[bitNumber]=bitName;
                } else if(parentElement == "flb") {
                        devices.last()->fuses.flb.bitName[bitNumber]=bitName;
                        devices.last()->fuses.flb.defaultState[bitNumber]=defaultState;
                } else if(parentElement == "fhb") {
                        devices.last()->fuses.fhb.bitName[bitNumber]=bitName;
                        devices.last()->fuses.fhb.defaultState[bitNumber]=defaultState;
                } else if(parentElement == "feb") {
                        devices.last()->fuses.feb.bitName[bitNumber]=bitName;
                        devices.last()->fuses.feb.defaultState[bitNumber]=defaultState;
                }
        }

        return true;
}

const QString & Avr8UsbProgDeviceDB::errorString() {
        return errorMessage;
}

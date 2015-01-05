#include "xmlparser.h"
#include <QtGui>
//#include <QtXml>
//#include <QDomDocument>

XmlParser::XmlParser(QWidget *parent) :
    QWidget(parent)
{
    xmlComponentCnt = 0;
}
void XmlParser::refreshDirFiles()
{

}

void XmlParser::getDirFiles()
{   

    filters << "*.txt" << "*.xml";
    QDir xmlDir(QDir::homePath() + "/.mds");

    if ( false == xmlDir.exists())
    {
        return;
    }

    xmlDir.setNameFilters(filters);
    Devices = xmlDir.entryList(filters);
    Devices.replaceInStrings(".txt", "");
    Devices.replaceInStrings(".xml", "");
    // get rid of eriks bullshit
    Devices.removeOne("session");
    Devices.removeOne("config");
    Devices.sort();

    qDebug()<< "PATH:" << xmlDir;
    qDebug() << Devices;
}

QString XmlParser::removeExistingComponent(QString & componentToRemove)
{
    for ( unsigned int i = 0; i < xmlComponentCnt;  i++)
    {
        if ( componentToRemove == xmlDefinedComponent[i].xmlName )
        {
            qDebug() << componentToRemove << "component To be removed > "<< xmlDefinedComponent[i].xmlName;
            return xmlDefinedComponent[i].xmlName;
        }
    }
}

unsigned int XmlParser::insertExistingComponent(QString & componentToAdd)
{
    for ( unsigned int i = 0; i < xmlComponentCnt;  i++)
    {
        if ( componentToAdd == xmlDefinedComponent[i].xmlName )
        {
            qDebug() << componentToAdd << "component number > "<< i;
            return i;
        }
    }
    return 99;
}

void XmlParser::writeXml(unsigned int structIndex)
{
    qDebug() << "struct index >" << structIndex;
    // uncomment this when done
    QString path = QDir::homePath();

    path.append("/.mds/");
    path.append(xmlDefinedComponent[structIndex].xmlName);
    path.append(".xml");

    qDebug()<< "PATH:" << path;
    QFile outFile(path);
    //QFile outFile("C:/hratky_s_qt/VHDLgen/testsoubory/xml1.txt");

    outFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&outFile);
    // root
    out << "<component Name=" << '"' << xmlDefinedComponent[structIndex].xmlName << '"' << '>' << endl;

    // port names
    for (int i = 0; i < 17; i++)
    {
        out << "\t<portName"<< i+1 <<">" << xmlDefinedComponent[structIndex].xmlPortName[i]
            << "</portName" << i+1 << ">" << endl;
    }

    // port values
    for ( int i = 0; i < 17; i++)
    {
        out << "\t<portValue" << i+1 << " value=" << '"' << xmlDefinedComponent[structIndex].xmlPortValue[i] << '"'
        << "/>" << endl;
    }

    // port directions
    for (int  i = 0; i < 17; i++)
    {
        out << "\t<portDirection" << i+1 <<">" << xmlDefinedComponent[structIndex].xmlPortDirection[i]
            << "</portDirection" << i+1 << ">" << endl;
    }

    // bus 1-8
    for (int  i = 0; i < 17; i++)
    {
        out << "\t<bus" << i+1 << " value=" << '"';
        if( true == xmlDefinedComponent[structIndex].xmlBus[i] )
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
        out << '"' << "/>" << endl;
    }
    // signal 1-8
    for (int  i = 0; i < 17; i++)
    {
        out << "\t<signal" << i+1 << " signal=" << '"';
        if( true == xmlDefinedComponent[structIndex].xmlSignal[i] )
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
        out << '"' << "/>" << endl;
    }
    // msbNumber 1-8
    for (int  i = 0; i < 17; i++)
    {
        out << "\t<msbNumber" << i+1 << " number=" << '"' << xmlDefinedComponent[structIndex].xmlMsbNumber[i]
            << '"' << "/>" << endl;
    }
    // lsbNumber 1-8
    for (int  i = 0; i < 17; i++)
    {
        out << "\t<lsbNumber" << i+1 << " number=" << '"' << xmlDefinedComponent[structIndex].xmlLsbNumber[i]
            << '"' << "/>" << endl;
    }
    // now Write generic attributes form
    // xmlGenericComponent
    for (int i = 0; i < 8; i++)
    {
        out << "\t<genericName"<< i+1 <<">" << xmlGenericComponent[structIndex].xmlGenericName[i]
            << "</genericName" << i+1 << ">" << endl;
    }

    // generic values
    for ( int i = 0; i < 8; i++)
    {
        out << "\t<genericValue" << i+1 << " value=" << '"' << xmlGenericComponent[structIndex].xmlGenericValue[i] << '"'
        << "/>" << endl;
    }

    // port type
    for (int  i = 0; i < 8; i++)
    {
        out << "\t<portType" << i+1 <<">" << xmlGenericComponent[structIndex].xmlPortType[i]
            << "</portType" << i+1 << ">" << endl;
    }

    // bus 1-8
    for (int  i = 0; i < 8; i++)
    {
        out << "\t<genBus" << i+1 << " value=" << '"';
        if( true == xmlGenericComponent[structIndex].xmlBus[i] )
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
        out << '"' << "/>" << endl;
    }
    // constant 1-8
    for (int  i = 0; i < 8; i++)
    {
        out << "\t<constant" << i+1 << " constant=" << '"';
        if( true == xmlGenericComponent[structIndex].xmlConstant[i] )
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
        out << '"' << "/>" << endl;
    }
    // generic msbNumber 1-8
    for (int  i = 0; i < 8; i++)
    {
        out << "\t<genMsbNumber" << i+1 << " number=" << '"' << xmlDefinedComponent[structIndex].xmlMsbNumber[i]
            << '"' << "/>" << endl;
    }
    // generic lsbNumber 1-8
    for (int  i = 0; i < 8; i++)
    {
        out << "\t<genLsbNumber" << i+1 << " number=" << '"' << xmlDefinedComponent[structIndex].xmlLsbNumber[i]
            << '"' << "/>" << endl;
    }
    out << "</component>";

    outFile.close();
}

unsigned int XmlParser::getFreeRoom()
{
    xmlComponentCnt++;
    qDebug() << "Get free room function counter (XML) : " << xmlComponentCnt;
    return xmlComponentCnt;
}

void XmlParser::readXml()
{
    //file[0].setFileName( ":/");
    //file[1].setFileName("../share/mds/components/Leds.txt");
    //file[2].setFileName("../share/mds/components/Buttons.txt");

    file[0].setFileName(":resources/xml/VHDLgen/UART.txt");
    file[0].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[0],getFreeRoom() - 1);

    file[1].setFileName(":resources/xml/VHDLgen/Leds.txt");
    file[1].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[1],getFreeRoom() - 1);

    file[2].setFileName(":resources/xml/VHDLgen/Buttons.txt");
    file[2].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[2],getFreeRoom() - 1);

    file[3].setFileName(":resources/xml/VHDLgen/I2C_interface.txt");
    file[3].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[3],getFreeRoom() - 1);

    file[4].setFileName(":resources/xml/VHDLgen/LCD_interface.txt");
    file[4].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[4],getFreeRoom() - 1);

    file[5].setFileName(":resources/xml/VHDLgen/SPI_slave.txt");
    file[5].open((QIODevice::ReadOnly | QIODevice::Text));
    xmlReadDevice(&file[5],getFreeRoom() - 1);

    getDirFiles();
    for ( int i=0; i < Devices.size(); i++)
    {
        qDebug() << "devices size : "<< Devices.size();
        qDebug() << "devices path : "<< ( QDir::homePath() + "/.mds/" + Devices[i] + ".xml" );
        file[6].setFileName( QDir::homePath() + "/.mds/" + Devices[i] + ".xml" );
        file[6].open((QIODevice::ReadOnly | QIODevice::Text));
        xmlReadDevice(&file[6],getFreeRoom() - 1);
    }
}

void XmlParser::xmlReadDevice( QIODevice *device, unsigned int componentStructIndex)
{
    QXmlStreamReader xml;
    xml.setDevice( device );
    //&device.open((QIODevice::ReadOnly | QIODevice::Text));

    while(!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        qDebug() << "TOKEN NUMBER > " << token;

        if (token == QXmlStreamReader::StartElement)
        {
            if ( "component" == xml.name() )
            {
                xmlDefinedComponent[componentStructIndex].xmlName = xml.attributes().value("Name").toString();
                while( !(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "component") )
                {
                    token = xml.readNext();
                    if ( token == QXmlStreamReader::StartElement )
                    {
                        if ( "portName1" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName2" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName3" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName4" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName5" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName6" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName7" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName8" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName9" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName10" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName11" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName12" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName13" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName14" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName15" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName16" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        if ( "portName17" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortName << xml.text().toString();
                        }
                        // port values
                        if ( "portValue1" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue2" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue3" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue4" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue5" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue6" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue7" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue8" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue9" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue10" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue11" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue12" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue13" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue14" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue15" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue16" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }
                        if ( "portValue17" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlPortValue << xml.attributes().value("value").toString();
                        }


                        // port directions
                        if ( "portDirection1" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection2" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection3" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection4" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection5" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection6" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection7" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection8" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection9" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection10" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection11" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection12" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection13" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection14" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection15" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection16" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "portDirection17" == xml.name())
                        {
                            xml.readNext();
                            xmlDefinedComponent[componentStructIndex].xmlPortDirection << xml.text().toString();
                        }
                        if ( "bus1" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[0] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[0] = false;
                            }
                        }
                        if ( "bus2" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[1] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[1] = false;
                            }
                        }
                        if ( "bus3" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[2] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[2] = false;
                            }
                        }
                        if ( "bus4" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[3] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[3] = false;
                            }
                        }
                        if ( "bus5" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[4] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[4] = false;
                            }
                        }
                        if ( "bus6" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[5] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[5] = false;
                            }
                        }
                        if ( "bus7" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[6] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[6] = false;
                            }
                        }
                        if ( "bus8" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[7] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[7] = false;
                            }
                        }
                        if ( "bus9" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[8] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[8] = false;
                            }
                        }
                        if ( "bus10" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[9] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[9] = false;
                            }
                        }
                        if ( "bus11" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[10] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[10] = false;
                            }
                        }
                        if ( "bus12" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[11] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[11] = false;
                            }
                        }
                        if ( "bus13" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[12] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[12] = false;
                            }
                        }
                        if ( "bus14" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[13] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[13] = false;
                            }
                        }
                        if ( "bus15" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[14] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[14] = false;
                            }
                        }
                        if ( "bus16" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[15] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[15] = false;
                            }
                        }
                        if ( "bus17" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[16] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlBus[16] = false;
                            }
                        }
                        if ( "signal1" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[0] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[0] = false;
                            }
                        }
                        if ( "signal2" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[1] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[1] = false;
                            }
                        }
                        if ( "signal3" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[2] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[2] = false;
                            }
                        }
                        if ( "signal4" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[3] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[3] = false;
                            }
                        }
                        if ( "signal5" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[4] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[4] = false;
                            }
                        }
                        if ( "signal6" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[5] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[5] = false;
                            }
                        }
                        if ( "signal7" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[6] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[6] = false;
                            }
                        }
                        if ( "signal8" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[7] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[7] = false;
                            }
                        }
                        if ( "signal9" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[8] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[8] = false;
                            }
                        }
                        if ( "signal10" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[9] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[9] = false;
                            }
                        }
                        if ( "signal11" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[10] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[10] = false;
                            }
                        }
                        if ( "signal12" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[11] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[11] = false;
                            }
                        }
                        if ( "signal13" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[12] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[12] = false;
                            }
                        }
                        if ( "signal14" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[13] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[13] = false;
                            }
                        }
                        if ( "signal15" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[14] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[14] = false;
                            }
                        }
                        if ( "signal16" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[15] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[15] = false;
                            }
                        }
                        if ( "signal17" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("signal").toString())
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[16] = true;
                            }
                            else
                            {
                                xmlDefinedComponent[componentStructIndex].xmlSignal[16] = false;
                            }
                        }
                        // MSB and LSB number
                        if ( "msbNumber1" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber2" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber3" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber4" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber5" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber6" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber7" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber8" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber9" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber10" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber11" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber12" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber13" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber14" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber15" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber16" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "msbNumber17" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber1" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber2" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber3" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber4" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber5" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber6" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber7" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber8" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber9" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber10" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber11" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber12" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber13" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber14" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber15" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber16" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "lsbNumber17" == xml.name() )
                        {
                            xmlDefinedComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        // now get generic attributes
                        // ******** GENERIC ****************
                        // --------------------------------------------------------------
                        if ( "genericName1" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName2" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName3" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName4" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName5" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName6" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName7" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        if ( "genericName8" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlGenericName << xml.text().toString();
                        }
                        // port values
                        if ( "genericValue1" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue2" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue3" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue4" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue5" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue6" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue7" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }
                        if ( "genericValue8" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlGenericValue << xml.attributes().value("value").toString();
                        }

                        // port types
                        if ( "portType1" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType2" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType3" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType4" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType5" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType6" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType7" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                        }
                        if ( "portType8" == xml.name())
                        {
                            xml.readNext();
                            xmlGenericComponent[componentStructIndex].xmlPortType << xml.text().toString();
                            qDebug() << " PORT TYPE SHIT :"<< xml.text().toString();
                        }
                        if ( "genBus1" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[0] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[0] = false;
                            }
                        }
                        if ( "genBus2" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[1] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[1] = false;
                            }
                        }
                        if ( "genBus3" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[2] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[2] = false;
                            }
                        }
                        if ( "genBus4" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[3] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[3] = false;
                            }
                        }
                        if ( "genBus5" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[4] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[4] = false;
                            }
                        }
                        if ( "genBus6" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[5] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[5] = false;
                            }
                        }
                        if ( "genBus7" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[6] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[6] = false;
                            }
                        }
                        if ( "genBus8" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("value").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[7] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlBus[7] = false;
                            }
                        }
                        // constants
                        if ( "constant1" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[0] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[0] = false;
                            }
                        }
                        if ( "constant2" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[1] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[1] = false;
                            }
                        }
                        if ( "constant3" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[2] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[2] = false;
                            }
                        }
                        if ( "constant4" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[3] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[3] = false;
                            }
                        }
                        if ( "constant5" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[4] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[4] = false;
                            }
                        }
                        if ( "constant6" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[5] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[5] = false;
                            }
                        }
                        if ( "constant7" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[6] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[6] = false;
                            }
                        }
                        if ( "constant8" == xml.name() )
                        {
                            if ( "true" == xml.attributes().value("constant").toString())
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[7] = true;
                            }
                            else
                            {
                                xmlGenericComponent[componentStructIndex].xmlConstant[7] = false;
                            }
                        }
                        // MSB and LSB number
                        if ( "genMsbNumber1" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber2" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber3" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber4" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber5" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber6" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber7" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genMsbNumber8" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlMsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber1" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber2" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber3" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber4" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber5" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber6" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber7" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                        if ( "genLsbNumber8" == xml.name() )
                        {
                            xmlGenericComponent[componentStructIndex].xmlLsbNumber << xml.attributes().value("number").toString();
                        }
                    }//if
                }//while
            }
        }
             if (xml.hasError())
             {
                qDebug() << "XML error: " << xml.errorString() << endl;
             }
             if (xml.atEnd())
             {
                qDebug() << "Reached end, done" << endl;
             }
        }
    device->close();
}

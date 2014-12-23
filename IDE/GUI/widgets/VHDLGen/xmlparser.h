#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QWidget>
#include <QtGui>

class XmlParser : public QWidget
{
    Q_OBJECT
public:
    explicit XmlParser(QWidget *parent = 0);
    //QFile  file1,file2,file3;
    QFile file[7];
    QStringList componentName;
    QStringList m_componentList;
    QStringList Devices,filters;
    unsigned int xmlComponentCnt;

    struct xml_component
    {
        QString xmlName;
        QStringList xmlPortName;
        QStringList xmlPortValue;
        QStringList xmlPortDirection;
        bool xmlBus[17];
        bool xmlSignal[17];
        QStringList xmlMsbNumber;
        QStringList xmlLsbNumber;
        //QStringList attributes;
    } xmlDefinedComponent[100];

    struct xml_generic
    {
        QStringList xmlGenericName;
        QStringList xmlGenericValue;
        QStringList xmlPortType;
        bool xmlBus[8];
        bool xmlConstant[8];
        QStringList xmlMsbNumber;
        QStringList xmlLsbNumber;

    } xmlGenericComponent[100];

    QStringList getComponent(void)
    {
        return componentName;
    }

    void writeXml(unsigned int structIndex);
    void readXml();
    void getDirFiles();
    unsigned int insertExistingComponent(QString & componentToAdd);
    void refreshDirFiles();
    void xmlReadDevice(QIODevice*,unsigned int);
    unsigned int getFreeRoom();

private:

signals:
    
public slots:

    
};

#endif // XMLPARSER_H

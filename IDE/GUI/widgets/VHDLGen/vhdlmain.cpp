// =============================================================================
/**
* @brief
* C++ interface:  Automatic generation of VHDL code for Picoblaze hardware components
*
*
* (C) copyright 2013 Moravia Microsystems, s.r.o.
*
* @author Martin Madron <martin.madron@moravia-microsystems.com>
* @ingroup PicoBlaze
* @file vhdlmain.cpp
*/
// =============================================================================

#include "vhdlmain.h"
#include "ui_vhdlmain.h"
//#include "createcomponent.h"
#include "ui_createcomponent.h"
#include <QtGui>
//#include "xmlparser.h"

#define     PORT            1
#define     PORTIN          2
#define     PORTOUT         3


VhdlMain::VhdlMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VhdlMain) //ui2(new Ui2::CreateComponent)
{
    ui->setupUi(this);

   // this->KCPSM3 =  new QRect(10,10,10,10);
    this->KCPSM  =  new QRect(20, 110, 120,50);
    this->MUX_in =  new QRect(150, 110, 130,50);
    this->MUX_out=  new QRect(290, 110, 125,50);

    // kcpsm versions to combo box
    QStringList items2 =(QStringList()<<"KCPSMcpld"<<"KCPSM1"<<"KCPSM2"<<"KCPSM3"<<"KCPSM6");
    ui->comboKcpsm->addItems(items2);
    ui->comboKcpsm->setCurrentIndex(4);

    rectList << "componentRect";
    this->componentRect = new QRect(20, 170, 200, 25);

    for ( unsigned int i = 0; i < 20; i++)
    {
        this->componentObject[i] = new QRect ( 0, 0 , 0, 0);
    }
    setMouseTracking ( true );
    //ui2->setupUi(widget);
    this->setWindowTitle("VHDL wizard");

    // variable initializations

    for (unsigned int i= 0; i < 20; i++ )
    {
        componentHighlight[i] = false;
    }

    portCount = 0;
    portInCount = 0;
    portOutCount = 0;
    componentCnt = 0;
    m_saveAddFlag = false;
    m_xmlNumberFlag = false;
    m_xmlEditFlag = false;
    m_xmlNumber = 0;
    m_xmlEditSpaceNum = 0;
    createWidget = false;

    bootDeviceList << "UART" << "Buttons" << "Leds" << "I2C_interface" << "LCD_interface" << "SPI_slave";

    entityName = "MPU_SYS";

    //ui->infoLabel->setText("Select .sym file generated after compilation to read PORT directives names");

    // set custom menu
    ui->listVystup->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listVystup,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint))
            );

    ui->Output->setReadOnly(true);
    ui->Input->setReadOnly(true);

     ui->pushButton->setIcon(QPixmap(":/resources/icons/projOpen.png"));
     ui->pushButton_2->setIcon(QPixmap(":/resources/icons/projOpen.png"));
     ui->pushDelete->setIcon(QPixmap(":/resources/icons/cancel.png"));
     ui->pushGenerate->setIcon(QPixmap(":/resources/icons/cog.png"));
     ui->pushButton_4->setIcon(QPixmap(":/resources/icons/add.png"));
     // cancel.png == delte  pushDelete
     // add.png  //create  pushButton_4
     // cog.png   generate        pushGenerate
    //QStringList attributes;
   // picoBlaze.name = "MPU_SYS_core";
   // portName << "clk" << "port_id" << "write_strobe" << "k_write_strobe" << "out_port" << "read_strobe"
   //          << "in_port" << "interrupt" << "interrupt_ack"
    //            portDirection << ""

    connect(ui->pushButton_2,
            SIGNAL(clicked()),
            this,
            SLOT(saveFile())
           );
    connect(ui->pushButton,
            SIGNAL(clicked()),
            this,
            SLOT(loadFile())
           );
    connect(ui->pushButton_4,
            SIGNAL(clicked()),
            this,
            SLOT(createComponent1())
           );
    connect(ui->pushGenerate,
            SIGNAL(clicked()),
            this,
            SLOT(printToFile())
           );
    connect(ui->pushDelete,
            SIGNAL(clicked()),
            this,
            SLOT(pushDelete())
           );
    connect(ui->listVystup,
            SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,
            SLOT(insertItem(QListWidgetItem*))
           );

    //ui->listVystup->addItems(Devices);

    ui->listVystup->addItems(bootDeviceList);
    //xmlParser.getDirFiles();
    //path.append("/.mds/");
    //path.append(xmlDefinedComponent[xmlComponentCnt].xmlName);
    //path.append(".xml");
    xmlParser.readXml();
    ui->listVystup->addItems(xmlParser.Devices);
    qDebug()<< "jjjjj";

}

VhdlMain::~VhdlMain()
{
    delete ui;    
}

void VhdlMain::insertItem( QListWidgetItem *componentItem )
{
    QString componentToAdd;
    componentToAdd = componentItem->text();

    if (    ( false == xmlParser.Devices.contains(componentToAdd))
         && ( false == bootDeviceList.contains(componentToAdd)  )  )
    {
        QMessageBox::warning ( this, componentToAdd, "There is no such component");
        return;
    }
    addExistingComponent(xmlParser.insertExistingComponent(componentToAdd));
}

void VhdlMain::addExistingComponent(unsigned int xmlNumber)
{
    if ( 99 == xmlNumber)
    {
        qDebug()<< "xmlnumber wrong > " << xmlNumber;
        return;
    }
    m_xmlNumberFlag = true;
    m_xmlNumber = xmlNumber;
    createWidget = false;
    pushOk();
}

VhdlMain::genericType VhdlMain::getEnum(QString & inString)
{
    if ( inString == "logic")       return LOGIC;
    if ( inString == "logicVector") return LOGIC_VECTOR;
    if ( inString == "integer")     return INTEGER;
    if ( inString == "positive")    return POSITIVE;
    if ( inString == "natural")     return NATURAL;
}

void VhdlMain::paintEvent(QPaintEvent *e)
{
    // initial definitions (pens, points, rect)
    QPainter painter;
    QPen graypen(Qt::gray);
    QPen blackpen(Qt::black);
    QPen blackpenRect(Qt::black);
    QPen blackpenSwitch(Qt::black);
    QPen DotRect(Qt::black);
    DotRect.setWidth(1);
    DotRect.setStyle(Qt::DashLine);
    QPen noDotRect(Qt::black);
    noDotRect.setWidth(2);
    noDotRect.setStyle(Qt::SolidLine);

    // pens
    graypen.setWidth(3);
    blackpenRect.setWidth(3);
    blackpenSwitch.setWidth(1);
    // rectangles

    static const QRect rectEntity(10,100,416,340);

    // the beginning of paint event
    painter.begin(this);
    painter.setPen(blackpenRect);

    painter.drawText(11,91,"MPU_SYS entity");
    painter.drawRect(rectEntity);

    if ( KCPSMhighlight == false)
    {
        painter.setPen(blackpen);
        painter.drawRect(*KCPSM);
        painter.drawText(*KCPSM, Qt::AlignCenter, tr("KCPSM"));
    }
    else
    {
        painter.setPen(blackpenRect);
        painter.drawRect(*KCPSM);
        painter.drawText(*KCPSM, Qt::AlignCenter, tr("KCPSM"));
    }
    if ( inHighlight == false)
    {
        painter.setPen(blackpen);
        painter.drawRect(*MUX_in);
        painter.drawText(*MUX_in, Qt::AlignCenter, tr("MUX in"));
    }
    else
    {
        painter.setPen(blackpenRect);
        painter.drawRect(*MUX_in);
        painter.drawText(*MUX_in, Qt::AlignCenter, tr("MUX in"));
    }
    if ( outHighlight == false)
    {
        painter.setPen(blackpen);
        painter.drawRect(*MUX_out);
        painter.drawText(*MUX_out, Qt::AlignCenter, tr("MUX out"));
    }
    else
    {
        painter.setPen(blackpenRect);
        painter.drawRect(*MUX_out);
        painter.drawText(*MUX_out, Qt::AlignCenter, tr("MUX out"));
    }

    // now ;ets drawing new components
    // draw components acording to their names,
    if ( 0 == componentCnt)
    {
        if ( true == m_componentRectHighlight)
        {
            painter.setPen(blackpenRect);
            rectList << "componentRect";
            painter.drawRect(*componentRect);
            painter.drawText(*componentRect, Qt::AlignCenter, tr("Create new component"));
        }
        else
        {
            painter.setPen(DotRect);
            rectList << "componentRect";
            painter.drawRect(*componentRect);
            painter.drawText(*componentRect, Qt::AlignCenter, tr("Create new component"));
        }
    }
    else
    {
        rectList.clear();
        //delete this->componentRect;
    }

    unsigned int p = 0;
    while ( false == definedComponent[p].name.isEmpty() )
    {
        if ( true == componentHighlight[p])
        {
            painter.setPen(blackpenRect);
        }
        else
        {
            painter.setPen(blackpen);
        }
        painter.drawRect(*componentObject[p]);

        // if name is too long, display dots instead
        if ( definedComponent[p].name.size() > 30)
        {
            QString str;
            str = definedComponent[p].name;
            str.chop( definedComponent[p].name.size() - 27 );
            str.append("...");
            painter.drawText(*componentObject[p], Qt::AlignCenter, str);
        }
        else
        {
            painter.drawText(*componentObject[p], Qt::AlignCenter, definedComponent[p].name);
        }
        p++;
    }
    painter.end();
}

void VhdlMain::customMenuRequested(QPoint pos)
{
   // QModelIndex index=table->indexAt(pos);

    QMenu *menu = new QMenu(this);
    //menu->addAction(new QAction("Remove", this));
    menu->addAction("Remove", this, SLOT(RemoveXml()));
    menu->addAction("Edit", this, SLOT(pushEditSlot()));
    menu->popup(ui->listVystup->viewport()->mapToGlobal(pos));
}

void VhdlMain::RemoveXml()
{
    QString componentToRemove = ui->listVystup->currentItem()->text();
    if ( componentToRemove == "UART")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    if ( componentToRemove == "Buttons")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    if ( componentToRemove == "Leds")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    if ( componentToRemove == "I2C_interface")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    if ( componentToRemove == "LCD_interface")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    if ( componentToRemove == "SPI_slave")
    {
        QMessageBox::warning ( this, "Edit", "This component is read only");
        return;
    }
    QFile FileToRemove;
    FileToRemove.setFileName(QDir::homePath() + "/.mds/" + componentToRemove + ".xml");

    if ( false == FileToRemove.remove())
    {
        QMessageBox::warning ( this, "Remove", "File cannot be removed");
        return;
    }
    xmlParser.Devices.removeOne(componentToRemove);
    ui->listVystup->clear();
    ui->listVystup->addItems(bootDeviceList);
    ui->listVystup->addItems(xmlParser.Devices);
}

void VhdlMain::pushEditSlot()
{
    if ( ui->listVystup->currentItem() == NULL)
    {
        qDebug() << "item not selected";
        QMessageBox::warning ( this, "Edit", "Please select component");
        return;
    }
    else
    {
       QString selectedText = ui->listVystup->currentItem()->text();
       if ( selectedText == "UART")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       if ( selectedText == "Buttons")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       if ( selectedText == "Leds")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       if ( selectedText == "I2C_interface")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       if ( selectedText == "LCD_interface")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       if ( selectedText == "SPI_slave")
       {
           QMessageBox::warning ( this, "Edit", "This component is read only");
           return;
       }
       m_editNumber = xmlParser.insertExistingComponent(selectedText);
       editSelectedComponent(m_editNumber);
    }
}

void VhdlMain::editSelectedComponent(unsigned int position)
{
    qDebug() << "item to edit :" << position;
    createComponent1();
    m_xmlEditFlag = true;
    m_xmlEditSpaceNum = position;


    ui2.editName->setText(xmlParser.xmlDefinedComponent[position].xmlName);

        ui2.editPort_9->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(0));
        ui2.editPort_10->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(1));
        ui2.editPort_11->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(2));
        ui2.editPort_12->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(3));
        ui2.editPort_13->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(4));
        ui2.editPort_14->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(5));
        ui2.editPort_15->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(6));
        ui2.editPort_16->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(7));
        ui2.editPort_17->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(8));
        ui2.editPort_18->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(9));
        ui2.editPort_19->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(10));
        ui2.editPort_20->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(11));
        ui2.editPort_21->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(12));
        ui2.editPort_22->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(13));
        ui2.editPort_23->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(14));
        ui2.editPort_24->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(15));
        ui2.editPort_25->setText(xmlParser.xmlDefinedComponent[position].xmlPortName.at(16));

    // get port directions
        int numDirection[17];
        for (int i = 0; i < 17; i++)
        {
            if (xmlParser.xmlDefinedComponent[position].xmlPortDirection.at(i) == "in")
                numDirection[i] = 0;
            if (xmlParser.xmlDefinedComponent[position].xmlPortDirection.at(i) == "out")
                numDirection[i] = 1;
            if (xmlParser.xmlDefinedComponent[position].xmlPortDirection.at(i) == "inout")
                numDirection[i] = 2;
        }

        ui2.comboDirection_9->setCurrentIndex(numDirection[0]);
        ui2.comboDirection_10->setCurrentIndex(numDirection[1]);
        ui2.comboDirection_11->setCurrentIndex(numDirection[2]);
        ui2.comboDirection_12->setCurrentIndex(numDirection[3]);
        ui2.comboDirection_13->setCurrentIndex(numDirection[4]);
        ui2.comboDirection_14->setCurrentIndex(numDirection[5]);
        ui2.comboDirection_15->setCurrentIndex(numDirection[6]);
        ui2.comboDirection_16->setCurrentIndex(numDirection[7]);
        ui2.comboDirection_17->setCurrentIndex(numDirection[8]);
        ui2.comboDirection_18->setCurrentIndex(numDirection[9]);
        ui2.comboDirection_19->setCurrentIndex(numDirection[10]);
        ui2.comboDirection_20->setCurrentIndex(numDirection[11]);
        ui2.comboDirection_21->setCurrentIndex(numDirection[12]);
        ui2.comboDirection_22->setCurrentIndex(numDirection[13]);
        ui2.comboDirection_23->setCurrentIndex(numDirection[14]);
        ui2.comboDirection_24->setCurrentIndex(numDirection[15]);
        ui2.comboDirection_25->setCurrentIndex(numDirection[16]);

    // get buses
    ui2.checkBus_9->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[0]);
    ui2.checkBus_10->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[1]);
    ui2.checkBus_11->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[2]);
    ui2.checkBus_12->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[3]);
    ui2.checkBus_13->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[4]);
    ui2.checkBus_14->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[5]);
    ui2.checkBus_15->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[6]);
    ui2.checkBus_16->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[7]);
    ui2.checkBus_17->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[8]);
    ui2.checkBus_18->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[9]);
    ui2.checkBus_19->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[10]);
    ui2.checkBus_20->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[11]);
    ui2.checkBus_21->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[12]);
    ui2.checkBus_22->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[13]);
    ui2.checkBus_23->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[14]);
    ui2.checkBus_24->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[15]);
    ui2.checkBus_25->setChecked(xmlParser.xmlDefinedComponent[position].xmlBus[16]);

    // get buses
    ui2.checkSignal_9->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[0]);
    ui2.checkSignal_10->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[1]);
    ui2.checkSignal_11->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[2]);
    ui2.checkSignal_12->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[3]);
    ui2.checkSignal_13->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[4]);
    ui2.checkSignal_14->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[5]);
    ui2.checkSignal_15->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[6]);
    ui2.checkSignal_16->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[7]);
    ui2.checkSignal_17->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[8]);
    ui2.checkSignal_18->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[9]);
    ui2.checkSignal_19->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[10]);
    ui2.checkSignal_20->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[11]);
    ui2.checkSignal_21->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[12]);
    ui2.checkSignal_22->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[13]);
    ui2.checkSignal_23->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[14]);
    ui2.checkSignal_24->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[15]);
    ui2.checkSignal_25->setChecked(xmlParser.xmlDefinedComponent[position].xmlSignal[16]);

    // get msb numbers

        ui2.editMSB_9->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(0));
        ui2.editMSB_10->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(1));
        ui2.editMSB_11->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(2));
        ui2.editMSB_12->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(3));
        ui2.editMSB_13->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(4));
        ui2.editMSB_14->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(5));
        ui2.editMSB_15->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(6));
        ui2.editMSB_16->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(7));
        ui2.editMSB_17->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(8));
        ui2.editMSB_18->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(9));
        ui2.editMSB_19->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(10));
        ui2.editMSB_20->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(11));
        ui2.editMSB_21->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(12));
        ui2.editMSB_22->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(13));
        ui2.editMSB_23->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(14));
        ui2.editMSB_24->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(15));
        ui2.editMSB_25->setText(xmlParser.xmlDefinedComponent[position].xmlMsbNumber.at(16));

    // get lsb numbers
        ui2.editLSB_9->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(0));
        ui2.editLSB_10->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(1));
        ui2.editLSB_11->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(2));
        ui2.editLSB_12->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(3));
        ui2.editLSB_13->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(4));
        ui2.editLSB_14->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(5));
        ui2.editLSB_15->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(6));
        ui2.editLSB_16->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(7));
        ui2.editLSB_17->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(8));
        ui2.editLSB_18->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(9));
        ui2.editLSB_19->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(10));
        ui2.editLSB_20->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(11));
        ui2.editLSB_21->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(12));
        ui2.editLSB_22->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(13));
        ui2.editLSB_23->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(14));
        ui2.editLSB_24->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(15));
        ui2.editLSB_25->setText(xmlParser.xmlDefinedComponent[position].xmlLsbNumber.at(16));

    // get initialized numbers

        ui2.editValue_9->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(0));
        ui2.editValue_10->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(1));
        ui2.editValue_11->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(2));
        ui2.editValue_12->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(3));
        ui2.editValue_13->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(4));
        ui2.editValue_14->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(5));
        ui2.editValue_15->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(6));
        ui2.editValue_16->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(7));
        ui2.editValue_17->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(8));
        ui2.editValue_18->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(9));
        ui2.editValue_19->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(10));
        ui2.editValue_20->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(11));
        ui2.editValue_21->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(12));
        ui2.editValue_22->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(13));
        ui2.editValue_23->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(14));
        ui2.editValue_24->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(15));
        ui2.editValue_25->setText(xmlParser.xmlDefinedComponent[position].xmlPortValue.at(16));

    // ***** GENERIC ******
    // get names of declared ports
         ui2.editPort_41->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(0));
         ui2.editPort_42->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(1));
         ui2.editPort_43->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(2));
         ui2.editPort_44->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(3));
         ui2.editPort_45->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(4));
         ui2.editPort_46->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(5));
         ui2.editPort_47->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(6));
         ui2.editPort_48->setText(xmlParser.xmlGenericComponent[position].xmlGenericName.at(7));

    // get port directions
          //<< "logic" << "logicVector" << "integer" << "positive" << "natural";
         for (int i = 0; i < 8; i++)
         {
             if (xmlParser.xmlGenericComponent[position].xmlPortType.at(i) == "logic")
                 numDirection[i] = 0;
             if (xmlParser.xmlGenericComponent[position].xmlPortType.at(i) == "logicVector")
                 numDirection[i] = 1;
             if (xmlParser.xmlGenericComponent[position].xmlPortType.at(i) == "integer")
                 numDirection[i] = 2;
             if (xmlParser.xmlGenericComponent[position].xmlPortType.at(i) == "positive")
                 numDirection[i] = 3;
             if (xmlParser.xmlGenericComponent[position].xmlPortType.at(i) == "natural")
                 numDirection[i] = 4;
         }

      ui2.comboType_41->setCurrentIndex(numDirection[0]);
      ui2.comboType_42->setCurrentIndex(numDirection[1]);
      ui2.comboType_43->setCurrentIndex(numDirection[2]);
      ui2.comboType_44->setCurrentIndex(numDirection[3]);
      ui2.comboType_45->setCurrentIndex(numDirection[4]);
      ui2.comboType_46->setCurrentIndex(numDirection[5]);
      ui2.comboType_47->setCurrentIndex(numDirection[6]);
      ui2.comboType_48->setCurrentIndex(numDirection[7]);

    // get buses
    ui2.checkBus_41->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[0]);
    ui2.checkBus_42->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[1]);
    ui2.checkBus_43->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[2]);
    ui2.checkBus_44->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[3]);
    ui2.checkBus_45->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[4]);
    ui2.checkBus_46->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[5]);
    ui2.checkBus_47->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[6]);
    ui2.checkBus_48->setChecked(xmlParser.xmlGenericComponent[position].xmlBus[7]);

    // get buses
    ui2.checkSignal_41->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[0]);
    ui2.checkSignal_42->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[1]);
    ui2.checkSignal_43->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[2]);
    ui2.checkSignal_44->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[3]);
    ui2.checkSignal_45->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[4]);
    ui2.checkSignal_46->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[5]);
    ui2.checkSignal_47->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[6]);
    ui2.checkSignal_48->setChecked(xmlParser.xmlGenericComponent[position].xmlConstant[7]);

    // get msb numbers
      ui2.editMSB_41->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(0));
      ui2.editMSB_42->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(1));
      ui2.editMSB_43->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(2));
      ui2.editMSB_44->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(3));
      ui2.editMSB_45->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(4));
      ui2.editMSB_46->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(5));
      ui2.editMSB_47->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(6));
      ui2.editMSB_48->setText(xmlParser.xmlGenericComponent[position].xmlMsbNumber.at(7));

    // get lsb numbers
      ui2.editLSB_41->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(0));
      ui2.editLSB_42->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(1));
      ui2.editLSB_11->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(2));
      ui2.editLSB_44->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(3));
      ui2.editLSB_45->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(4));
      ui2.editLSB_46->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(5));
      ui2.editLSB_47->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(6));
      ui2.editLSB_48->setText(xmlParser.xmlGenericComponent[position].xmlLsbNumber.at(7));

    // get initialized numbers
      ui2.editValue_41->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(0));
      ui2.editValue_42->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(1));
      ui2.editValue_11->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(2));
      ui2.editValue_44->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(3));
      ui2.editValue_45->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(4));
      ui2.editValue_46->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(5));
      ui2.editValue_47->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(6));
      ui2.editValue_48->setText(xmlParser.xmlGenericComponent[position].xmlGenericValue.at(7));
}

void VhdlMain::mousePressEvent(QMouseEvent* pressEvent)
{
//     if(pressEvent->button() == Qt::RightButton)
//     {
//         qDebug() << "right click";
//     }

    if ( rectList.contains("componentRect") == true)
    {
        if ( this->componentRect->contains( pressEvent->pos() ) == true)
        {
             createComponent1();
             return;
        }
    }

    if ( this->KCPSM->contains( pressEvent->pos() ) == true)
    {
        this->RectInfo = new QWidget(0);
        // define new widget
        ui3.setupUi(RectInfo);
        RectInfo->setWindowTitle("Component info");
        RectInfo->setWindowModality(Qt::ApplicationModal);
        RectInfo->show();
                                        
        const int tabStop = 4; // 4 characters
        ui3.textInfo->setReadOnly(true);
        // get names and values of port directives, put them into global structures
        // IN OUT ports
        QTextCursor cursor_textinfo(ui3.textInfo->textCursor());
        ui3.textInfo->clear();

        QFontMetrics metrics(ui3.textInfo->font());
        ui3.textInfo->setTabStopWidth(tabStop * metrics.width(' '));
        QTextCharFormat format = cursor_textinfo.charFormat();
        format.setForeground(QBrush(Qt::blue));
        format.setFontWeight(QFont::Bold);


        QString port;
        port.clear();
        QTextStream out(&port);

        // KCPSM ver 

        out << " COMPONENT NAME:"  << "\n";
        out << ui->comboKcpsm->currentText() << "\n" << "\n";
        if ( ui->comboKcpsm->currentText() == "KCPSM6")
        {
            out << "GENERIC:" << endl;
            out << " HWbuild" << endl << endl;
        }
        out << "PORTS:" << endl;
        
    switch ( ui->comboKcpsm->currentIndex() )
    {
        // kcpsmcpld
        case 0:
            out << " Pico_Data_in" <<"\n";
            out << " Pico_Data_out" <<"\n";
            out << " Pico_Port_ID" <<"\n";
            out << " Pico_rd" <<"\n";
            out << " Pico_wr" <<"\n";
            out << " Pico_int" <<"\n";
            out << " Pico_clk" <<"\n";
            break;
        // kcpsm1
        case 1:
            out << " Pico_Data_in" <<"\n";
            out << " Pico_Data_out" <<"\n";
            out << " Pico_Port_ID" <<"\n";
            out << " Pico_rd" <<"\n";
            out << " Pico_wr" <<"\n";
            out << " Pico_int" <<"\n";
            out << " Pico_clk" <<"\n";
            break;
        // kcpsm2
        case 2:
            out << " Pico_Data_in" <<"\n";
            out << " Pico_Data_out" <<"\n";
            out << " Pico_Port_ID" <<"\n";
            out << " Pico_rd" <<"\n";
            out << " Pico_wr" <<"\n";
            out << " Pico_int" <<"\n";
            out << " Pico_clk" <<"\n";
            break;
        // kcpsm3
        case 3:
            out << " Pico_Data_in" <<"\n";
            out << " Pico_Data_out" <<"\n";
            out << " Pico_Port_ID" <<"\n";
            out << " Pico_rd" <<"\n";
            out << " Pico_wr" <<"\n";
            out << " Pico_int" <<"\n";
            out << " Pico_int_ack" <<"\n";
            out << " Pico_clk" <<"\n";
            break;
        // kcpsm6
        case 4:
            out << " Pico_Data_in" <<"\n";
            out << " Pico_Data_out" <<"\n";
            out << " Pico_Port_ID" <<"\n";
            out << " Pico_rd" <<"\n";
            out << " Pico_wr" <<"\n";
            out << " Pico_k_wr" <<"\n";
            out << " Pico_int" <<"\n";
            out << " Pico_int_ack" <<"\n";
            out << " Pico_clk" <<"\n";
            out << " Pico_sleep" <<"\n";
            break;
        default: qDebug() << "wrong kcpsm selected";
            break;
    }

        cursor_textinfo.insertText(port);
        ui3.textInfo->setTextCursor(cursor_textinfo);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);
        ui3.textInfo->find("GENERIC:");
        cursor_textinfo = ui3.textInfo->textCursor();
        cursor_textinfo.setCharFormat(format);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);
        ui3.textInfo->find("COMPONENT NAME:");
        cursor_textinfo = ui3.textInfo->textCursor();
        cursor_textinfo.setCharFormat(format);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);
        ui3.textInfo->find("PORTS");
        cursor_textinfo = ui3.textInfo->textCursor();
        cursor_textinfo.setCharFormat(format);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);
    }

    if ( this->MUX_out->contains( pressEvent->pos() ) == true)
    {
        this->RectInfo = new QWidget(0);
        // define new widget
        ui3.setupUi(RectInfo);
        RectInfo->setWindowTitle("Component info");
        RectInfo->setWindowModality(Qt::ApplicationModal);
        RectInfo->show();

        const int tabStop = 4; // 4 characters

        QFontMetrics metrics(ui3.textInfo->font());
        ui3.textInfo->setTabStopWidth(tabStop * metrics.width(' '));
        ui3.textInfo->setReadOnly(true);
        // get names and values of port directives, put them into global structures
        // IN OUT ports
        QTextCursor cursor_textinfo(ui3.textInfo->textCursor());
        ui3.textInfo->clear();

        QTextCharFormat format = cursor_textinfo.charFormat();
        format.setForeground(QBrush(Qt::blue));
        format.setFontWeight(QFont::Bold);


        QString port;
        port.clear();
        QTextStream out(&port);

        // OUTPUT MULTIPLEXER
        out << "-------------------------------------------------" <<"\n";
        out << " OUTPUT PORTS:" << "\n" << "\n";

        for (unsigned int i = 0; i < portCount; i++)
        {
            out << '\t' << portInOut[i].name << endl;
        }
        for (unsigned int i = 0; i < portOutCount; i++)
        {
            out << '\t' << portOut[i].name << endl;
        }
        cursor_textinfo.insertText(port);
        ui3.textInfo->setTextCursor(QTextCursor(ui3.textInfo->document()));
        ui3.textInfo->find("OUTPUT PORTS");
        cursor_textinfo = ui3.textInfo->textCursor();
        cursor_textinfo.setCharFormat(format);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);

    }

    if ( this->MUX_in->contains( pressEvent->pos() ) == true)
    {
        this->RectInfo = new QWidget(0);
        // define new widget
        ui3.setupUi(RectInfo);
        RectInfo->setWindowTitle("Component info");
        RectInfo->setWindowModality(Qt::ApplicationModal);
        RectInfo->show();

        const int tabStop = 4; // 4 characters
        QFontMetrics metrics(ui3.textInfo->font());

        ui3.textInfo->setTabStopWidth(tabStop * metrics.width(' '));
        ui3.textInfo->setReadOnly(true);
        // get names and values of port directives, put them into global structures
        // IN OUT ports
        QTextCursor cursor_textinfo(ui3.textInfo->textCursor());
        ui3.textInfo->clear();
        QTextCharFormat format = cursor_textinfo.charFormat();
        format.setForeground(QBrush(Qt::blue));
        format.setFontWeight(QFont::Bold);

        QString port;
        port.clear();
        QTextStream out(&port);

        // INPUT MULTIPLEXER
        out << "-------------------------------------------------" <<"\n";
        out << " INPUT PORTS:" << "\n" << "\n";

        for (unsigned int i = 0; i < portCount; i++)
        {
            out << '\t' << portInOut[i].name << endl;

        }
        for (unsigned int i = 0; i < portInCount; i++)
        {
            out << '\t' << portIn[i].name << endl;
        }

        cursor_textinfo.insertText(port);
        ui3.textInfo->setTextCursor(QTextCursor(ui3.textInfo->document()));
        format.setForeground(QBrush(Qt::blue));
        format.setFontWeight(QFont::Bold);
        ui3.textInfo->find("INPUT PORTS");
        cursor_textinfo = ui3.textInfo->textCursor();
        cursor_textinfo.setCharFormat(format);
        cursor_textinfo.setPosition(0);
        ui3.textInfo->setTextCursor(cursor_textinfo);
    }

    unsigned int p = 1;
    while ( true == this->componentObject[p-1]->isValid())
    {
         if ( this->componentObject[p-1]->contains( pressEvent->pos() ) == true)
         {
             this->RectInfo = new QWidget(0);
             // define new widget
             ui3.setupUi(RectInfo);
             RectInfo->setWindowTitle("Component info");
             RectInfo->setWindowModality(Qt::ApplicationModal);
             RectInfo->show();

             const int tabStop = 4; // 4 characters

             QFontMetrics metrics(ui3.textInfo->font());
             ui3.textInfo->setTabStopWidth(tabStop * metrics.width(' '));
             ui3.textInfo->setReadOnly(true);

             // IN OUT ports
             QTextCursor cursor_textinfo(ui3.textInfo->textCursor());
             ui3.textInfo->clear();

             QTextCharFormat format = cursor_textinfo.charFormat();
             format.setForeground(QBrush(Qt::blue));
             format.setFontWeight(QFont::Bold);

             QString portString;
             portString.clear();
             QTextStream out(&portString);

                 out << " COMPONENT NAME: "<< endl;
                 out << "      "<< definedComponent[p-1].name << endl;
                 out << endl;
                 int pIngen = 0;
                 if (  false == genericComponent[p-1].genericName.at(pIngen).isEmpty())
                 {
                     out << " GENERIC: " <<"\n" <<"\n";
                 }
                 while ( false == genericComponent[p-1].genericName.at(pIngen).isEmpty() )
                 {
                     out << "      " << genericComponent[p-1].genericName.at(pIngen);
                     m_spaceNum = tabsNumber(genericComponent[p-1].genericName.at(pIngen));
                     for (unsigned int  l = 0; l <= m_spaceNum; l++)
                     {
                         out << ' ';
                     }
                     if ( genericComponent[p-1].genericValue.at(pIngen).isEmpty() == false )
                     {
                         out << ": " << genericComponent[p-1].genericValue[pIngen] << endl;
                     }
                     else
                     {
                         out << endl;
                     }
                     pIngen++;
                 }
                 out << endl;
                 out << " PORTS: " <<"\n" <<"\n";
                 unsigned int pIn;
                 pIn = 0;
                 while ( true != definedComponent[p-1].portName.at(pIn).isEmpty() )
                 {
                     out << "      " << definedComponent[p-1].portName.at(pIn);
                     out << endl;
                     pIn++;
                 }
                 cursor_textinfo.insertText(portString);
                 ui3.textInfo->setTextCursor(cursor_textinfo);
                 ui3.textInfo->setTextCursor(QTextCursor(ui3.textInfo->document()));
                 ui3.textInfo->find("PORTS:");
                 cursor_textinfo = ui3.textInfo->textCursor();
                 cursor_textinfo.setCharFormat(format);
                 cursor_textinfo.setPosition(0);
                 ui3.textInfo->setTextCursor(cursor_textinfo);
                 ui3.textInfo->find("GENERIC:");
                 cursor_textinfo = ui3.textInfo->textCursor();
                 cursor_textinfo.setCharFormat(format);
                 cursor_textinfo.setPosition(0);
                 ui3.textInfo->setTextCursor(cursor_textinfo);
                 ui3.textInfo->find("COMPONENT NAME:");
                 cursor_textinfo = ui3.textInfo->textCursor();
                 cursor_textinfo.setCharFormat(format);
                 cursor_textinfo.setPosition(0);
                 ui3.textInfo->setTextCursor(cursor_textinfo);
                 break;
        }
         p++;
    }
    update();
}

void VhdlMain::mouseMoveEvent ( QMouseEvent * moveEvent )
{
   if ( this->KCPSM->contains( moveEvent->pos() ) == true)
   {
        KCPSMhighlight = true;
   }
   else
   {
        KCPSMhighlight = false;
   }
   if ( this->MUX_in->contains( moveEvent->pos() ) == true)
   {
        inHighlight = true;
   }
   else
   {
        inHighlight = false;
   }
   if ( this->MUX_out->contains( moveEvent->pos() ) == true)
   {
        outHighlight = true;
   }
   else
   {
        outHighlight = false;
   }

   unsigned int p = 0;
   while ( p < 20 && true == componentObject[p]->isValid() )
   {
        if ( this->componentObject[p]->contains( moveEvent->pos() ) == true)
        {
            componentHighlight[p] = true;
        }
        else
        {
            componentHighlight[p] = false;
        }
        p++;
   }

   if ( rectList.contains("componentRect") == true)
   {
       if ( this->componentRect->contains( moveEvent->pos() ) == true)
       {
            m_componentRectHighlight = true;
       }
       else
       {
            m_componentRectHighlight = false;
       }
    }
    update();
}

void VhdlMain::createComponent1()
{
    // define new widget
    //QWidget *componentWizard = new QWidget(0);
    this->componentWizard = new QWidget(0);
    ui2.setupUi(componentWizard);
    componentWizard->setWindowTitle("Component wizard");
    componentWizard->setWindowModality(Qt::ApplicationModal);
    componentWizardInitialization();
    componentWizard->show();
    componentWizardConstruct();
    createWidget = true;
}

void VhdlMain::pushDelete()
{
    // nothing to delete
    if ( 0 == componentCnt)
    {
        qDebug() << "returned";
        return;
    }

//    }
    definedComponent[componentCnt - 1].name.clear();
  //  qDebug() << "list after" << existingNamesList;

    // TODO > clear same
    definedComponent[componentCnt - 1].portName.clear();
    definedComponent[componentCnt - 1].portDirection.clear();
    definedComponent[componentCnt - 1].portValue.clear();
    definedComponent[componentCnt - 1].lsbNumber.clear();
    definedComponent[componentCnt - 1].msbNumber.clear();

    // delete xmlComponent also
 //   xmlParser.xmlDefinedComponent[componentCnt - 1].xmlName.clear();
 //   xmlParser.xmlDefinedComponent[componentCnt - 1].xmlPortName.clear();
 //   xmlParser.xmlDefinedComponent[componentCnt - 1].xmlPortDirection.clear();
 //   xmlParser.xmlDefinedComponent[componentCnt - 1].xmlPortValue.clear();
  //  xmlParser.xmlDefinedComponent[componentCnt - 1].xmlLsbNumber.clear();
  //  xmlParser.xmlDefinedComponent[componentCnt - 1].xmlMsbNumber.clear();

    // generic deletion
    genericComponent[componentCnt - 1].genericName.clear();
    genericComponent[componentCnt - 1].portType.clear();
    genericComponent[componentCnt - 1].genericValue.clear();
    genericComponent[componentCnt - 1].lsbNumber.clear();
    genericComponent[componentCnt - 1].msbNumber.clear();

    //delete this->componentObject[componentCnt-1];
    this->componentObject[componentCnt-1]->setRect(0,0,0,0);
    componentCnt--;
    update();
}

void VhdlMain::validateName(unsigned int ValIndex)
{
    QStringList indexCounter;

    for (unsigned int i = 0; i < componentCnt; i++)
    {
        if ( definedComponent[i].name == definedComponent[ValIndex].name )
        {
            existingNamesList << definedComponent[ValIndex].name;
            indexCounter << definedComponent[ValIndex].name;
        }
    }
    qDebug() << indexCounter;
    qDebug() << indexCounter.count();

    if ( indexCounter.count() > 1 )
    {
        for ( int i = 0; i < 17; i++)
        {
            if ( true == definedComponent[ValIndex].portName.at(i).isEmpty() )
            {
                break;
            }
            definedComponent[ValIndex].portName[i].push_back("_" + QString::number(indexCounter.count()));
           // qDebug() <<"succes";
        }
        for ( int i = 0; i < 8; i++)
        {
            if ( true == genericComponent[ValIndex].genericName.at(i).isEmpty() )
            {
                break;
            }
            genericComponent[ValIndex].genericName[i].append("_" + QString::number(indexCounter.count()));
        }
    }
  //  if ( true == existingNamesList.contains(definedComponent[ValIndex].name,Qt::CaseInsensitive ) )
//    {
     //   for ( int i = 0; i < existingNamesList.size(); i++)
     //   {
      //      if ( existingNamesList[i] == definedComponent[ValIndex].name)
      //      {
      //          apperanceCount++;
      //      }
    //    }
      //  if ( apperanceCount > 1)
     ////   {
     //       apperanceCount--;
    //        QString str;
    //        str.setNum(apperanceCount,10);
    //        definedComponent[whichHolder].name.append(str);
    //    }
    //}
}

void VhdlMain::saveAdd()
{
    m_saveAddFlag = true;
    pushOk();
}

void VhdlMain::pushOk()
{   
    if ( true == createWidget)
    {
        if (    ( true == xmlParser.Devices.contains(ui2.editName->text()))
             || ( true == bootDeviceList.contains(ui2.editName->text())  )  )
        {
            QMessageBox::warning ( this,"Name","Component with this name already exist");
            return;
        }
    }
    componentCnt++;
    if ( componentCnt == 21)
    {
        componentCnt--;
        if ( m_xmlNumberFlag == false)
        {
            this->componentWizard->close();
            createWidget = false;
        }
        QMessageBox::warning ( this, "Components", "Maximum number of components reached");
        return;
    }

    // TO DO HANDLOVAT PREPSANI

    if ( true == m_saveAddFlag)
    {
        unsigned int writeSpace;
        if ( m_xmlEditFlag == true)
        {
            writeSpace = m_xmlEditSpaceNum;
            m_xmlEditFlag = false;
               xmlParser.xmlDefinedComponent[writeSpace].xmlName.clear();
               xmlParser.xmlDefinedComponent[writeSpace].xmlPortName.clear();
               xmlParser.xmlDefinedComponent[writeSpace].xmlPortDirection.clear();
               xmlParser.xmlDefinedComponent[writeSpace].xmlPortValue.clear();
               xmlParser.xmlDefinedComponent[writeSpace].xmlLsbNumber.clear();
               xmlParser.xmlDefinedComponent[writeSpace].xmlMsbNumber.clear();
               xmlParser.xmlGenericComponent[writeSpace].xmlGenericName.clear();
               xmlParser.xmlGenericComponent[writeSpace].xmlGenericValue.clear();
               xmlParser.xmlGenericComponent[writeSpace].xmlLsbNumber.clear();
               xmlParser.xmlGenericComponent[writeSpace].xmlMsbNumber.clear();
               xmlParser.xmlGenericComponent[writeSpace].xmlPortType.clear();
        }
        else
        {
            writeSpace = ( xmlParser.getFreeRoom() - 1 );
        }
        qDebug() << "write space" << writeSpace;
        xmlParser.xmlDefinedComponent[writeSpace].xmlName = ui2.editName->text();

        xmlParser.xmlDefinedComponent[writeSpace].xmlPortName
            << ui2.editPort_9->text()
            << ui2.editPort_10->text()
            << ui2.editPort_11->text()
            << ui2.editPort_12->text()
            << ui2.editPort_13->text()
            << ui2.editPort_14->text()
            << ui2.editPort_15->text()
            << ui2.editPort_16->text()
            << ui2.editPort_17->text()
            << ui2.editPort_18->text()
            << ui2.editPort_19->text()
            << ui2.editPort_20->text()
            << ui2.editPort_21->text()
            << ui2.editPort_22->text()
            << ui2.editPort_23->text()
            << ui2.editPort_24->text()
            << ui2.editPort_25->text()
            << "";             // for checking if last
        qDebug() << "PORT NAMES EDITED "<< xmlParser.xmlDefinedComponent[writeSpace].xmlPortName;

        // get port directions
        xmlParser.xmlDefinedComponent[writeSpace].xmlPortDirection
            << ui2.comboDirection_9->currentText()
            << ui2.comboDirection_10->currentText()
            << ui2.comboDirection_11->currentText()
            << ui2.comboDirection_12->currentText()
            << ui2.comboDirection_13->currentText()
            << ui2.comboDirection_14->currentText()
            << ui2.comboDirection_15->currentText()
            << ui2.comboDirection_16->currentText()
            << ui2.comboDirection_17->currentText()
            << ui2.comboDirection_18->currentText()
            << ui2.comboDirection_19->currentText()
            << ui2.comboDirection_20->currentText()
            << ui2.comboDirection_21->currentText()
            << ui2.comboDirection_22->currentText()
            << ui2.comboDirection_23->currentText()
            << ui2.comboDirection_24->currentText()
            << ui2.comboDirection_25->currentText();

        // get buses
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[0] = ui2.checkBus_9->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[1] = ui2.checkBus_10->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[2] = ui2.checkBus_11->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[3] = ui2.checkBus_12->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[4] = ui2.checkBus_13->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[5] = ui2.checkBus_14->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[6] = ui2.checkBus_15->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[7] = ui2.checkBus_16->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[8] = ui2.checkBus_17->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[9] = ui2.checkBus_18->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[10] = ui2.checkBus_19->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[11] = ui2.checkBus_20->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[12] = ui2.checkBus_21->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[13] = ui2.checkBus_22->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[14] = ui2.checkBus_23->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[15] = ui2.checkBus_24->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlBus[16] = ui2.checkBus_25->isChecked();

        // get buses
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[0] = ui2.checkSignal_9->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[1] = ui2.checkSignal_10->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[2] = ui2.checkSignal_11->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[3] = ui2.checkSignal_12->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[4] = ui2.checkSignal_13->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[5] = ui2.checkSignal_14->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[6] = ui2.checkSignal_15->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[7] = ui2.checkSignal_16->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[8] = ui2.checkSignal_17->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[9] = ui2.checkSignal_18->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[10] = ui2.checkSignal_19->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[11] = ui2.checkSignal_20->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[12] = ui2.checkSignal_21->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[13] = ui2.checkSignal_22->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[14] = ui2.checkSignal_23->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[15] = ui2.checkSignal_24->isChecked();
        xmlParser.xmlDefinedComponent[writeSpace].xmlSignal[16] = ui2.checkSignal_25->isChecked();
        // get msb numbers
        xmlParser.xmlDefinedComponent[writeSpace].xmlMsbNumber
            << ui2.editMSB_9->text()
            << ui2.editMSB_10->text()
            << ui2.editMSB_11->text()
            << ui2.editMSB_12->text()
            << ui2.editMSB_13->text()
            << ui2.editMSB_14->text()
            << ui2.editMSB_15->text()
            << ui2.editMSB_16->text()
            << ui2.editMSB_17->text()
            << ui2.editMSB_18->text()
            << ui2.editMSB_19->text()
            << ui2.editMSB_20->text()
            << ui2.editMSB_21->text()
            << ui2.editMSB_22->text()
            << ui2.editMSB_23->text()
            << ui2.editMSB_24->text()
            << ui2.editMSB_25->text();

        // get lsb numbers
        xmlParser.xmlDefinedComponent[writeSpace].xmlLsbNumber
            << ui2.editLSB_9->text()
            << ui2.editLSB_10->text()
            << ui2.editLSB_11->text()
            << ui2.editLSB_12->text()
            << ui2.editLSB_13->text()
            << ui2.editLSB_14->text()
            << ui2.editLSB_15->text()
            << ui2.editLSB_16->text()
            << ui2.editLSB_17->text()
            << ui2.editLSB_18->text()
            << ui2.editLSB_19->text()
            << ui2.editLSB_20->text()
            << ui2.editLSB_21->text()
            << ui2.editLSB_22->text()
            << ui2.editLSB_23->text()
            << ui2.editLSB_24->text()
            << ui2.editLSB_25->text();

        // get initialized numbers
        xmlParser.xmlDefinedComponent[writeSpace].xmlPortValue
            << ui2.editValue_9->text()
            << ui2.editValue_10->text()
            << ui2.editValue_11->text()
            << ui2.editValue_12->text()
            << ui2.editValue_13->text()
            << ui2.editValue_14->text()
            << ui2.editValue_15->text()
            << ui2.editValue_16->text()
            << ui2.editValue_17->text()
            << ui2.editValue_18->text()
            << ui2.editValue_19->text()
            << ui2.editValue_20->text()
            << ui2.editValue_21->text()
            << ui2.editValue_22->text()
            << ui2.editValue_23->text()
            << ui2.editValue_24->text()
            << ui2.editValue_25->text();

        // ***** GENERIC ******
        // get names of declared ports
        xmlParser.xmlGenericComponent[writeSpace].xmlGenericName << ui2.editPort_41->text()
             << ui2.editPort_42->text()
             << ui2.editPort_43->text()
             << ui2.editPort_44->text()
             << ui2.editPort_45->text()
             << ui2.editPort_46->text()
             << ui2.editPort_47->text()
             << ui2.editPort_48->text()
             << "";             // for checking if last

        // get port directions
        xmlParser.xmlGenericComponent[writeSpace].xmlPortType << ui2.comboType_41->currentText()
              << ui2.comboType_42->currentText()
              << ui2.comboType_43->currentText()
              << ui2.comboType_44->currentText()
              << ui2.comboType_45->currentText()
              << ui2.comboType_46->currentText()
              << ui2.comboType_47->currentText()
              << ui2.comboType_48->currentText();

        // get buses
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[0] = ui2.checkBus_41->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[1] = ui2.checkBus_42->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[2] = ui2.checkBus_43->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[3] = ui2.checkBus_44->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[4] = ui2.checkBus_45->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[5] = ui2.checkBus_46->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[6] = ui2.checkBus_47->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlBus[7] = ui2.checkBus_48->isChecked();

        // get buses
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[0] = ui2.checkSignal_41->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[1] = ui2.checkSignal_42->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[2] = ui2.checkSignal_43->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[3] = ui2.checkSignal_44->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[4] = ui2.checkSignal_45->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[5] = ui2.checkSignal_46->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[6] = ui2.checkSignal_47->isChecked();
        xmlParser.xmlGenericComponent[writeSpace].xmlConstant[7] = ui2.checkSignal_48->isChecked();

        // get msb numbers
        xmlParser.xmlGenericComponent[writeSpace].xmlMsbNumber     << ui2.editMSB_41->text()
          << ui2.editMSB_42->text()
          << ui2.editMSB_43->text()
          << ui2.editMSB_44->text()
          << ui2.editMSB_45->text()
          << ui2.editMSB_46->text()
          << ui2.editMSB_47->text()
          << ui2.editMSB_48->text();

        // get lsb numbers
        xmlParser.xmlGenericComponent[writeSpace].xmlLsbNumber     << ui2.editLSB_41->text()
          << ui2.editLSB_42->text()
          << ui2.editLSB_11->text()
          << ui2.editLSB_44->text()
          << ui2.editLSB_45->text()
          << ui2.editLSB_46->text()
          << ui2.editLSB_47->text()
          << ui2.editLSB_48->text();

        // get initialized numbers
        xmlParser.xmlGenericComponent[writeSpace].xmlGenericValue     << ui2.editValue_41->text()
          << ui2.editValue_42->text()
          << ui2.editValue_11->text()
          << ui2.editValue_44->text()
          << ui2.editValue_45->text()
          << ui2.editValue_46->text()
          << ui2.editValue_47->text()
          << ui2.editValue_48->text();

         xmlParser.writeXml(writeSpace);
         xmlParser.Devices << xmlParser.xmlDefinedComponent[writeSpace].xmlName;
         ui->listVystup->addItem(xmlParser.xmlDefinedComponent[writeSpace].xmlName);
         qDebug() << "name of writen xml component: "<< xmlParser.xmlDefinedComponent[writeSpace].xmlName;
         m_saveAddFlag = false;

         ui->listVystup->clear();
         xmlParser.Devices.clear();
         ui->listVystup->addItems(bootDeviceList);
         xmlParser.readXml();
         ui->listVystup->addItems(xmlParser.Devices);
    }

    if ( true == m_xmlNumberFlag )
    {
        definedComponent[componentCnt - 1].name = xmlParser.xmlDefinedComponent[m_xmlNumber].xmlName;

        // get component name
      //QRect(20, 170, 200, 25);
        xPos = 20;
        yPos = 145 + componentCnt * 25;
        yPosNew = 145 + ( componentCnt - 10) * 25;

        if ( this->componentObject[componentCnt-1]->isValid() == false )
        {
            if ( componentCnt > 10)
            {
                this->componentObject[componentCnt-1] = new QRect ( xPos + 200, yPosNew  , 200, 25);
            }
            else
            {
                this->componentObject[componentCnt-1] = new QRect ( xPos, yPos , 200, 25);
            }
        }
        // get names of declared ports
        for ( int i = 0; i < 17; i++)
        {
            definedComponent[componentCnt - 1].portName
                    << xmlParser.xmlDefinedComponent[m_xmlNumber].xmlPortName[i];
                if (16 == i)
                {
                    definedComponent[componentCnt - 1].portName << "";
                }

            // get port directions
            definedComponent[componentCnt - 1].portDirection
                << xmlParser.xmlDefinedComponent[m_xmlNumber].xmlPortDirection[i];

            // get buses
            definedComponent[componentCnt - 1].bus[i] = xmlParser.xmlDefinedComponent[m_xmlNumber].xmlBus[i];

            // get signals
            definedComponent[componentCnt - 1].signal[i] = xmlParser.xmlDefinedComponent[m_xmlNumber].xmlSignal[i];

            // get msb numbers
            definedComponent[componentCnt - 1].msbNumber
              << xmlParser.xmlDefinedComponent[m_xmlNumber].xmlMsbNumber[i];

            // get lsb numbers
            definedComponent[componentCnt - 1].lsbNumber
                << xmlParser.xmlDefinedComponent[m_xmlNumber].xmlLsbNumber[i];

            // get initialized numbers
            definedComponent[componentCnt - 1].portValue
                << xmlParser.xmlDefinedComponent[m_xmlNumber].xmlPortValue[i];
        }

        // IF GENERIC IS SELECTED, iNSERT GENERIC PARAMTERS
        // ************************************************

        for ( int i = 0; i < 8; i++)
        {
            // get names of declared ports
            genericComponent[componentCnt - 1].genericName
                << xmlParser.xmlGenericComponent[m_xmlNumber].xmlGenericName[i];
                if ( 7 == i)
                {
                    // for checking if last
                    genericComponent[componentCnt - 1].genericName    << "";
                }

            // get port directions
            genericComponent[componentCnt - 1].portType
                << xmlParser.xmlGenericComponent[m_xmlNumber].xmlPortType[i];

            // get buses
            genericComponent[componentCnt - 1].bus[i] = xmlParser.xmlGenericComponent[m_xmlNumber].xmlBus[i];

            // get constamts
            genericComponent[componentCnt - 1].constant[i] = xmlParser.xmlGenericComponent[m_xmlNumber].xmlConstant[i];

            // get msb numbers
                genericComponent[componentCnt - 1].msbNumber
                << xmlParser.xmlGenericComponent[m_xmlNumber].xmlMsbNumber[i];

            // get lsb numbers
                genericComponent[componentCnt - 1].lsbNumber
                 << xmlParser.xmlGenericComponent[m_xmlNumber].xmlLsbNumber[i];

            // get initialized numbers
                genericComponent[componentCnt - 1].genericValue
                << xmlParser.xmlGenericComponent[m_xmlNumber].xmlGenericValue[i];
        }

        // chceck if there is another component with same name, if so, add index to it
        validateName(componentCnt - 1);
        m_xmlNumberFlag = false;
    }
    else
    {
        definedComponent[componentCnt - 1].name = ui2.editName->text();
        if ( true == definedComponent[componentCnt - 1].name.isEmpty())
        {
            QMessageBox::warning ( this, "Name", "Please insert valid component name");
            componentCnt--;
            return;
        }

        // get component name
      //QRect(20, 170, 200, 25);
        xPos = 20;
        yPos = 145 + componentCnt * 25;
        yPosNew = 145 + ( componentCnt - 10) * 25;

        if ( this->componentObject[componentCnt-1]->isValid() == false )
        {
            if ( componentCnt > 10)
            {
                this->componentObject[componentCnt-1] = new QRect ( xPos + 200, yPosNew  , 200, 25);
            }
            else
            {
                this->componentObject[componentCnt-1] = new QRect ( xPos, yPos , 200, 25);
            }
        }


        // get names of declared ports
        definedComponent[componentCnt - 1].portName << ui2.editPort_9->text()
             << ui2.editPort_10->text()
             << ui2.editPort_11->text()
             << ui2.editPort_12->text()
             << ui2.editPort_13->text()
             << ui2.editPort_14->text()
             << ui2.editPort_15->text()
             << ui2.editPort_16->text()
             << ui2.editPort_17->text()
             << ui2.editPort_18->text()
             << ui2.editPort_19->text()
             << ui2.editPort_20->text()
             << ui2.editPort_21->text()
             << ui2.editPort_22->text()
             << ui2.editPort_23->text()
             << ui2.editPort_24->text()
             << ui2.editPort_25->text()
             << "";             // for checking if last

        // get port directions
        definedComponent[componentCnt - 1].portDirection << ui2.comboDirection_9->currentText()
              << ui2.comboDirection_10->currentText()
              << ui2.comboDirection_11->currentText()
              << ui2.comboDirection_12->currentText()
              << ui2.comboDirection_13->currentText()
              << ui2.comboDirection_14->currentText()
              << ui2.comboDirection_15->currentText()
              << ui2.comboDirection_16->currentText()
              << ui2.comboDirection_17->currentText()
              << ui2.comboDirection_18->currentText()
              << ui2.comboDirection_19->currentText()
              << ui2.comboDirection_20->currentText()
              << ui2.comboDirection_21->currentText()
              << ui2.comboDirection_22->currentText()
              << ui2.comboDirection_23->currentText()
              << ui2.comboDirection_24->currentText()
              << ui2.comboDirection_25->currentText();

        // get buses
        definedComponent[componentCnt - 1].bus[0] = ui2.checkBus_9->isChecked();
        definedComponent[componentCnt - 1].bus[1] = ui2.checkBus_10->isChecked();
        definedComponent[componentCnt - 1].bus[2] = ui2.checkBus_11->isChecked();
        definedComponent[componentCnt - 1].bus[3] = ui2.checkBus_12->isChecked();
        definedComponent[componentCnt - 1].bus[4] = ui2.checkBus_13->isChecked();
        definedComponent[componentCnt - 1].bus[5] = ui2.checkBus_14->isChecked();
        definedComponent[componentCnt - 1].bus[6] = ui2.checkBus_15->isChecked();
        definedComponent[componentCnt - 1].bus[7] = ui2.checkBus_16->isChecked();
        definedComponent[componentCnt - 1].bus[8] = ui2.checkBus_17->isChecked();
        definedComponent[componentCnt - 1].bus[9] = ui2.checkBus_18->isChecked();
        definedComponent[componentCnt - 1].bus[10] = ui2.checkBus_19->isChecked();
        definedComponent[componentCnt - 1].bus[11] = ui2.checkBus_20->isChecked();
        definedComponent[componentCnt - 1].bus[12] = ui2.checkBus_21->isChecked();
        definedComponent[componentCnt - 1].bus[13] = ui2.checkBus_22->isChecked();
        definedComponent[componentCnt - 1].bus[14] = ui2.checkBus_23->isChecked();
        definedComponent[componentCnt - 1].bus[15] = ui2.checkBus_24->isChecked();
        definedComponent[componentCnt - 1].bus[16] = ui2.checkBus_25->isChecked();

        // get buses
        definedComponent[componentCnt - 1].signal[0] = ui2.checkSignal_9->isChecked();
        definedComponent[componentCnt - 1].signal[1] = ui2.checkSignal_10->isChecked();
        definedComponent[componentCnt - 1].signal[2] = ui2.checkSignal_11->isChecked();
        definedComponent[componentCnt - 1].signal[3] = ui2.checkSignal_12->isChecked();
        definedComponent[componentCnt - 1].signal[4] = ui2.checkSignal_13->isChecked();
        definedComponent[componentCnt - 1].signal[5] = ui2.checkSignal_14->isChecked();
        definedComponent[componentCnt - 1].signal[6] = ui2.checkSignal_15->isChecked();
        definedComponent[componentCnt - 1].signal[7] = ui2.checkSignal_16->isChecked();
        definedComponent[componentCnt - 1].signal[8] = ui2.checkSignal_17->isChecked();
        definedComponent[componentCnt - 1].signal[9] = ui2.checkSignal_18->isChecked();
        definedComponent[componentCnt - 1].signal[10] = ui2.checkSignal_19->isChecked();
        definedComponent[componentCnt - 1].signal[11] = ui2.checkSignal_20->isChecked();
        definedComponent[componentCnt - 1].signal[12] = ui2.checkSignal_21->isChecked();
        definedComponent[componentCnt - 1].signal[13] = ui2.checkSignal_22->isChecked();
        definedComponent[componentCnt - 1].signal[14] = ui2.checkSignal_23->isChecked();
        definedComponent[componentCnt - 1].signal[15] = ui2.checkSignal_24->isChecked();
        definedComponent[componentCnt - 1].signal[16] = ui2.checkSignal_25->isChecked();

        // get msb numbers
        definedComponent[componentCnt - 1].msbNumber
          << ui2.editMSB_9->text()
          << ui2.editMSB_10->text()
          << ui2.editMSB_11->text()
          << ui2.editMSB_12->text()
          << ui2.editMSB_13->text()
          << ui2.editMSB_14->text()
          << ui2.editMSB_15->text()
          << ui2.editMSB_16->text()
          << ui2.editMSB_17->text()
          << ui2.editMSB_18->text()
          << ui2.editMSB_19->text()
          << ui2.editMSB_20->text()
          << ui2.editMSB_21->text()
          << ui2.editMSB_22->text()
          << ui2.editMSB_23->text()
          << ui2.editMSB_24->text()
          << ui2.editMSB_25->text();

        // get lsb numbers
        definedComponent[componentCnt - 1].lsbNumber
            <<  ui2.editLSB_9->text()
            << ui2.editLSB_10->text()
            << ui2.editLSB_11->text()
            << ui2.editLSB_12->text()
            << ui2.editLSB_13->text()
            << ui2.editLSB_14->text()
            << ui2.editLSB_15->text()
            << ui2.editLSB_16->text()
            << ui2.editLSB_17->text()
            << ui2.editLSB_18->text()
            << ui2.editLSB_19->text()
            << ui2.editLSB_20->text()
            << ui2.editLSB_21->text()
            << ui2.editLSB_22->text()
            << ui2.editLSB_23->text()
            << ui2.editLSB_24->text()
            << ui2.editLSB_25->text();

        // get initialized numbers
        definedComponent[componentCnt - 1].portValue
            << ui2.editValue_9->text()
            << ui2.editValue_10->text()
            << ui2.editValue_11->text()
            << ui2.editValue_12->text()
            << ui2.editValue_13->text()
            << ui2.editValue_14->text()
            << ui2.editValue_15->text()
            << ui2.editValue_16->text()
            << ui2.editValue_17->text()
            << ui2.editValue_18->text()
            << ui2.editValue_19->text()
            << ui2.editValue_20->text()
            << ui2.editValue_21->text()
            << ui2.editValue_22->text()
            << ui2.editValue_23->text()
            << ui2.editValue_24->text()
            << ui2.editValue_25->text();

        // IF GENERIC IS SELECTED, iNSERT GENERIC PARAMTERS
        // ************************************************

        // get names of declared ports
        genericComponent[componentCnt - 1].genericName << ui2.editPort_41->text()
             << ui2.editPort_42->text()
             << ui2.editPort_43->text()
             << ui2.editPort_44->text()
             << ui2.editPort_45->text()
             << ui2.editPort_46->text()
             << ui2.editPort_47->text()
             << ui2.editPort_48->text()
             << "";             // for checking if last

        // get port directions
        genericComponent[componentCnt - 1].portType << ui2.comboType_41->currentText()
              << ui2.comboType_42->currentText()
              << ui2.comboType_43->currentText()
              << ui2.comboType_44->currentText()
              << ui2.comboType_45->currentText()
              << ui2.comboType_46->currentText()
              << ui2.comboType_47->currentText()
              << ui2.comboType_48->currentText();

        // get buses
        genericComponent[componentCnt - 1].bus[0] = ui2.checkBus_41->isChecked();
        genericComponent[componentCnt - 1].bus[1] = ui2.checkBus_42->isChecked();
        genericComponent[componentCnt - 1].bus[2] = ui2.checkBus_43->isChecked();
        genericComponent[componentCnt - 1].bus[3] = ui2.checkBus_44->isChecked();
        genericComponent[componentCnt - 1].bus[4] = ui2.checkBus_45->isChecked();
        genericComponent[componentCnt - 1].bus[5] = ui2.checkBus_46->isChecked();
        genericComponent[componentCnt - 1].bus[6] = ui2.checkBus_47->isChecked();
        genericComponent[componentCnt - 1].bus[7] = ui2.checkBus_48->isChecked();

        // get buses
        genericComponent[componentCnt - 1].constant[0] = ui2.checkSignal_41->isChecked();
        genericComponent[componentCnt - 1].constant[1] = ui2.checkSignal_42->isChecked();
        genericComponent[componentCnt - 1].constant[2] = ui2.checkSignal_43->isChecked();
        genericComponent[componentCnt - 1].constant[3] = ui2.checkSignal_44->isChecked();
        genericComponent[componentCnt - 1].constant[4] = ui2.checkSignal_45->isChecked();
        genericComponent[componentCnt - 1].constant[5] = ui2.checkSignal_46->isChecked();
        genericComponent[componentCnt - 1].constant[6] = ui2.checkSignal_47->isChecked();
        genericComponent[componentCnt - 1].constant[7] = ui2.checkSignal_48->isChecked();

        // get msb numbers
        genericComponent[componentCnt - 1].msbNumber     << ui2.editMSB_41->text()
          << ui2.editMSB_42->text()
          << ui2.editMSB_43->text()
          << ui2.editMSB_44->text()
          << ui2.editMSB_45->text()
          << ui2.editMSB_46->text()
          << ui2.editMSB_47->text()
          << ui2.editMSB_48->text();

        // get lsb numbers
        genericComponent[componentCnt - 1].lsbNumber     << ui2.editLSB_41->text()
          << ui2.editLSB_42->text()
          << ui2.editLSB_11->text()
          << ui2.editLSB_44->text()
          << ui2.editLSB_45->text()
          << ui2.editLSB_46->text()
          << ui2.editLSB_47->text()
          << ui2.editLSB_48->text();

        // get initialized numbers
        genericComponent[componentCnt - 1].genericValue     << ui2.editValue_41->text()
          << ui2.editValue_42->text()
          << ui2.editValue_11->text()
          << ui2.editValue_44->text()
          << ui2.editValue_45->text()
          << ui2.editValue_46->text()
          << ui2.editValue_47->text()
          << ui2.editValue_48->text();

        qDebug() << componentCnt << " component cnt ";
        // chceck if there is another component with same name, if so, add index to it
        validateName(componentCnt - 1);

        // print xml
        // This will fill xml class component structure
        // Struct > xmlDefinedComponent
        // get names of declared ports
        // name
        // ONLY IF SAVEADD IS PUSHED

        // PREPISE POZOOOOR

        update();
        this->componentWizard->close();
        createWidget = false;
    }
    update();
}
void VhdlMain::pushCancel()
{
    this->componentWizard->close();
    createWidget = false;
}

void VhdlMain::setPath(QString in_path)
{
  m_path = in_path;    
}

void VhdlMain::saveFile()
{
    savePath = QFileDialog::getSaveFileName(this,"Output file", m_path);
    ui->Output->setText(savePath);
    //ui->infoLabel->setText("");
}

void VhdlMain::loadFile()
{
    // get path

    loadPath.clear();
    loadPath = QFileDialog::getOpenFileName(this,"Input file",m_path, tr("Symbol table (*.stbl);; All files (*.*)"));
    QFile file(loadPath);
    ui->Input->setText(loadPath);

    clearStructures();
    // open device for read only
    file.open(QIODevice::ReadOnly | QIODevice::Text);
  //  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
 //   {
  //      qDebug()<< "file not opened";
   //     return;
   // }

    // read from file, get number of port directives
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        if ( true == line.contains("portout", Qt::CaseInsensitive) )
        {
            portOutList << line;
            portOutCount++;
        }
        else if ( true == line.contains("portin", Qt::CaseInsensitive) )
        {
            portInList << line;
            portInCount++;
        }
        else if ( true == line.contains("port", Qt::CaseInsensitive) )
        {
            portList << line;
            portCount++;
        }
        line = in.readLine();
    }
    file.close();
    // get names and values of port directives, put them into global structures

    // IN OUT ports
    //QString port = QString ("%1
    //%2\n").arg(portInOut[i].name,QString::number(portInOut[i].value,16).toUpper().prepend("0x"));

    splitList(portOutCount,PORTOUT);
    splitList(portInCount,PORTIN);
    splitList(portCount,PORT);

    //ui->infoLabel->setText("Select Output file");
}

void VhdlMain::splitList(unsigned int count, unsigned int portType)
{
    QString simplifiedString;
    simplifiedString.clear();

    int f = 1;
    switch( portType)
    {
        default: qDebug()<< "wrong port type passed"; break;

        case PORT:
            for (unsigned int i = 0; i < count; i++)
            {
                simplifiedString = portList.at(i).simplified();
                splitList1 = simplifiedString.split(" "); //,QString::SkipEmptyParts

                // get name
                portInOut[i].name = splitList1.at(0);
                while( splitList1.at(f) == QString(".") )
                {
                    f++;
                }
                portInOut[i].value = splitList1.at(f + 1).toUInt(0,16);
            }
            break;
        case PORTIN:
            for (unsigned int i = 0; i < count; i++)
            {
                simplifiedString = portInList.at(i).simplified();
                splitList1 = simplifiedString.split(" "); //,QString::SkipEmptyParts
                // get name
                portIn[i].name = splitList1.at(0);
                while( splitList1.at(f) == QString(".") )
                {
                    f++;
                }
                portIn[i].value = splitList1.at(f + 1).toUInt(0,16);
            }
            break;
        case PORTOUT:
            for (unsigned int i = 0; i < count; i++)
            {
                simplifiedString = portOutList.at(i).simplified();
                splitList1 = simplifiedString.split(" "); //,QString::SkipEmptyParts
                // get name
                portOut[i].name = splitList1.at(0);
                while( splitList1.at(f) == QString(".") )
                {
                    f++;
                }
                portOut[i].value = splitList1.at(f + 1).toUInt(0,16);
            }
            break;
    }
    return;
}

void VhdlMain::printToFile()
{
    if ( NULL == savePath || NULL == loadPath)
    {
        qDebug()<< "no files selected";
        QMessageBox::warning ( this, "Select files", "Please select Input ( .stbl ) and Output file");
        return;
    }
    QDate Date = QDate::currentDate();
    QTime Time = QTime::currentTime();
    int month = Date.month();
    int day = Date.day();
    int year = Date.year();
    int sec = Time.second();
    int hour = Time.hour();
    int min = Time.minute();


    // create file to write
    qDebug()<< savePath;
    QFile outFile(savePath);

    outFile.open(QIODevice::WriteOnly | QIODevice::Text);
   // if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
  //  {
  //      qDebug()<< "file not opened";
  //      return;
  //  }
    QTextStream out(&outFile);

    out << "--File created: " << hour << ":" << min << ":" << sec << " " << day << "." << month << "." << year <<"\n";
    out << "--This file has been generated by MDS VHDL wizard" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    out << "--" << endl;
    out << "--              *** PLEASE NOTE THIS IS NOT A COMPLETE DESIGN *** " << endl;
    out << "--" << endl;
    out << "-- This file contains sections of VHDL code intended to be helpful" << endl;
    out << "-- reference when using a PicoBlaze processor" << endl;
    out << "-------------------------------------------------" <<"\n";
    out << "library IEEE;" <<"\n";
    out << "use IEEE.STD_LOGIC_1164.ALL;" <<"\n";
    out << "use IEEE.STD_LOGIC_ARITH.ALL;" <<"\n";
    out << "use IEEE.STD_LOGIC_UNSIGNED.ALL;" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    out << "ENTITY " << entityName << " IS"<<"\n";
    out << "    PORT(" <<"\n";
    out << "         );" << endl;
    out << "END " << entityName <<"\n" <<"\n";

    out << "-------------------------------------------------" <<"\n";
    out << "ARCHITECTURE Behavioral OF " << entityName << " IS" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    // TO DO : ENTITY

    // generate constants for input and output multiplexer
    // generate Input and output multiplexer      
    out << "-------------------------------------------------" <<"\n";
    out << "-- PicoBlaze peripherals PORT IDs generated from your assembler design" <<"\n";

    if ( 0 != portCount )
    {
        for (unsigned int i = 0; i < portCount; i++)
        {           
            out << " CONSTANT " << portInOut[i].name;
            m_spaceNum = tabsNumber(portInOut[i].name);
            for (unsigned int  l = 0; l <= m_spaceNum; l++)
            {
                out << ' ';
            }
            out << ": STD_LOGIC_VECTOR( 7 DOWNTO 0):= X"  << '"' << QString::number(portInOut[i].value,16).toUpper() << '"' << ';' << endl;
        }
    }
    if ( 0 != portInCount )
    {
        for (unsigned int i = 0; i < portInCount; i++)
        {
            out << " CONSTANT " << portIn[i].name;
            m_spaceNum = tabsNumber(portIn[i].name);
            for (unsigned int  l = 0; l <= m_spaceNum; l++)
            {
                out << ' ';
            }
            out << ": STD_LOGIC_VECTOR( 7 DOWNTO 0):= X"  << '"' << QString::number(portIn[i].value,16).toUpper();
            out << '"' << ';' << endl;
        }
    }
    if ( 0 != portOutCount )
    {
        for (unsigned int i = 0; i < portOutCount; i++)
        {
            out << " CONSTANT " << portOut[i].name;
            m_spaceNum = tabsNumber(portOut[i].name);
            for (unsigned int  l = 0; l <= m_spaceNum; l++)
            {
                out << ' ';
            }
            out << ": STD_LOGIC_VECTOR( 7 DOWNTO 0):= X"  << '"' << QString::number(portOut[i].value,16).toUpper();
            out << '"' << ';' << endl;
        }
    }
    // print generic CONSTANTS if Generic is available
    if ( 0 != componentCnt )
    {
        for ( unsigned int i = 0; i < componentCnt; i++)
        {
            unsigned int p = 0;
            while ( false == genericComponent[i].genericName[p].isEmpty() )
            {
                if ( false == genericComponent[i].constant[p])
                {
                    continue;
                }

                out << " CONSTANT " << genericComponent[i].genericName[p];
                m_spaceNum = tabsNumber(genericComponent[i].genericName[p]);
                for (unsigned int  l = 0; l <= m_spaceNum; l++)
                {
                    out << ' ';
                }
                out << ": ";

                // BUS // EMPTY //
                switch ( getEnum(genericComponent[i].portType[p]) )
                {
                    case LOGIC:
                            if ( true == genericComponent[i].genericValue[p].isEmpty() )
                            {
                                out << "STD_LOGIC;" << endl;
                            }
                            else
                            {
                                out << "STD_LOGIC:= '" << genericComponent[i].genericValue[p] << "';" << endl;
                            }
                        break;
                    case LOGIC_VECTOR:
                            if ( true == genericComponent[i].genericValue[p].isEmpty() )
                            {
                                out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                << " DOWNTO " << genericComponent[i].lsbNumber[p]  << ");" << endl;
                            }
                            else
                            {
                                m_hex = genericComponent[i].genericValue[p].toInt(0,10);
                                out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                    << " DOWNTO " << genericComponent[i].lsbNumber[p] << ")";
                                out << ":= X" << '"' << QString::number( m_hex, 16).toUpper() << '"' << ";" << endl;
                            }
                        break;
                    case INTEGER:
                        if ( true == genericComponent[i].genericValue[p].isEmpty() )
                        {
                            out << "INTEGER;" << endl;
                        }
                        else
                        {
                            out << "INTEGER:="<< genericComponent[i].genericValue[p] << ";" << endl;
                        }
                            break;
                    case POSITIVE:
                        if ( true == genericComponent[i].genericValue[p].isEmpty() )
                        {
                            out << "INTEGER;" << endl;
                        }
                        else
                        {
                            out << "INTEGER:="<< genericComponent[i].genericValue[p] << ";" << endl;
                        }
                        break;
                    case NATURAL:
                        if ( true == genericComponent[i].genericValue[p].isEmpty() )
                        {
                            out << "INTEGER;" << endl;
                        }
                        else
                        {
                            out << "INTEGER:="<< genericComponent[i].genericValue[p] << ";" << endl;
                        }
                        break;
                    default: qDebug() << " Bad enum number";
                        break;
                 }//switch
             p++;
            }
        }
    }

    out << " ---------------------------------------------------------------------" <<"\n";
    out << " --Signal definitions" <<"\n";
    out << " ---------------------------------------------------------------------" <<"\n";
    out << " -- processor interface" <<"\n";
    switch ( ui->comboKcpsm->currentIndex() )
    {
        // kcpsmcpld
        case 0:
            out << " SIGNAL Pico_Data_in:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Data_out:           STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Port_ID:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_rd:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_wr:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_int:                STD_LOGIC :='0';" <<"\n";
            out << " SIGNAL Pico_clk:                STD_LOGIC;" <<"\n";
            break;
        // kcpsm1
        case 1:
            out << " SIGNAL Pico_Data_in:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Data_out:           STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Port_ID:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_rd:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_wr:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_int:                STD_LOGIC :='0';" <<"\n";
            out << " SIGNAL Pico_clk:                STD_LOGIC;" <<"\n";
            break;
        // kcpsm2
        case 2:
            out << " SIGNAL Pico_Data_in:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Data_out:           STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Port_ID:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_rd:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_wr:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_int:                STD_LOGIC :='0';" <<"\n";
            out << " SIGNAL Pico_clk:                STD_LOGIC;" <<"\n";
            break;
        // kcpsm3
        case 3:
            out << " SIGNAL Pico_Data_in:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Data_out:           STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Port_ID:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_rd:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_wr:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_int:                STD_LOGIC :='0';" <<"\n";
            out << " SIGNAL Pico_int_ack:            STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_clk:                STD_LOGIC;" <<"\n";
            break;
        // kcpsm6
        case 4:
            out << " SIGNAL Pico_Data_in:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Data_out:           STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_Port_ID:            STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << " SIGNAL Pico_rd:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_wr:                 STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_k_wr:               STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_int:                STD_LOGIC :='0';" <<"\n";
            out << " SIGNAL Pico_int_ack:            STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_clk:                STD_LOGIC;" <<"\n";
            out << " SIGNAL Pico_sleep:              STD_LOGIC;" <<"\n";
            break;
        default: qDebug() << "wrong kcpsm selected";
            break;
    }
    // done > signals from user defined components
    if ( 0 != componentCnt )
    {
        for ( unsigned int i = 0; i < componentCnt; i++)
        {
            unsigned int p;
            p = 0;
            while ( false == definedComponent[i].portName.at(p).isEmpty() )
            {
                if ( true == definedComponent[i].signal[p])
                {
                    out << " SIGNAL " << definedComponent[i].portName.at(p) << ": ";
                    m_spaceNum = tabsNumber(definedComponent[i].portName.at(p)) + 2;
                    for (unsigned int  l = 0; l <= m_spaceNum; l++)
                    {
                        out << ' ';
                    }
                    if ( false == definedComponent[i].bus[p])
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            out << "STD_LOGIC:= '" << definedComponent[i].portValue[p] << "';" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC;" << endl;
                        }
                    }
                    else
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            m_hex = definedComponent[i].portValue[p].toInt(0,10);
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << ")";
                            out << ":= X" << '"' << QString::number( m_hex,16).toUpper() << '"' << ";" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << ");" << endl;
                        }
                    }
                }
                p++;
            }
        }
    }

    out << " ---------------------------------------------------------------------" <<"\n";
    out << "COMPONENT MPU_SYS_core" <<"\n";
    switch ( ui->comboKcpsm->currentIndex() )
    {
        // kcpsmcpld
        case 0:
            out << "PORT(" <<"\n";
            out << "    clk                  : IN  STD_LOGIC;" <<"\n";
            out << "    port_id              : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    write_strobe         : OUT STD_LOGIC;" <<"\n";
            out << "    out_port             : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    read_strobe          : OUT STD_LOGIC;" <<"\n";
            out << "    in_port              : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    interrupt            : IN  STD_LOGIC;" <<"\n";
            out << "    END COMPONENT;" <<"\n";
            break;
        // kcpsm1
        case 1:
            out << "PORT(" <<"\n";
            out << "    clk                  : IN  STD_LOGIC;" <<"\n";
            out << "    port_id              : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    write_strobe         : OUT STD_LOGIC;" <<"\n";
            out << "    out_port             : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    read_strobe          : OUT STD_LOGIC;" <<"\n";
            out << "    in_port              : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    interrupt            : IN  STD_LOGIC;" <<"\n";
            out << "    END COMPONENT;" <<"\n";
            break;
        // kcpsm2
        case 2:
            out << "PORT(" <<"\n";
            out << "    clk                  : IN  STD_LOGIC;" <<"\n";
            out << "    port_id              : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    write_strobe         : OUT STD_LOGIC;" <<"\n";
            out << "    out_port             : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    read_strobe          : OUT STD_LOGIC;" <<"\n";
            out << "    in_port              : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    interrupt            : IN  STD_LOGIC;" <<"\n";
            out << "    END COMPONENT;" <<"\n";
            break;
        // kcpsm3
        case 3:
            out << "PORT(" <<"\n";
            out << "    clk                  : IN  STD_LOGIC;" <<"\n";
            out << "    port_id              : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    write_strobe         : OUT STD_LOGIC;" <<"\n";
            out << "    out_port             : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    read_strobe          : OUT STD_LOGIC;" <<"\n";
            out << "    in_port              : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    interrupt            : IN  STD_LOGIC;" <<"\n";
            out << "    interrupt_ack        : OUT STD_LOGIC);" <<"\n";
            out << "    END COMPONENT;" <<"\n";
            break;
        // kcpsm6
        case 4:
            out << "GENERIC(HW_build         : STD_LOGIC_VECTOR(7 DOWNTO 0))" <<"\n";
            out << "PORT(" <<"\n";
            out << "    clk                  : IN  STD_LOGIC;" <<"\n";
            out << "    port_id              : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    write_strobe         : OUT STD_LOGIC;" <<"\n";
            out << "    k_write_strobe       : OUT STD_LOGIC;" <<"\n";
            out << "    out_port             : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    read_strobe          : OUT STD_LOGIC;" <<"\n";
            out << "    in_port              : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);" <<"\n";
            out << "    interrupt            : IN  STD_LOGIC;" <<"\n";
            out << "    interrupt_ack        : OUT STD_LOGIC;" <<"\n";
            out << "    sleep                : IN  STD_LOGIC);" <<"\n";
            out << "    END COMPONENT;" <<"\n";
            break;
        default: qDebug() << "wrong kcpsm selected";
            break;
    }

    // Print defined components
    //  TODOCOMPONENTS VYPIS GNERIC
    if ( 0 != componentCnt )
    {
        for ( unsigned int i = 0; i < componentCnt; i++)
        {
            int countNumber = 0;
            out << "-------------------------------------------------" <<"\n";
            countNumber = getNumberOfInstances( definedComponent[i].name, true);
            if ( countNumber > 1)
            {
                out << " COMPONENT " << definedComponent[i].name << countNumber << endl;
            }
            else
            {
                out << " COMPONENT " << definedComponent[i].name <<"\n";
            }
            if ( false == genericComponent[i].genericName[0].isEmpty() )
            {
                unsigned int p;
                p = 0;
                out << " GENERIC MAP(" <<"\n";

                while ( false == genericComponent[i].genericName[p].isEmpty() )
                {
                    out << '\t' << genericComponent[i].genericName[p];
                    m_spaceNum = tabsNumber(genericComponent[i].genericName[p]);
                    for (unsigned int  l = 0; l <= m_spaceNum; l++)
                    {
                        out << ' ';
                    }
                    out << ": ";

                    // BUS // EMPTY //
                    switch ( getEnum(genericComponent[i].portType[p]) )
                    {
                        case LOGIC:
                                if ( true == genericComponent[i].genericValue[p].isEmpty() )
                                {
                                    if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                    {
                                        out << "STD_LOGIC);" << endl;
                                    }
                                    else
                                    {
                                        out << "STD_LOGIC;" << endl;
                                    }
                                }
                                else
                                {
                                    if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                    {
                                        out << "STD_LOGIC:= '" << genericComponent[i].genericValue[p] << "');" << endl;
                                    }
                                    else
                                    {
                                        out << "STD_LOGIC:= '" << genericComponent[i].genericValue[p] << "';" << endl;
                                    }
                                }
                            break;
                        case LOGIC_VECTOR:
                                if ( true == genericComponent[i].genericValue[p].isEmpty() )
                                {
                                    if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                    {
                                        out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                        << " DOWNTO " << genericComponent[i].lsbNumber[p]  << "));" << endl;
                                    }
                                    else
                                    {
                                        out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                        << " DOWNTO " << genericComponent[i].lsbNumber[p]  << ");" << endl;
                                    }
                                }
                                else
                                {
                                    if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                    {
                                        m_hex = genericComponent[i].genericValue[p].toInt(0,10);
                                        out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                            << " DOWNTO " << genericComponent[i].lsbNumber[p] << ")";
                                        out << ":= X" << '"' << QString::number( m_hex,16).toUpper() << '"' << ");" << endl;
                                    }
                                    else
                                    {
                                        m_hex = genericComponent[i].genericValue[p].toInt(0,10);
                                        out << "STD_LOGIC_VECTOR(" << genericComponent[i].msbNumber[p]
                                            << " DOWNTO " << genericComponent[i].lsbNumber[p] << ")";
                                        out << ":= X" << '"' << QString::number( m_hex,16).toUpper() << '"' << ";" << endl;
                                    }
                                }
                            break;
                        case INTEGER:
                            if ( true == genericComponent[i].genericValue[p].isEmpty() )
                            {
                                if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                {
                                    out << "INTEGER);" << endl;
                                }
                                else
                                {
                                    out << "INTEGER;" << endl;
                                }
                            }
                            else
                            {
                                if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                {
                                    out << "INTEGER:="<< genericComponent[i].genericValue[p] << ");" << endl;
                                }
                                else
                                {
                                    out << "INTEGER:="<< genericComponent[i].genericValue[p] << ";" << endl;
                                }
                            }
                                break;
                        case POSITIVE:
                            if ( true == genericComponent[i].genericValue[p].isEmpty() )
                            {
                                if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                {
                                    out << "POSITIVE);" << endl;
                                }
                                else
                                {
                                    out << "POSITIVE;" << endl;
                                }
                            }
                            else
                            {
                                if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                                {
                                    out << "POSITIVE:="<< genericComponent[i].genericValue[p] << ");" << endl;
                                }
                                else
                                {
                                    out << "POSITIVE:="<< genericComponent[i].genericValue[p] << ";" << endl;
                                }
                            }
                            break;
                        case NATURAL:
                        if ( true == genericComponent[i].genericValue[p].isEmpty() )
                        {
                            if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                            {
                                out << "NATURAL);" << endl;
                            }
                            else
                            {
                                out << "NATURAL;" << endl;
                            }
                        }
                        else
                        {
                            if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                            {
                                out << "NATURAL:="<< genericComponent[i].genericValue[p] << ");" << endl;
                            }
                            else
                            {
                                out << "NATURAL:="<< genericComponent[i].genericValue[p] << ";" << endl;
                            }
                        }
                        break;
                        default: qDebug() << " Bad enum number";
                            break;
                     }//switch
                 p++;
                }
            }
            out << " PORT(" <<"\n";
            unsigned int p;
            p = 0;
            while ( true != definedComponent[i].portName.at(p).isEmpty() )
            {
                out << "    " << definedComponent[i].portName.at(p);
                m_spaceNum = tabsNumber(definedComponent[i].portName.at(p));
                for (unsigned int  l = 0; l <= m_spaceNum; l++)
                {
                    out << ' ';
                }
                out << ": ";
                // differs for IN, OUT, INOUT
                if ( "inout" == definedComponent[i].portDirection[p] )
                {
                    out << definedComponent[i].portDirection[p].toUpper() << "    ";
                }
                else if ( "out" == definedComponent[i].portDirection[p] )
                {
                    out << definedComponent[i].portDirection[p].toUpper() << "      ";
                }
                else
                {
                    out << definedComponent[i].portDirection[p].toUpper() << "       ";
                }

                if ( false == definedComponent[i].bus[p])
                {
                    if ( definedComponent[i].portName[p+1].isEmpty() == false)
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            out << "STD_LOGIC:= '" << definedComponent[i].portValue[p] << "';" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC;" << endl;
                        }
                    }
                    else
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            out << "STD_LOGIC:= '" << definedComponent[i].portValue[p] << "');" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC);" << endl;
                        }
                    }
                }
                else
                {
                    if ( definedComponent[i].portName.at(p+1).isEmpty() == true)
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            m_hex = definedComponent[i].portValue[p].toInt(0,10);
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << ")";
                            out << ":= X" << '"' << QString::number( m_hex,16).toUpper() << '"' << ");" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << "));" << endl;
                        }
                    }
                    else
                    {
                        if ( false == definedComponent[i].portValue[p].isEmpty() )
                        {
                            m_hex = definedComponent[i].portValue[p].toInt(0,10);
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << ")";
                            out << ":= X" << '"' << QString::number( m_hex,16).toUpper()  << '"' << ";" << endl;
                        }
                        else
                        {
                            out << "STD_LOGIC_VECTOR(" << definedComponent[i].msbNumber.at(p)
                                << " DOWNTO " << definedComponent[i].lsbNumber.at(p) << ");" << endl;
                        }
                    }
                }
                p++;
            }
            out << " END COMPONENT;" <<"\n" <<"\n";
        }
    }

    out << "-------------------------------------------------" <<"\n";
    out << "BEGIN" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    out << "" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    out << "-- PicoBlaze processor (Program ROM included in the MPU module)" <<"\n" <<"\n";
    out << "MPU_SYS_core_i : MPU_SYS_core" <<"\n";
    switch ( ui->comboKcpsm->currentIndex() )
    {
        // kcpsmcpld
        case 0:
            out << "PORT MAP(" <<"\n";
            out << "clk                 => Pico_clk," <<"\n";
            out << "port_id             => Pico_Port_ID," <<"\n";
            out << "write_strobe        => Pico_wr," <<"\n";
            out << "out_port            => Pico_Data_out," <<"\n";
            out << "read_strobe         => Pico_rd," <<"\n";
            out << "in_port             => Pico_Data_in," <<"\n";
            out << "interrupt           => Pico_int," <<"\n";
            out << "-------------------------------------------------" <<"\n" <<"\n";
            break;
        // kcpsm1
        case 1:
            out << "PORT MAP(" <<"\n";
            out << "clk                 => Pico_clk," <<"\n";
            out << "port_id             => Pico_Port_ID," <<"\n";
            out << "write_strobe        => Pico_wr," <<"\n";
            out << "out_port            => Pico_Data_out," <<"\n";
            out << "read_strobe         => Pico_rd," <<"\n";
            out << "in_port             => Pico_Data_in," <<"\n";
            out << "interrupt           => Pico_int," <<"\n";
            out << "-------------------------------------------------" <<"\n" <<"\n";
            break;
        // kcpsm2
        case 2:
            out << "PORT MAP(" <<"\n";
            out << "clk                 => Pico_clk," <<"\n";
            out << "port_id             => Pico_Port_ID," <<"\n";
            out << "write_strobe        => Pico_wr," <<"\n";
            out << "out_port            => Pico_Data_out," <<"\n";
            out << "read_strobe         => Pico_rd," <<"\n";
            out << "in_port             => Pico_Data_in," <<"\n";
            out << "interrupt           => Pico_int," <<"\n";
            out << "-------------------------------------------------" <<"\n" <<"\n";
            break;
        // kcpsm3
        case 3:
            out << "PORT MAP(" <<"\n";
            out << "clk                 => Pico_clk," <<"\n";
            out << "port_id             => Pico_Port_ID," <<"\n";
            out << "write_strobe        => Pico_wr," <<"\n";
            out << "out_port            => Pico_Data_out," <<"\n";
            out << "read_strobe         => Pico_rd," <<"\n";
            out << "in_port             => Pico_Data_in," <<"\n";
            out << "interrupt           => Pico_int," <<"\n";
            out << "interrupt_ack       => Pico_int_ack);" <<"\n";
            out << "-------------------------------------------------" <<"\n" <<"\n";
            break;
        // kcpsm6
        case 4:
            out << "GENERIC MAP(" <<"\n";
            out << "    HW_build            => HW_build)" <<"\n";
            out << "PORT MAP(" <<"\n";
            out << "clk                 => Pico_clk," <<"\n";
            out << "port_id             => Pico_Port_ID," <<"\n";
            out << "write_strobe        => Pico_wr," <<"\n";
            out << "k_write_strobe      => Pico_k_wr," <<"\n";
            out << "out_port            => Pico_Data_out," <<"\n";
            out << "read_strobe         => Pico_rd," <<"\n";
            out << "in_port             => Pico_Data_in," <<"\n";
            out << "interrupt           => Pico_int," <<"\n";
            out << "interrupt_ack       => Pico_int_ack);" <<"\n";
            out << "sleep               => Pico_sleep);" <<"\n";
            out << "-------------------------------------------------" <<"\n" <<"\n";
            break;
        default: qDebug() << "wrong kcpsm selected";
            break;
    }

    out << "--" << endl;
    out << "-- Tie these inputs Low until you need them. Tying 'Pico_int' to 'Pico_int_ack'" << endl;
    out << "-- and 'Pico_sleep' to zero (kcpsm6 only)" << endl;
    out << "-- preserves both signals for future use and avoids a warning message." << endl;
    out << "--" << endl;

    if ( ui->comboKcpsm->currentText() == "KCPSM6" )
    {
        out << "Pico_sleep <= '0';" << endl;
    }
    out << "Pico_int   <= Pico_int_ack;" << endl << endl;

    // input and output multiplexer, using existing port ids
    // OUTPUT MULTIPLEXER
    out << "-------------------------------------------------" <<"\n";
    out << "--" << endl;
    out << "--output multiplexer using PORT_ids from your assembler design ( generated from .stbl file)" << endl;
    out << "--" << endl;
    out << "-------------------------------------------------" <<"\n";
    out << " output_ports:" << " PROCESS(clk) BEGIN" << "\n";
    out << '\t' << "IF rising_edge(clk) THEN" << "\n";
    out << '\t' << '\t' << "IF Pico_wr = '1' THEN" << "\n";
    out << '\t' << '\t' << '\t' << "CASE Pico_Port_ID IS" << "\n";

    for (unsigned int i = 0; i < portCount; i++)
    {
        out << '\t' << '\t' << '\t' << '\t' << "WHEN " << portInOut[i].name;
        m_spaceNum = tabsNumber(portInOut[i].name);
        for (unsigned int  l = 0; l <= m_spaceNum; l++)
        {
            out << ' ';
        }
        out << "=>";
        out << '\t' << '\t' << "Desired output connection" << '\t' << "<=" << " Pico_Data_out;" << "\n";
    }
    for (unsigned int i = 0; i < portOutCount; i++)
    {
        out << '\t' << '\t' << '\t' << '\t' << "WHEN " << portOut[i].name;
        m_spaceNum = tabsNumber(portOut[i].name);
        for (unsigned int  l = 0; l <= m_spaceNum; l++)
        {
            out << ' ';
        }
        out << "=>";
        out << '\t' << '\t' << "Desired output connection" << '\t' << "<=" << " Pico_Data_out;" << "\n";
    }

    out << '\t' << '\t' << '\t' << '\t' << "WHEN OTHERS" << '\t' << '\t' << "  => NULL;" << "\n";
    out << '\t' << '\t' << '\t' << "END IF;" << "\n";
    out << '\t' << '\t' << "END IF;" << "\n";
    out << '\t' << "END IF;" << "\n";
    out  << "END PROCESS output_ports;" << "\n";

    // INPUT MULTIPLEXER
    out << "-------------------------------------------------" <<"\n";
    out << "--" << endl;
    out << "--input multiplexer using PORT_ids from your assembler design ( generated from .stbl file)" << endl;
    out << "--" << endl;
    out << "-------------------------------------------------" <<"\n";
    out << " input_ports:" << " PROCESS(clk) BEGIN" << "\n";
    out << '\t' << "IF rising_edge(clk) THEN" << "\n";
    out << '\t' << '\t' << "IF Pico_rd = '1' THEN" << "\n";
    out << '\t' << '\t' << '\t' << "CASE Pico_Port_ID IS" << "\n";

    for (unsigned int i = 0; i < portCount; i++)
    {
        out << '\t' << '\t' << '\t' << '\t' << "WHEN " << portInOut[i].name;
        m_spaceNum = tabsNumber(portInOut[i].name);
        for (unsigned int  l = 0; l <= m_spaceNum; l++)
        {
            out << ' ';
        }
        out << "=>";
        out << '\t' << "Pico_Data_in " << "<= " << '\t' << "Desired input connection;"  "\n";
    }
    for (unsigned int i = 0; i < portInCount; i++)
    {
        out << '\t' << '\t' << '\t' << '\t' << "WHEN " << portIn[i].name;
        m_spaceNum = tabsNumber(portIn[i].name);
        for (unsigned int  l = 0; l <= m_spaceNum; l++)
        {
            out << ' ';
        }
        out << "=>";
        out << '\t' << "Pico_Data_in " << "<= " << '\t' << "Desired input connection;"  "\n";
    }

    out << '\t' << '\t' << '\t' << '\t' << "WHEN OTHERS" << '\t' << '\t' << "  =>" << '\t' << "NULL;" << "\n";
    out << '\t' << '\t' << '\t' << "END IF;" << "\n";
    out << '\t' << '\t' << "END IF;" << "\n";
    out << '\t' << "END IF;" << "\n";
    out  << "END PROCESS input_ports;" << "\n";

    out << "--" << endl;
    out << "-- Component instantiation" << endl;
    out << "--" << endl;

    // INSTANTIATE COMPONENTS
    if ( 0 != componentCnt )
    {
        for ( unsigned int i = 0; i < componentCnt; i++)
        {
            int countNumber = 0;
            out << "-------------------------------------------------" <<"\n";
            countNumber = getNumberOfInstances(definedComponent[i].name, false );
            if ( countNumber > 1)
            {
                out << definedComponent[i].name << "_"  << countNumber << "_i";
                out << " : " << definedComponent[i].name << countNumber <<"\n";
            }
            else
            {
                out << definedComponent[i].name << "_i"  << " : " << definedComponent[i].name <<"\n";
            }
            if ( false == genericComponent[i].genericName[0].isEmpty() )
            {
                unsigned int p;
                p = 0;
                out << " GENERIC MAP(" <<"\n";
                while ( false == genericComponent[i].genericName[p].isEmpty() )
                {
                    out << "    " << genericComponent[i].genericName[p];
                    m_spaceNum = tabsNumber(genericComponent[i].genericName[p]);
                    for (unsigned int  l = 0; l <= m_spaceNum; l++)
                    {
                        out << ' ';
                    }
                    out  << "=> ";
                    if ( genericComponent[i].genericName[p+1].isEmpty() == true)
                    {
                        out  << genericComponent[i].genericName[p] << ")" << endl;
                    }
                    else
                    {
                        out  << genericComponent[i].genericName[p] << "," << endl;
                    }
                    p++;
                }
            }
            unsigned int p;
            p = 0;
            out << " PORT MAP(" <<"\n";
            while ( true != definedComponent[i].portName[p].isEmpty() )
            {
                out << "    " << definedComponent[i].portName[p];
                m_spaceNum = tabsNumber(definedComponent[i].portName[p]);
                for (unsigned int  l = 0; l <= m_spaceNum; l++)
                {
                    out << ' ';
                }
                out  << "=> ";
                if ( definedComponent[i].portName.at(p+1).isEmpty() == true)
                {
                    out  << definedComponent[i].portName[p] << ");" << endl;
                }
                else
                {
                    out  << definedComponent[i].portName[p] << "," << endl;
                }
                p++;
            }
        }
    }
    out << "-------------------------------------------------" <<"\n" <<"\n";
    out << "-------------------------------------------------" <<"\n";
    out << "END Behavioral;" << endl;
    out << "-------------------------------------------------" <<"\n";
    outFile.close();
    ui->InfoLabel->setText("Done");
    
    printedComponents.clear();
    printedComponentsDeclaration.clear();
}

int VhdlMain::getNumberOfInstances(QString & componentToInstantiate, bool Declaration)
{
    int instanceCounter = 0;
    if ( Declaration == true)
    {
        printedComponentsDeclaration << componentToInstantiate;
        for ( int i = 0; i < printedComponentsDeclaration.size(); i++)
        {
            if ( componentToInstantiate == printedComponentsDeclaration[i] )
            {
                instanceCounter++;
            }
        }
        return instanceCounter;
    }
    else
    {
        printedComponents << componentToInstantiate;
        for ( int i = 0; i < printedComponents.size(); i++)
        {
            if ( componentToInstantiate == printedComponents[i] )
            {
                instanceCounter++;
            }
        }
        return instanceCounter;
    }
}

unsigned int VhdlMain::tabsNumber(QString inputString)
{
    unsigned int tabsNum;
    tabsNum = 90;
    tabsNum -= inputString.size();
    if ( tabsNum < 70)
    {
        return 0;
    }
    else
    {
        return tabsNum - 70;
    }
}


void VhdlMain::componentWizardInitialization()
{
    // PORT INITIALIZATION

    // set initial configuration of widget componentwizzard
    // disable bus definitions
    ui2.editLSB_9->setDisabled(true);
    ui2.editLSB_10->setDisabled(true);
    ui2.editLSB_11->setDisabled(true);
    ui2.editLSB_12->setDisabled(true);
    ui2.editLSB_13->setDisabled(true);
    ui2.editLSB_14->setDisabled(true);
    ui2.editLSB_15->setDisabled(true);
    ui2.editLSB_16->setDisabled(true);
    ui2.editLSB_17->setDisabled(true);
    ui2.editLSB_18->setDisabled(true);
    ui2.editLSB_19->setDisabled(true);
    ui2.editLSB_20->setDisabled(true);
    ui2.editLSB_21->setDisabled(true);
    ui2.editLSB_22->setDisabled(true);
    ui2.editLSB_23->setDisabled(true);
    ui2.editLSB_24->setDisabled(true);
    ui2.editLSB_25->setDisabled(true);

    ui2.editMSB_9->setDisabled(true);
    ui2.editMSB_10->setDisabled(true);
    ui2.editMSB_11->setDisabled(true);
    ui2.editMSB_12->setDisabled(true);
    ui2.editMSB_13->setDisabled(true);
    ui2.editMSB_14->setDisabled(true);
    ui2.editMSB_15->setDisabled(true);
    ui2.editMSB_16->setDisabled(true);   
    ui2.editMSB_17->setDisabled(true);
    ui2.editMSB_18->setDisabled(true);
    ui2.editMSB_19->setDisabled(true);
    ui2.editMSB_20->setDisabled(true);
    ui2.editMSB_21->setDisabled(true);
    ui2.editMSB_22->setDisabled(true);
    ui2.editMSB_23->setDisabled(true);
    ui2.editMSB_24->setDisabled(true);
    ui2.editMSB_25->setDisabled(true);

    // set combo box items
    QStringList items=(QStringList()<<"in"<<"out"<<"inout");
    ui2.comboDirection_9->addItems(items);
    ui2.comboDirection_10->addItems(items);
    ui2.comboDirection_11->addItems(items);
    ui2.comboDirection_12->addItems(items);
    ui2.comboDirection_13->addItems(items);
    ui2.comboDirection_14->addItems(items);
    ui2.comboDirection_15->addItems(items);
    ui2.comboDirection_16->addItems(items);
    ui2.comboDirection_17->addItems(items);
    ui2.comboDirection_18->addItems(items);
    ui2.comboDirection_19->addItems(items);
    ui2.comboDirection_20->addItems(items);
    ui2.comboDirection_21->addItems(items);
    ui2.comboDirection_22->addItems(items);
    ui2.comboDirection_23->addItems(items);
    ui2.comboDirection_24->addItems(items);
    ui2.comboDirection_25->addItems(items);

    // set initial items
    ui2.comboDirection_9->setCurrentIndex(0);
    ui2.comboDirection_10->setCurrentIndex(0);
    ui2.comboDirection_11->setCurrentIndex(0);
    ui2.comboDirection_12->setCurrentIndex(0);
    ui2.comboDirection_13->setCurrentIndex(0);
    ui2.comboDirection_14->setCurrentIndex(0);
    ui2.comboDirection_15->setCurrentIndex(0);
    ui2.comboDirection_16->setCurrentIndex(0);    
    ui2.comboDirection_17->setCurrentIndex(0);
    ui2.comboDirection_18->setCurrentIndex(0);
    ui2.comboDirection_19->setCurrentIndex(0);
    ui2.comboDirection_20->setCurrentIndex(0);
    ui2.comboDirection_21->setCurrentIndex(0);
    ui2.comboDirection_22->setCurrentIndex(0);
    ui2.comboDirection_23->setCurrentIndex(0);
    ui2.comboDirection_24->setCurrentIndex(0);
    ui2.comboDirection_25->setCurrentIndex(0);

    // set validator to avoid strange numbers
    QRegExpValidator *decValidator = new QRegExpValidator(QRegExp("[0-9UXWLHuxwlh]{3}"), this);
    ui2.editLSB_9->setValidator(decValidator);
    ui2.editLSB_10->setValidator(decValidator);
    ui2.editLSB_11->setValidator(decValidator);
    ui2.editLSB_12->setValidator(decValidator);
    ui2.editLSB_13->setValidator(decValidator);
    ui2.editLSB_14->setValidator(decValidator);
    ui2.editLSB_15->setValidator(decValidator);
    ui2.editLSB_16->setValidator(decValidator);
    ui2.editLSB_17->setValidator(decValidator);
    ui2.editLSB_18->setValidator(decValidator);
    ui2.editLSB_19->setValidator(decValidator);
    ui2.editLSB_20->setValidator(decValidator);
    ui2.editLSB_21->setValidator(decValidator);
    ui2.editLSB_22->setValidator(decValidator);
    ui2.editLSB_23->setValidator(decValidator);
    ui2.editLSB_24->setValidator(decValidator);
    ui2.editLSB_25->setValidator(decValidator);
    ui2.editMSB_9->setValidator(decValidator);
    ui2.editMSB_10->setValidator(decValidator);
    ui2.editMSB_11->setValidator(decValidator);
    ui2.editMSB_12->setValidator(decValidator);
    ui2.editMSB_13->setValidator(decValidator);
    ui2.editMSB_14->setValidator(decValidator);
    ui2.editMSB_15->setValidator(decValidator);
    ui2.editMSB_16->setValidator(decValidator);
    ui2.editMSB_17->setValidator(decValidator);
    ui2.editMSB_18->setValidator(decValidator);
    ui2.editMSB_19->setValidator(decValidator);
    ui2.editMSB_20->setValidator(decValidator);
    ui2.editMSB_21->setValidator(decValidator);
    ui2.editMSB_22->setValidator(decValidator);
    ui2.editMSB_23->setValidator(decValidator);
    ui2.editMSB_24->setValidator(decValidator);
    ui2.editMSB_25->setValidator(decValidator);

    // value validation
    this->valueBinValidator = new QRegExpValidator(QRegExp("[01UXWLHuxwlh]"), this);
    this->valueValidator =    new QRegExpValidator(QRegExp("[0-9UXWLHuxwlh]{5}"), this);

    ui2.editValue_9->setValidator(valueValidator);
    ui2.editValue_10->setValidator(valueValidator);
    ui2.editValue_11->setValidator(valueValidator);
    ui2.editValue_12->setValidator(valueValidator);
    ui2.editValue_13->setValidator(valueValidator);
    ui2.editValue_14->setValidator(valueValidator);
    ui2.editValue_15->setValidator(valueValidator);
    ui2.editValue_16->setValidator(valueValidator);
    ui2.editValue_17->setValidator(valueValidator);
    ui2.editValue_18->setValidator(valueValidator);
    ui2.editValue_19->setValidator(valueValidator);
    ui2.editValue_20->setValidator(valueValidator);
    ui2.editValue_21->setValidator(valueValidator);
    ui2.editValue_22->setValidator(valueValidator);
    ui2.editValue_23->setValidator(valueValidator);
    ui2.editValue_24->setValidator(valueValidator);
    ui2.editValue_25->setValidator(valueValidator);

    // GENERIC INITIALIYATION
    ui2.editLSB_41->setDisabled(true);
    ui2.editLSB_42->setDisabled(true);
    ui2.editLSB_43->setDisabled(true);
    ui2.editLSB_44->setDisabled(true);
    ui2.editLSB_45->setDisabled(true);
    ui2.editLSB_46->setDisabled(true);
    ui2.editLSB_47->setDisabled(true);
    ui2.editLSB_48->setDisabled(true);

    ui2.checkBus_41->setDisabled(true);
    ui2.checkBus_42->setDisabled(true);
    ui2.checkBus_43->setDisabled(true);
    ui2.checkBus_44->setDisabled(true);
    ui2.checkBus_45->setDisabled(true);
    ui2.checkBus_46->setDisabled(true);
    ui2.checkBus_47->setDisabled(true);
    ui2.checkBus_48->setDisabled(true);


    ui2.editMSB_41->setDisabled(true);
    ui2.editMSB_42->setDisabled(true);
    ui2.editMSB_43->setDisabled(true);
    ui2.editMSB_44->setDisabled(true);
    ui2.editMSB_45->setDisabled(true);
    ui2.editMSB_46->setDisabled(true);
    ui2.editMSB_47->setDisabled(true);
    ui2.editMSB_48->setDisabled(true);

    // set combo box items
    items.clear();
    items << "logic" << "logicVector" << "integer" << "positive" << "natural";

    ui2.comboType_41->addItems(items);
    ui2.comboType_42->addItems(items);
    ui2.comboType_43->addItems(items);
    ui2.comboType_44->addItems(items);
    ui2.comboType_45->addItems(items);
    ui2.comboType_46->addItems(items);
    ui2.comboType_47->addItems(items);
    ui2.comboType_48->addItems(items);

    // set initial items
    ui2.comboType_41->setCurrentIndex(0);
    ui2.comboType_42->setCurrentIndex(0);
    ui2.comboType_43->setCurrentIndex(0);
    ui2.comboType_44->setCurrentIndex(0);
    ui2.comboType_45->setCurrentIndex(0);
    ui2.comboType_46->setCurrentIndex(0);
    ui2.comboType_47->setCurrentIndex(0);
    ui2.comboType_48->setCurrentIndex(0);

    // set validator to avoid strange numbers
    ui2.editLSB_41->setValidator(decValidator);
    ui2.editLSB_42->setValidator(decValidator);
    ui2.editLSB_43->setValidator(decValidator);
    ui2.editLSB_44->setValidator(decValidator);
    ui2.editLSB_45->setValidator(decValidator);
    ui2.editLSB_46->setValidator(decValidator);
    ui2.editLSB_47->setValidator(decValidator);
    ui2.editLSB_48->setValidator(decValidator);
    ui2.editMSB_41->setValidator(decValidator);
    ui2.editMSB_42->setValidator(decValidator);
    ui2.editMSB_43->setValidator(decValidator);
    ui2.editMSB_44->setValidator(decValidator);
    ui2.editMSB_45->setValidator(decValidator);
    ui2.editMSB_46->setValidator(decValidator);
    ui2.editMSB_47->setValidator(decValidator);
    ui2.editMSB_48->setValidator(decValidator);
}

void VhdlMain::busChecked()
{
    emit valueEditingFinished("text in lineEdit");
    // enable or disable bus definitions
    if ( true == ui2.checkBus_9->isChecked())
    {
        ui2.editLSB_9->setDisabled(false);
        ui2.editMSB_9->setDisabled(false);
        ui2.editValue_9->setValidator(valueValidator);
    }
    else
    {
        ui2.editLSB_9->setDisabled(true);
        ui2.editMSB_9->setDisabled(true);
    }
    if ( true == ui2.checkBus_10->isChecked())
    {
        ui2.editLSB_10->setDisabled(false);
        ui2.editMSB_10->setDisabled(false);

    }
    else
    {
        ui2.editLSB_10->setDisabled(true);
        ui2.editMSB_10->setDisabled(true);
    }
    if ( true == ui2.checkBus_11->isChecked())
    {
        ui2.editLSB_11->setDisabled(false);
        ui2.editMSB_11->setDisabled(false);
    }
    else
    {
        ui2.editLSB_11->setDisabled(true);
        ui2.editMSB_11->setDisabled(true);
    }
    if ( true == ui2.checkBus_12->isChecked())
    {
        ui2.editLSB_12->setDisabled(false);
        ui2.editMSB_12->setDisabled(false);
    }
    else
    {
        ui2.editLSB_12->setDisabled(true);
        ui2.editMSB_12->setDisabled(true);
    }
    if ( true == ui2.checkBus_13->isChecked())
    {
        ui2.editLSB_13->setDisabled(false);
        ui2.editMSB_13->setDisabled(false);
    }
    else
    {
        ui2.editLSB_13->setDisabled(true);
        ui2.editMSB_13->setDisabled(true);
    }
    if ( true == ui2.checkBus_14->isChecked())
    {
        ui2.editLSB_14->setDisabled(false);
        ui2.editMSB_14->setDisabled(false);
    }
    else
    {
        ui2.editLSB_14->setDisabled(true);
        ui2.editMSB_14->setDisabled(true);
    }
    if ( true == ui2.checkBus_15->isChecked())
    {
        ui2.editLSB_15->setDisabled(false);
        ui2.editMSB_15->setDisabled(false);
    }
    else
    {
        ui2.editLSB_15->setDisabled(true);
        ui2.editMSB_15->setDisabled(true);
    }
    if ( true == ui2.checkBus_16->isChecked())
    {
        ui2.editLSB_16->setDisabled(false);
        ui2.editMSB_16->setDisabled(false);

    }
    else
    {
        ui2.editLSB_16->setDisabled(true);
        ui2.editMSB_16->setDisabled(true);

    }
    if ( true == ui2.checkBus_17->isChecked())
    {
        ui2.editLSB_17->setDisabled(false);
        ui2.editMSB_17->setDisabled(false);

    }
    else
    {
        ui2.editLSB_17->setDisabled(true);
        ui2.editMSB_17->setDisabled(true);
    }
    if ( true == ui2.checkBus_18->isChecked())
    {
        ui2.editLSB_18->setDisabled(false);
        ui2.editMSB_18->setDisabled(false);

    }
    else
    {
        ui2.editLSB_18->setDisabled(true);
        ui2.editMSB_18->setDisabled(true);
    }
    if ( true == ui2.checkBus_19->isChecked())
    {
        ui2.editLSB_19->setDisabled(false);
        ui2.editMSB_19->setDisabled(false);

    }
    else
    {
        ui2.editLSB_19->setDisabled(true);
        ui2.editMSB_19->setDisabled(true);
    }
    if ( true == ui2.checkBus_20->isChecked())
    {
        ui2.editLSB_20->setDisabled(false);
        ui2.editMSB_20->setDisabled(false);

    }
    else
    {
        ui2.editLSB_20->setDisabled(true);
        ui2.editMSB_20->setDisabled(true);
    }
    if ( true == ui2.checkBus_21->isChecked())
    {
        ui2.editLSB_21->setDisabled(false);
        ui2.editMSB_21->setDisabled(false);

    }
    else
    {
        ui2.editLSB_21->setDisabled(true);
        ui2.editMSB_21->setDisabled(true);
    }
    if ( true == ui2.checkBus_22->isChecked())
    {
        ui2.editLSB_22->setDisabled(false);
        ui2.editMSB_22->setDisabled(false);

    }
    else
    {
        ui2.editLSB_22->setDisabled(true);
        ui2.editMSB_22->setDisabled(true);
    }
    if ( true == ui2.checkBus_23->isChecked())
    {
        ui2.editLSB_23->setDisabled(false);
        ui2.editMSB_23->setDisabled(false);

    }
    else
    {
        ui2.editLSB_23->setDisabled(true);
        ui2.editMSB_23->setDisabled(true);
    }
    if ( true == ui2.checkBus_24->isChecked())
    {
        ui2.editLSB_24->setDisabled(false);
        ui2.editMSB_24->setDisabled(false);

    }
    else
    {
        ui2.editLSB_24->setDisabled(true);
        ui2.editMSB_24->setDisabled(true);
    }
    // GENERIC TAB
    if ( true == ui2.checkBus_41->isChecked() && true == ui2.checkBus_41->isEnabled() )
    {
        ui2.editLSB_41->setDisabled(false);
        ui2.editMSB_41->setDisabled(false);

    }
    else
    {
        ui2.editLSB_41->setDisabled(true);
        ui2.editMSB_41->setDisabled(true);

    }
    if ( true == ui2.checkBus_42->isChecked() && true == ui2.checkBus_42->isEnabled())
    {
        ui2.editLSB_42->setDisabled(false);
        ui2.editMSB_42->setDisabled(false);

    }
    else
    {
        ui2.editLSB_42->setDisabled(true);
        ui2.editMSB_42->setDisabled(true);

    }
    if ( true == ui2.checkBus_43->isChecked() && true == ui2.checkBus_43->isEnabled())
    {
        ui2.editLSB_43->setDisabled(false);
        ui2.editMSB_43->setDisabled(false);

    }
    else
    {
        ui2.editLSB_43->setDisabled(true);
        ui2.editMSB_43->setDisabled(true);

    }
    if ( true == ui2.checkBus_44->isChecked() && true == ui2.checkBus_44->isEnabled())
    {
        ui2.editLSB_44->setDisabled(false);
        ui2.editMSB_44->setDisabled(false);

    }
    else
    {
        ui2.editLSB_44->setDisabled(true);
        ui2.editMSB_44->setDisabled(true);

    }
    if ( true == ui2.checkBus_45->isChecked() && true == ui2.checkBus_45->isEnabled())
    {
        ui2.editLSB_45->setDisabled(false);
        ui2.editMSB_45->setDisabled(false);

    }
    else
    {
        ui2.editLSB_45->setDisabled(true);
        ui2.editMSB_45->setDisabled(true);

    }
    if ( true == ui2.checkBus_46->isChecked() && true == ui2.checkBus_46->isEnabled())
    {
        ui2.editLSB_46->setDisabled(false);
        ui2.editMSB_46->setDisabled(false);

    }
    else
    {
        ui2.editLSB_46->setDisabled(true);
        ui2.editMSB_46->setDisabled(true);

    }
    if ( true == ui2.checkBus_47->isChecked() && true == ui2.checkBus_47->isEnabled())
    {
        ui2.editLSB_47->setDisabled(false);
        ui2.editMSB_47->setDisabled(false);

    }
    else
    {
        ui2.editLSB_47->setDisabled(true);
        ui2.editMSB_47->setDisabled(true);

    }
    if ( true == ui2.checkBus_48->isChecked() && true == ui2.checkBus_48->isEnabled())
    {
        ui2.editLSB_48->setDisabled(false);
        ui2.editMSB_48->setDisabled(false);

    }
    else
    {
        ui2.editLSB_48->setDisabled(true);
        ui2.editMSB_48->setDisabled(true);

    }
}

void VhdlMain::valueEditingFinished(QString /*textValue*/)
{
    if ( ( false == ui2.checkBus_9->isChecked() ) && ( ui2.editValue_9->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_9->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_9->setStyleSheet( QString( "background-color: white") );
    }

    if ( ( false == ui2.checkBus_10->isChecked() ) && ( ui2.editValue_10->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_10->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_10->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_11->isChecked() ) && ( ui2.editValue_11->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_11->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_11->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_12->isChecked() ) && ( ui2.editValue_12->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_12->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_12->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_13->isChecked() ) && ( ui2.editValue_13->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_13->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_13->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_14->isChecked() ) && ( ui2.editValue_14->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_14->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_14->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_15->isChecked() ) && ( ui2.editValue_15->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_15->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_15->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_16->isChecked() ) && ( ui2.editValue_16->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_16->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_16->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_17->isChecked() ) && ( ui2.editValue_17->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_17->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_17->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_18->isChecked() ) && ( ui2.editValue_18->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_18->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_18->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_19->isChecked() ) && ( ui2.editValue_19->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_19->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_19->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_20->isChecked() ) && ( ui2.editValue_20->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_20->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_20->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_21->isChecked() ) && ( ui2.editValue_21->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_21->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_21->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_22->isChecked() ) && ( ui2.editValue_22->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_22->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_22->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_23->isChecked() ) && ( ui2.editValue_23->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_23->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_23->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_24->isChecked() ) && ( ui2.editValue_24->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_24->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_24->setStyleSheet( QString( "background-color: white") );
    }
    if ( ( false == ui2.checkBus_25->isChecked() ) && ( ui2.editValue_25->text().toInt(0,10) > 1 ) )
    {
        ui2.editValue_25->setStyleSheet( QString( "background-color: red") );
        //ui.edtBox->setStyleSheet( QString( "background-color: gray"));
    }
    else
    {
        ui2.editValue_25->setStyleSheet( QString( "background-color: white") );
    }
}

void VhdlMain::componentWizardConstruct()
{
    ui2.pushSaveAdd->setIcon(QPixmap(":/resources/icons/disk.png"));
    ui2.pushOk->setIcon(QPixmap(":/resources/icons/add.png"));
    ui2.pushCancel->setIcon(QPixmap(":/resources/icons/cancel.png"));// editing signals

    // TO DO
    //STD_ULOGIC
    // ‘U’,uninitialized
    // ‘X’,strong 0 or 1 (= unknown)
    // ‘0’strong 0
    // ‘1’,strong 1
    // ‘Z’,high impedance
    // ‘W’,weak 0 or 1 (= unknown)
    // ‘L’,weak 0
    // ‘H’,weak 1
    // ‘-’,don`t care)

    connect(ui2.editValue_9,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_10,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_11,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_12,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_13,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_14,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_15,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_16,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_17,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_18,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_19,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_20,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_21,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_22,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_23,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_24,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_25,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    // GENERIC ui
    connect(ui2.editValue_41,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_42,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_43,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_44,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_45,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_46,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_47,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );
    connect(ui2.editValue_48,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(valueEditingFinished(QString))
           );

    // connect Type combobox IN GENERIC tab
    connect(ui2.comboType_41,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged1( int ))
           );
    connect(ui2.comboType_42,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged2( int ))
           );
    connect(ui2.comboType_43,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged3( int ))
           );
    connect(ui2.comboType_44,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged4( int ))
           );
    connect(ui2.comboType_45,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged5( int ))
           );
    connect(ui2.comboType_46,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged6( int ))
           );
    connect(ui2.comboType_47,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged7( int ))
           );
    connect(ui2.comboType_48,
            SIGNAL(currentIndexChanged( int ) ),
            this,
            SLOT(typeIndexChanged8( int ))
           );

    // connect chceck box signals
    connect(ui2.checkBus_9,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_10,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_11,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_12,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_13,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_14,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_15,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_16,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_17,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_18,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_19,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_20,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_21,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_22,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_23,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_24,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_25,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    // GENERIC ui
    connect(ui2.checkBus_41,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_42,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_43,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_44,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_45,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_46,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_47,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );
    connect(ui2.checkBus_48,
            SIGNAL(clicked()),
            this,
            SLOT(busChecked())
           );

    // Cancel and OK buttons
    connect(ui2.pushCancel,
            SIGNAL(clicked()),
            this,
            SLOT(pushCancel())
           );
    connect(ui2.pushOk,
            SIGNAL(clicked()),
            this,
            SLOT(pushOk())
           );
    connect(ui2.pushSaveAdd,
            SIGNAL(clicked()),
            this,
            SLOT(saveAdd())
           );
    return;
}

void VhdlMain::typeIndexChanged1( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_41->setDisabled(true);
            ui2.editLSB_41->setDisabled(true);
            ui2.editMSB_41->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_41->setDisabled(false);
            emit busChecked();
        }
            break;
    }
    emit valueEditingFinished("text in lineEdit");
}

void VhdlMain::typeIndexChanged2( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_42->setDisabled(true);
            ui2.editLSB_42->setDisabled(true);
            ui2.editMSB_42->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_42->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged3( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_43->setDisabled(true);
            ui2.editLSB_43->setDisabled(true);
            ui2.editMSB_43->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_43->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged4( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_44->setDisabled(true);
            ui2.editLSB_44->setDisabled(true);
            ui2.editMSB_44->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_44->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged5( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_45->setDisabled(true);
            ui2.editLSB_45->setDisabled(true);
            ui2.editMSB_45->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_45->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged6( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_46->setDisabled(true);
            ui2.editLSB_46->setDisabled(true);
            ui2.editMSB_46->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_46->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged7( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_47->setDisabled(true);
            ui2.editLSB_47->setDisabled(true);
            ui2.editMSB_47->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_47->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::typeIndexChanged8( int index)
{
    switch ( index )
    {
        case 0:
        case 2:
        case 3:
        case 4:
            ui2.checkBus_48->setDisabled(true);
            ui2.editLSB_48->setDisabled(true);
            ui2.editMSB_48->setDisabled(true);
            break;
        default:
        {
            ui2.checkBus_48->setDisabled(false);
            emit busChecked();
        }
            break;
    }
}

void VhdlMain::clearStructures()
{

    // set initial value to qstring lists
    portOutList.clear();
    longerList.clear();
    splitList2.clear();
    splitList1.clear();
    portList.clear();
    portInList.clear();
    portCount = 0;
    portInCount = 0;
    portOutCount = 0;

    int g = 0;
    while ( true != portInOut[g].name.isEmpty())
    {
        portInOut[g].name.clear();
        portInOut[g].type.clear();
        g++;
    }
    g = 0;
    while ( true != portIn[g].name.isEmpty())
    {
        portIn[g].name.clear();
        portIn[g].type.clear();
        g++;
    }
    g = 0;
    while ( true != portOut[g].name.isEmpty())
    {
        portOut[g].name.clear();
        portOut[g].type.clear();
        g++;
    }
}


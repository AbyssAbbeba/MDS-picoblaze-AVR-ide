#include "simswitch.h"
#include "ui_simswitch.h"
#include "../../../../simulators/MCUSim/PicoBlaze/PicoBlazeIO.h"
#include <QtGui>
#include <QList>

#define bit0    0x01
#define bit1    0x02
#define bit2    0x04
#define bit3    0x08
#define bit4    0x10
#define bit5    0x20
#define bit6    0x40
#define bit7    0x80

const QRect SimSwitch::switch_rect[8] = {
    {415, 50, 30, 30},
    {360, 50, 30, 30},
    {305, 50, 30, 30},
    {250, 50, 30, 30},
    {195, 50, 30, 30},
    {140, 50, 30, 30},
    {85,  50, 30, 30},
    {30, 50,  30, 30}
};



SimSwitch::SimSwitch(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Switch Panel");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
        return;
    }
        setMouseTracking ( true );
    //variable initializations
    address = 0;
    switchStatus = 0;
    flag_change = true;
    ui.Vystup->setText("0");
    
    boxNum[0] = ui.switchBox_7->currentIndex();
    boxNum[1] = ui.switchBox_6->currentIndex();
    boxNum[2] = ui.switchBox_5->currentIndex();
    boxNum[3] = ui.switchBox_4->currentIndex();
    boxNum[4] = ui.switchBox_3->currentIndex();
    boxNum[5] = ui.switchBox_2->currentIndex();
    boxNum[6] = ui.switchBox_1->currentIndex();
    boxNum[7] = ui.switchBox_0->currentIndex();

    for ( int i = 0; i <= 7; i++)
    {
        switchState[i] = 0;
        rectHighlight[i] = 0;
    }
    flag_0 = false; flag_1 = false; flag_2 = false; flag_3 = false;
    flag_4 = false; flag_5 = false; flag_6 = false;

    CreateItems();

    QRegExpValidator *hexValidator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{1,2}"), this);
    ui.lineEditAddress->setValidator(hexValidator);

    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << address;

    std::vector<int> mask = { PicoBlazeIO::EVENT_PICOBLAZEIO_WRITE };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask); //8

    connect(ui.lineEditAddress,
            SIGNAL(editingFinished()),
            this,
            SLOT(addrChanged())
           );

    connect(controlUnit,
            SIGNAL(updateRequest(int)),
            this,
            SLOT(handleUpdateRequest(int))
           );
/*
    connect(ui.switchBox_0 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_0(int)));
    connect(ui.switchBox_1 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_1(int)));
    connect(ui.switchBox_2 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_2(int)));
    connect(ui.switchBox_3 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_3(int)));
    connect(ui.switchBox_4 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_4(int)));
    connect(ui.switchBox_5 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_5(int)));
    connect(ui.switchBox_6 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_6(int)));
    connect(ui.switchBox_7 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange_7(int)));
*/
    deviceChanged();  //8
    ValueChanged();

}

void SimSwitch::deviceChanged()  //8
{
    if ( NULL == m_simControlUnit )
    {
       qDebug() << "PortHexEdit: m_simControlUnit is NULL";
    }
    if ( NULL == m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO) )
    {
        qDebug() << "PortHexEdit: m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO) is NULL";
    }
    m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO));
    deviceReset();
}


void SimSwitch::deviceReset()  //8
{
    ui.Vystup->setText(QString::number(this->switchStatus));
    m_plio->write(address,switchStatus); 
}

void SimSwitch::handleUpdateRequest(int mask) //8
{
    if ( 4 & mask)
    {
        ui.Vystup->setText(QString::number(this->switchStatus));
        m_plio->write(address,switchStatus);
    }
}

void SimSwitch::handleEvent(int subsysId, int eventId, int locationOrReason, int detail) //8
{
   
    if (MCUSimSubsys::ID_PLIO == subsysId)
    {
        switch ( eventId )
        {
//             case MCUSimPureLogicIO::EVENT_PLIO_READ:
//             {
//                 m_plio->write(address)
//                 break;
//             }
            case PicoBlazeIO::EVENT_PICOBLAZEIO_WRITE:
            {
                if ( locationOrReason == address)
                {
                    switchStatus = detail;
                    ui.Vystup->setText(QString::number(this->switchStatus));
                    update();
                }
            }
            default:
            {
                break;
            }
        }
    }
}


void SimSwitch::setReadOnly(bool /*readOnly*/)
{
    
}

// main loop, event
void SimSwitch::ValueChanged()
{
  //  update();
}

void SimSwitch::addrChanged()
{
    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << "changed address" << address;

    ui.Vystup->setText(QString::number(this->switchStatus));
    m_plio->write(address,switchStatus);
}

void SimSwitch::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    QPen redpen    (Qt::red);
    QPen graypen    (Qt::gray);
    QPen blackpen   (Qt::black);
    QPen blackpenRect   (Qt::black);
    QPen blackpenSwitch   (Qt::black);
    graypen.setWidth(3);
    blackpenRect.setWidth(3);
    blackpenSwitch.setWidth(1);

    unsigned char index = 0;
    // supply
    QPoint p1(30,20);
    QPoint psup1(33,24);    QPoint psup2(27,24);
    QPoint psup3(30,32);
   // odpor
    QPoint p10(33,32);    QPoint p11(27,32);    QPoint p12(27,45);    QPoint p13(23,45);
    QPoint p14(30,45);    QPoint p15(30,52);
    // pin
    QPoint pPIN(0,52);
    QPoint pPIN2(0,110);
    // switch
    QPoint psw1(30,70);     QPoint pswitch1(10,70); QPoint pswitch1ext(30,70);
    QPoint pswitch2(30,90); QPoint pswitch2ext(30,90);
    QPoint psw2(30,105);

    QPoint pgnd1(27,105);QPoint pgnd2(33,105);
    //painter.drawArc(psw1.x(),psw1.y(),2,2,2,2);

    painter.begin(this);
    painter.setPen(blackpen);

    // shifting
    for (int i = 0; i < 15;i++)
    {
         p1.ry()--;     psup1.ry()--;    psup2.ry()--;
         p10.ry()--;    p11.ry()--;  p12.ry()--; p13.ry()--; p14.ry()--;   p15.ry()--;
         pPIN.ry()--;   pPIN2.ry()--;
         psup3.ry()--;
         psw1.ry()--;   pswitch1.ry()--; pswitch2.ry()--;
         psw2.ry()--;   pswitch2ext.ry()--; pswitch1ext.ry()--;
         pgnd1.ry()--; pgnd2.ry()--;
    }

    unsigned int x_pos = 25;
    for ( int i = 0; i <= 7; i++)
    {
        p1.setX(20 + x_pos);  psup1.setX(23 + x_pos);    psup2.setX(17 + x_pos);

        p10.setX(23 + x_pos);   p11.setX(17 + x_pos);  p12.setX(17 + x_pos);
        p13.setX(23 + x_pos);
        p14.setX(20 + x_pos);   p15.setX(20 + x_pos);  pPIN.setX(x_pos-7);
        psup3.setX(20 + x_pos ); pPIN2.setX(x_pos-7);

        psw1.setX(20 + x_pos);     pswitch1.setX(10 + x_pos); pswitch2.setX(20 + x_pos);
        psw2.setX(20 + x_pos);      pswitch2ext.setX(18 + x_pos);
        pswitch1ext.setX(18 + x_pos);

        pgnd1.setX(14 + x_pos); pgnd2.setX(26 + x_pos);

        x_pos += 55;

      //  y_pos += 55;
        painter.setPen(blackpen);

        painter.drawLine(p10,p11);
        painter.drawLine(p10,p13);
        painter.drawLine(p11,p12);
        painter.drawLine(p14,p15);
        painter.drawLine(p12,p13);
        painter.drawLine(p1,psup2);
        painter.drawLine(p1,psup1);
        painter.drawLine(p15,pPIN);
        painter.drawLine(p1,psup3);
        painter.drawLine(pPIN,pPIN2);
        painter.drawLine(psw1,p15);
        painter.drawLine(pgnd1,pgnd2);
        QRect kontakt1(psw1.x()-2,psw1.y(),4,4);
        QRect kontakt2(pswitch2.x()-2,pswitch2.y()-4,4,4);
        painter.drawEllipse(kontakt1);
        painter.drawEllipse(kontakt2);

        // switch state
        //------------------------------------------
        if ( flag_change == false)
        {
            painter.setPen(blackpenSwitch);
            switch (i)
                {
                case 0:
                    if ( switchStatus & bit7)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 1:
                    if ( switchStatus & bit6)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 2:
                    if ( switchStatus & bit5)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 3:
                    if ( switchStatus & bit4)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 4:
                    if ( switchStatus & bit3)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 5:
                    if ( switchStatus & bit2)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 6:
                    if ( switchStatus & bit1)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
                case 7:
                    if ( switchStatus & bit0)
                    {
                        //closed
                        painter.drawLine(pswitch2ext,pswitch1ext);
                        painter.drawLine(pswitch2,psw2);
                    }
                    else
                    {
                        //opened
                        painter.drawLine(pswitch2ext,pswitch1);
                        painter.drawLine(pswitch2,psw2);
                    }
                    break;
            }
        }
        else
        {
            if ( i == 0)
            {
                index = ui.switchBox_7->currentIndex();
            }
            else if ( i == 1)
            {
                index = ui.switchBox_6->currentIndex();
            }
            else if ( i == 2)
            {
                index = ui.switchBox_5->currentIndex();
            }
            else if ( i == 3)
            {
                index = ui.switchBox_4->currentIndex();
            }
            else if ( i == 4)
            {
                index = ui.switchBox_3->currentIndex();
            }
            else if ( i == 5)
            {
                index = ui.switchBox_2->currentIndex();
            }
            else if ( i == 6)
            {
                index = ui.switchBox_1->currentIndex();
            }
            else if ( i == 7)
            {
                index = ui.switchBox_0->currentIndex();
            }

            switch ( index )
            {
            case 0: if ( switchStatus & bit0) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 1: if ( switchStatus & bit1) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 2: if ( switchStatus & bit2) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 3: if ( switchStatus & bit3) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 4: if ( switchStatus & bit4) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 5: if ( switchStatus & bit5) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 6: if ( switchStatus & bit6) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            case 7: if ( switchStatus & bit7) { painter.drawLine(pswitch2ext,pswitch1ext);  painter.drawLine(pswitch2,psw2);}
                    else                      { painter.drawLine(pswitch2ext,pswitch1);     painter.drawLine(pswitch2,psw2);}
                break;
            default: qDebug()<< "default in painting, wrong";
                break;
            }
        }

//------------------------------------------------
        // rectangles
        if ( rectHighlight[i] == false)
        {
            painter.setPen(graypen);
            painter.drawRect(switch_rect[i]);
        }
        else
        {
            painter.setPen(blackpenRect);
            painter.drawRect(switch_rect[i]);
        }
    }
//-----------------------------------------------
    painter.end();
}

void SimSwitch::mousePressEvent(QMouseEvent* event)
{
    for (int i = 7; i >= 0;i--)
    {
        if ( switch_rect[i].contains( event->pos() )  == true)
        {
            //switchState[i] = !switchState[i];
            rectClicked(i);
        }

        // test2 = test2 & (test << i);
       // qDebug() << test2;
    }

    //QString hodnota = "0";
    ui.Vystup->setText(QString::number(this->switchStatus));
    m_plio->write(address,switchStatus);
    update();
}

void SimSwitch::rectClicked(int pos)
{
    int groupBoxNumber;
    // Position of clicked rectangle, now find out if group box has been edited

    switch ( pos )
    {
        case 0:     groupBoxNumber = ui.switchBox_0->currentIndex();     break;
        case 1:     groupBoxNumber = ui.switchBox_1->currentIndex();     break;
        case 2:     groupBoxNumber = ui.switchBox_2->currentIndex();     break;
        case 3:     groupBoxNumber = ui.switchBox_3->currentIndex();     break;
        case 4:     groupBoxNumber = ui.switchBox_4->currentIndex();     break;
        case 5:     groupBoxNumber = ui.switchBox_5->currentIndex();     break;
        case 6:     groupBoxNumber = ui.switchBox_6->currentIndex();     break;
        case 7:     groupBoxNumber = ui.switchBox_7->currentIndex();     break;
        default:    qDebug()<< " wrong box";                              break;
    }
    //switchState[]
    

    switchStatus ^= (1 << groupBoxNumber);
    qDebug()<< switchStatus;
    return;
}

void SimSwitch::mouseMoveEvent ( QMouseEvent * ev )
{
    for (int i = 0; i <= 7;i++)
    {
        if ( switch_rect[i].contains( ev->pos() )  == true)
        {
            rectHighlight[i] = true;
        }
        else
        {
            rectHighlight[i] = false;
        }
    }
    update();
}

/*
void SimSwitch::handleChange_0(int change)
{
    flag_change = true;

    for ( int i=1; i<=7;i++)
    {
        if ( boxNum[i] = boxNum[0])
        {
            changeBox(i,change);
            break;
        }
        else
            qDebug()<< "wrong box";
    }
}
void SimSwitch::handleChange_1(int change)
{
    flag_change = true;

    for ( int i=0;i<=7;i++)
        qDebug()<< boxNum[i] << "array 0 first";

    for ( int i=2; i<=7;i++)
    {
        if ( boxNum[i] == boxNum[1])
        {
            changeBox(i,change);
            break;
        }
    }

    if ( boxNum[0] == boxNum[1])
    {
        qDebug()<< "sutu?";
        changeBox(0,change);
    }
}

void SimSwitch::handleChange_2(int change)
{
    flag_change = true;

    for ( int i=3; i<=7;i++)
    {
        if ( boxNum[i] == boxNum[2])
        {
            changeBox(i,change);
            break;
        }
        else
        {
            qDebug()<< "wrong box";
        }
    }
    for ( int i=0; i<=1;i++)
    {
        if ( boxNum[i] == boxNum[2])
        {
            changeBox(i,change);
        }
    }
}
void SimSwitch::handleChange_3(int change)
{
    flag_change = true;

    for ( int i=4; i<=7;i++)
    {
        if ( boxNum[i] == boxNum[3])
        {
            changeBox(i,change);
            break;
        }
        else
            qDebug()<< "wrong box";
    }
    for ( int i=0; i<=2;i++)
    {
        if ( boxNum[i] == boxNum[3])
        {
            changeBox(i,change);
        }
    }
}
void SimSwitch::handleChange_4(int change)
{
    flag_change = true;

    for ( int i=5; i<=7;i++)
    {
        if ( boxNum[i] == boxNum[4])
        {
            changeBox(i,change);
            break;
        }
        else
            qDebug()<< "wrong box";
    }
    for ( int i=0; i<=3;i++)
    {
        if ( boxNum[i] == boxNum[4])
        {
            changeBox(i,change);
        }
    }
}
void SimSwitch::handleChange_5(int change)
{
    flag_change = true;

    for ( int i=6; i<=7;i++)
    {
        if ( boxNum[i] == boxNum[5])
        {
            changeBox(i,change);
            break;
        }
    }
    for ( int i=0; i<=4;i++)
    {
        if ( boxNum[i] == boxNum[5])
        {
            changeBox(i,change);
        }
    }
}
void SimSwitch::handleChange_6(int change)
{
    flag_change = true;


        if ( boxNum[7] == boxNum[6])
        {
            changeBox(7,change);
        }

    for ( int i=0; i<=5;i++)
    {
        if ( boxNum[i] == boxNum[6])
        {
            changeBox(i,change);
        }
    }
}
void SimSwitch::handleChange_7(int change)
{
    flag_change = true;

    for ( int i=0; i<=6;i++)
    {
        if ( boxNum[i] == boxNum[7])
        {
            changeBox(i,change);
            break;
        }
    }
}
*/

void SimSwitch::changeBox(int i,int boxNumber)
{
    // 7 7
    qDebug() << i << "which box "<< boxNumber << " box number to change";
    switch (i)
    {
    case 0: ui.switchBox_0->setCurrentIndex(boxNumber); return;
    case 1: ui.switchBox_1->setCurrentIndex(boxNumber); return;
    case 2: ui.switchBox_2->setCurrentIndex(boxNumber); return;
    case 3: ui.switchBox_3->setCurrentIndex(boxNumber); return;
    case 4: ui.switchBox_4->setCurrentIndex(boxNumber); return;
    case 5: ui.switchBox_5->setCurrentIndex(boxNumber); return;
    case 6: ui.switchBox_6->setCurrentIndex(boxNumber); return;
    case 7: ui.switchBox_7->setCurrentIndex(boxNumber); return;
    }
    return;
}

void SimSwitch::CreateItems(void)
{
    ui.switchBox_0->addItem(QString("0"));
    ui.switchBox_0->addItem(QString("1"));
    ui.switchBox_0->addItem(QString("2"));
    ui.switchBox_0->addItem(QString("3"));
    ui.switchBox_0->addItem(QString("4"));
    ui.switchBox_0->addItem(QString("5"));
    ui.switchBox_0->addItem(QString("6"));
    ui.switchBox_0->addItem(QString("7"));

    ui.switchBox_1->addItem(QString("0"));
    ui.switchBox_1->addItem(QString("1"));
    ui.switchBox_1->addItem(QString("2"));
    ui.switchBox_1->addItem(QString("3"));
    ui.switchBox_1->addItem(QString("4"));
    ui.switchBox_1->addItem(QString("5"));
    ui.switchBox_1->addItem(QString("6"));
    ui.switchBox_1->addItem(QString("7"));

    ui.switchBox_2->addItem(QString("0"));
    ui.switchBox_2->addItem(QString("1"));
    ui.switchBox_2->addItem(QString("2"));
    ui.switchBox_2->addItem(QString("3"));
    ui.switchBox_2->addItem(QString("4"));
    ui.switchBox_2->addItem(QString("5"));
    ui.switchBox_2->addItem(QString("6"));
    ui.switchBox_2->addItem(QString("7"));

    ui.switchBox_3->addItem(QString("0"));
    ui.switchBox_3->addItem(QString("1"));
    ui.switchBox_3->addItem(QString("2"));
    ui.switchBox_3->addItem(QString("3"));
    ui.switchBox_3->addItem(QString("4"));
    ui.switchBox_3->addItem(QString("5"));
    ui.switchBox_3->addItem(QString("6"));
    ui.switchBox_3->addItem(QString("7"));

    ui.switchBox_4->addItem(QString("0"));
    ui.switchBox_4->addItem(QString("1"));
    ui.switchBox_4->addItem(QString("2"));
    ui.switchBox_4->addItem(QString("3"));
    ui.switchBox_4->addItem(QString("4"));
    ui.switchBox_4->addItem(QString("5"));
    ui.switchBox_4->addItem(QString("6"));
    ui.switchBox_4->addItem(QString("7"));

    ui.switchBox_5->addItem(QString("0"));
    ui.switchBox_5->addItem(QString("1"));
    ui.switchBox_5->addItem(QString("2"));
    ui.switchBox_5->addItem(QString("3"));
    ui.switchBox_5->addItem(QString("4"));
    ui.switchBox_5->addItem(QString("5"));
    ui.switchBox_5->addItem(QString("6"));
    ui.switchBox_5->addItem(QString("7"));

    ui.switchBox_6->addItem(QString("0"));
    ui.switchBox_6->addItem(QString("1"));
    ui.switchBox_6->addItem(QString("2"));
    ui.switchBox_6->addItem(QString("3"));
    ui.switchBox_6->addItem(QString("4"));
    ui.switchBox_6->addItem(QString("5"));
    ui.switchBox_6->addItem(QString("6"));
    ui.switchBox_6->addItem(QString("7"));

    ui.switchBox_7->addItem(QString("0"));
    ui.switchBox_7->addItem(QString("1"));
    ui.switchBox_7->addItem(QString("2"));
    ui.switchBox_7->addItem(QString("3"));
    ui.switchBox_7->addItem(QString("4"));
    ui.switchBox_7->addItem(QString("5"));
    ui.switchBox_7->addItem(QString("6"));
    ui.switchBox_7->addItem(QString("7"));



    ui.switchBox_0->setCurrentIndex(0);
    ui.switchBox_1->setCurrentIndex(1);
    ui.switchBox_2->setCurrentIndex(2);
    ui.switchBox_3->setCurrentIndex(3);
    ui.switchBox_4->setCurrentIndex(4);
    ui.switchBox_5->setCurrentIndex(5);
    ui.switchBox_6->setCurrentIndex(6);
    ui.switchBox_7->setCurrentIndex(7);
    return;
}


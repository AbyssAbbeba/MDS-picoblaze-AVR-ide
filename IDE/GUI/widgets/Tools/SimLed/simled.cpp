#include "simled.h"
#include "ui_simled.h"
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



Leds_sim::Leds_sim(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("LED panel");
    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
        return;
    }

    //variable initializations
    value = 0;
    address = 0;
    flag_0 = false; flag_1 = false; flag_2 = false; flag_3 = false;
    flag_4 = false; flag_5 = false; flag_6 = false;

    CreateItems();

    QRegExpValidator *hexValidator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{2}"), this);
    ui.lineEditAddress->setValidator(hexValidator);
    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << address;

    std::vector<int> mask = { //8
                                MCUSimPureLogicIO::EVENT_PLIO_WRITE,
                                MCUSimPureLogicIO::EVENT_PLIO_WRITE_END
                            };
    controlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask); //8

    connect(ui.lineEditAddress,
            SIGNAL(editingFinished()),
            this,
            SLOT(addrChanged())
           );

        connect(ui.comboDecoder,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(ValueChanged())
           );


    connect(controlUnit,
            SIGNAL(updateRequest(int)),
            this,
            SLOT(handleUpdateRequest(int))
           );
    
        connect(ui.ledBox_0 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_1 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_2 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_3 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_4 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_5 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_6 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
        connect(ui.ledBox_7 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));

    deviceChanged();  //8
    ValueChanged();
    
}

void Leds_sim::DecoderChanged(int o)
{
    ValueChanged();
}

void Leds_sim::handleChange(int /*o*/)
{
    ValueChanged();
}

void Leds_sim::deviceChanged()  //8
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


void Leds_sim::deviceReset()  //8
{
    ValueChanged();          
}

void Leds_sim::handleUpdateRequest(int /*mask*/) //8
{
    
}

void Leds_sim::handleEvent(int subsysId, int eventId, int locationOrReason, int detail) //8
{
    if (MCUSimSubsys::ID_PLIO == subsysId)
    {
        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
            {
                this->value = m_plio->getOutputArray()[this->address];
                break;
            }
            default:
            {
                break;
            }
        }
    }
    ValueChanged();
}


void Leds_sim::setReadOnly(bool /*readOnly*/)
{
}

// main loop, event
void Leds_sim::ValueChanged()
{
    if ( ui.comboDecoder->currentIndex() != 0 )
    {
        ValueDecode();
    }
    else
    {
        DisplayNumber(value);
    }
    update();
}

void Leds_sim::addrChanged()
{
    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << "changed address" << address;
    
    this->value = m_plio->getOutputArray()[this->address];  
    ValueChanged();
}

void Leds_sim::DisplayNumber(unsigned char Numero)
{
    if ( Numero & bit0)
    {
        flag_0 = false;
    }
    else
    {
        flag_0 = true;
    }
//--------------------------
    if ( Numero & bit1)
    {
        flag_1 = false;
    }
    else
    {
        flag_1 = true;
    }
//--------------------------
    if ( Numero & bit2)
    {
        flag_2 = false;
    }
    else
    {
        flag_2 = true;
    }
//--------------------------
    if ( Numero & bit3)
    {
        flag_3 = false;
    }
    else
    {
        flag_3 = true;
    }

//--------------------------
    if ( Numero & bit4)
    {
        flag_4 = false;
    }
    else
    {
        flag_4 = true;
    }
//--------------------------
    if ( Numero & bit5)
    {
        flag_5 = false;
    }
    else
    {
        flag_5 = true;
    }
//--------------------------
    if ( Numero & bit6)
    {
        flag_6 = false;
    }
    else
    {
        flag_6 = true;
    }
//--------------------------
    if ( Numero & bit7)
    {
        flag_7 = false;
    }
    else
    {
        flag_7 = true;
    }
    return;
}

void Leds_sim::ValueDecode(void)
{
    // Bin to BCD
    if ( ui.comboDecoder->currentIndex() == 1 )
    {
        unsigned char HighNibble = 0, LowNibble = 0;

        LowNibble = (value & 0x0f);
        qDebug() << LowNibble << " low";
        HighNibble = (value & 0xf0) >> 4;
        qDebug() << HighNibble << " high";

        switch( HighNibble )
        {
            default:    qDebug() << "wrong value";
                break;
            case     0b00000000:    HighNibble = 0b00000000;          break;
            case     0b00000001:    HighNibble = 0b00000001;          break;
            case     0b00000010:    HighNibble = 0b00000010;          break;
            case     0b00000011:    HighNibble = 0b00000000;          break;
            case     0b00000100:    HighNibble = 0b00000000;          break;
            case     0b00000101:    HighNibble = 0b00000000;          break;
            case     0b00000110:    HighNibble = 0b00000000;          break;
            case     0b00000111:    HighNibble = 0b00000000;          break;
            case     0b00001000:    HighNibble = 0b00000000;          break;
            case     0b00001001:    HighNibble = 0b00000000;          break;
      }
       return;
    }

    if ( ui.comboDecoder->currentIndex() == 2 )
    {
        unsigned char gray_value = 0;
        // Bin to Gray
        gray_value = value;
        gray_value = (gray_value >> 1) ^ gray_value;
        gray_value = ~gray_value;
        qDebug() << gray_value << " Gray value";
        qDebug() << value << " value";
        DisplayNumber(gray_value);
        return;
    }
    return;
}

void Leds_sim::paintEvent(QPaintEvent */*e*/)
{
    unsigned char index = 0;
    // dioda
    QPoint p1(20,20);
    QPoint p2(20,35);
    QPoint p3(15,35);
    QPoint p4(25,35);     QPoint pB(28,37); QPoint pC(32,43);    QPoint pH(29,43); QPoint pI(32,40);
    QPoint p5(20,45);     QPoint pD(28,44); QPoint pE(32,50);    QPoint pF(29,50); QPoint pG(32,47);
    QPoint p6(15,45);
    QPoint p7(25,45);
    QPoint p8(20,50);
    QPoint p9(20,52);
   // odpor
    QPoint p10(23,52);    QPoint p11(17,52);    QPoint p12(17,65);    QPoint p13(23,65);
    QPoint p14(20,65);    QPoint p15(20,72);
    //supply
    QPoint p16(23,24);    QPoint p17(17,24);

    // brush set - soon collors
    QPainter painter(this);

    QPen graypen    (Qt::gray);
    QPen blackpen   (Qt::black);

    painter.setPen(graypen);

    unsigned int x_pos = 25;

    // shifting
    for (int i = 0; i < 10;i++)
    {
        p1.ry()--;  p2.ry()--;    p3.ry()--;   p4.ry()--;
        p5.ry()--;  p6.ry()--;    p7.ry()--;   p8.ry()--;
        p9.ry()--;  p10.ry()--;   p11.ry()--;  p12.ry()--;
        p13.ry()--; p14.ry()--;   p15.ry()--;  p16.ry()--;
        p17.ry()--;

        pB.ry()--;   pC.ry()--;  pD.ry()--;   pE.ry()--;
        pF.ry()--;   pG.ry()--;  pH.ry()--;   pI.ry()--;
    }

    for ( int i = 0; i <= 7; i++)
    {
        p1.setX(20 + x_pos);  p2.setX(20 + x_pos);    p3.setX(15 + x_pos);   p4.setX(25 + x_pos);
        p5.setX(20 + x_pos);  p6.setX(15 + x_pos);    p7.setX(25 + x_pos);   p8.setX(20 + x_pos);
        p9.setX(20 + x_pos);  p10.setX(23 + x_pos);   p11.setX(17 + x_pos);  p12.setX(17 + x_pos);
        p13.setX(23 + x_pos); p14.setX(20 + x_pos);   p15.setX(20 + x_pos);  p16.setX(23 + x_pos);
        p17.setX(17 + x_pos);

        pB.setX(28 + x_pos);   pC.setX(32 + x_pos);  pD.setX(28 + x_pos);   pE.setX(32 + x_pos);
        pF.setX(29 + x_pos);   pG.setX(32 + x_pos);  pH.setX(29 + x_pos);   pI.setX(32 + x_pos);
        x_pos += 55;

      //  y_pos += 55;

        painter.drawLine(p1,p2);
        painter.drawLine(p3,p2);
        painter.drawLine(p4,p2);
        painter.drawLine(p4,p5);
        painter.drawLine(p3,p5);
        painter.drawLine(p6,p7);
        painter.drawLine(p5,p8);
        painter.drawLine(p8,p9);//
        painter.drawLine(p10,p11);
        painter.drawLine(p10,p13);
        painter.drawLine(p11,p12);
        painter.drawLine(p14,p15);
        painter.drawLine(p12,p13);
        painter.drawLine(p16,p1);
        painter.drawLine(p1,p17);

        painter.drawLine(pB,pC);
        painter.drawLine(pD,pE);

        painter.drawLine(pF,pE);
        painter.drawLine(pG,pE);
        painter.drawLine(pH,pC);
        painter.drawLine(pI,pC);
    }
    QPixmap pix_green(":resources/icons/button-green.png");
    QPixmap pix_red  (":resources/icons/button-red.png");
    if ( pix_red.isNull())
    {
        qDebug()<< " red pix neni";
    }
    if ( pix_green.isNull())
    {
        qDebug()<< "green pix neni";
    }

    painter.setBrush(Qt::gray);
    painter.setBrush(Qt::red);
    painter.setPen(blackpen);

    // translate and copy
    unsigned int offset = 55;
    unsigned int x = 15,y = 35,w = 16,h = 16;
    QRectF rectangle(x, y, w, h);


    // led 7
    index = ui.ledBox_7->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::red)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
//--------------------------------------------------------------------
    // led 6
    index = ui.ledBox_6->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    rectangle.translate(offset,0);
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::gray)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }
//----------------------------------------------------------
    // led 5
    index = ui.ledBox_5->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    rectangle.translate(offset,0);
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::gray)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }
//--------------------------------------------------------------------
    // led 4
    index = ui.ledBox_4->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    rectangle.translate(offset,0);
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::gray)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }

//--------------------------------------------------------------------
    // led 3
    index = ui.ledBox_3->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    rectangle.translate(offset,0);
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::gray)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }
//----------------------------------------------------------
    // led 2
    index = ui.ledBox_2->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    rectangle.translate(offset,0);
    painter.drawEllipse(rectangle);
    if ( painter.brush().color() == Qt::gray)
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
    }
    else
    {
        painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
    }

    //--------------------------------------------------------------------
        // led 1
        index = ui.ledBox_1->currentIndex();

        switch ( index )
        {
        case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        default: qDebug()<< "default in painting, wrong";
            break;
        }
        rectangle.translate(offset,0);
        painter.drawEllipse(rectangle);
        if ( painter.brush().color() == Qt::gray)
        {
            painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
        }
        else
        {
            painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
        }
    //----------------------------------------------------------
        // led 0
        index = ui.ledBox_0->currentIndex();

        switch ( index )
        {
        case 0: if ( flag_0  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 1: if ( flag_1  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 2: if ( flag_2  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 3: if ( flag_3  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 4: if ( flag_4  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 5: if ( flag_5  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 6: if ( flag_6  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 7: if ( flag_7  == false ) { painter.setBrush(Qt::gray);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        default: qDebug()<< "default in painting, wrong";
            break;
        }
        rectangle.translate(offset,0);
        painter.drawEllipse(rectangle);
        if ( painter.brush().color() == Qt::gray)
        {
            painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_green.scaledToHeight(rectangle.height()));
        }
        else
        {
            painter.drawPixmap(QPoint(rectangle.x(),rectangle.y()),pix_red.scaledToHeight(rectangle.height()));
        }
        //pix.scaledToHeight(rectangle.height());
    painter.end();
}


void Leds_sim::CreateItems(void)
{
    ui.ledBox_0->addItem(QString("0"));
    ui.ledBox_0->addItem(QString("1"));
    ui.ledBox_0->addItem(QString("2"));
    ui.ledBox_0->addItem(QString("3"));
    ui.ledBox_0->addItem(QString("4"));
    ui.ledBox_0->addItem(QString("5"));
    ui.ledBox_0->addItem(QString("6"));
    ui.ledBox_0->addItem(QString("7"));

    ui.ledBox_1->addItem(QString("0"));
    ui.ledBox_1->addItem(QString("1"));
    ui.ledBox_1->addItem(QString("2"));
    ui.ledBox_1->addItem(QString("3"));
    ui.ledBox_1->addItem(QString("4"));
    ui.ledBox_1->addItem(QString("5"));
    ui.ledBox_1->addItem(QString("6"));
    ui.ledBox_1->addItem(QString("7"));

    ui.ledBox_2->addItem(QString("0"));
    ui.ledBox_2->addItem(QString("1"));
    ui.ledBox_2->addItem(QString("2"));
    ui.ledBox_2->addItem(QString("3"));
    ui.ledBox_2->addItem(QString("4"));
    ui.ledBox_2->addItem(QString("5"));
    ui.ledBox_2->addItem(QString("6"));
    ui.ledBox_2->addItem(QString("7"));

    ui.ledBox_3->addItem(QString("0"));
    ui.ledBox_3->addItem(QString("1"));
    ui.ledBox_3->addItem(QString("2"));
    ui.ledBox_3->addItem(QString("3"));
    ui.ledBox_3->addItem(QString("4"));
    ui.ledBox_3->addItem(QString("5"));
    ui.ledBox_3->addItem(QString("6"));
    ui.ledBox_3->addItem(QString("7"));

    ui.ledBox_4->addItem(QString("0"));
    ui.ledBox_4->addItem(QString("1"));
    ui.ledBox_4->addItem(QString("2"));
    ui.ledBox_4->addItem(QString("3"));
    ui.ledBox_4->addItem(QString("4"));
    ui.ledBox_4->addItem(QString("5"));
    ui.ledBox_4->addItem(QString("6"));
    ui.ledBox_4->addItem(QString("7"));

    ui.ledBox_5->addItem(QString("0"));
    ui.ledBox_5->addItem(QString("1"));
    ui.ledBox_5->addItem(QString("2"));
    ui.ledBox_5->addItem(QString("3"));
    ui.ledBox_5->addItem(QString("4"));
    ui.ledBox_5->addItem(QString("5"));
    ui.ledBox_5->addItem(QString("6"));
    ui.ledBox_5->addItem(QString("7"));

    ui.ledBox_6->addItem(QString("0"));
    ui.ledBox_6->addItem(QString("1"));
    ui.ledBox_6->addItem(QString("2"));
    ui.ledBox_6->addItem(QString("3"));
    ui.ledBox_6->addItem(QString("4"));
    ui.ledBox_6->addItem(QString("5"));
    ui.ledBox_6->addItem(QString("6"));
    ui.ledBox_6->addItem(QString("7"));

    ui.ledBox_7->addItem(QString("0"));
    ui.ledBox_7->addItem(QString("1"));
    ui.ledBox_7->addItem(QString("2"));
    ui.ledBox_7->addItem(QString("3"));
    ui.ledBox_7->addItem(QString("4"));
    ui.ledBox_7->addItem(QString("5"));
    ui.ledBox_7->addItem(QString("6"));
    ui.ledBox_7->addItem(QString("7"));



    ui.ledBox_0->setCurrentIndex(0);
    ui.ledBox_1->setCurrentIndex(1);
    ui.ledBox_2->setCurrentIndex(2);
    ui.ledBox_3->setCurrentIndex(3);
    ui.ledBox_4->setCurrentIndex(4);
    ui.ledBox_5->setCurrentIndex(5);
    ui.ledBox_6->setCurrentIndex(6);
    ui.ledBox_7->setCurrentIndex(7);

    ui.comboDecoder->addItem(QString("NONE"));
    ui.comboDecoder->addItem(QString("BCD"));
    ui.comboDecoder->addItem(QString("GRAY"));
    ui.comboDecoder->setCurrentIndex(0);

}


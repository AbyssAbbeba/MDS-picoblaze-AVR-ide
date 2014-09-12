#include "sim7seg.h"
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



Sim7Seg::Sim7Seg(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("7 Segment");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    if ( NULL == controlUnit )
    {
        qDebug() << "PicoBlazeGrid: controlUnit is NULL";
        return;
    }
    flag_change = false;

    //variable initializations
    value = 0;
    address = 0;
    flag_0 = false; flag_1 = false; flag_2 = false; flag_3 = false;
    flag_4 = false; flag_5 = false; flag_6 = false;

    CreateItems();

    QRegExpValidator *hexValidator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{2}"), this);
    ui.lineEditAddress->setValidator(hexValidator);

    ui.lineEditHex->setReadOnly(true);
    ui.lineEditDec->setReadOnly(true);

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
            SLOT(comboChanged(int))
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

void Sim7Seg::mousePressEvent(QMouseEvent  *event)
{
    qDebug()<< event->x() << "x";
    qDebug()<< event->y()<<  "y";
}

void Sim7Seg::comboChanged(int c)
{
    switch ( c )
    {
        case 0:     ui.ledBox_0->setDisabled(false);
                    ui.ledBox_1->setDisabled(false);
                    ui.ledBox_2->setDisabled(false);
                    ui.ledBox_3->setDisabled(false);
                    ui.ledBox_4->setDisabled(false);
                    ui.ledBox_5->setDisabled(false);
                    ui.ledBox_6->setDisabled(false);
                    ui.ledBox_7->setDisabled(false);
                    ui.ledBox_0->setCurrentIndex(pin_status[0]);
                    ui.ledBox_1->setCurrentIndex(pin_status[1]);
                    ui.ledBox_2->setCurrentIndex(pin_status[2]);
                    ui.ledBox_3->setCurrentIndex(pin_status[3]);
                    ui.ledBox_4->setCurrentIndex(pin_status[4]);
                    ui.ledBox_5->setCurrentIndex(pin_status[5]);
                    ui.ledBox_6->setCurrentIndex(pin_status[6]);
                    ui.ledBox_7->setCurrentIndex(pin_status[7]);
            break;
        case 1:
                    flag_change = true;
                    pin_status[0] = ui.ledBox_0->currentIndex();
                    pin_status[1] = ui.ledBox_1->currentIndex();
                    pin_status[2] = ui.ledBox_2->currentIndex();
                    pin_status[3] = ui.ledBox_3->currentIndex();
                    pin_status[4] = ui.ledBox_4->currentIndex();
                    pin_status[5] = ui.ledBox_5->currentIndex();
                    pin_status[6] = ui.ledBox_6->currentIndex();
                    pin_status[7] = ui.ledBox_7->currentIndex();
                    ui.ledBox_0->setCurrentIndex(0);
                    ui.ledBox_1->setCurrentIndex(1);
                    ui.ledBox_2->setCurrentIndex(2);
                    ui.ledBox_3->setCurrentIndex(3);
                    ui.ledBox_4->setCurrentIndex(4);
                    ui.ledBox_5->setCurrentIndex(5);
                    ui.ledBox_6->setCurrentIndex(6);
                    ui.ledBox_7->setCurrentIndex(7);
                    ui.ledBox_0->setDisabled(true);
                    ui.ledBox_1->setDisabled(true);
                    ui.ledBox_2->setDisabled(true);
                    ui.ledBox_3->setDisabled(true);
                    ui.ledBox_4->setDisabled(true);
                    ui.ledBox_5->setDisabled(true);
                    ui.ledBox_6->setDisabled(true);
                    ui.ledBox_7->setDisabled(true);
            break;
        case 2:
                    flag_change = true;
                    pin_status[0] = ui.ledBox_0->currentIndex();
                    pin_status[1] = ui.ledBox_1->currentIndex();
                    pin_status[2] = ui.ledBox_2->currentIndex();
                    pin_status[3] = ui.ledBox_3->currentIndex();
                    pin_status[4] = ui.ledBox_4->currentIndex();
                    pin_status[5] = ui.ledBox_5->currentIndex();
                    pin_status[6] = ui.ledBox_6->currentIndex();
                    pin_status[7] = ui.ledBox_7->currentIndex();
                    ui.ledBox_0->setCurrentIndex(0);
                    ui.ledBox_1->setCurrentIndex(1);
                    ui.ledBox_2->setCurrentIndex(2);
                    ui.ledBox_3->setCurrentIndex(3);
                    ui.ledBox_4->setCurrentIndex(4);
                    ui.ledBox_5->setCurrentIndex(5);
                    ui.ledBox_6->setCurrentIndex(6);
                    ui.ledBox_7->setCurrentIndex(7);
                    ui.ledBox_0->setDisabled(true);
                    ui.ledBox_1->setDisabled(true);
                    ui.ledBox_2->setDisabled(true);
                    ui.ledBox_3->setDisabled(true);
                    ui.ledBox_4->setDisabled(true);
                    ui.ledBox_5->setDisabled(true);
                    ui.ledBox_6->setDisabled(true);
                    ui.ledBox_7->setDisabled(true);
            break;
        default: qDebug()  << "combobox change fault";
            break;
    }
    ValueChanged();
}

void Sim7Seg::handleChange(int /*o*/)
{
    ValueChanged();
}

void Sim7Seg::deviceChanged()  //8
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


void Sim7Seg::deviceReset()  //8
{
    this->value = m_plio->getOutputArray()[this->address];
    ValueChanged();
}

void Sim7Seg::handleUpdateRequest(int mask) //8
{
    if ( 4 & mask)
    {
        this->value = m_plio->getOutputArray()[this->address];
        ValueChanged();
    }
}

void Sim7Seg::handleEvent(int subsysId, int eventId, int /*locationOrReason*/, int /*detail*/) //8
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


void Sim7Seg::setReadOnly(bool /*readOnly*/)
{
}

// main loop, event
void Sim7Seg::ValueChanged()
{
    if ( ui.comboDecoder->currentIndex() != 0 )
    {
        ValueDecode();
    }
    else
    {
        DisplayNumber(value);
    }
    ui.lineEditDec->setText(QString::number(value,10));
    ui.lineEditHex->setText(QString::number(value,16).toUpper());
    update();
}

void Sim7Seg::addrChanged()
{
    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << "changed address" << address;

    this->value = m_plio->getOutputArray()[this->address];
    ValueChanged();
}

void Sim7Seg::DisplayNumber(unsigned char Numero)
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

void Sim7Seg::ValueDecode(void)
{
    // Bin to BCD
    // low nibble
    if ( ui.comboDecoder->currentIndex() == 1 )
    {
        unsigned char bcd_value = 0;

        bcd_value = value & 0x0F;

        switch ( bcd_value )
        {
            case 0b00000000:    bcd_value = ~0b00111111; break;
            case 0b00000001:    bcd_value = ~0b00000110; break;
            case 0b00000010:    bcd_value = ~0b01011011; break;
            case 0b00000011:    bcd_value = ~0b01001111; break;
            case 0b00000100:    bcd_value = ~0b01100110; break;
            case 0b00000101:    bcd_value = ~0b01101101; break;
            case 0b00000110:    bcd_value = ~0b01111101; break;
            case 0b00000111:    bcd_value = ~0b00000111; break;
            case 0b00001000:    bcd_value = ~0b01111111; break;
            case 0b00001001:    bcd_value = ~0b01100111; break;
        }

        DisplayNumber(bcd_value);
        return;
    }
    // high nibble
    if ( ui.comboDecoder->currentIndex() == 2 )
    {
        unsigned char bcd_value = 0;

        bcd_value = value & 0xF0;

        switch ( bcd_value )
        {
            case 0b00000000:    bcd_value = ~0b00111111; break;
            case 0b00010000:    bcd_value = ~0b00000110; break;
            case 0b00100000:    bcd_value = ~0b01011011; break;
            case 0b00110000:    bcd_value = ~0b01001111; break;
            case 0b01000000:    bcd_value = ~0b01100110; break;
            case 0b01010000:    bcd_value = ~0b01101101; break;
            case 0b01100000:    bcd_value = ~0b01111101; break;
            case 0b01110000:    bcd_value = ~0b00000111; break;
            case 0b10000000:    bcd_value = ~0b01111111; break;
            case 0b10010000:    bcd_value = ~0b01100111; break;
        }

        DisplayNumber(bcd_value);
        return;
    }
}

void Sim7Seg::paintEvent(QPaintEvent */*e*/)
{
    unsigned char index = 0;
    // dioda


    // brush set - soon collors
    QPainter painter(this);

    QPen graypen    (Qt::gray);
    QPen blackpen   (Qt::black);
    QPen redpen    (Qt::red);

    painter.setPen(blackpen);
    // draw
   QPointF p1(210,160);
   //A
    painter.drawLine(138,19,138,10);
    painter.drawLine(220,10,138,10);
    painter.drawLine(220,10,220,17);
    //B
    painter.drawLine(183,56,223,56);
    painter.drawLine(223,59,223,56);
    //C
    painter.drawLine(174,131,209,131);
    //D
    painter.drawLine(228,181,228,153);
    painter.drawLine(228,153,119,153);
    painter.drawLine(119,184,119,153);
        //E
    painter.drawLine(47,170,59,170);
    painter.drawLine(59,170,59,155);
    painter.drawLine(59,155,74,155);
        //F
    painter.drawLine(49,133,59,133);
    painter.drawLine(59,133,59,85);
    painter.drawLine(59,85,87,85);
        //G
    painter.drawLine(48,70,128,70);
    painter.drawLine(128,70,128,94);
        //P
    painter.drawLine(14,31,7,31);
    painter.drawLine(7,31,7,193);
    painter.drawLine(183,193,7,193);
    painter.drawLine(183,193,183,180);
 // list.at(4)->boundingRect().center()

    //QPixmap pix_green(":resources/icons/button-green.png");
    //QPixmap pix_red  (":resources/icons/button-red.png");

    painter.setBrush(Qt::white);
    unsigned int x = 171,y = 173 - 10,w = 20,h = 20;
    QRectF rectangle(x, y, w, h);
//         0 top
//         1 levej
//         2 pravej
//         3 prostredek
//         4 levej
//         5 pravej
//         6 spodek
//         7 tečka

    // led 7
    index = ui.ledBox_7->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawEllipse(rectangle);
    painter.drawText(rectangle.adjusted(+3, 0, -1, -1), QString(QChar('P')), QTextOption(Qt::AlignCenter));
//--------------------------------------------------------------------
    // led 6
    index = ui.ledBox_6->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    // paint
    painter.drawConvexPolygon(*(list.at(3)));
    painter.drawText((list.at(3))->boundingRect(), QString(QChar('G')), QTextOption(Qt::AlignCenter));

----------------------------------------------------
    // led 5
    index = ui.ledBox_5->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawConvexPolygon(*(list.at(1)));
    painter.drawText((list.at(1))->boundingRect(), QString(QChar('F')), QTextOption(Qt::AlignCenter));

//--------------------------------------------------------------------
    // led 4
    index = ui.ledBox_4->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawConvexPolygon(*(list.at(4)));
    painter.drawText((list.at(4))->boundingRect(), QString(QChar('E')), QTextOption(Qt::AlignCenter));

//--------------------------------------------------------------------
    // led 3
    index = ui.ledBox_3->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawConvexPolygon(*(list.at(6)));
    painter.drawText((list.at(6))->boundingRect(), QString(QChar('D')), QTextOption(Qt::AlignCenter));

//----------------------------------------------------------
    // led 2
    index = ui.ledBox_2->currentIndex();

    switch ( index )
    {
    case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
            else                    { painter.setBrush(Qt::red) ;}
        break;
    default: qDebug()<< "default in painting, wrong";
        break;
    }
    painter.drawConvexPolygon(*(list.at(5)));
    painter.drawText((list.at(5))->boundingRect(), QString(QChar('C')), QTextOption(Qt::AlignCenter));

    //--------------------------------------------------------------------
        // led 1
        index = ui.ledBox_1->currentIndex();

        switch ( index )
        {
        case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        default: qDebug()<< "default in painting, wrong";
            break;
        }
        painter.drawConvexPolygon(*(list.at(2)));
        painter.drawText((list.at(2))->boundingRect(), QString(QChar('B')), QTextOption(Qt::AlignCenter));

    //----------------------------------------------------------
        // led 0
        index = ui.ledBox_0->currentIndex();

        switch ( index )
        {
        case 0: if ( flag_0  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 1: if ( flag_1  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 2: if ( flag_2  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 3: if ( flag_3  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 4: if ( flag_4  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 5: if ( flag_5  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 6: if ( flag_6  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        case 7: if ( flag_7  == false ) { painter.setBrush(Qt::white);}
                else                    { painter.setBrush(Qt::red) ;}
            break;
        default: qDebug()<< "default in painting, wrong";
            break;
        }
        painter.drawConvexPolygon(*(list.at(0)));
        painter.drawText((list.at(0))->boundingRect(), QString(QChar('A')), QTextOption(Qt::AlignCenter));

    painter.end();
}


void Sim7Seg::CreateItems(void)
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
    ui.comboDecoder->addItem(QString("BCDlowNibb"));
    ui.comboDecoder->addItem(QString("BCDhighNibb"));
    ui.comboDecoder->setCurrentIndex(0);

    unsigned int y_pos,x_pos;
    x_pos = 35 + 67;
    y_pos = 25 - 10;
    QVector<QPointF> points0; //0 10   10 0    61 0    71 10  61 20   10 20
    points0.append(QPointF(0.0 + x_pos, 10.0 + y_pos));
    points0.append(QPointF(10.0 + x_pos, 0.0 +  y_pos));
    points0.append(QPointF(61.0 + x_pos, 0.0 +  y_pos));
    points0.append(QPointF(71.0 + x_pos, 10.0 + y_pos));
    points0.append(QPointF(61.0 + x_pos, 20.0 + y_pos));
    points0.append(QPointF(10.0 + x_pos, 20.0 + y_pos));
    QPolygonF *widgetPolygon0 = new QPolygonF(points0);
    list.append(widgetPolygon0);

    x_pos = 13 + 67;
    y_pos = 40 - 10;
    QVector<QPointF> points1; //19 0  29 10  21 54  9 66  0 57   8 11
    points1.append(QPointF(19.0 + x_pos, 0.0 + y_pos));
    points1.append(QPointF(29.0 + x_pos, 10.0 + y_pos));
    points1.append(QPointF(21.0 + x_pos, 54.0 + y_pos));
    points1.append(QPointF(9.0 + x_pos, 66.0 + y_pos));
    points1.append(QPointF(0.0 + x_pos, 57.0 + y_pos));
    points1.append(QPointF(8.0 + x_pos, 11.0 + y_pos));
    QPolygonF *widgetPolygon1 = new QPolygonF(points1);
    list.append(widgetPolygon1);

    x_pos = 90 + 67;
    y_pos = 40 - 10;
    QVector<QPointF> points2; //19 0  29 10  21 54  9 66  0 57   8 11
    points2.append(QPointF(19.0 + x_pos, 0.0 + y_pos));
    points2.append(QPointF(29.0 + x_pos, 10.0 + y_pos));
    points2.append(QPointF(21.0 + x_pos, 54.0 + y_pos));
    points2.append(QPointF(9.0 + x_pos, 66.0 + y_pos));
    points2.append(QPointF(0.0 + x_pos, 57.0 + y_pos));
    points2.append(QPointF(8.0 + x_pos, 11.0 + y_pos));
    QPolygonF *widgetPolygon2 = new QPolygonF(points2);
    list.append(widgetPolygon2);

    x_pos = 23 + 67;
    y_pos = 102 - 10;
    QVector<QPointF> points3; //0 10   10 0    61 0    71 10  61 20   10 20
    points3.append(QPointF(0.0 + x_pos, 10.0 + y_pos));
    points3.append(QPointF(10.0 + x_pos, 0.0 + y_pos));
    points3.append(QPointF(61.0 + x_pos, 0.0 + y_pos));
    points3.append(QPointF(71.0 + x_pos, 10.0 + y_pos));
    points3.append(QPointF(61.0 + x_pos, 20.0 + y_pos));
    points3.append(QPointF(10.0 + x_pos, 20.0 + y_pos));
    QPolygonF *widgetPolygon3 = new QPolygonF(points3);
    list.append(widgetPolygon3);

    x_pos = 0 + 67;
    y_pos = 115 - 10;
    QVector<QPointF> points4; //19 0  29 10  21 54  9 66  0 57   8 11
    points4.append(QPointF(19.0 + x_pos, 0.0 + y_pos));
    points4.append(QPointF(29.0 + x_pos, 10.0 + y_pos));
    points4.append(QPointF(21.0 + x_pos, 54.0 + y_pos));
    points4.append(QPointF(9.0 + x_pos, 66.0 + y_pos));
    points4.append(QPointF(0.0 + x_pos, 57.0 + y_pos));
    points4.append(QPointF(8.0 + x_pos, 11.0 + y_pos));
    QPolygonF *widgetPolygon4 = new QPolygonF(points4);
    list.append(widgetPolygon4);

    x_pos = 80 + 67;
    y_pos = 115 - 10;
    QVector<QPointF> points5; //19 0  29 10  21 54  9 66  0 57   8 11
    points5.append(QPointF(19.0 + x_pos, 0.0 + y_pos));
    points5.append(QPointF(29.0 + x_pos, 10.0 + y_pos));
    points5.append(QPointF(21.0 + x_pos, 54.0 + y_pos));
    points5.append(QPointF(9.0 + x_pos, 66.0 + y_pos));
    points5.append(QPointF(0.0 + x_pos, 57.0 + y_pos));
    points5.append(QPointF(8.0 + x_pos, 11.0 + y_pos));
    QPolygonF *widgetPolygon5 = new QPolygonF(points5);
    list.append(widgetPolygon5);

    x_pos = 14 + 67;
    y_pos = 173 - 10;
    QVector<QPointF> points6; //0 10   10 0    61 0    71 10  61 20   10 20
    points6.append(QPointF(0.0 + x_pos, 10.0 + y_pos));
    points6.append(QPointF(10.0 + x_pos, 0.0 + y_pos));
    points6.append(QPointF(61.0 + x_pos, 0.0 + y_pos));
    points6.append(QPointF(71.0 + x_pos, 10.0 + y_pos));
    points6.append(QPointF(61.0 + x_pos, 20.0 + y_pos));
    points6.append(QPointF(10.0 + x_pos, 20.0 + y_pos));
    QPolygonF *widgetPolygon6 = new QPolygonF(points6);
    list.append(widgetPolygon6);

}


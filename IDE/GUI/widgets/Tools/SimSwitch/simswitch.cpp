#include "simswitch.h"
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



SimSwitch::SimSwitch(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Switch Sim");
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

//         connect(ui.ledBox_0 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_1 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_2 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_3 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_4 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_5 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_6 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));
//         connect(ui.ledBox_7 ,SIGNAL(currentIndexChanged(int)),this,SLOT(handleChange(int)));

    deviceChanged();  //8
    ValueChanged();

}

void SimSwitch::mousePressEvent(QMouseEvent  *event)
{
    qDebug()<< event->x() << "x";
    qDebug()<< event->y()<<  "y";
}

void SimSwitch::comboChanged(int c)
{
    ValueChanged();
}

void SimSwitch::handleChange(int /*o*/)
{
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
        this->value = GetValue();
        m_plio->getInputArray()[address] = this->value; 
}

void SimSwitch::handleUpdateRequest(int mask) //8
{
    if ( 4 & mask)
    {
        this->value = GetValue();
        m_plio->getInputArray()[address] = this->value; 
    }
}

void SimSwitch::handleEvent(int subsysId, int eventId, int /*locationOrReason*/, int /*detail*/) //8
{
    if (MCUSimSubsys::ID_PLIO == subsysId)
    {
        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {
                this->value = GetValue();
                m_plio->getInputArray()[address] = this->value; 
                //m_hexEditIn->getVal(address);
                //this->value = m_plio->getOutputArray()[this->address];
                break;
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
    update();
}

void SimSwitch::addrChanged()
{
    address = ui.lineEditAddress->text().toUInt(0,10);
    qDebug() << "changed address" << address;

    this->value = GetValue();
    m_plio->getInputArray()[address] = this->value; 
}

unsigned char SimSwitch::GetValue(void)
{
    unsigned char Numero;
    Numero = value;
    
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
    return Numero;
}

void SimSwitch::paintEvent(QPaintEvent */*e*/)
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


    painter.setBrush(Qt::white);
    unsigned int x = 171,y = 173 - 10,w = 20,h = 20;
    QRectF rectangle(x, y, w, h);

    painter.end();
}


void SimSwitch::CreateItems(void)
{


}


/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup StackWidget
 * @file stackwidget.cpp
 */


#include "stackwidget.h"
#include <QtGui>
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeStack.h"
#include "../../errordialog/errordlg.h"

StackWidget::StackWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
    : QWidget(parent)
{
    if ( NULL == controlUnit )
    {
        qDebug() << "StackWidget: controlUnit is NULL, this should never happen";
    }
    
    this->subsys = subsys;
    //this->setMinimumHeight(225);
    this->setFixedWidth(120);

    this->numWidth = 3;

    QFont leFont("Ubuntu Mono");
    leFont.setPixelSize(13);
    
    this->leInput = new QLineEdit(this);
    this->lwStack = new QListWidget(this);
    this->lwStack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    //this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->btnPush = new QPushButton("Push", this);
    this->btnPop = new QPushButton("Pop", this);
    m_lblStack = new QLabel("Stack", this);
    m_leSP = new QLineEdit(this);
    m_leSP->setReadOnly(true);

    m_leSP->setFont(leFont);
    this->leInput->setFont(leFont);

    //this->leInput->setFixedWidth(42);
    this->leInput->setFixedHeight(17);
    this->leInput->setMinimumWidth(30);
    //this->btnPush->setFixedWidth(27);
    this->btnPush->setFixedHeight(17);
    //this->btnPop->setFixedWidth(27);
    this->btnPop->setFixedHeight(17);
    m_leSP->setFixedHeight(17);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_lblStack, 0, 0, 1, 2);
    layout->addWidget(m_leSP, 0, 2);
    layout->addWidget(leInput, 1, 0);
    layout->addWidget(btnPush, 1, 1);
    layout->addWidget(btnPop, 1, 2);
    layout->addWidget(lwStack, 2, 0, 1, 3);
    this->setLayout(layout);
    layout->setRowStretch(2, 1);

    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(1);

    QFont fontBtn = this->btnPush->font();
    fontBtn.setPointSize(8);
    this->btnPush->setFont(fontBtn);
    this->btnPop->setFont(fontBtn);

    //QFont fontLE = this->leInput->font();
    //fontLE.setPointSize(9);
    //fontLE.setFamily("Ubuntu Mono");
    //this->leInput->setFont(fontLE);
    this->leInput->setMaxLength(this->numWidth);
    
    QFont fontLW = this->lwStack->font();
    fontLW.setPixelSize(13);
    fontLW.setFamily("Ubuntu Mono");
    this->lwStack->setFont(fontLW);
    //this->lwStack->setFixedWidth(100);
    this->lwStack->setStyleSheet(QString(
                                         "QScrollBar:vertical"
                                         "{"
                                             "border: 0px; "
                                             "background: white;"
                                             "width: 2px;"
                                             "margin: 0px 0 0px 0;"
                                         "}"
                                         "QScrollBar::handle:vertical"
                                         "{"
                                             "background: black;"
                                         "}"
                                        )
                                );

    //QPixmap pixSP(10,10);
    //pixSP.fill(Qt::green);
    //this->iconSP = new QIcon(pixSP);

    this->sp = 0;

    //this->leInput->setInputMask("Hhhh");
    
    connect(btnPush, SIGNAL(clicked()), this, SLOT(push()));
    connect(btnPop, SIGNAL(clicked()), this, SLOT(pop()));

    std::vector<int> mask;
    mask.push_back(MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN);
    mask.push_back(PicoBlazeStack::EVENT_STACK_OVERFLOW);
    mask.push_back(PicoBlazeStack::EVENT_STACK_UNDERFLOW);
    mask.push_back(PicoBlazeStack::EVENT_STACK_SP_CHANGED);
    controlUnit->registerObserver(this, subsys, mask);

    deviceChanged();
}


StackWidget::~StackWidget()
{
    //delete iconSP;
}


void StackWidget::push()
{
    bool ok = false;
    int number = this->leInput->text().toInt(&ok, 16);
    if ( 32 >= this->sp && ok == true && number > 0 && number <= 0x3FF)
    {
        leInput->setStyleSheet("QLineEdit{background: none;}");

        QString text = "0x";
        for (int i = this->leInput->text().length(); i < this->numWidth; i++)
        {
            text.append("0");
        }
        this->lwStack->item(this->sp)->setText(text + this->leInput->text().toUpper());

        this->sp++;
        this->m_memory->directWrite(lwStack->count()-1, this->leInput->text().toInt());
        this->m_memory->setSP(this->sp);
    }
    else
    {
        //if (ok != true)
        //{
        //    qDebug() << "bool, you fucked up";
        //}
        //qDebug() << number;
        this->leInput->setStyleSheet("QLineEdit{background: red;}");
    }
}


void StackWidget::pop()
{
    if ( 0 != this->sp )
    {
        this->sp--;
        this->leInput->setText(this->lwStack->item(this->sp)->text().right(this->numWidth));
        this->leInput->setStyleSheet("QLineEdit{background: none;}");
        this->lwStack->item(this->sp)->setText("");
        this->m_memory->setSP(this->sp);
        /*for (int i = lwStack->count()-1; i > lwStack->currentRow(); i--)
        {
            uint value;
            this->m_memory->directRead(i, value);
            this->m_memory->directWrite(i-1, value);
        }*/
        //this->m_memory->directWrite(lwStack->count()-1, 0);
        //delete this->lwStack->currentItem();
    }
}


void StackWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    //qDebug() << "StackWidget: handleEvent()";
    if ( subsysId != this->subsys )
    {
        qDebug("StackWidget: Invalid event received, event ignored.");
        return;
    }

    //int idx = locationOrReason - m_startingAddress;
    if ( (locationOrReason < 0) || (locationOrReason > m_size) )
    {
        if (locationOrReason == -1 && eventId == PicoBlazeStack::EVENT_STACK_UNDERFLOW)
        {
            error(ErrorCode::ERR_STACK_UNDERFLOW);
            emit stopSimSig();
        }
        else
        {
            qDebug("StackWidget: Invalid address, event ignored.");
        }
        return;
    }

    switch ( eventId )
    {
        case MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN:
        {
            //qDebug() << "StackWidget: event: mem cell" << locationOrReason << "changed to" << detail;
            QString number = QString::number(detail, 16);
            QString text = "0x";
            for (int i = number.length(); i < this->numWidth; i++)
            {
                text.append("0");
            }
            this->lwStack->item(locationOrReason)->setText(text + number.toUpper());
            this->lwStack->item(locationOrReason)->setBackground(Qt::yellow);


            break;
        }
        case PicoBlazeStack::EVENT_STACK_SP_CHANGED:
        {
            //qDebug() << "StackWidget: sp changed:" << locationOrReason;
            //this->lwStack->item(sp)->setIcon(NULL);
            //this->lwStack->item(locationOrReason)->setIcon(iconSP);
            if (locationOrReason != 0)
            {
                this->lwStack->item(locationOrReason-1)->setBackground(Qt::green);
            }
            if (locationOrReason < this->sp)
            {
                if (this->sp != 0)
                {
                    this->lwStack->item(this->sp-1)->setText("");
                    this->lwStack->item(this->sp-1)->setBackground(Qt::yellow);
                }
            }
            else
            {
                if (this->sp != 0)
                {
                    this->lwStack->item(this->sp-1)->setBackground(Qt::yellow);
                }
            }
            if (locationOrReason > 0xF)
            {
                m_leSP->setText("0x" + QString::number(locationOrReason, 16).toUpper());
            }
            else
            {
                m_leSP->setText("0x0" + QString::number(locationOrReason, 16).toUpper());
            }
            m_leSP->setStyleSheet("background-color: yellow");
            this->sp = locationOrReason;
            break;
        }
        case PicoBlazeStack::EVENT_STACK_OVERFLOW:
        {
            if (true == this->warningOptions.stackOverflow)
            {
                error(ErrorCode::ERR_STACK_OVERFLOW);
                emit stopSimSig();
            }
            break;
        }
        case PicoBlazeStack::EVENT_STACK_UNDERFLOW:
        {
            if (true == this->warningOptions.stackUnderflow)
            {
                error(ErrorCode::ERR_STACK_UNDERFLOW);
                emit stopSimSig();
            }
            break;
        }
        default:
            qDebug("Invalid event received, event ignored.");
            break;
    }
    //qDebug() << "StackWidget: return handleEvent()";
}


void StackWidget::deviceChanged()
{
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "StackWidget: m_simControlUnit is NULL";
    }
    //qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
    m_memory = dynamic_cast<PicoBlazeStack*>(m_simControlUnit->getSimSubsys(this->subsys));
    //qDebug() << "StackWidget: SubsysId " << this->subsys;
    if ( NULL == m_memory )
    {
        qDebug() << "StackWidget: m_memory is NULL";
    }
    /*switch ( m_simControlUnit->getArch() )
    {
        case MCUSim::ARCH_AVR8:
        {
            const McuDeviceSpecAVR8 * avr8devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_simControlUnit->getDeviceSpec());

            m_startingAddress = 0;
            m_size = avr8devSpec->m_dataMemory.m_regFileSize;

            break;
        }
        default:
            qDebug("Unknown device architecture.");
            return;
    }*/
    m_size = m_memory->size();
    //qDebug() << "StackWidget: size" << m_size;

    this->lwStack->clear();
    for (int i = 0; i < m_size; i++)
    {
        /*QListWidgetItem *item = */new QListWidgetItem("", this->lwStack, i);
    }

    deviceReset();
}


void StackWidget::deviceReset()
{
    this->sp = 0;
    for (int i = 0; i < m_size; i++)
    {
        this->lwStack->item(i)->setText("");
    }
    this->setWarningOpt(GuiCfg::getInstance().getWarningsOpt());
}


void StackWidget::setReadOnly(bool /*readOnly*/)
{
    
}


void StackWidget::unhighlight()
{
    for (int i = 0; i < m_size; i++)
    {
        this->lwStack->item(i)->setBackground(this->lwStack->palette().base());
    }
    if (this->sp > 0)
    {
        this->lwStack->item(this->sp-1)->setBackground(Qt::green);
    }
    m_leSP->setStyleSheet("background-color: none");
}


void StackWidget::updateWidget()
{
    uint value;
    this->sp = this->m_memory->getSP();
    for (int i = 0; i < this->sp; i++)
    {
        this->m_memory->directRead(i, value);
        if (value != (uint)this->lwStack->item(i)->text().toInt(0, 16))
        {
            this->lwStack->item(i)->setBackground(Qt::yellow);
            QString number = QString::number(value, 16);
            QString text = "0x";
            for (int j = number.length(); j < this->numWidth; j++)
            {
                text.append("0");
            }
            this->lwStack->item(i)->setText(text + number.toUpper());
        }
        else
        {
            this->lwStack->item(i)->setBackground(this->lwStack->palette().base());
        }
    }
    for (int i = this->sp; i < m_size; i++)
    {
        this->lwStack->item(i)->setBackground(this->lwStack->palette().base());
        this->lwStack->item(i)->setText("");
    }
    if (this->sp > 0)
    {
        this->lwStack->item(this->sp-1)->setBackground(Qt::green);
    }
}




void StackWidget::setWarningOpt(GuiCfg::WarningsOpt options)
{
    this->warningOptions = options;
}
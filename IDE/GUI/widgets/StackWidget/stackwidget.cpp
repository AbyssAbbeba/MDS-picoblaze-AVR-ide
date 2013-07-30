#include "stackwidget.h"
#include <QtGui>
#include "../../../simulators/SimControl/MCUSimControl.h"

StackWidget::StackWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys)
    : QWidget(parent)
{
    if ( NULL == controlUnit )
    {
        qDebug() << "StackWidget: controlUnit is NULL, this should never happen";
    }
    
    this->subsys = subsys;
    this->setMinimumHeight(205);
    
    this->leInput = new QLineEdit(this);
    this->lwStack = new QListWidget(this);
    this->btnPush = new QPushButton("Push", this);
    this->btnPop = new QPushButton("Pop", this);

    this->leInput->setMaximumWidth(42);
    this->leInput->setMaximumHeight(17);
    this->btnPush->setMaximumWidth(27);
    this->btnPush->setMaximumHeight(17);
    this->btnPop->setMaximumWidth(27);
    this->btnPop->setMaximumHeight(17);

    this->lwStack->move(0,25);
    this->leInput->move(0,0);
    this->btnPush->move(44,0);
    this->btnPop->move(72,0);

    QFont fontBtn = this->btnPush->font();
    fontBtn.setPointSize(8);
    this->btnPush->setFont(fontBtn);
    this->btnPop->setFont(fontBtn);

    QFont fontLE = this->leInput->font();
    fontLE.setPointSize(9);
    this->leInput->setFont(fontLE);
    
    QFont fontLW = this->lwStack->font();
    fontLW.setPointSize(9);
    this->lwStack->setFont(fontLW);
    
    connect(btnPush, SIGNAL(clicked()), this, SLOT(push()));
    connect(btnPop, SIGNAL(clicked()), this, SLOT(pop()));

    std::vector<int> mask;
    mask.push_back(MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED);
    controlUnit->registerObserver(this, subsys, mask);

    deviceChanged();
}


void StackWidget::push()
{
    if ( 32 >= this->lwStack->count() )
    {
        QListWidgetItem *item = new QListWidgetItem(this->leInput->text(), this->lwStack);
        this->lwStack->setCurrentRow(this->lwStack->count() - 1);
    }
}


void StackWidget::pop()
{
    if ( 0 != this->lwStack->count() )
    {
        this->leInput->setText(this->lwStack->currentItem()->text());
        delete this->lwStack->currentItem();
    }
}


StackWidget::~StackWidget()
{

}


void StackWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    if ( subsysId != this->subsys )
    {
        qDebug("Invalid event received, event ignored.");
        return;
    }

    int idx = locationOrReason - m_startingAddress;
    if ( (idx < 0) || (idx > m_size) )
    {
        qDebug("Invalid address, event ignored.");
        return;
    }

    switch ( eventId )
    {
        case MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED:
        {
            uint value;
            m_memory->directRead(locationOrReason, value);
            qDebug() << "StackWidget: event: mem cell changed to" << value;
            qDebug() << "StackWidget: not implemented yet";


            break;
        }
        default:
            qDebug("Invalid event received, event ignored.");
            break;
    }
}


void StackWidget::deviceChanged()
{
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "StackWidget: m_simControlUnit is NULL";
    }
    qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
    m_memory = dynamic_cast<MCUSim::Memory*>(m_simControlUnit->getSimSubsys(this->subsys));
    qDebug() << "StackWidget: SubsysId " << this->subsys;
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

    deviceReset();
}


void StackWidget::deviceReset()
{
}


void StackWidget::setReadOnly(bool readOnly)
{
}
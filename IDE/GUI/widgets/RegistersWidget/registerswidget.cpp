#include <QtGui>
#include "registerswidget.h"

RegistersWidget::RegistersWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys)
    : QTableWidget(parent)
{
    if ( NULL == controlUnit )
    {
        qDebug() << "RegistersWidget: controlUnit is NULL, this should never happen";
    }
    
    this->setMaximumWidth(200);
    this->setColumnCount(6);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->setShowGrid(false);
    this->setColumnWidth(0, 30);
    this->setColumnWidth(1, 30);
    this->setColumnWidth(2, 40);
    this->setColumnWidth(3, 30);
    this->setColumnWidth(4, 30);
    this->setColumnWidth(5, 40);
    
    QFont font = this->font();
    font.setPointSize(9);
    this->setFont(font);

    this->update = false;
    this->subsys = subsys;
    std::vector<int> mask;
    mask.push_back(MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED);
    controlUnit->registerObserver(this, subsys, mask);


    deviceChanged();
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(updateValue(int, int)));
}


RegistersWidget::~RegistersWidget()
{

}


void RegistersWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
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
            if ( 5 < idx )
            {
                this->item((idx+1)%6, 4)->setText(QString::number(value, 10));
                this->item((idx+1)%6, 5)->setText(QString::number(value, 16));
            }
            else
            {
                this->item((idx+1)%6, 1)->setText(QString::number(value, 10));
                this->item((idx+1)%6, 2)->setText(QString::number(value, 16));
            }
            qDebug() << "RegistersWidget: event: mem cell changed to" << value;
            


            break;
        }
        default:
            qDebug("Invalid event received, event ignored.");
            break;
    }
}


void RegistersWidget::deviceChanged()
{
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "RegistersWidget: m_simControlUnit is NULL";
    }
    qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
    m_memory = dynamic_cast<MCUSim::Memory*>(m_simControlUnit->getSimSubsys(this->subsys));
    qDebug() << "RegistersWidget: SubsysId " << this->subsys;
    if ( NULL == m_memory )
    {
        qDebug() << "RegistersWidget: m_memory is NULL";
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


void RegistersWidget::deviceReset()
{
    this->clear();
    this->setRowCount(m_size/2);
    for (int i = 0; i < m_size/2; i++)
    {
        QTableWidgetItem *reg1 = new QTableWidgetItem("S" + QString::number(i, 16).toUpper());
        reg1->setFlags(Qt::NoItemFlags);
        this->setItem(i, 0, reg1);

        QTableWidgetItem *dec1 = new QTableWidgetItem(QString::number(0, 10));
        dec1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        this->setItem(i, 1, dec1);

        QTableWidgetItem *hex1 = new QTableWidgetItem(QString::number(0, 16));
        hex1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        this->setItem(i, 2, hex1);
        
        QTableWidgetItem *reg2 = new QTableWidgetItem("S" + QString::number(i + m_size/2, 16).toUpper());
        reg2->setFlags(Qt::NoItemFlags);
        this->setItem(i, 3, reg2);

        QTableWidgetItem *dec2 = new QTableWidgetItem(QString::number(0, 10));
        dec2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        this->setItem(i, 4, dec2);

        QTableWidgetItem *hex2 = new QTableWidgetItem(QString::number(0, 16));
        hex2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        this->setItem(i, 5, hex2);

        this->setRowHeight(i, 17);
    }
    /*for (int i = 0; i < 6; i++)
    {
        this->setColumnWidth(i, 20);
    }*/
}


void RegistersWidget::setReadOnly(bool readOnly)
{
}


void RegistersWidget::updateValue(int row, int column)
{
    qDebug() << "RegistersWidget: updateValue()";
    if ( false == this->update )
    {
        this->update = true;
        if ( 0 == (column+1)%3 )
        {
            int value = this->item(row, column)->text().toInt(0, 16);
            this->item(row, column-1)->setText(QString::number(value, 10));
        }
        else
        {
            int value = this->item(row, column)->text().toInt(0, 10);
            this->item(row, column+1)->setText(QString::number(value, 16).toUpper());
        }
        this->update = false;
    }
    qDebug() << "RegistersWidget: return updateValue()";
}
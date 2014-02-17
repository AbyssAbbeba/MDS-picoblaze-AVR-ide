#include <QtGui>
#include "registerswidget.h"

RegistersWidget::RegistersWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
    : QTableWidget(parent)
{
    if ( NULL == controlUnit )
    {
        qDebug() << "RegistersWidget: controlUnit is NULL, this should never happen";
    }
    
    this->setMaximumWidth(305);
    this->setMinimumWidth(305);
    this->setColumnCount(8);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->setShowGrid(false);
    this->setColumnWidth(0, 30);
    this->setColumnWidth(1, 30);
    this->setColumnWidth(2, 25);
    this->setColumnWidth(3, 65);
    this->setColumnWidth(4, 30);
    this->setColumnWidth(5, 30);
    this->setColumnWidth(6, 25);
    this->setColumnWidth(7, 65);
    
    //QFont font = this->font();
    this->setFont(QFont("UbuntuMono", 10));
    //font.setPointSize(9);
    //this->setFont(font);

    this->update = false;
    this->subsys = subsys;
    std::vector<int> mask;
    mask.push_back(MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN);
    controlUnit->registerObserver(this, subsys, mask);


    deviceChanged();
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(updateValue(int, int)));
}


RegistersWidget::~RegistersWidget()
{

}


void RegistersWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int detail)
{
    //qDebug() << "RegistersWidget: handleEvent()";
    if ( subsysId != this->subsys )
    {
        qDebug("Invalid event received, event ignored.");
        return;
    }

    //int idx = locationOrReason - m_startingAddress;
    if ( (locationOrReason < 0) || (locationOrReason > m_size) )
    {
        qDebug("RegistersWidget: Invalid address, event ignored.");
        qDebug() << "RegistersWidget: location" << locationOrReason;
        qDebug() << "RegistersWidget: start and size "<< m_startingAddress << m_size;
        return;
    }

    switch ( eventId )
    {
        case MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN:
        {
            this->update = true;
            uint value;
            m_memory->directRead(locationOrReason, value);
            //qDebug() << "RegistersWidget: location" << locationOrReason << idx;
            if ( 7 < locationOrReason )
            {
                if (value > 99)
                {
                    this->item(locationOrReason-8, 5)->setText(QString::number(value, 10));
                }
                else if (value > 9)
                {
                    this->item(locationOrReason-8, 5)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    this->item(locationOrReason-8, 5)->setText("00" + QString::number(value, 10));
                }
                
                if (value > 15)
                {
                    this->item(locationOrReason-8, 6)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    this->item(locationOrReason-8, 6)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                this->item(locationOrReason-8, 7)->setText(bin);

                
                this->item(locationOrReason-8, 5)->setBackground(Qt::yellow);
                this->item(locationOrReason-8, 6)->setBackground(Qt::yellow);
                this->item(locationOrReason-8, 7)->setBackground(Qt::yellow);
            }
            else
            {
                if (value > 99)
                {
                    this->item(locationOrReason, 1)->setText(QString::number(value, 10));
                }
                else if (value > 9)
                {
                    this->item(locationOrReason, 1)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    this->item(locationOrReason, 1)->setText("00" + QString::number(value, 10));
                }

                if (value > 15)
                {
                    this->item(locationOrReason, 2)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    this->item(locationOrReason, 2)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                this->item(locationOrReason, 3)->setText(bin);
                
                this->item(locationOrReason, 1)->setBackground(Qt::yellow);
                this->item(locationOrReason, 2)->setBackground(Qt::yellow);
                this->item(locationOrReason, 3)->setBackground(Qt::yellow);
            }
            //qDebug() << "RegistersWidget: event: mem cell changed to" << value;
            this->update = false;

            break;
        }
        default:
            qDebug("Invalid event received, event ignored.");
            break;
    }
    //qDebug() << "RegistersWidget: return handleEvent()";
}


void RegistersWidget::deviceChanged()
{
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "RegistersWidget: m_simControlUnit is NULL";
    }
    qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
    m_memory = dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(this->subsys));
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
    this->update = true;
    this->clear();
    this->setRowCount(m_size/2);
    for (int i = 0; i < m_size/2; i++)
    {
        QTableWidgetItem *reg1 = new QTableWidgetItem("S" + QString::number(i, 16).toUpper());
        reg1->setFlags(Qt::NoItemFlags);
        this->setItem(i, 0, reg1);

        QTableWidgetItem *dec1 = new QTableWidgetItem("00" + QString::number(0, 10));
        dec1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        dec1->setBackground(this->palette().base().color());
        this->setItem(i, 1, dec1);

        QTableWidgetItem *hex1 = new QTableWidgetItem("0" + QString::number(0, 16));
        hex1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        hex1->setBackground(this->palette().base().color());
        this->setItem(i, 2, hex1);

        QTableWidgetItem *bin1 = new QTableWidgetItem("0000000" + QString::number(0, 2));
        bin1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        bin1->setBackground(this->palette().base().color());
        this->setItem(i, 3, bin1);
        
        QTableWidgetItem *reg2 = new QTableWidgetItem("S" + QString::number(i + m_size/2, 16).toUpper());
        reg2->setFlags(Qt::NoItemFlags);
        setItem(i, 4, reg2);

        QTableWidgetItem *dec2 = new QTableWidgetItem("00" + QString::number(0, 10));
        dec2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        dec2->setBackground(this->palette().base().color());
        this->setItem(i, 5, dec2);

        QTableWidgetItem *hex2 = new QTableWidgetItem("0" + QString::number(0, 16));
        hex2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        hex2->setBackground(this->palette().base().color());
        this->setItem(i, 6, hex2);
        
        QTableWidgetItem *bin2 = new QTableWidgetItem("0000000" + QString::number(0, 2));
        bin2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        bin2->setBackground(this->palette().base().color());
        this->setItem(i, 7, bin2);

        this->setRowHeight(i, 17);
    }
    this->update = false;
}


void RegistersWidget::setReadOnly(bool readOnly)
{
}


void RegistersWidget::updateValue(int row, int column)
{
    //qDebug() << "RegistersWidget: updateValue()";
    if ( false == this->update )
    {
        this->update = true;
        int value;
        if ( column == 2 || column == 6 )
        {
            //HEX
            //qDebug() << "update 1" << column + 1;
            value = this->item(row, column)->text().toInt(0, 16);
            if ( 255 < value )
            {
                this->item(row, column-1)->setText(QString::number(255, 10));
                this->item(row, column)->setText(QString::number(255, 16).toUpper());
                this->item(row, column+1)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                this->item(row, column-1)->setText("00" + QString::number(0, 10));
                this->item(row, column)->setText("0" + QString::number(0, 16).toUpper());
                this->item(row, column+1)->setText("0000000" + QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    this->item(row, column-1)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    this->item(row, column-1)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    this->item(row, column-1)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    this->item(row, column)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    this->item(row, column)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                this->item(row, column+1)->setText(bin);
            }
        }
        else if (column == 1 || column == 5)
        {
            //qDebug() << "update 2" << column + 1;
            //DEC
            value = this->item(row, column)->text().toInt(0, 10);
            if ( 255 < value )
            {
                this->item(row, column)->setText(QString::number(255, 10));
                this->item(row, column+1)->setText(QString::number(255, 16).toUpper());
                this->item(row, column+2)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                this->item(row, column)->setText("00" + QString::number(0, 10));
                this->item(row, column+1)->setText("0" + QString::number(0, 16).toUpper());
                this->item(row, column+2)->setText(QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    this->item(row, column)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    this->item(row, column)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    this->item(row, column)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    this->item(row, column+1)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    this->item(row, column+1)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                this->item(row, column+2)->setText(bin);
            }
        }
        else
        {
            //qDebug() << "update 3" << column + 1;
            value = this->item(row, column)->text().toInt(0, 2);
            if ( 255 < value )
            {
                this->item(row, column-2)->setText(QString::number(255, 10));
                this->item(row, column-1)->setText(QString::number(255, 16).toUpper());
                this->item(row, column)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                this->item(row, column-2)->setText("00" + QString::number(0, 10));
                this->item(row, column-1)->setText("0" + QString::number(0, 16).toUpper());
                this->item(row, column)->setText(QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    this->item(row, column-2)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    this->item(row, column-2)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    this->item(row, column-2)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    this->item(row, column-1)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    this->item(row, column-1)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                this->item(row, column)->setText(bin);
            }
        }
        if (column > 4)
        {
            this->m_memory->directWrite(row+m_size/2, value);
        }
        else
        {
            this->m_memory->directWrite(row, value);
        }
        this->update = false;
    }
    //qDebug() << "RegistersWidget: return updateValue()";
}


void RegistersWidget::unhighlight()
{
    //qDebug() << "RegistersWidget: unhighlight()";
    this->update = true;
    for (int i = 0; i < this->rowCount(); i++)
    {
        this->item(i, 1)->setBackground(this->palette().base().color());
        this->item(i, 2)->setBackground(this->palette().base().color());
        this->item(i, 3)->setBackground(this->palette().base().color());
        this->item(i, 5)->setBackground(this->palette().base().color());
        this->item(i, 6)->setBackground(this->palette().base().color());
        this->item(i, 7)->setBackground(this->palette().base().color());
    }
    this->update = false;
    //qDebug() << "RegistersWidget: return unhighlight()";
}
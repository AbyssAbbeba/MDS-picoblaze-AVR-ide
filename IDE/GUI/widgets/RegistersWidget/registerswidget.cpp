/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup RegistersWidget
 * @file registerswidget.cpp
 */


#include <QtGui>
#include "registerswidget.h"
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/SimControl/MCUSimControl.h"

RegistersWidget::RegistersWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
    : QWidget(parent)
{
    if ( NULL == controlUnit )
    {
        qDebug() << "RegistersWidget: controlUnit is NULL, this should never happen";
    }
    //this->setMaximumWidth(305);
    //this->setMinimumWidth(305);
    m_layout = new QVBoxLayout(this);
    m_lblReg = new QLabel("Registers", this);
    m_tblReg = new QTableWidget(this);
    m_layout->addWidget(m_lblReg);
    m_layout->addWidget(m_tblReg);
    this->setLayout(m_layout);
    m_tblReg->setColumnCount(8);
    m_tblReg->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tblReg->verticalHeader()->hide();
    m_tblReg->horizontalHeader()->hide();
    m_tblReg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tblReg->setShowGrid(false);
    m_tblReg->setColumnWidth(0, 30);
    m_tblReg->setColumnWidth(1, 30);
    m_tblReg->setColumnWidth(2, 25);
    m_tblReg->setColumnWidth(3, 65);
    m_tblReg->setColumnWidth(4, 30);
    m_tblReg->setColumnWidth(5, 30);
    m_tblReg->setColumnWidth(6, 25);
    m_tblReg->setColumnWidth(7, 65);

    //QFont font = this->font();
    QFont font("Ubuntu Mono");
    font.setPixelSize(13);
    m_tblReg->setFont(font);
    this->setFixedWidth(320);
    //font.setPointSize(9);
    //this->setFont(font);

    m_tblReg->setStyleSheet(QString(
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

    this->update = false;
    this->subsys = subsys;
    std::vector<int> mask;
    mask =  {
                MCUSimMemory::EVENT_MEM_ERR_RD_NONEXISTENT,
                MCUSimMemory::EVENT_MEM_ERR_WR_NONEXISTENT,
                MCUSimMemory::EVENT_MEM_ERR_RD_NOT_IMPLEMENTED,
                MCUSimMemory::EVENT_MEM_ERR_WR_NOT_IMPLEMENTED,
                MCUSimMemory::EVENT_MEM_ERR_RD_ACCESS_DENIED,
                MCUSimMemory::EVENT_MEM_ERR_WR_ACCESS_DENIED,

                MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED,
                //MCUSimMemory::EVENT_MEM_WRN_RD_DEFAULT,
                MCUSimMemory::EVENT_MEM_WRN_RD_WRITE_ONLY,
                MCUSimMemory::EVENT_MEM_WRN_WR_READ_ONLY,
                MCUSimMemory::EVENT_MEM_WRN_RD_PAR_WRITE_ONLY,
                MCUSimMemory::EVENT_MEM_WRN_WR_PAR_READ_ONLY,
                MCUSimMemory::EVENT_MEM_WRN_RD_RESERVED_READ,
                MCUSimMemory::EVENT_MEM_WRN_WR_RESERVED_WRITTEN,

                MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN,
            };
    controlUnit->registerObserver(this, subsys, mask);


    deviceChanged();
    connect(m_tblReg, SIGNAL(cellChanged(int, int)), this, SLOT(updateValue(int, int)));
    connect(m_tblReg, SIGNAL(cellClicked(int,int)), this, SLOT(unhighlightCell(int, int)));
}


RegistersWidget::~RegistersWidget()
{

}


void RegistersWidget::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/)
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
        case MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED:
        {
            if (true == m_warningOptions.memReadUndef)
            {
                qDebug() << "read undef";
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_INF_WR_VAL_WRITTEN:
        {
            if (false == this->update)
            {
                this->update = true;
                uint value;
                m_memory->directRead(locationOrReason, value);
                qDebug() << "RegistersWidget: location" << locationOrReason;
                int size = m_size/2;
                if ( size-1 < locationOrReason )
                {
                    if (value > 99)
                    {
                        m_tblReg->item(locationOrReason-size, 5)->setText(QString::number(value, 10));
                    }
                    else if (value > 9)
                    {
                        m_tblReg->item(locationOrReason-size, 5)->setText("0" + QString::number(value, 10));
                    }
                    else
                    {
                        m_tblReg->item(locationOrReason-size, 5)->setText("00" + QString::number(value, 10));
                    }

                    if (value > 15)
                    {
                        m_tblReg->item(locationOrReason-size, 6)->setText(QString::number(value, 16).toUpper());
                    }
                    else
                    {
                        m_tblReg->item(locationOrReason-size, 6)->setText("0" + QString::number(value, 16).toUpper());
                    }
                    QString bin = QString::number(value, 2);
                    for (int i = bin.size(); i < 8 ; i++)
                    {
                        bin.prepend("0");
                    }
                    m_tblReg->item(locationOrReason-size, 7)->setText(bin);


                    m_tblReg->item(locationOrReason-size, 5)->setBackground(Qt::yellow);
                    m_tblReg->item(locationOrReason-size, 6)->setBackground(Qt::yellow);
                    m_tblReg->item(locationOrReason-size, 7)->setBackground(Qt::yellow);
                }
                else
                {
                    if (value > 99)
                    {
                        m_tblReg->item(locationOrReason, 1)->setText(QString::number(value, 10));
                    }
                    else if (value > 9)
                    {
                        m_tblReg->item(locationOrReason, 1)->setText("0" + QString::number(value, 10));
                    }
                    else
                    {
                        m_tblReg->item(locationOrReason, 1)->setText("00" + QString::number(value, 10));
                    }

                    if (value > 15)
                    {
                        m_tblReg->item(locationOrReason, 2)->setText(QString::number(value, 16).toUpper());
                    }
                    else
                    {
                        m_tblReg->item(locationOrReason, 2)->setText("0" + QString::number(value, 16).toUpper());
                    }
                    QString bin = QString::number(value, 2);
                    for (int i = bin.size(); i < 8 ; i++)
                    {
                        bin.prepend("0");
                    }
                    m_tblReg->item(locationOrReason, 3)->setText(bin);

                    m_tblReg->item(locationOrReason, 1)->setBackground(Qt::yellow);
                    m_tblReg->item(locationOrReason, 2)->setBackground(Qt::yellow);
                    m_tblReg->item(locationOrReason, 3)->setBackground(Qt::yellow);
                }
                //qDebug() << "RegistersWidget: event: mem cell changed to" << value;
                this->update = false;
            }

            break;
        }
        default:
        {
            qDebug() << "RegistersWidget: Invalid event received, event ignored. Event id:" << eventId;
            break;
        }
    }
    //qDebug() << "RegistersWidget: return handleEvent()";
}


void RegistersWidget::deviceChanged()
{
    //qDebug() << "RegistersWidget: deviceChanged()";
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "RegistersWidget: m_simControlUnit is NULL";
    }
    //qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
    m_memory = dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(this->subsys));
    //qDebug() << "RegistersWidget: SubsysId " << this->subsys;
    if ( NULL == m_memory )
    {
        qDebug() << "RegistersWidget: m_memory is NULL";
    }
    //else
    //{
        //qDebug() << "RegistersWidget: m_memory not NULL";
        //qDebug() << "RegistersWidget: m_memory address" << (void*)m_memory;
    //}
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

    this->update = true;
    m_tblReg->clear();
    m_tblReg->setRowCount(m_size/2);
    for (int i = 0; i < m_size/2; i++)
    {
        QTableWidgetItem *reg1 = new QTableWidgetItem("S" + QString::number(i, 16).toUpper());
        reg1->setFlags(Qt::NoItemFlags);
        m_tblReg->setItem(i, 0, reg1);

        QTableWidgetItem *dec1 = new QTableWidgetItem("00" + QString::number(0, 10));
        dec1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        dec1->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 1, dec1);

        QTableWidgetItem *hex1 = new QTableWidgetItem("0" + QString::number(0, 16));
        hex1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        hex1->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 2, hex1);

        QTableWidgetItem *bin1 = new QTableWidgetItem("0000000" + QString::number(0, 2));
        bin1->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        bin1->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 3, bin1);

        QTableWidgetItem *reg2 = new QTableWidgetItem("S" + QString::number(i + m_size/2, 16).toUpper());
        reg2->setFlags(Qt::NoItemFlags);
        m_tblReg->setItem(i, 4, reg2);

        QTableWidgetItem *dec2 = new QTableWidgetItem("00" + QString::number(0, 10));
        dec2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        dec2->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 5, dec2);

        QTableWidgetItem *hex2 = new QTableWidgetItem("0" + QString::number(0, 16));
        hex2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        hex2->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 6, hex2);

        QTableWidgetItem *bin2 = new QTableWidgetItem("0000000" + QString::number(0, 2));
        bin2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        bin2->setBackground(m_tblReg->palette().base().color());
        m_tblReg->setItem(i, 7, bin2);

        m_tblReg->setRowHeight(i, 17);
    }
    this->update = false;

    deviceReset();
}


void RegistersWidget::deviceReset()
{
    this->updateWidget();
    this->unhighlight();
}


void RegistersWidget::setReadOnly(bool /*readOnly*/)
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
            value = m_tblReg->item(row, column)->text().toInt(0, 16);
            if ( 255 < value )
            {
                m_tblReg->item(row, column-1)->setText(QString::number(255, 10));
                m_tblReg->item(row, column)->setText(QString::number(255, 16).toUpper());
                m_tblReg->item(row, column+1)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                m_tblReg->item(row, column-1)->setText("00" + QString::number(0, 10));
                m_tblReg->item(row, column)->setText("0" + QString::number(0, 16).toUpper());
                m_tblReg->item(row, column+1)->setText("0000000" + QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    m_tblReg->item(row, column-1)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    m_tblReg->item(row, column-1)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    m_tblReg->item(row, column-1)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    m_tblReg->item(row, column)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    m_tblReg->item(row, column)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                m_tblReg->item(row, column+1)->setText(bin);
            }
        }
        else if (column == 1 || column == 5)
        {
            //qDebug() << "update 2" << column + 1;
            //DEC
            value = m_tblReg->item(row, column)->text().toInt(0, 10);
            if ( 255 < value )
            {
                m_tblReg->item(row, column)->setText(QString::number(255, 10));
                m_tblReg->item(row, column+1)->setText(QString::number(255, 16).toUpper());
                m_tblReg->item(row, column+2)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                m_tblReg->item(row, column)->setText("00" + QString::number(0, 10));
                m_tblReg->item(row, column+1)->setText("0" + QString::number(0, 16).toUpper());
                m_tblReg->item(row, column+2)->setText(QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    m_tblReg->item(row, column)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    m_tblReg->item(row, column)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    m_tblReg->item(row, column)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    m_tblReg->item(row, column+1)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    m_tblReg->item(row, column+1)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                m_tblReg->item(row, column+2)->setText(bin);
            }
        }
        else
        {
            //qDebug() << "update 3" << column + 1;
            value = m_tblReg->item(row, column)->text().toInt(0, 2);
            if ( 255 < value )
            {
                m_tblReg->item(row, column-2)->setText(QString::number(255, 10));
                m_tblReg->item(row, column-1)->setText(QString::number(255, 16).toUpper());
                m_tblReg->item(row, column)->setText(QString::number(255, 2));
            }
            else if ( 0 > value )
            {
                m_tblReg->item(row, column-2)->setText("00" + QString::number(0, 10));
                m_tblReg->item(row, column-1)->setText("0" + QString::number(0, 16).toUpper());
                m_tblReg->item(row, column)->setText(QString::number(0, 2));
            }
            else
            {
                if (value >= 100)
                {
                    m_tblReg->item(row, column-2)->setText(QString::number(value, 10));
                }
                else if (value >= 10)
                {
                    m_tblReg->item(row, column-2)->setText("0" + QString::number(value, 10));
                }
                else
                {
                    m_tblReg->item(row, column-2)->setText("00" + QString::number(value, 10));
                }

                if (value > 0xF)
                {
                    m_tblReg->item(row, column-1)->setText(QString::number(value, 16).toUpper());
                }
                else
                {
                    m_tblReg->item(row, column-1)->setText("0" + QString::number(value, 16).toUpper());
                }
                QString bin = QString::number(value, 2);
                for (int i = bin.size(); i < 8 ; i++)
                {
                    bin.prepend("0");
                }
                m_tblReg->item(row, column)->setText(bin);
            }
        }
        if (column > 4)
        {
            m_memory->directWrite(row+m_size/2, value);
        }
        else
        {
            m_memory->directWrite(row, value);
        }
        m_simControlUnit->dispatchEvents();
        this->update = false;
    }
    //qDebug() << "RegistersWidget: return updateValue()";
}


void RegistersWidget::unhighlight()
{
    //qDebug() << "RegistersWidget: unhighlight()";
    this->update = true;
    for (int i = 0; i < m_tblReg->rowCount(); i++)
    {
        m_tblReg->item(i, 1)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(i, 2)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(i, 3)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(i, 5)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(i, 6)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(i, 7)->setBackground(m_tblReg->palette().base().color());
    }
    this->update = false;
    //qDebug() << "RegistersWidget: return unhighlight()";
}


void RegistersWidget::updateWidget()
{
    qDebug() << "RegistersWidget: update widget";
    this->update = true;
    uint value;
    QString bin;
    for (int i = 0; i < m_size/2; i++)
    {
        this->m_memory->directRead(i, value);
        //dec, hex, bin, first half, Si
        if (value != m_tblReg->item(i,1)->text().toUInt(0, 10))
        {
            m_tblReg->item(i, 1)->setBackground(Qt::yellow);
            m_tblReg->item(i, 2)->setBackground(Qt::yellow);
            m_tblReg->item(i, 3)->setBackground(Qt::yellow);

            if (value < 10)
            {
                m_tblReg->item(i, 1)->setText("00" + QString::number(value, 10));
            }
            else if (value < 100)
            {
                m_tblReg->item(i, 1)->setText("0" + QString::number(value, 10));
            }
            else
            {
                m_tblReg->item(i, 1)->setText(QString::number(value, 10));
            }

            if (value < 0x10)
            {
                m_tblReg->item(i, 2)->setText("0" + QString::number(value, 16).toUpper());
            }
            else
            {
                m_tblReg->item(i, 2)->setText(QString::number(value, 16).toUpper());
            }

            bin = QString::number(value, 2);
            for (int i = bin.size(); i < 8 ; i++)
            {
                bin.prepend("0");
            }
            m_tblReg->item(i, 3)->setText(bin);
        }
        else
        {
            m_tblReg->item(i, 1)->setBackground(m_tblReg->palette().base().color());
            m_tblReg->item(i, 2)->setBackground(m_tblReg->palette().base().color());
            m_tblReg->item(i, 3)->setBackground(m_tblReg->palette().base().color());
        }

        //dec, hex, bin, second half, S(i+m_size/2)
        this->m_memory->directRead(i+m_size/2, value);
        if (value != m_tblReg->item(i,5)->text().toUInt(0, 10))
        {
            m_tblReg->item(i, 5)->setBackground(Qt::yellow);
            m_tblReg->item(i, 6)->setBackground(Qt::yellow);
            m_tblReg->item(i, 7)->setBackground(Qt::yellow);

            if (value < 10)
            {
                m_tblReg->item(i, 5)->setText("00" + QString::number(value, 10));
            }
            else if (value < 100)
            {
                m_tblReg->item(i, 5)->setText("0" + QString::number(value, 10));
            }
            else
            {
                m_tblReg->item(i, 5)->setText(QString::number(value, 10));
            }

            if (value < 0x10)
            {
                m_tblReg->item(i, 6)->setText("0" + QString::number(value, 16).toUpper());
            }
            else
            {
                m_tblReg->item(i, 6)->setText(QString::number(value, 16).toUpper());
            }

            bin = QString::number(value, 2);
            for (int i = bin.size(); i < 8 ; i++)
            {
                bin.prepend("0");
            }
            m_tblReg->item(i, 7)->setText(bin);
        }
        else
        {
            m_tblReg->item(i, 5)->setBackground(m_tblReg->palette().base().color());
            m_tblReg->item(i, 6)->setBackground(m_tblReg->palette().base().color());
            m_tblReg->item(i, 7)->setBackground(m_tblReg->palette().base().color());
        }
    }
    this->update = false;
}


void RegistersWidget::unhighlightCell(int row, int column)
{
    if (column < 4)
    {
        m_tblReg->item(row, 1)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(row, 2)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(row, 3)->setBackground(m_tblReg->palette().base().color());
    }
    else
    {
        m_tblReg->item(row, 5)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(row, 6)->setBackground(m_tblReg->palette().base().color());
        m_tblReg->item(row, 7)->setBackground(m_tblReg->palette().base().color());
    }
}


void RegistersWidget::setWarningOpt(GuiCfg::WarningsOpt options)
{
    m_warningOptions = options;
}
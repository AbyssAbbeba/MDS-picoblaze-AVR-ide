/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuMemoryView
 * @file McuMemoryView.cpp
 */


#include "McuMemoryView.h"

#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QApplication>

#include "../HexEdit/hexedit.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
//#include "McuDeviceSpecAVR8.h"

McuMemoryView::McuMemoryView(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_size(0),
	  m_hexEdit(NULL)
{
    std::vector<int> mask;
    this->subsys = subsys;
    mask.push_back(MCUSimMemory::EVENT_MEM_INF_WR_VAL_CHANGED);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_WR_NONEXISTENT);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_RD_NONEXISTENT);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_WR_NOT_IMPLEMENTED);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_RD_NOT_IMPLEMENTED);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_RD_ACCESS_DENIED);
    mask.push_back(MCUSimMemory::EVENT_MEM_ERR_WR_ACCESS_DENIED);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_RD_DEFAULT);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_RD_WRITE_ONLY);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_WR_READ_ONLY);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_RD_PAR_WRITE_ONLY);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_WR_PAR_READ_ONLY);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_RD_RESERVED_READ);
    mask.push_back(MCUSimMemory::EVENT_MEM_WRN_WR_RESERVED_WRITTEN);
	controlUnit->registerObserver(
		this,
		subsys,
        mask);

    if ( NULL == controlUnit )
    {
        qDebug() << "McuMemoryView: controlUnit is NULL";
    }
	m_layout = new QGridLayout(this);
    m_lblScratch = new QLabel("Scratchpad RAM", this);
	setLayout(m_layout);

    m_layout->setVerticalSpacing(0);

    m_layout->addWidget(m_lblScratch, 0, 1);

	deviceChanged();
}

McuMemoryView::~McuMemoryView()
{
	deleteHexEdit();
	delete m_layout;
}

inline void McuMemoryView::deleteHexEdit()
{
	if ( NULL != m_hexEdit )
    {
		delete m_hexEdit;
        m_hexEdit = NULL;
	}
}


void McuMemoryView::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/)
{
	if ( this->subsys != subsysId )
    {
		qDebug("Invalid event received, event ignored.");
		return;
 	}

	int idx = locationOrReason;// - m_startingAddress;
	if ( (idx < 0) || (idx > m_size) )
    {
		qDebug("Invalid address, event ignored.");
		return;
	}
	
	switch ( eventId )
    {
		case MCUSimMemory::EVENT_MEM_INF_WR_VAL_CHANGED:
        {
			uint value;
			m_memory->directRead(locationOrReason, value);
            //qDebug() << "McuMemoryView: event: mem cell changed to" << value;

 			m_hexEdit->setVal(idx, (char)value);
 			m_hexEdit->setHighlighted(idx, true);

			break;
		}
        case MCUSimMemory::EVENT_MEM_ERR_WR_NONEXISTENT:
        {
            if (true == this->warningOptions.memWriteNonexist)
            {
                error(ErrorCode::ERR_MEM_WR_NONEXISTENT);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_ERR_RD_NONEXISTENT:
        {
            if (true == this->warningOptions.memReadNonexist)
            {
                error(ErrorCode::ERR_MEM_RD_NONEXISTENT);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_ERR_WR_NOT_IMPLEMENTED:
        {
            if (true == this->warningOptions.memWriteUnimplement)
            {
                error(ErrorCode::ERR_MEM_WR_NOT_IMPLEMENTED);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_ERR_RD_NOT_IMPLEMENTED:
        {
            if (true == this->warningOptions.memReadUnimplement)
            {
                error(ErrorCode::ERR_MEM_RD_NOT_IMPLEMENTED);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_ERR_RD_ACCESS_DENIED:
        {
            if (true == this->warningOptions.memReadAccess)
            {
                error(ErrorCode::ERR_MEM_RD_ACCESS_DENIED);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_ERR_WR_ACCESS_DENIED:
        {
            if (true == this->warningOptions.memWriteAccess)
            {
                error(ErrorCode::ERR_MEM_WR_ACCESS_DENIED);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_RD_UNDEFINED:
        {
            if (true == this->warningOptions.memReadUndef)
            {
                error(ErrorCode::ERR_MEM_RD_UNDEFINED, QString::number(locationOrReason, 16));
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_RD_DEFAULT:
        {
            if (true == this->warningOptions.memReadDefault)
            {
                error(ErrorCode::ERR_MEM_RD_DEFAULT);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_RD_WRITE_ONLY:
        {
            if (true == this->warningOptions.memReadWriteOnly)
            {
                error(ErrorCode::ERR_MEM_RD_WRITE_ONLY);       
                emit stopSimSig();                                                      
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_WR_READ_ONLY:
        {
            if (true == this->warningOptions.memWriteReadOnly)
            {
                error(ErrorCode::ERR_MEM_WR_READ_ONLY);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_RD_PAR_WRITE_ONLY:
        {
            if (true == this->warningOptions.memReadPartWriteOnly)
            {
                error(ErrorCode::ERR_MEM_RD_PAR_WRITE_ONLY);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_WR_PAR_READ_ONLY:
        {
            if (true == this->warningOptions.memWritePartReadOnly)
            {
                error(ErrorCode::ERR_MEM_WR_PAR_READ_ONLY);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_RD_RESERVED_READ:
        {
            if (true == this->warningOptions.memReadReserved)
            {
                error(ErrorCode::ERR_MEM_RD_RESERVED_READ);
                emit stopSimSig();
            }
            break;
        }
        case MCUSimMemory::EVENT_MEM_WRN_WR_RESERVED_WRITTEN:
        {
            if (true == this->warningOptions.memWriteReserved)
            {
                error(ErrorCode::ERR_MEM_WR_RESERVED_WRITTEN);
                emit stopSimSig();
            }
            break;
        }

		default:
			qDebug("Invalid event received, event ignored.");
			break;
	}
}


void McuMemoryView::deviceChanged()
{
    //qDebug() << "McuMemoryView: deviceChanged()";
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "McuMemoryView: m_simControlUnit is NULL";
    }
    //qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
	m_memory = dynamic_cast<MCUSimMemory*>(m_simControlUnit->getSimSubsys(this->subsys));
    //qDebug() << "McuMemoryView: SubsysId " << this->subsys;
    if ( NULL == m_memory )
    {
        qDebug() << "McuMemoryView: m_memory is NULL";
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
	deleteHexEdit();
	m_hexEdit = new HexEdit(this, false, m_size, 8);
	connect(m_hexEdit, SIGNAL(textChanged(int)), this, SLOT(changeValue(int)));
	m_layout->addWidget(m_hexEdit, 1, 0, 1, 5);
    QApplication::processEvents();
    m_hexEdit->fixHeight();
    //m_hexEdit->show();
    //this->setFixedWidth(m_hexEdit->width());
	deviceReset();
    /*if (0 == m_size)
    {
        this->hide();
        this->m_hexEdit->hide();
    }*/
    /*else
    {
        this->show();
        this->m_hexEdit->show();
    }*/
    //qDebug() << "McuMemoryView: return deviceChanged()";
}


void McuMemoryView::changeValue(int address)
{
 	m_memory->directWrite(address, m_hexEdit->getVal(address));
}


void McuMemoryView::deviceReset()
{
    //qDebug() << "McuMemoryView: deviceReset()"; 
	if ( NULL == m_hexEdit )
    {
		return;
	}

    uint value;
	for ( int i = 0; i < m_size; i++ )
    {

		//uint address = i;// + m_startingAddress;
		m_memory->directRead(i, value);

 		m_hexEdit->setVal(i, (unsigned char)value);
	}
    m_hexEdit->fixHeight();
    m_hexEdit->scrollToTop();
    //qDebug() << "McuMemoryView: return deviceReset()"; 
}


void McuMemoryView::setReadOnly(bool readOnly)
{
	if ( NULL == m_hexEdit ) {
		return;
	}

 	m_hexEdit->setReadOnly(readOnly);
// 	for ( int i = 0; i < m_size; i++ ) {
// 		if ( false == readOnly ) {
// 			m_hexEdit->setHighlighted(i, false);
// 		}
// 	}
}


void McuMemoryView::fixHeight()
{
    m_hexEdit->fixHeight();
}


void McuMemoryView::unhighlight()
{
    m_hexEdit->unhighlight();
}


void McuMemoryView::updateWidget()
{
    uint value;
    for ( int i = 0; i < m_size; i++ )
    {
        //uint address = i;// + m_startingAddress;
        m_memory->directRead(i, value);

        if (value != m_hexEdit->getVal(i))
        {
            m_hexEdit->setVal(i, (unsigned char)value);
            m_hexEdit->setHighlighted(i, true);
        }
        /*else
        {
            m_hexEdit->setHighlighted(i, false);
        }*/
    }
}


int McuMemoryView::getSize()
{
    return m_size;
}


void McuMemoryView::setWarningOpt(GuiCfg::WarningsOpt options)
{
    this->warningOptions = options;
}


void McuMemoryView::resizeEvent(QResizeEvent */*event*/)
{
    m_hexEdit->fixHeight();
}
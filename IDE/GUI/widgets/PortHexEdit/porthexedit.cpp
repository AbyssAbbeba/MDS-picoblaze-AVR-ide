/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2012
 *
 */

#include "porthexedit.h"

//#include <QGridLayout>
#include <QDebug>

#include "../HexEdit/hexedit.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
//#include "McuDeviceSpecAVR8.h"

PortHexEdit::PortHexEdit(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_size(0),
	  m_hexEditIn(NULL),
	  m_hexEditOut(NULL)
{
    qDebug() << "PortHexEdit: PortHexEdit()";
    std::vector<int> mask;
    this->subsys = subsys;
    mask.push_back(MCUSimMemory::EVENT_MEM_INF_WR_VAL_CHANGED);
	controlUnit->registerObserver(
		this,
		subsys,
        mask);

    if ( NULL == controlUnit )
    {
        qDebug() << "PortHexEdit: controlUnit is NULL";
    }
	//m_layout = new QHBoxLayout(this);
	//setLayout(m_layout);
	//this->m_hexEditOut->hide();
    this->visibleIn = true;

	deviceChanged();
    qDebug() << "PortHexEdit: return PortHexEdit()";
}

PortHexEdit::~PortHexEdit()
{
	deleteHexEdit();
	//delete m_layout;
}

inline void PortHexEdit::deleteHexEdit()
{
	if ( NULL != m_hexEditIn )
    {
		delete m_hexEditIn;
	}
	if ( NULL != m_hexEditOut )
    {
        delete m_hexEditOut;
    }
}

void PortHexEdit::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/)
{
	if ( this->subsys != subsysId )
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
        case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
        {
			uint value = m_plio->getOutputArray()[locationOrReason];
            qDebug() << "PortHexEdit: event: mem cell changed to" << (unsigned char)value;

 			m_hexEditOut->setVal(idx, (unsigned char)value);
// 			m_hexEdit->setHighlighted(idx, true);

			break;
		}
        case MCUSimPureLogicIO::EVENT_PLIO_READ:
        {
            uint value = m_plio->getInputArray()[locationOrReason];
            qDebug() << "PortHexEdit: event: mem cell changed to" << (unsigned char)value;

            m_hexEditIn->setVal(idx, (unsigned char)value);
//          m_hexEdit->setHighlighted(idx, true);

            break;
        }
		default:
			qDebug("Invalid event received, event ignored.");
			break;
	}
}


void PortHexEdit::deviceChanged()
{
    qDebug() << "PortHexEdit: deviceChanged()";
    if ( NULL == m_simControlUnit )
    {
        qDebug() << "PortHexEdit: m_simControlUnit is NULL";
    }
    if ( NULL == m_simControlUnit->getSimSubsys(this->subsys) )
    {
        qDebug() << "PortHexEdit: SubsysId " << this->subsys;
        qDebug() << "PortHexEdit: m_simControlUnit->getSimSubsys(this->subsys) is NULL";
    }
	m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(this->subsys));
    //if ( NULL == m_memory )
    //{
    //    qDebug() << "PortHexEdit: m_memory is NULL";
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
    m_size = m_plio->getNumberOfPorts();
	deleteHexEdit();
	m_hexEditIn = new HexEdit(this, false, m_size, 8);
    m_hexEditIn->show();
    m_hexEditIn->verticalScrollBar()->hide();
    m_hexEditOut = new HexEdit(this, false, m_size, 8);
    m_hexEditOut->hide();
    m_hexEditOut->verticalScrollBar()->hide();
    //m_hexEditIn->fixHeight();
    //m_hexEditOut->fixHeight();
	connect(m_hexEditIn, SIGNAL(textChanged(int)), this, SLOT(changeValueIn(int)));
    connect(m_hexEditOut, SIGNAL(textChanged(int)), this, SLOT(changeValueOut(int)));
	//m_layout->addWidget(m_hexEditIn);

	deviceReset();
    qDebug() << "PortHexEdit: return deviceChanged()";
}


void PortHexEdit::changeValueIn(int address)
{
 	//m_memory->directWrite(address, m_hexEdit->getVal(address));
}


void PortHexEdit::changeValueOut(int address)
{
    //m_memory->directWrite(address, m_hexEdit->getVal(address));
}


void PortHexEdit::deviceReset()
{
    qDebug() << "PortHexEdit: deviceReset()";
	if ( NULL == m_hexEditIn || NULL == m_hexEditOut )
    {
		return;
	}

	for ( int i = 0; i < m_size; i++ )
    {

		//uint address = i;

		uint value = 0;
        value = m_plio->getInputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
 		m_hexEditIn->setVal(i, (unsigned char)value);
        //qDebug() << "PortHexEdit: in value is" << (unsigned char)value;

        value = m_plio->getOutputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
        m_hexEditOut->setVal(i, (unsigned char)value);
        //qDebug() << "PortHexEdit: out value is" << (unsigned char)value;
	}
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
    qDebug() << "PortHexEdit: return deviceReset()";
}


void PortHexEdit::setReadOnly(bool readOnly)
{
    qDebug() << "PortHexEdit: setReadOnly()";
	if ( NULL == m_hexEditIn || NULL == m_hexEditOut )
    {
		return;
	}

 	m_hexEditIn->setReadOnly(readOnly);
    m_hexEditOut->setReadOnly(readOnly);
// 	for ( int i = 0; i < m_size; i++ ) {
// 		if ( false == readOnly ) {
// 			m_hexEdit->setHighlighted(i, false);
// 		}
// 	}
    qDebug() << "PortHexEdit: return setReadOnly()";
}


void PortHexEdit::fixHeight()
{
    qDebug() << "PortHexEdit: fixHeight()";
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
    qDebug() << "PortHexEdit: return fixHeight()";
}


void PortHexEdit::switchPorts()
{
    qDebug() << "PortHexEdit: switchPorts()";
    if ( true == this->visibleIn )
    {
        this->m_hexEditIn->hide();
        this->m_hexEditOut->show();
        this->visibleIn = false;
    }
    else
    {
        this->m_hexEditOut->hide();
        this->m_hexEditIn->show();
        this->visibleIn = true;
    }
    qDebug() << "PortHexEdit: return switchPorts()";
}
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

#include <QGridLayout>
#include <QDebug>

#include "../HexEdit/hexedit.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
//#include "McuDeviceSpecAVR8.h"

PortHexEdit::PortHexEdit(QWidget * parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_size(0),
	  m_hexEdit(NULL)
{
    std::vector<int> mask;
    this->subsys = subsys;
    mask.push_back(MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED);
	controlUnit->registerObserver(
		this,
		subsys,
        mask);

    if ( NULL == controlUnit )
    {
        qDebug() << "PortHexEdit: controlUnit is NULL";
    }
	m_layout = new QHBoxLayout(this);
	setLayout(m_layout);

	deviceChanged();
}

PortHexEdit::~PortHexEdit()
{
	deleteHexEdit();
	delete m_layout;
}

inline void PortHexEdit::deleteHexEdit()
{
	if ( NULL != m_hexEdit )
    {
		delete m_hexEdit;
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
		case MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED:
        {
			uint value;
			m_memory->directRead(locationOrReason, value);
            qDebug() << "PortHexEdit: event: mem cell changed to" << value;

 			m_hexEdit->setVal(idx, (char)value);
// 			m_hexEdit->setHighlighted(idx, true);

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
    qDebug() << m_simControlUnit->getSimSubsys(this->subsys);
	m_memory = dynamic_cast<MCUSim::Memory*>(m_simControlUnit->getSimSubsys(this->subsys));
    qDebug() << "PortHexEdit: SubsysId " << this->subsys;
    if ( NULL == m_memory )
    {
        qDebug() << "PortHexEdit: m_memory is NULL";
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
	m_layout->addWidget(m_hexEdit);

	deviceReset();
    qDebug() << "PortHexEdit: return deviceChanged()";
}


void PortHexEdit::changeValue(int address)
{
 	m_memory->directWrite(address, m_hexEdit->getVal(address));
}


void PortHexEdit::deviceReset()
{
    qDebug() << "PortHexEdit: deviceReset()";
	if ( NULL == m_hexEdit )
    {
		return;
	}

	for ( int i = 0; i < m_size; i++ )
    {

		uint address = i + m_startingAddress;
		uint value;
		m_memory->directRead(address, value);

 		m_hexEdit->setVal(i, (char)value);
	}
    m_hexEdit->fixHeight();
    qDebug() << "PortHexEdit: return deviceReset()";
}


void PortHexEdit::setReadOnly(bool readOnly)
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


void PortHexEdit::fixHeight()
{
    m_hexEdit->fixHeight();
}
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "McuMemoryView.h"

#include <QGridLayout>

#include "hexedit.h"
#include "MCUSimControl.h"
#include "McuDeviceSpecAVR8.h"

McuMemoryView::McuMemoryView(MCUSimControl * controlUnit, QWidget * parent)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_size(0),
	  m_hexEdit(NULL)
{
        std::vector<int> mask;
        mask.push_back(MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED);
	controlUnit->registerObserver(
		this,
		MCUSim::Subsys::ID_MEM_DATA,
                mask);

	m_layout = new QGridLayout(this);
	setLayout(m_layout);

	deviceChanged();
}

McuMemoryView::~McuMemoryView() {
	deleteHexEdit();
	delete m_layout;
}

inline void McuMemoryView::deleteHexEdit() {
	if ( NULL != m_hexEdit ) {
		delete m_hexEdit;
	}
}

void McuMemoryView::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/) {
	if ( MCUSim::Subsys::ID_MEM_DATA != subsysId ) {
		qDebug("Invalid event received, event ignored.");
		return;
	}

	int idx = locationOrReason - m_startingAddress;
	if ( (idx < 0) || (idx > m_size) ) {
		qDebug("Invalid address, event ignored.");
		return;
	}

	switch ( eventId ) {
		case MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED: {
			uint value;

			m_memory->directRead(locationOrReason, value);

 			m_hexEdit->setVal(idx, (char)value);
// 			m_hexEdit->setHighlighted(idx, true);

			break;
		}
		default:
			qDebug("Invalid event received, event ignored.");
			break;
	}
}

void McuMemoryView::deviceChanged() {
	m_memory = dynamic_cast<MCUSim::Memory*>(m_simControlUnit->getSimSubsys(MCUSim::Subsys::ID_MEM_DATA));

	switch ( m_simControlUnit->getArch() ) {
		case MCUSim::ARCH_AVR8: {
			const McuDeviceSpecAVR8 * avr8devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_simControlUnit->getDeviceSpec());

			m_startingAddress = 0;
			m_size = avr8devSpec->m_dataMemory.m_regFileSize;

			break;
		}
		default:
			qDebug("Unknown device architecture.");
			return;
	}

	deleteHexEdit();
	m_hexEdit = new HexEdit(this, false, m_size, 8);
	connect(m_hexEdit, SIGNAL(textChanged(int)), this, SLOT(changeValue(int)));
	m_layout->addWidget(m_hexEdit, 0, 0);

	deviceReset();
}

void McuMemoryView::changeValue(int address) {
 	m_memory->directWrite(address, m_hexEdit->getVal(address));
}

void McuMemoryView::deviceReset() {
	if ( NULL == m_hexEdit ) {
		return;
	}

	for ( int i = 0; i < m_size; i++ ) {

		uint address = i + m_startingAddress;
		uint value;
		m_memory->directRead(address, value);

 		m_hexEdit->setVal(i, (char)value);
	}
}

void McuMemoryView::setReadOnly(bool readOnly) {
	if ( NULL == m_hexEdit ) {
		return;
	}

 	//m_hexEdit->setReadOnly(readOnly);
// 	for ( int i = 0; i < m_size; i++ ) {
// 		if ( false == readOnly ) {
// 			m_hexEdit->setHighlighted(i, false);
// 		}
// 	}
}

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

#include "RegDisplayList.h"

#include <vector>
#include <QVBoxLayout>
#include <QStringList>

#include "RegDisplay.h"
#include "MCUSimControl.h"
#include "McuDeviceSpecAVR8.h"

RegDisplayList::RegDisplayList(MCUSimControl * controlUnit, QWidget * parent)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_numberOfDisplays(0),
	  m_regDisplays(NULL)
{
	parentWidget = parent;
        std::vector<int> mask;
        mask.push_back(MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED);
	controlUnit->registerObserver(
		this,
		MCUSim::Subsys::ID_MEM_DATA,
		mask);


	deviceChanged();
}

RegDisplayList::~RegDisplayList() {
	deleteDisplays();
}

inline void RegDisplayList::deleteDisplays() {
	if ( NULL == m_regDisplays ) {
		return;
	}

	m_numberOfDisplays = 0;
	for ( int i = 0; i < m_numberOfDisplays; i++ ) {
		if ( NULL != m_regDisplays[i] ) {
// 			disconnect(m_regDisplays[idx], SIGNAL(valueChanged(uint, uint), this, SLOT(changeValue(uint, uint))));
			delete m_regDisplays[i];
		}
	}
	delete[] m_regDisplays;
}

void RegDisplayList::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/) {
	if ( MCUSim::Subsys::ID_MEM_DATA != subsysId ) {
		qDebug("Invalid event received, event ignored.");
		return;
	}

	int idx = locationOrReason - m_startingAddress;
	if ( (idx < 0) || (idx > m_numberOfDisplays) ) {
		qDebug("Invalid address, event ignored.");
		return;
	}

	switch ( eventId ) {
		case MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED: {
			uint value;

			m_memory->directRead(locationOrReason, value);

			m_regDisplays[idx]->setValue(value);
			m_regDisplays[idx]->setHighlighted(true);

			break;
		}
		default:
			qDebug("Invalid event received, event ignored.");
			break;
	}
}

void RegDisplayList::deviceChanged() {
	m_memory = dynamic_cast<MCUSim::Memory*>(m_simControlUnit->getSimSubsys(MCUSim::Subsys::ID_MEM_DATA));

	switch ( m_simControlUnit->getArch() ) {
		case MCUSim::ARCH_AVR8: {
			const McuDeviceSpecAVR8 * avr8devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_simControlUnit->getDeviceSpec());

			m_startingAddress = avr8devSpec->m_dataMemory.m_regFileSize;
			m_numberOfDisplays = avr8devSpec->m_dataMemory.m_ioRegSize;

			break;
		}
		default:
			qDebug("Unknown device architecture.");
			return;
	}

	std::vector<MCUSimControl::SFRRegDesc> listOfSFR;
	m_simControlUnit->getListOfSFR(listOfSFR);

	/*
	 * Generate new set of register displays
	 */
	deleteDisplays();

	m_regDisplays = new RegDisplay * [m_numberOfDisplays];
	for ( int i = 0; i < m_numberOfDisplays; i++ ) {
		m_regDisplays[i] = NULL;
	}

	for ( std::vector<MCUSimControl::SFRRegDesc>::iterator it = listOfSFR.begin(); it != listOfSFR.end(); it++ ) {
		int idx = ( it->m_address - m_startingAddress );

		if ( (idx < 0) || (idx > m_numberOfDisplays) ) {
			qDebug("SFR address out of range.");
			continue;
		}

		if ( 0 == it->m_bitNames[0].size() ) {
			m_regDisplays[idx] = new RegDisplay(
				it->m_address,
				QString::fromStdString(it->m_regName),
				QString::fromStdString(it->m_regNameTip),
				it->m_mask,
				NULL,
				NULL,
				NULL,
				parentWidget);
		} else {
			QStringList bitNames, toolsTips, statusTips;

			for ( int j = 0; j < 8; j++ ) {
				bitNames << QString::fromStdString(it->m_bitNames[j]);
				toolsTips << QString::fromStdString(it->m_toolsTips[j]);
				statusTips << QString::fromStdString(it->m_statusTips[j]);
			}

			m_regDisplays[idx] = new RegDisplay(
				it->m_address,
				QString::fromStdString(it->m_regName),
				QString::fromStdString(it->m_regNameTip),
				it->m_mask,
				&bitNames,
				&toolsTips,
				&statusTips,
                                parentWidget);
		}

		connect(m_regDisplays[idx], SIGNAL(valueChanged(uint, uint)), this, SLOT(changeValue(uint, uint)));

	}

	deviceReset();

	for ( int i = 0; i < m_numberOfDisplays; i++ ) {
		if ( NULL != m_regDisplays[i] ) {
			m_regDisplays[i]->setHighlighted(false);
		}
	}
}

void RegDisplayList::changeValue(uint address, uint value) {
	m_memory->directWrite(address, value);
}

void RegDisplayList::deviceReset() {
	for ( int i = 0; i < m_numberOfDisplays; i++ ) {
		if ( NULL == m_regDisplays[i] ) {
			continue;
		}

		uint address = i + m_startingAddress;
		uint value;
		m_memory->directRead(address, value);

		m_regDisplays[i]->setValue(value);
	}
}

void RegDisplayList::setReadOnly(bool readOnly) {
	for ( int i = 0; i < m_numberOfDisplays; i++ ) {
		if ( NULL != m_regDisplays[i] ) {
			m_regDisplays[i]->setReadOnly(readOnly);

			if ( false == readOnly ) {
				m_regDisplays[i]->setHighlighted(false);
			}
		}
	}
}

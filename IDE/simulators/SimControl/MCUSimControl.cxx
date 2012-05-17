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

#include "MCUSimControl.h"

#include "MCUSimObserver.h"

#include "HexFile.h"

#include "AVR8Sim.h"
#include "AVR8ProgramMemory.h"

#include "McuSimCfgMgr.h"
#include "McuDeviceSpec.h"
#include "McuDeviceSpecAVR8.h"

#include <QDebug>

MCUSimControl::MCUSimControl(const char * deviceName) : m_simulator(NULL) {
	changeDevice(deviceName);
}

MCUSimControl::~MCUSimControl() {
	if ( NULL != m_simulator ) {
		delete m_simulator;
	}
}

void MCUSimControl::start(const char * hexFileName) {

	// Reset the simulator
	m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
	reset();

	// Load data file to the program memory
	HexFile hexFile;
	try {
		hexFile.clearAndLoad(hexFileName);
	} catch ( DataFile::DataFileException & e ) {
		// TODO: implement a proper error handling here
		qDebug("Failed to load program memory from the given IHEX file.");
		return;
	}
	switch ( m_architecture ) {
		case MCUSim::ARCH_AVR8:
			dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(&hexFile);
			break;
		default:
			// TODO: implement a proper error handling here
			qDebug("Unknown device architecture.");
			return;

	}

	m_simulatorLog->clear();

	allObservers_setReadOnly(false);
}

void MCUSimControl::stop() {
	allObservers_setReadOnly(true);
}

void MCUSimControl::step() {
	if ( NULL == m_simulator ) {
		return;
	}

	m_simulator->executeInstruction();
	dispatchEvents();
}

void MCUSimControl::stepOver() {
	qDebug("MCUSimControl::stepOver is not implemented yet!");
}

void MCUSimControl::animate() {
	qDebug("MCUSimControl::animate is not implemented yet!");
}

void MCUSimControl::run() {
	qDebug("MCUSimControl::run is not implemented yet!");
}

void MCUSimControl::reset() {
	if ( NULL == m_simulator ) {
		return;
	}

	m_simulator->reset(MCUSim::RSTMD_MCU_RESET);
	allObservers_deviceReset();
}

bool MCUSimControl::changeDevice(const char * deviceName) {
	if ( NULL != m_simulator ) {
		delete m_simulator;
	}

	m_deviceSpec = McuSimCfgMgr::getInstance()->getDeviceSpec(deviceName);
	if ( NULL == m_deviceSpec ) {
		qDebug("Failed to retrieve the device configuration specification.");
		return false;
	}

	m_architecture = m_deviceSpec->m_arch;

	switch ( m_architecture ) {
		case MCUSim::ARCH_AVR8:
			m_simulator = new AVR8Sim();
			break;
		default:
			qDebug("Unknown device architecture.");
			return false;

	}

	m_simulatorLog = m_simulator->getLog();

	McuSimCfgMgr::getInstance()->setupSimulator(deviceName, m_simulator->getConfig());
	m_simulator->reset(MCUSim::RSTMD_NEW_CONFIG);
	m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);

	allObservers_deviceChanged();
	return true;
}

const char * MCUSimControl::getDeviceName() const {
	if ( NULL == m_simulator ) {
		return "";
	} else {
		return m_deviceSpec->m_name.toStdString().c_str();
	}
}

MCUSim::Arch MCUSimControl::getArch() const {
	if ( NULL == m_simulator ) {
		return MCUSim::ARCH_INVALID;
	} else {
		return m_architecture;
	}
}

MCUSim::Subsys * MCUSimControl::getSimSubsys(MCUSim::Subsys::SubsysId id) {
	if ( NULL == m_simulator ) {
		return NULL;
	} else {
		return m_simulator->getSubsys(id);
	}
}

MCUSim * MCUSimControl::directAccess() {
	return m_simulator;
}

const McuDeviceSpec * MCUSimControl::getDeviceSpec() const {
	if ( NULL == m_simulator ) {
		return NULL;
	} else {
		return m_deviceSpec;
	}
}

bool MCUSimControl::initialized() const {
	return ( NULL != m_simulator );
}

void MCUSimControl::registerObserver(
	MCUSimObserver * observer,
	const MCUSim::Subsys::SubsysId simSubsysToObserve,
	const int subsysEventsToObserve)
{
	for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ ) {
		if ( simSubsysToObserve & i ) {
			unregisterSpecificObserver(MCUSim::Subsys::SubsysId(i), observer);
			m_observers[i].push_back(std::make_pair(observer, subsysEventsToObserve));
		}
	}

	observer->setControlUnit(this);
}

inline bool MCUSimControl::unregisterSpecificObserver(
	MCUSim::Subsys::SubsysId subsysId,
	MCUSimObserver * observer)
{
	int size = m_observers[subsysId].size();
	bool result = false;

	for ( int i = 0; i < size; i ++ ) {
		if ( m_observers[subsysId][i].first == observer ) {
			m_observers[subsysId].erase( m_observers[subsysId].begin() + i );
			result = true;
			size--;
		}
	}

	return result;
}

bool MCUSimControl::unregisterObserver(MCUSimObserver * observer) {
	bool result = false;

	for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ ) {
		result |= unregisterSpecificObserver(MCUSim::Subsys::SubsysId(i), observer);
	}

	return result;
}

void MCUSimControl::dispatchEvents() {
	int subsysId, eventId, locationOrReason, detail;

	while ( 0 != m_simulatorLog->getEvent(subsysId, eventId, locationOrReason, detail)) {
		if ( (subsysId >= MCUSim::Subsys::ID__MAX__) || (subsysId < 0) ) {
			qDebug("Invalid subsysId");
			continue;
		}

		std::vector<std::pair<MCUSimObserver*, int> >::iterator it;
		for ( it = m_observers[subsysId].begin(); it != m_observers[subsysId].end(); it++ ) {
			if ( eventId & it->second ) {
				it->first->handleEvent(subsysId, eventId, locationOrReason, detail);
			}
		}
	}
}

void MCUSimControl::allObservers_deviceChanged() {
	for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ ) {
		std::vector<std::pair<MCUSimObserver*, int> >::iterator it;
		for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ ) {
			it->first->deviceChanged();
		}
	}
}

void MCUSimControl::allObservers_deviceReset() {
	for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ ) {
		std::vector<std::pair<MCUSimObserver*, int> >::iterator it;
		for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ ) {
			it->first->deviceReset();
		}
	}
}

void MCUSimControl::allObservers_setReadOnly(bool readOnly) {
	for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ ) {
		std::vector<std::pair<MCUSimObserver*, int> >::iterator it;
		for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ ) {
			it->first->setReadOnly(readOnly);
		}
	}
}

bool MCUSimControl::getListOfSFR(std::vector<SFRRegDesc> & sfr) {
	if ( NULL == m_simulator ) {
		return false;
	}

	sfr.clear();

	switch ( m_architecture ) {
		case MCUSim::ARCH_AVR8: {
			const McuDeviceSpecAVR8 * devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_deviceSpec);

			for ( uint i = 0; i < devSpec->m_dataMemory.m_ioRegSize; i++ ) {
				if ( 0 == devSpec->m_dataMemory.m_ioRegDesc[i].m_name.size() ) {
					continue;
				}

				sfr.push_back(SFRRegDesc());

				sfr.back().m_address = i + devSpec->m_dataMemory.m_regFileSize;
				sfr.back().m_regName = devSpec->m_dataMemory.m_ioRegDesc[i].m_name;
				std::string m_regNameTip = devSpec->m_dataMemory.m_ioRegDesc[i].m_desc;
				sfr.back().m_mask = uint8_t (
					((devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff00) >> 8)
						|
					((devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff0000) >> 16) );

				for ( int j = 0; j < 8; j++ ) {
					sfr.back().m_bitNames[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_name;
					sfr.back().m_toolsTips[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_ttip;
					sfr.back().m_statusTips[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_stip;
				}
			}

			break;
		}
		default: {
			return false;
		}
	}

	return true;
}

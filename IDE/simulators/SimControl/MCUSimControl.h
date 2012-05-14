/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup SimControl
 * @file MCUSimControl.h
 */

#ifndef MCUSIMCONTROL_H
#define MCUSIMCONTROL_H

// Forward declarations
class MCUSimObserver;
class McuDeviceSpec;

#include "MCUSim.h"
#include <vector>
#include <string>
#include <utility>
#include <QObject>

class MCUSimControl : public QObject {
	Q_OBJECT
public:
	struct SFRRegDesc {
		int m_address;
		std::string m_regName;
		std::string m_regNameTip;
		std::string m_bitNames[8];
		std::string m_toolsTips[8];
		std::string m_statusTips[8];
		uint8_t m_mask;
	};

	MCUSimControl(const char * deviceName);
	virtual ~MCUSimControl();

	void registerObserver(
		MCUSimObserver * observer,
		const MCUSim::Subsys::SubsysId simSubsysToObserve,
		const int subsysEventsToObserve);
	bool unregisterObserver(MCUSimObserver * observer);

	bool initialized() const;
	const char * getDeviceName() const;
	MCUSim::Arch getArch() const;
	MCUSim::Subsys * getSimSubsys(MCUSim::Subsys::SubsysId id);
	MCUSim * directAccess();
	const McuDeviceSpec * getDeviceSpec() const;

	bool getListOfSFR(std::vector<SFRRegDesc> & sfr);

public slots:
	void start(const char * hexFileName);
	void stop();

	void step();
	void stepOver();
	void animate();
	void run();
	void reset();

	bool changeDevice(const char * deviceName);

private:
	MCUSimControl() {};

	MCUSim::Arch m_architecture;
	MCUSim * m_simulator;
	MCUSim::EventLogger * m_simulatorLog;
	const McuDeviceSpec * m_deviceSpec;

	std::vector<std::pair<MCUSimObserver*, int> > m_observers[MCUSim::Subsys::ID__MAX__];

	void dispatchEvents();
	inline bool unregisterSpecificObserver(MCUSim::Subsys::SubsysId subsysId, MCUSimObserver * observer);
	void allObservers_deviceChanged();
	void allObservers_deviceReset();
	void allObservers_setReadOnly(bool readOnly);
};

#endif // MCUSIMCONTROL_H

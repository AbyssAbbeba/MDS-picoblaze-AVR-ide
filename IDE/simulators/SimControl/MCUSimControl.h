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
class DbgFile;

#include "MCUSim.h"
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <QObject>

class MCUSimControl : public QObject {
	Q_OBJECT
public:
	enum CompilerID {
		COMPILER_NATIVE,
		COMPILER_SDCC
	};
	enum DataFileType {
		DBGFILEID_HEX
	};

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

	/// This method can be called multiple times to register an observer to multiple subsystems
	void registerObserver(
		MCUSimObserver * observer,
		MCUSim::Subsys::SubsysId simSubsysToObserve,
		const std::vector<int> & subsysEventsToObserve);
	void registerObserver(
		MCUSimObserver * observer,
		MCUSim::Subsys::SubsysId simSubsysToObserve,
		uint64_t events = 0xFFFFFFFFFFFFFFFFULL);
	bool unregisterObserver(MCUSimObserver * observer);

	void getLineNumber(int * lineNumber, std::string * filename);
	const DbgFile * getSourceInfo();

	bool initialized() const;
	const char * getDeviceName() const;
	MCUSim::Arch getArch() const;
	MCUSim::Subsys * getSimSubsys(MCUSim::Subsys::SubsysId id);
	MCUSim * directAccess();
	const McuDeviceSpec * getDeviceSpec() const;

	bool getListOfSFR(std::vector<SFRRegDesc> & sfr);

public slots:
	bool start(std::string & fileName, CompilerID compilerId, DataFileType dataFileType = DBGFILEID_HEX);
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
	DbgFile * m_dbgFile;
	MCUSim::CPU * m_simCpu;
	MCUSim::EventLogger * m_simulatorLog;
	const McuDeviceSpec * m_deviceSpec;

	std::vector<std::pair<MCUSimObserver*, uint64_t> > m_observers[MCUSim::Subsys::ID__MAX__];

	void dispatchEvents();
	void allObservers_deviceChanged();
	void allObservers_deviceReset();
	void allObservers_setReadOnly(bool readOnly);

	inline bool unregisterSpecificObserver(MCUSim::Subsys::SubsysId subsysId, MCUSimObserver * observer);
};

#endif // MCUSIMCONTROL_H

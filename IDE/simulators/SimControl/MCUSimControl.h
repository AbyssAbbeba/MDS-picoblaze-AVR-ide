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

class MCUSimObserver;

#include "MCUSim.h"

#include <QObject>

class MCUSimControl : public QObject {
	Q_OBJECT
public:
	MCUSimControl(const char * deviceName);
	virtual ~MCUSimControl();

	void registerObserver(MCUSim::Subsys::SubsysId simSubsysToObserve, MCUSimObserver * observer);
	void unregisterObserver(MCUSimObserver * observer);

	const char * getDeviceName() const;

public slots:
	void start(const char * workingDirectory, const char * baseFileName);
	void stop();

	void step();
	void stepOver();
	void animate();
	void run();
	void deviceReset();

	bool changeDevice(const char * deviceName);

private:
	MCUSimControl() {};
};

#endif // MCUSIMCONTROL_H

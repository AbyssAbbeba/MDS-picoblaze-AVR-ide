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
 * @file MCUSimObserver.h
 */

#ifndef MCUSIMOBSERVER_H
#define MCUSIMOBSERVER_H

class MCUSimControl;

#include "MCUSim.h"

#include <QObject>

class MCUSimObserver : public QObject {
	Q_OBJECT
public:
	MCUSimObserver();
	MCUSimObserver(MCUSimControl * managedBy);
	virtual ~MCUSimObserver();

	void setControlUnit(MCUSimControl * managedBy);

public slots:
	void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
	void deviceChanged();
	void deviceReset();
	void setReadOnly(bool readOnly);
};

#endif // MCUSIMOBSERVER_H

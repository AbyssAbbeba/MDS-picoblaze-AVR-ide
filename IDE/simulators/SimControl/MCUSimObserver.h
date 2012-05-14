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

// Forward declarations
class MCUSimControl;

class MCUSimObserver {
public:
	MCUSimObserver();
	virtual ~MCUSimObserver();

	void setControlUnit(MCUSimControl * managedBy);

	virtual void handleEvent(int subsysId, int eventId, int locationOrReason, int detail) = 0;
	virtual void deviceChanged() = 0;
	virtual void deviceReset() = 0;
	virtual void setReadOnly(bool readOnly) = 0;

protected:
	MCUSimControl * m_simControlUnit;
};

#endif // MCUSIMOBSERVER_H

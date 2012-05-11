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

#include "MCUSimObserver.h"

MCUSimObserver::MCUSimObserver() {
}
MCUSimObserver::MCUSimObserver(MCUSimControl * managedBy) {
}
MCUSimObserver::~MCUSimObserver() {
}

void MCUSimObserver::setControlUnit(MCUSimControl * managedBy) {
}

void MCUSimObserver::handleEvent(int subsysId, int eventId, int locationOrReason, int detail) {
}

void MCUSimObserver::deviceChanged() {
}
void MCUSimObserver::deviceReset() {
}
void MCUSimObserver::setReadOnly(bool readOnly) {
}

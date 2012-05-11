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

MCUSimControl::MCUSimControl(const char * deviceName) {
}
MCUSimControl::~MCUSimControl() {
}

void MCUSimControl::start(const char * workingDirectory, const char * baseFileName) {
}
void MCUSimControl::stop() {
}

void MCUSimControl::step() {
}
void MCUSimControl::stepOver() {
}
void MCUSimControl::animate() {
}
void MCUSimControl::run() {
}
void MCUSimControl::deviceReset() {
}

bool MCUSimControl::changeDevice(const char * deviceName) {
}

void MCUSimControl::registerObserver(MCUSim::Subsys::SubsysId simSubsysToObserve, MCUSimObserver * observer) {
}
void MCUSimControl::unregisterObserver(MCUSimObserver * observer) {
}
const char * MCUSimControl::getDeviceName() const {
}


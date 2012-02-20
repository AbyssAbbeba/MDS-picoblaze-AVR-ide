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

#include "AVR8Sim.h"

#include "AVR8InstructionSet.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8Fuses.h"
#include "AVR8InterruptController.h"

AVR8Sim::AVR8Sim() : MCUSim() {
}
MCUSim::Subsys * AVR8Sim::getSubsys(Subsys::SubsysId id) {
}
MCUSim::RetCode AVR8Sim::setConfig(Config * newConfig) {
}
MCUSim::EventLogger * AVR8Sim::getLog() {
}
MCUSim::Arch AVR8Sim::arch() {
}
MCUSim::Mode AVR8Sim::mode() {
}

/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup PIC8
 * @file PIC8Sim.h
 */

#ifndef PIC8SIM_H
#define PIC8SIM_H

// Forward declarations
class PIC8InstructionSet;
class PIC8Config;
class PIC8DataMemory;
class PIC8ProgramMemory;
class PIC8ConfigWord;
class PIC8IO;
class PIC8ClockControl;
class PIC8Stack;

#include "../MCUSim.h"

#include <vector>

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8Sim
 */
class PIC8Sim : public MCUSim {
    friend class PIC8Config;

public:
    PIC8Sim();
    ~PIC8Sim();

    Clock::ClockSource & getClockSource();

    void reset(ResetMode mode);
    float cycles2time(int numOfCycles) {
        return ( numOfCycles * m_clockPeriod );
    }
    int executeInstruction();
    int timeStep(float timeStep);

    Subsys * getSubsys(Subsys::SubsysId id);
    Config & getConfig();
    EventLogger * getLog() {
        return m_eventLogger;
    }

    Family family() const;
    Arch arch() const;
    Mode mode() const {
        return m_processorMode;
    }
    const char * name() const;

protected:
    PIC8Config * m_config;

    PIC8InstructionSet * m_instructionSet;
    PIC8ProgramMemory * m_programMemory;
    PIC8DataMemory * m_dataMemory;
    PIC8ConfigWord * m_configWord;
    PIC8IO * m_io;
    PIC8ClockControl * m_clockControl;
    PIC8Stack * m_stack;

    EventLogger * m_eventLogger;

    Mode m_processorMode;

    float m_clockPeriod;

    float m_time;
    int m_clockCycles;

private:
    std::vector<Subsys*> m_subSystems;

    inline void deleteSubSystems();
    inline void checkSubSystems() const;
    inline void regSubSys(Subsys * subSystem);

    inline void resetToInitialValues();
    inline void loadConfig();
    inline void mcuReset();
};

#endif // PIC8SIM_H

#ifndef CORE_H
#define CORE_H

#include <QtGui>


class Core
{
    struct current
    {
        unsigned long long nanoSecs;
        unsigned long long clockCycles;
        unsigned long long instructions;
        unsigned long long programBytes;
        unsigned long long interrupts;
        unsigned long long subPrograms;
        unsigned long long returns;
        unsigned long long interruptReturns;
        unsigned long long breakpoints;
    } cntCurrent, cntStop, cntOverAll;

public:
    Core();

    // core functions defining stop watch object and variables
    void addNanoSec();
    void addClockCycle();
    void addInstrction();
    void addProgramByte();
    void addInterrupt();
    void addSubProg();
    void addReturn();
    void addInterruptReturn();
    void addBreakPoint();
    bool isCoreStoped();

    // functions
    void clearCurrent();
    void clearStop();
    void shutDown(bool command)
    {
        shutDownFlag = command;
    }

    bool isStoped()
    {
        return stopFlag;
    }
    bool isStopChecked()
    {
        return stopCheckBox;
    }
    bool getShutDownStatus()
    {
        return shutDownFlag;
    }
    //current getStruct();
    unsigned long long getData(int);

    //
    struct current * structPtrCurrent;
    struct current * structPtrStop;
    struct current * structPtrOverall;

private:
    bool    stopFlag;
    bool    stopCheckBox;
    bool    shutDownFlag;
};

#endif // CORE_H

#include "core.h"
#include <QtGui>
Core::Core()
{
    stopFlag = false;
    shutDownFlag = false;

    // pointers to structures
    structPtrCurrent = &cntCurrent;
    structPtrStop = &cntStop;
    structPtrOverall = &cntOverAll;

    cntCurrent.nanoSecs = 0;
    cntCurrent.clockCycles = 0;
    cntCurrent.instructions = 0;
    cntCurrent.programBytes = 0;
    cntCurrent.interrupts = 0;
    cntCurrent.subPrograms = 0;
    cntCurrent.returns = 0;
    cntCurrent.interruptReturns = 0;
    cntCurrent.breakpoints = 0;

    cntStop.nanoSecs = 0;
    cntStop.clockCycles = 0;
    cntStop.instructions = 0;
    cntStop.programBytes = 0;
    cntStop.interrupts = 0;
    cntStop.subPrograms = 0;
    cntStop.returns = 0;
    cntStop.interruptReturns = 0;
    cntStop.breakpoints = 0;

    cntOverAll.nanoSecs = 0;
    cntOverAll.clockCycles = 0;
    cntOverAll.instructions = 0;
    cntOverAll.programBytes = 0;
    cntOverAll.interrupts = 0;
    cntOverAll.subPrograms = 0;
    cntOverAll.returns = 0;
    cntOverAll.interruptReturns = 0;
    cntOverAll.breakpoints = 0;
}

bool Core::isCoreStoped()
{
    if ( cntCurrent.nanoSecs == cntStop.nanoSecs  && cntStop.nanoSecs != 0)                  return true;
    if ( cntCurrent.clockCycles == cntStop.clockCycles && cntStop.clockCycles!= 0 )            return true;
    if ( cntCurrent.instructions == cntStop.instructions && cntStop.instructions!= 0 )          return true;
    if ( cntCurrent.programBytes == cntStop.programBytes && cntStop.programBytes!= 0 )          return true;
    if ( cntCurrent.interrupts == cntStop.interrupts && cntStop.interrupts != 0 )              return true;
    if ( cntCurrent.subPrograms == cntStop.subPrograms && cntStop.subPrograms!= 0 )            return true;
    if ( cntCurrent.returns == cntStop.returns && cntStop.returns != 0 )                    return true;
    if ( cntCurrent.interruptReturns == cntStop.interruptReturns && cntStop.interruptReturns != 0 )  return true;  
    if ( cntCurrent.breakpoints == cntStop.breakpoints && cntStop.breakpoints != 0 )        return true;
    return false;
}

void Core::addNanoSec()
{
    if ( getShutDownStatus()  == true )
        return;

    cntCurrent.nanoSecs += 1;
    cntOverAll.nanoSecs += 1;

    if ( isCoreStoped() == true)
        shutDown(false);
}
void Core::addClockCycle()
{
    if ( getShutDownStatus()  == true )
        return;

    cntCurrent.clockCycles+= 1;
    cntOverAll.clockCycles += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addInstrction()
{
    if ( getShutDownStatus()  == true)
        return;

    cntCurrent.instructions += 1;
    cntOverAll.instructions += 1;
    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addProgramByte()
{
    if ( getShutDownStatus()  == true)
        return;

    cntCurrent.programBytes += 1;
    cntOverAll.programBytes += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addInterrupt()
{
    if ( getShutDownStatus()  == true )
        return;

    cntCurrent.interrupts += 1;
    cntOverAll.interrupts += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addSubProg()
{
    if ( getShutDownStatus()  == true )
        return;

    cntCurrent.subPrograms += 1;
    cntOverAll.subPrograms += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addReturn()
{
    if ( getShutDownStatus()  == true )
        return;

    cntCurrent.returns+= 1;
    cntOverAll.returns += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addInterruptReturn()
{
    if ( getShutDownStatus() == true )
        return;

    cntCurrent.interruptReturns += 1;
    cntOverAll.interruptReturns+= 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
void Core::addBreakPoint()
{
    if ( getShutDownStatus() == true )
        return;

    cntCurrent.breakpoints += 1;
    cntOverAll.breakpoints += 1;

    if ( isCoreStoped() == true)
        shutDown(true);
}
//Core::current Core::getStruct()
//{
 //   structPtr = &cntCurrent;
 //   return cntCurrent;
//
unsigned long long Core::getData(int what)
{
    switch ( what )
    {
        case 0:
        return cntCurrent.nanoSecs;

        case 1:
        return cntOverAll.nanoSecs;

        case 2:
        return cntCurrent.clockCycles;

        case 3:
        return cntOverAll.clockCycles;

        case 4:
        return cntCurrent.instructions;

        case 5:
        return cntOverAll.instructions;

        case 6:
        return cntCurrent.programBytes;

        case 7:
        return cntOverAll.programBytes;

        case 8:
        return cntCurrent.interrupts;

        case 9:
        return cntOverAll.interrupts;

        case 10:
        return cntCurrent.subPrograms;

        case 11:
        return cntOverAll.subPrograms;

        case 12:
        return cntCurrent.returns;

        case 13:
        return cntOverAll.returns;

        case 14:
        return cntCurrent.interruptReturns;

        case 15:
        return cntOverAll.interruptReturns;

        case 16:
        return cntCurrent.breakpoints;

        case 17:
        return cntOverAll.breakpoints;
    }
}

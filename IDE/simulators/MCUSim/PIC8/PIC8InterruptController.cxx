// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup PIC8
 * @file PIC8InterruptController.cxx
 */
// =============================================================================

#include "PIC8InterruptController.h"

#include "PIC8DataMemory.h"
#include "PIC8InstructionSet.h"
#include "PIC8Stack.h"

PIC8InterruptController::PIC8InterruptController()
{
}

PIC8InterruptController::~PIC8InterruptController()
{
}

PIC8InterruptController * PIC8InterruptController::link ( MCUSim::EventLogger * eventLogger,
                                              MCUSim::Mode        * processorMode,
                                              PIC8DataMemory      * dataMemory,
                                              PIC8InstructionSet  * instructionSet,
                                              PIC8Stack           * stack )
{
    Subsys::link(eventLogger, ID_INTERRUPTS);
    m_processorMode = processorMode;
    m_dataMemory = dataMemory;
    m_instructionSet = instructionSet;
    m_stack = stack;
    return this;
}


void PIC8InterruptController::reset ( MCUSim::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8InterruptController::mcuReset()
{
    m_actInterruptCounter = 0;
    m_interruptToExecute = INTVEC_NONE;

    for ( int i = 0; i < INTVEC__MAX__; i++ )
    {
        m_interruptCounter[i] = 0;
        m_intReqWithoutFlag[i] = false;
    }
}

bool PIC8InterruptController::retfie()
{
    if ( 0 != m_actInterruptCounter )
    {
        // Set Global Interrupt Enable Flag in INTCON register
        m_dataMemory->setBitFast(PIC8RegNames::INTCON, PIC8RegNames::INTCON_GIE);

        m_actInterruptCounter--;
        logEvent(EVENT_INT_LEAVING_INTERRUPT, m_instructionSet->getProgramCounter());

        return true;
    }
    else
    {
        return false;
    }
}

int PIC8InterruptController::autoInterrupt()
{
    // Treat RESET as a special case
    if ( ( true == m_intReqWithoutFlag[INTVEC_RESET] )
             &&
         ( true == m_config.m_possibleInterrupts[INTVEC_RESET] ) )
    {
        m_intReqWithoutFlag[INTVEC_RESET] = false;
        return -1;
    }

    // Check whether interrupts are enabled at system level (flag GIE in INTCON register)
    if ( false == m_dataMemory->readBitFast(PIC8RegNames::INTCON, PIC8RegNames::INTCON_GIE) )
    {
        m_interruptToExecute = INTVEC_NONE;
        return 0;
    }

    if ( ( INTVEC_NONE != m_interruptToExecute ) && ( MCUSim::MD_SLEEP != *m_processorMode ) )
    {
        return executeInterrupt(m_interruptToExecute);
    }

    m_interruptToExecute = detectPendingInterrupt();

    if ( INTVEC_NONE != m_interruptToExecute )
    {
        if ( MCUSim::MD_SLEEP == *m_processorMode )
        {
            return 1;
        }
        else
        {
            logEvent(EVENT_INT_ENTERING_INTERRUPT, m_instructionSet->getProgramCounter(), m_interruptToExecute);
        }
    }

    return 0;
}

void PIC8InterruptController::genIntReq ( InterruptVector interrupt )
{
    m_intReqWithoutFlag[interrupt] = true;
}

inline PIC8InterruptController::InterruptVector PIC8InterruptController::detectPendingInterrupt()
{
    // Read INTCON register
    register unsigned int intcon = m_dataMemory->readFast(PIC8RegNames::INTCON);

    // External Interrupt Request 0
    if (
         // Does the interrupt exist?
         ( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
             &&
         // Is it enabled?
         ( intcon & PIC8RegNames::INTCON_INTE )
             &&
         // Is it requested?
         ( intcon & PIC8RegNames::INTCON_INTF )
       )
    {
        return INTVEC_INT0;
    }

    // TMR0 overflow interrupt
    if (
         // Does the interrupt exist?
         ( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
             &&
         // Is it enabled?
         ( intcon & PIC8RegNames::INTCON_T0IE )
             &&
         // Is it requested?
         ( intcon & PIC8RegNames::INTCON_T0IF )
       )
    {
        return INTVEC_TMR0;
    }

    // PORTB change interrupts (pins RB7:RB4)
    if (
         // Does the interrupt exist?
         ( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
             &&
         // Is it enabled?
         ( intcon & PIC8RegNames::INTCON_RBIE )
             &&
         // Is it requested?
         ( intcon & PIC8RegNames::INTCON_RBIF )
       )
    {
        return INTVEC_PORTB;
    }

    // Data EEPROM write complete interrupt
    if (
         // Does the interrupt exist?
         ( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
             &&
         // Is it enabled?
         ( intcon & PIC8RegNames::INTCON_EEIE )
             &&
         // Is it requested?
         ( true == m_dataMemory->readBitFast(PIC8RegNames::EECON1, PIC8RegNames::EECON1_EEIF) )
       )
    {
        return INTVEC_EEPROM;
    }

    return INTVEC_NONE;
}

inline int PIC8InterruptController::executeInterrupt ( InterruptVector vector )
{
    m_interruptToExecute = INTVEC_NONE;

    // Clear Global Interrupt Enable Flag in INTCON register
    m_dataMemory->clearBitFast(PIC8RegNames::INTCON, PIC8RegNames::INTCON_GIE);

    // Increment counter of active interrupts
    m_actInterruptCounter++;

    // Push current PC value on the stack
    m_stack->pushOnStack(m_instructionSet->getProgramCounter());

    // Jump to the destination address (interrupt vector)
    m_instructionSet->setProgramCounter(INTERRUPT_VECTOR);

    // Increment statistical counter
    m_interruptCounter[vector]++;

    return 3;
}

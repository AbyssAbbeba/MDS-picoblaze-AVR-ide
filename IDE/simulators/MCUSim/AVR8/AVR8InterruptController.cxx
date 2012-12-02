// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2012 Moravia Microsystems, s.r.o.
 *
 * @authors Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup AVR8
 * @file AVR8InterruptController.cxx
 */
// =============================================================================

#include "AVR8InterruptController.h"
#include "AVR8InstructionSet.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8FusesAndLocks.h"
#include "AVR8Sim.h"
#include "AVR8BootLoader.h"

AVR8InterruptController * AVR8InterruptController::link ( MCUSim::EventLogger     * eventLogger,
                                                          AVR8InstructionSet      * instructionSet,
                                                          AVR8ProgramMemory       * programMemory,
                                                          AVR8DataMemory          * dataMemory,
                                                          AVR8FusesAndLocks       * fusesAndLocks,
                                                          AVR8Sim::SleepMode      * sleepMode,
                                                          AVR8BootLoader          * bootLoader )
{
    Subsys::link(eventLogger, ID_INTERRUPTS);

    m_instructionSet        = instructionSet;
    m_programMemory         = programMemory;
    m_dataMemory            = dataMemory;
    m_fusesAndLocks         = fusesAndLocks;
    m_sleepMode             = sleepMode;
    m_bootLoader            = bootLoader;

    return this;
}

void AVR8InterruptController::reti()
{
    // Set Global Interrupt Enable Flag in SREG
    m_dataMemory->setBitFast(AVR8RegNames::SREG, AVR8RegNames::SREG_I);

    m_actInterruptCounter--;
    logEvent(EVENT_INT_LEAVING_INTERRUPT, m_instructionSet->getProgramCounter());
}

int AVR8InterruptController::autoInterrupt()
{
    // Treat RESET as a special cas
    if ( ( true == m_intReqWithoutFlag[INTVEC_RESET] )
             &&
         ( true == m_config.m_possibleInterrupts[INTVEC_RESET] ) )
    {
        m_intReqWithoutFlag[INTVEC_RESET] = false;
        return -1;
    }

    if ( false == m_dataMemory->readBitFast(AVR8RegNames::SREG, AVR8RegNames::SREG_I) )
    {
        m_interruptToExecute = INTVEC_NONE;
        return 0;
    }

    if ( INTVEC_NONE != m_interruptToExecute )
    {
        return executeInterrupt(m_interruptToExecute);
    }

    m_interruptToExecute = detectPendingInterrupt();

    if ( INTVEC_NONE != m_interruptToExecute )
    {
        logEvent(EVENT_INT_ENTERING_INTERRUPT, m_instructionSet->getProgramCounter(), m_interruptToExecute);
    }

    return 0;
}

void AVR8InterruptController::genIntReq ( AVR8InterruptController::InterruptVector interrupt )
{
    m_intReqWithoutFlag[interrupt] = true;
}

inline int AVR8InterruptController::executeInterrupt ( AVR8InterruptController::InterruptVector vector )
{
    m_interruptToExecute = INTVEC_NONE;

    if ( false == confirmInterrupt(vector) )
    {
        logEvent(EVENT_INT_INTERRUPT_CANCELED, m_instructionSet->getProgramCounter(), m_interruptToExecute);
        return 0;
    }

    // Clear Global Interrupt Enable Flag in SREG
    m_dataMemory->clearBitFast(AVR8RegNames::SREG, AVR8RegNames::SREG_I);

    // Clear the interrupt flag, if it's required
    if ( -1 != m_interruptFlagToClear[0] )
    {
        m_dataMemory->clearBitFast(m_interruptFlagToClear[0], m_interruptFlagToClear[1]);
        m_interruptFlagToClear[0] = -1;
    }

    // Increment counter of active interrupts
    m_actInterruptCounter++;

    // Push current PC value on the stack
    unsigned int pc = m_instructionSet->getProgramCounter();
    m_dataMemory->pushOnStack(pc & 0xff);
    pc >>= 8;
    m_dataMemory->pushOnStack(pc & 0xff);

    // Jump to the destination address (interrupt vector)
    unsigned int destinationAddress = (unsigned int)vector;
    if ( true == m_dataMemory->readBitFast(AVR8RegNames::GICR, AVR8RegNames::GICR_IVSEL) )
    {
        destinationAddress += m_bootLoader->getBootAddress();
    }
    m_instructionSet->setProgramCounter(destinationAddress);

    // Increment statistical counter
    m_interruptCounter[vector]++;

    if ( AVR8Sim::SLEEPMD_NONE != *m_sleepMode )
    {
        /*
         * If an interrupt occurs when the MCU is in sleep mode, the interrupt execution response
         * time is increased by four clock cycles. This increase comes in addition to the start-up
         * time from the selected sleep mode.
         */
        return 8; // <-- This is probably specific to AVR with 16b PC, e.g. ATmega8A
    }
    else
    {
        return 4; // <-- This is probably specific to AVR with 16b PC, e.g. ATmega8A
    }
}

inline AVR8InterruptController::InterruptVector AVR8InterruptController::detectPendingInterrupt()
{
    register unsigned int reg0; // This one is used much more often
    unsigned int reg1;

    m_interruptFlagToClear[0] = -1;

    if ( true == m_config.m_interruptGroups[INTGRP_EXTERNAL] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::GICR);
        reg1 = m_dataMemory->readFast(AVR8RegNames::GIFR);

        // External Interrupt Request 0
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_INT0] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::GICR_INT0 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::GIFR_INTF0 )
           )
        {
            /*
             * When an event on the INT0 pin triggers an interrupt request, INTF0 becomes set (one).
             * If the I bit in SREG and the INT0 bit in GICR are set (one), the MCU will jump to the
             * corresponding Interrupt Vector. The flag is cleared when the interrupt routine is
             * executed. Alternatively, the flag can be cleared by writing a logical one to it.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::GIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::GIFR_INTF0;
            return INTVEC_INT0;
        }

        // External Interrupt Request 1
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_INT1] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::GICR_INT1 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::GIFR_INTF1 )
           )
        {
            /*
             *(Same as the "External Interrupt Request 0".)
             */
            m_interruptFlagToClear[0] = AVR8RegNames::GIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::GIFR_INTF1;
            return INTVEC_INT1;
        }
    }

    if ( true == m_config.m_interruptGroups[INTGRP_TIMER012] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::TIMSK);
        reg1 = m_dataMemory->readFast(AVR8RegNames::TIFR);

        // Timer/Counter2 Compare Match
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER2_COMP] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_OCIE2 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_OCF2 )
           )
        {
            /*
             * OCF2 is cleared by hardware when executing the corresponding interrupt Handling Vector.
             * Alternatively, OCF2 is cleared by writing a logic one to
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_OCF2;
            return INTVEC_TIMER2_COMP;
        }

        // Timer/Counter2 Overflow
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER2_OVF] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_TOIE2 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_TOV2 )
           )
        {
            /*
             * TOV2 is cleared by hardware when executing the corresponding interrupt Handling Vector.
             * Alternatively, TOV2 is cleared by writing a logic one to the flag.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_TOV2;
            return INTVEC_TIMER2_OVF;
        }

        // Timer/Counter1 Capture Event
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_CAPT] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_TICIE1 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_ICF1 )
           )
        {
            /*
             * ICF1 is automatically cleared when the Input Capture Interrupt Vector is executed.
             * Alternatively, ICF1 can be cleared by writing a logic one to its bit location.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_ICF1;
            return INTVEC_TIMER1_CAPT;
        }

        // Timer/Counter1 Compare Match A
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_COMPA] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_OCIE1A )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_OCF1A )
           )
        {
            /*
             * OCF1A is automatically cleared when the Output Compare Match A Interrupt Vector is executed.
             * Alternatively, OCF1A can be cleared by writing a logic one to its bit location.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_OCF1A;
            return INTVEC_TIMER1_COMPA;
        }

        // Timer/Counter1 Compare Match B
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_COMPB] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_OCIE1B )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_OCF1B )
           )
        {
            /*
             * OCF1B is automatically cleared when the Output Compare Match B Interrupt Vector is executed.
             * Alternatively, OCF1B can be cleared by writing a logic one to its bit location.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_OCF1B;
            return INTVEC_TIMER1_COMPB;
        }

        // Timer/Counter1 Overflow
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER1_OVF] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_TOIE1 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_TOV1 )
           )
        {
            /*
             * TOV1 is automatically cleared when the Timer/Counter1 Overflow Interrupt Vector is executed.
             * Alternatively, TOV1 can be cleared by writing a logic one to its bit location.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_TOV1;
            return INTVEC_TIMER1_OVF;
        }

        // Timer/Counter0 Overflow
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_TIMER0_OVF] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::TIMSK_TOIE0 )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::TIFR_TOV0 )
           )
        {
            /*
             * The bit TOV0 is set (one) when an overflow occurs in Timer/Counter0. TOV0 is
             * cleared by hardware when executing the corresponding interrupt Handling Vector.
             * Alternatively, TOV0 is cleared by writing a logic one to the flag. When the
             * SREG I-bit, TOIE0 (Timer/Counter0 Overflow Interrupt Enable), and TOV0 are set
             * (one), the Timer/Counter0 Overflow interrupt is executed.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::TIFR;
            m_interruptFlagToClear[1] = AVR8RegNames::TIFR_TOV0;
            return INTVEC_TIMER0_OVF;
        }
    }

    // Serial Transfer Complete
    if (
         // Does the interrupt exist?
         ( true == m_config.m_possibleInterrupts[INTVEC_SPI_STC] )
             &&
         // Is it enabled?
         ( true == m_dataMemory->readBitFast(AVR8RegNames::SPCR, AVR8RegNames::SPCR_SPIE) )
             &&
         // Is it requested?
         ( true == m_dataMemory->readBitFast(AVR8RegNames::SPSR, AVR8RegNames::SPSR_SPIF) )
       )
    {
        /*
         * An interrupt is generated if SPIE in SPCR is set and global interrupts are enabled.SPIF is
         * cleared by hardware when executing the corresponding interrupt Handling Vector. Alternatively,
         * the SPIF bit is cleared by first reading the SPI Status Register with SPIF set, then
         * accessing the SPI Data Register (SPDR).
         */
        m_interruptFlagToClear[0] = AVR8RegNames::SPSR;
        m_interruptFlagToClear[1] = AVR8RegNames::SPSR_SPIF;
        return INTVEC_SPI_STC;
    }

    if ( true == m_config.m_interruptGroups[INTGRP_UART] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::UCSRB);
        reg1 = m_dataMemory->readFast(AVR8RegNames::UCSRA);

        // USART, Rx Complete
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_USART_RXC] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::UCSRB_RXCIE )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::UCSRA_RXC )
           )
        {
            /*
             * A USART Receive Complete interrupt will be generated only if the RXCIE bit is written to one,
             * the Global Interrupt Flag in SREG is written to one and the RXC bit in UCSRA is set.
             */
            return INTVEC_USART_RXC;
        }

        // USART Data Register Empty
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_USART_UDRE] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::UCSRB_UDRIE )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::UCSRA_UDRE )
           )
        {
            /*
             * A Data Register Empty interrupt will be generated only if the UDRIE bit is written to one,
             * the Global Interrupt Flag in SREG is written to one and the UDRE bit in UCSRA is set.
             */
            return INTVEC_USART_UDRE;
        }

        // Tx Complete
        if (
             // Does the interrupt exist?
             ( true == m_config.m_possibleInterrupts[INTVEC_USART_TXC] )
                 &&
             // Is it enabled?
             ( reg0 & AVR8RegNames::UCSRB_TXCIE )
                 &&
             // Is it requested?
             ( reg1 & AVR8RegNames::UCSRA_TXC )
           )
        {
            /*
             * The TXC Flag bit is automatically cleared when a transmit complete interrupt is executed,
             * or it can be cleared by writing a one to its bit location.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::UCSRA;
            m_interruptFlagToClear[1] = AVR8RegNames::UCSRA_TXC;
            return INTVEC_USART_TXC;
        }
    }

    // ADC Conversion Complete
    if ( true == m_config.m_possibleInterrupts[INTVEC_ADC] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::ADCSRA);

        if (
             // Is it enabled?
             ( reg0 & AVR8RegNames::ADCSRA_ADIE )
                 &&
             // Is it requested?
             ( reg0 & AVR8RegNames::ADCSRA_ADIF )
           )
        {
            /*
             * ADIF is cleared by hardware when executing the corresponding interrupt Handling Vector.
             * Alternatively, ADIF is cleared by writing a logical one to the flag.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::ADCSRA;
            m_interruptFlagToClear[1] = AVR8RegNames::ADCSRA_ADIF;
            return INTVEC_ADC;
        }
    }

    // EEPROM Ready
    if ( true == m_config.m_possibleInterrupts[INTVEC_EE_RDY] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::EECR);

        if (
             // Is it enabled?
             ( reg0 & AVR8RegNames::EECR_EERIE )
                 &&
             // Is it requested?
             ( true == m_intReqWithoutFlag[INTVEC_EE_RDY] )
           )
        {
            /*
             * The EEPROM Ready interrupt generates a constant interrupt when EEWE is cleared.
             */
            return INTVEC_EE_RDY;
        }
    }

    // Analog Comparator
    if ( true == m_config.m_possibleInterrupts[INTVEC_ANA_COMP] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::ACSR);

        if (
             // Is it enabled?
             ( reg0 & AVR8RegNames::ACSR_ACIE )
                 &&
             // Is it requested?
             ( reg0 & AVR8RegNames::ACSR_ACI )
           )
        {
            /*
             * ACI is cleared by hardware when executing the corresponding interrupt Handling Vector.
             * Alternatively, ACI is cleared by writing a logic one to the flag.
             */
            m_interruptFlagToClear[0] = AVR8RegNames::ACSR;
            m_interruptFlagToClear[1] = AVR8RegNames::ACSR_ACI;
            return INTVEC_ANA_COMP;
        }
    }

    //  Two-wire Serial Interface
    if ( true == m_config.m_possibleInterrupts[INTVEC_TWI] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::TWCR);

        if (
             // Is it enabled?
             ( reg0 & AVR8RegNames::TWCR_TWIE )
                 &&
             // Is it requested?
             ( reg0 & AVR8RegNames::TWCR_TWINT )
           )
        {
            /*
             * The TWI interrupt request will be activated for as long as the TWINT Flag is high.
             */
            return INTVEC_TWI;
        }
    }

    //  Store Program Memory Ready
    if ( true == m_config.m_possibleInterrupts[INTVEC_SPM_RDY] )
    {
        reg0 = m_dataMemory->readFast(AVR8RegNames::SPMCR);

        if (
             // Is it enabled?
             ( reg0 & AVR8RegNames::SPMCR_SPMIE )
                 &&
             // Is it requested?
             ( 0 == ( reg0 & AVR8RegNames::SPMCR_SPMEN ) )
           )
        {
            /*
             * The SPM ready Interrupt will be executed as long as the SPMEN bit in the SPMCR Register is cleared.
             */
            return INTVEC_SPM_RDY;
        }
    }

    return INTVEC_NONE;
}

inline bool AVR8InterruptController::confirmInterrupt ( AVR8InterruptController::InterruptVector vector )
{
    /*
     * If Interrupt Vectors are placed in the Boot Loader section, interrupts are disabled
     * while executing from the Application section.
     */
    if (
         (true == m_dataMemory->readBitFast(AVR8RegNames::GICR, AVR8RegNames::GICR_IVSEL))
             &&
         (
             (true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB01])
                 ||
             (true == (*m_fusesAndLocks)[AVR8FusesAndLocks::LB_BLB12])
         )
       )
    {
        return false;
    }

    switch ( vector )
    {
        case INTVEC_INT0:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::GICR, AVR8RegNames::GICR_INT0) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::GIFR, AVR8RegNames::GIFR_INTF0) ) );

        case INTVEC_INT1:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::GICR, AVR8RegNames::GICR_INT1) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::GIFR, AVR8RegNames::GIFR_INTF1) ) );

        case INTVEC_TIMER2_COMP:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE2) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF2) ) );

        case INTVEC_TIMER2_OVF:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE2) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV2) ) );

        case INTVEC_TIMER1_CAPT:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TICIE1) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_ICF1) ) );

        case INTVEC_TIMER1_COMPA:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE1A) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF1A) ) );

        case INTVEC_TIMER1_COMPB:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_OCIE1B) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_OCF1B) ) );

        case INTVEC_TIMER1_OVF:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE1) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV1) ) );

        case INTVEC_TIMER0_OVF:
            return (
                ( true == m_dataMemory->readBitFast(AVR8RegNames::TIMSK, AVR8RegNames::TIMSK_TOIE0) )
                    &&
                ( true == m_dataMemory->readBitFast(AVR8RegNames::TIFR, AVR8RegNames::TIFR_TOV0) )
            );

        case INTVEC_SPI_STC:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::SPCR, AVR8RegNames::SPCR_SPIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::SPSR, AVR8RegNames::SPSR_SPIF) ) );

        case INTVEC_USART_RXC:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRB, AVR8RegNames::UCSRB_RXCIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRA, AVR8RegNames::UCSRA_RXC) ) );

        case INTVEC_USART_UDRE:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRB, AVR8RegNames::UCSRB_UDRIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRA, AVR8RegNames::UCSRA_UDRE) ) );

        case INTVEC_USART_TXC:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRB, AVR8RegNames::UCSRB_TXCIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::UCSRA, AVR8RegNames::UCSRA_TXC) ) );

        case INTVEC_ADC:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::ADCSRA, AVR8RegNames::ADCSRA_ADIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::ADCSRA, AVR8RegNames::ADCSRA_ADIF) ) );

        case INTVEC_EE_RDY:
            if ( ( true == m_dataMemory->readBitFast(AVR8RegNames::EECR, AVR8RegNames::EECR_EERIE) )
                     &&
                 ( true == m_intReqWithoutFlag[INTVEC_EE_RDY] ) )
            {
                m_intReqWithoutFlag[INTVEC_EE_RDY] = false;
                return true;
            }

        case INTVEC_ANA_COMP:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::ACSR, AVR8RegNames::ACSR_ACIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::ACSR, AVR8RegNames::ACSR_ACI) ) );

        case INTVEC_TWI:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::TWCR, AVR8RegNames::TWCR_TWIE) )
                         &&
                     ( true == m_dataMemory->readBitFast(AVR8RegNames::TWCR, AVR8RegNames::TWCR_TWINT) ) );

        case INTVEC_SPM_RDY:
            return ( ( true == m_dataMemory->readBitFast(AVR8RegNames::SPMCR, AVR8RegNames::SPMCR_SPMIE) )
                         &&
                     ( false == m_dataMemory->readBitFast(AVR8RegNames::SPMCR, AVR8RegNames::SPMCR_SPMEN) ) );

        default:
            return false;
    }
}

void AVR8InterruptController::reset ( MCUSim::ResetMode mode )
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

inline void AVR8InterruptController::mcuReset()
{
    m_actInterruptCounter = 0;
    m_interruptToExecute = INTVEC_NONE;
    m_interruptFlagToClear[0] = -1;
    m_interruptFlagToClear[1] = -1;

    for ( int i = 0; i < INTVEC__MAX__; i++ )
    {
        m_interruptCounter[i] = 0;
        m_intReqWithoutFlag[i] = false;
    }
}

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PIC8
 * @file PIC8ISP.cxx
 */
// =============================================================================

#include "PIC8ISP.h"

#include "PIC8ProgramMemory.h"
#include "PIC8IO.h"
#include "PIC8ConfigWord.h"
#include "PIC8DataEEPROM.h"

PIC8ISP * PIC8ISP::link ( MCUSimEventLogger * eventLogger,
                          PIC8ProgramMemory   * programMemory,
                          PIC8DataEEPROM      * dataEEPROM,
                          PIC8IO              * io,
                          PIC8ConfigWord      * configWord )
{
    MCUSimSubsys::link(eventLogger, ID_ISP);
    m_programMemory = programMemory;
    m_dataEEPROM = dataEEPROM;
    m_io = io;
    m_configWord = configWord;
    return this;
}

PIC8ISP::PIC8ISP()
{
}

PIC8ISP::~PIC8ISP()
{
}

void PIC8ISP::reset ( MCUSimBase::ResetMode mode )
{
    switch ( mode )
    {
        case MCUSim::RSTMD_NEW_CONFIG:
            loadConfig();
            break;
        case MCUSim::RSTMD_MCU_RESET:
            mcuReset();
            break;
        default:
            // Irrelevant requests are silently ignored
            break;
    }
}

inline void PIC8ISP::loadConfig()
{
}

inline void PIC8ISP::mcuReset()
{
    m_clk = false;
    m_position = 0;
    m_address = 0;
    m_delay = 0;
    m_lastCommand = COMMAND_NONE;

    clearInBuffer();
    clearOutBuffer();

    m_bufferForDataMem = -1;
    m_bufferForProgMem = -1;
}

void PIC8ISP::timeStep ( float timeStep )
{
    if ( false == m_config.m_enabled )
    {
        return;
    }

    bool clk = m_io->getLog(PIC8PinNames::SPF_CLOCK);

    if ( m_delay > 0 )
    {
        m_delay -= timeStep;
    }
    else
    {
        m_delay = 0;
        if ( 0 == m_position )
        {
            logEvent(EVENT_ISP_OPERATION_COMPLETED, 0, m_lastCommand);
        }
    }

    if ( m_clk != clk )
    {
        if ( m_delay > 0 )
        {
            m_delay = 0;
            logEvent(EVENT_ISP_OPERATION_NOT_COMPLETED_YET);
        }

        if ( ( false == m_clk ) && ( true == clk ) )
        {
            // Rising edge
            m_io->setLog(PIC8PinNames::SPF_DATA, transmitBit());
        }
        else
        {
            // Falling edge
            receiveBit(m_io->getLog(PIC8PinNames::SPF_DATA));
        }

        incrPosition();
    }

    m_clk = clk;
}

inline void PIC8ISP::incrPosition()
{
    m_position++;
    m_position %= SEQUENCE_LENGTH;
}

inline bool PIC8ISP::transmitBit()
{
    return m_outBuffer[m_position];
}

inline void PIC8ISP::receiveBit ( bool bit )
{
    m_inBuffer[m_position] = bit;
    if ( ( COMMAND_LENGTH - 1 ) == m_position )
    {
        int command = 0;
        for ( unsigned int i = 0; i < COMMAND_LENGTH; i++ )
        {
            if ( true == m_inBuffer[i] )
            {
                command |= ( 1 << i );
            }
        }
        preProcessCommand(command);
    }
    else if ( ( SEQUENCE_LENGTH - 1 ) == m_position )
    {
        if ( true == m_commandAccepted )
        {
            postProcessCommand();
            logEvent(EVENT_ISP_COMMAND_PROCESSED);
        }
        m_position = 0;
        clearInBuffer();
        clearOutBuffer();
    }
}

inline void PIC8ISP::postProcessCommand()
{

    switch ( m_lastCommand )
    {
        case COMMAND_LOAD_CONFIGURATION:
            m_bufferForProgMem = getDataPartFromInBuffer();
            break;

        case COMMAND_LOAD_DATA_FOR_PROGRAM_MEMORY:
            m_bufferForProgMem = getDataPartFromInBuffer();
            break;

        case COMMAND_LOAD_DATA_FOR_DATA_MEMORY:
            m_bufferForDataMem = ( 0xff & getDataPartFromInBuffer() );
            break;

        case COMMAND_READ_DATA_FROM_PROGRAM_MEMORY:
            // Do nothing here.
            break;

        case COMMAND_READ_DATA_FROM_DATA_MEMORY:
            // Do nothing here.
            break;

        case COMMAND_INCREMENT_ADDRESS:
            // Do nothing here.
            break;

        case COMMAND_BEGIN_ERASE_PROGRAMMING_CYCLE:
            postProcessCommandBeginEraseProgrammingCycle();
            break;

        case COMMAND_BEGIN_PROGRAMMING_ONLY_CYCLE:
            postProcessCommandBeginProgrammingOnlyCycle();
            break;

        case COMMAND_BULK_ERASE_PROGRAM_MEMORY:
            break;

        case COMMAND_BULK_ERASE_DATA_MEMORY:
            break;

        case COMMAND_BULK_ERASE_SETUP1:
            break;

        case COMMAND_BULK_ERASE_SETUP2:
            break;

        default:
            break;
    }
}

inline void PIC8ISP::preProcessCommand ( int command )
{
    m_commandAccepted = false;
    m_lastCommand = COMMAND_NONE;

    switch ( command )
    {
        case 0b000000:
        case 0b010000:
        case 0b100000:
        case 0b110000:
            commandLoadConfiguration(command);
            break;

        case 0b000001:
            commandBulkEraseSetup1(command);
            break;

        case 0b000010:
        case 0b010010:
        case 0b100010:
        case 0b110010:
            commandLoadDataForProgramMemory(command);
            break;

        case 0b000011:
        case 0b010011:
        case 0b100011:
        case 0b110011:
            commandLoadDataForDataMemory(command);
            break;

        case 0b000100:
        case 0b010100:
        case 0b100100:
        case 0b110100:
            commandReadDataFromProgramMemory(command);
            break;

        case 0b000101:
        case 0b010101:
        case 0b100101:
        case 0b110101:
            commandReadDataFromDataMemory(command);
            break;

        case 0b000110:
        case 0b010110:
        case 0b100110:
        case 0b110110:
            commandIncrementAddress(command);
            break;

        case 0b000111:
            commandBulkEraseSetup2(command);
            break;

        case 0b001000:
            commandBeginEraseProgrammingCycle(command);
            break;

        case 0b001001:
        case 0b011001:
        case 0b101001:
        case 0b111001:
            commandBulkEraseProgramMemory(command);
            break;

        case 0b001011:
        case 0b011011:
        case 0b101011:
        case 0b111011:
            commandBulkEraseDataMemory(command);
            break;

        case 0b011000:
            commandBeginProgrammingOnlyCycle(command);
            break;

        default:
            logEvent(EVENT_ISP_INVALID_COMMAND, command);
            return;
    }

    if ( false == m_commandAccepted )
    {
        logEvent(EVENT_ISP_UNSUPPORTED_COMMAND, 0, command);
    }
    else
    {
        logEvent(EVENT_ISP_COMMAND_ACCEPTED, 0, m_lastCommand);
    }
}

inline void PIC8ISP::commandLoadConfiguration ( int command )
{
    if ( 0b000000 != command )
    {
        if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_LOAD_CONFIGURATION;
    m_address = CONFIG_MEM_START_ADDRESS;
    m_delay = m_config.m_delayBetweenCmdAndData;
    logEvent(EVENT_ISP_ADDRESS_CHANGED, 0, m_address);
}

inline void PIC8ISP::commandLoadDataForProgramMemory ( int command )
{
    if ( 0b000010 != command )
    {
        if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_LOAD_DATA_FOR_PROGRAM_MEMORY;
    m_delay = m_config.m_delayBetweenCmdAndData;
}

inline void PIC8ISP::commandReadDataFromProgramMemory ( int command )
{
    if ( 0b000100 != command )
    {
        if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_READ_DATA_FROM_PROGRAM_MEMORY;
    m_delay = m_config.m_delayBetweenCmdAndData;

    unsigned int tmpData;
    m_programMemory->directRead(m_address, tmpData);
    setDataPartInOutputBuffer(tmpData);
}

inline void PIC8ISP::commandIncrementAddress ( int command )
{
    if ( 0b000110 != command )
    {
        if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
                &&
             ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_INCREMENT_ADDRESS;
    m_address++;
    logEvent(EVENT_ISP_ADDRESS_CHANGED, 0, m_address);
}

inline void PIC8ISP::commandBeginEraseProgrammingCycle ( int )
{
    if ( -1 == m_bufferForDataMem || -1 == m_bufferForProgMem )
    {
        logEvent(EVENT_ISP_ERROR_BUFFER_EMPTY);
        return;
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BEGIN_ERASE_PROGRAMMING_CYCLE;
}

inline void PIC8ISP::commandBeginProgrammingOnlyCycle ( int )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
    {
        return;
    }

    if ( -1 == m_bufferForDataMem || -1 == m_bufferForProgMem )
    {
        logEvent(EVENT_ISP_ERROR_BUFFER_EMPTY);
        return;
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BEGIN_PROGRAMMING_ONLY_CYCLE;
}

inline void PIC8ISP::commandLoadDataForDataMemory ( int command )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F8X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
    {
        return;
    }

    if ( 0b000011 != command )
    {
        if ( COMMANDSET_PIC16F8X == m_config.m_commandSet )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_LOAD_DATA_FOR_DATA_MEMORY;
    m_delay = m_config.m_delayBetweenCmdAndData;
}

inline void PIC8ISP::commandReadDataFromDataMemory ( int command )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F87X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F8X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
    {
        return;
    }

    if ( 0b000101 != command )
    {
        if ( COMMANDSET_PIC16F8X == m_config.m_commandSet )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_READ_DATA_FROM_DATA_MEMORY;
    m_delay = m_config.m_delayBetweenCmdAndData;

    unsigned int tmpData;
    m_dataEEPROM->directRead(m_address, tmpData);
    setDataPartInOutputBuffer(tmpData);
}

inline void PIC8ISP::commandBulkEraseProgramMemory ( int command )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F8X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
    {
        return;
    }

    if ( 0b001001 != command )
    {
        if ( COMMANDSET_PIC16F8X == m_config.m_commandSet )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BULK_ERASE_PROGRAM_MEMORY;

    // TODO: This command is not implemented yet
}

inline void PIC8ISP::commandBulkEraseDataMemory ( int command )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F8X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F84A != m_config.m_commandSet ) )
    {
        return;
    }

    if ( 0b001011 != command )
    {
        if ( COMMANDSET_PIC16F8X == m_config.m_commandSet )
        {
            return;
        }
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BULK_ERASE_DATA_MEMORY;

    // TODO: This command is not implemented yet
}

inline void PIC8ISP::commandBulkEraseSetup1 ( int )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F87X != m_config.m_commandSet ) )
    {
        return;
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BULK_ERASE_SETUP1;

    // TODO: This command is not implemented yet
}

inline void PIC8ISP::commandBulkEraseSetup2 ( int )
{
    if ( ( COMMANDSET_PIC16F62X != m_config.m_commandSet )
            &&
         ( COMMANDSET_PIC16F87X != m_config.m_commandSet ) )
    {
        return;
    }

    m_commandAccepted = true;
    m_lastCommand = COMMAND_BULK_ERASE_SETUP2;

    // TODO: This command is not implemented yet
}

inline void PIC8ISP::clearInBuffer()
{
    for ( unsigned int i = 0; i < SEQUENCE_LENGTH; i++ )
    {
        m_inBuffer[i] = false;
    }
}

inline void PIC8ISP::clearOutBuffer()
{
    for ( unsigned int i = 0; i < SEQUENCE_LENGTH; i++ )
    {
        m_outBuffer[i] = false;
    }
}

inline void PIC8ISP::postProcessCommandBeginEraseProgrammingCycle()
{
    if ( -1 != m_bufferForDataMem )
    {
        m_dataEEPROM->write(m_address, m_bufferForDataMem);
        m_bufferForDataMem = -1;
    }
    if ( -1 != m_bufferForProgMem )
    {
        m_programMemory->write(m_address, m_bufferForDataMem);
        m_bufferForDataMem = -1;
    }
}

inline void PIC8ISP::postProcessCommandBeginProgrammingOnlyCycle()
{
    if ( -1 == m_bufferForDataMem || -1 == m_bufferForProgMem )
    {
        logEvent(EVENT_ISP_ERROR_BUFFER_EMPTY);
    }
    else
    {
        m_delay = m_config.m_progTime;
    }

    if ( -1 != m_bufferForDataMem )
    {
        unsigned int tmpData = m_dataEEPROM->read(m_address);
        tmpData &= ( unsigned int ) m_bufferForDataMem;
        m_bufferForDataMem = -1;
        m_dataEEPROM->write(m_address, tmpData);
    }
    if ( -1 != m_bufferForProgMem )
    {
        unsigned int tmpData = m_programMemory->read(m_address);
        tmpData &= ( unsigned int ) m_bufferForDataMem;
        m_bufferForDataMem = -1;
        m_programMemory->write(m_address, tmpData);
    }
}

inline int PIC8ISP::getDataPartFromInBuffer() const
{
    int data = 0;

    for ( unsigned int i = ( COMMAND_LENGTH + 1 ), j = 0; i < ( SEQUENCE_LENGTH - 1 ); i++, j++ )
    {
        if ( true == m_inBuffer[i] )
        {
            data |= ( 1 << j );
        }
    }

    return data;
}

inline void PIC8ISP::setDataPartInOutputBuffer ( int data )
{
    for ( unsigned int i = ( COMMAND_LENGTH + 1 ), j = 0; i < ( SEQUENCE_LENGTH - 1 ); i++, j++ )
    {
        m_outBuffer[i] = ( data & ( 1 << j ) ) ? true : false;
    }
}

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLineCommands.cxx
 */
// =============================================================================

#include "SimCmdLineCommands.h"

// Feature set configuration.
#include "mds.h"

// Standard headers.
#include <utility>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <iostream>

// Used for i18n only.
#include <QObject>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

// OS compatibility.
#include "utilities/os/os.h"

// MCU memory data container libraries.
#include "utilities/MCUDataFiles/MCUDataFiles.h"

// MCU native debug file.
#include "utilities/DbgFile/DbgFileNative.h"

#ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
#   include "AdaptableSim/AdaptableSimStatusFlagsBase.h"
#   include "AdaptableSim/AdaptableSimInterruptController.h"
#endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

#ifdef MDS_FEATURE_PICOBLAZE
#   include "PicoBlaze/PicoBlazeInstructionSet.h"
#   include "PicoBlaze/PicoBlazeStatusFlagsBase.h"
#   include "PicoBlaze/PicoBlazeInterruptController.h"
#endif // MDS_FEATURE_PICOBLAZE

const std::map<std::string, SimCmdLineCommands::MemorySpace> SimCmdLineCommands::S_MEM_SPACE =
{
    { "portin",         SP_PORTIN       },
    { "portout",        SP_PORTOUT      },
    { "register",       SP_REGS         },
    { "data",           SP_DATA         },
    { "code",           SP_CODE         },
    { "stack",          SP_STACK        },
};

constexpr boost::regex::flag_type reFlags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );
const boost::regex SimCmdLineCommands::m_reBinaryPrefix      = boost::regex ( "0b[01]+",     reFlags );
const boost::regex SimCmdLineCommands::m_reBinarySuffix      = boost::regex ( "[01]+b",      reFlags );
const boost::regex SimCmdLineCommands::m_reOctalPrefix       = boost::regex ( "0[0-7]+",     reFlags );
const boost::regex SimCmdLineCommands::m_reOctalSuffix       = boost::regex ( "[0-7]+[oq]",  reFlags );
const boost::regex SimCmdLineCommands::m_reDecimalPrefix     = boost::regex ( "[0-9]+",      reFlags );
const boost::regex SimCmdLineCommands::m_reDecimalSuffix     = boost::regex ( "[0-9]+d",     reFlags );
const boost::regex SimCmdLineCommands::m_reHexadecimalPrefix = boost::regex ( "0x[0-9A-F]+", reFlags );
const boost::regex SimCmdLineCommands::m_reHexadecimalSuffix = boost::regex ( "[0-9A-F]+h",  reFlags );

SimCmdLineBase::ExitCode SimCmdLineCommands::commandLoop()
{
    std::string input;
    std::string command;
    std::string subcommand;
    unsigned int pos;

    m_simControl->registerObserver(&m_eventObserver, MCUSimSubsys::ID_INVALID);

    while ( false == std::cin.eof() )
    {
        if ( true == std::cin.bad() )
        {
            std::cerr << QObject::tr("Error: unable to read input.").toStdString() << std::endl;
            return EXIT_ERROR_CLI;
        }

        std::getline(std::cin, input);

        // Remove comments
        size_t comment = input.find('#');
        if ( std::string::npos != comment )
        {
            input.resize(comment);
        }

        // Remove trailing white space.
        if ( false == input.empty() )
        {
            while ( isspace(input.back()) )
            {
                input.pop_back();
            }
        }

        pos = 0;
        readWord(command, input, pos);
        readWord(subcommand, input, pos);

        if ( true == command.empty() )
        {
            continue;
        }

        processCommand(command, subcommand, input, pos);

        if ( input.size() != pos )
        {
            input = input.substr(pos);
            std::cerr << QObject::tr("Warning: trailing `%1' ignored.").arg(input.c_str()).toStdString() << std::endl;
        }
    }

    return EXIT_CODE_SUCCESS;
}

inline void SimCmdLineCommands::done()
{
    std::cout << "done" << std::endl;
}

inline void SimCmdLineCommands::processCommand ( const std::string & command,
                                                 const std::string & subcommand,
                                                 const std::string & input,
                                                 unsigned int & pos )
{
    bool subcmdInvalid = false;

    if ( "sim" == command )
    {
        if ( "step" == subcommand )
        {
            Mutex mutex(m_simControl);

            if ( MCUSimControl::SS_IDLE != m_simControl->getSimState() )
            {
                std::cout << QObject::tr("Error: simulation is already in progress.").toStdString() << std::endl;
                return;
            }

            int steps = 1;

            {
                std::string argument;
                readWord(argument, input, pos);
                if ( false == argument.empty() )
                {
                    if ( false == readInt(steps, argument) )
                    {
                        return;
                    }
                }
            }

            if ( steps < 1 )
            {
                std::cout << QObject::tr("Error: number of steps has to be a positive integer.").toStdString()
                          << std::endl;
                return;
            }

            for ( int i = 0; i < steps; i++ )
            {
                m_simControl->stepProgram();
            }

            done();
        }
        else if ( "reset" == subcommand )
        {
            Mutex mutex(m_simControl);
            m_simControl->resetProgram();
            done();
        }
        else if ( "animate" == subcommand )
        {
            if ( MCUSimControl::SS_IDLE != m_simControl->getSimState() )
            {
                std::cout << QObject::tr("Error: simulation is already in progress.").toStdString() << std::endl;
                return;
            }
            m_simControl->animateProgram(true);
            done();
        }
        else if ( "run" == subcommand )
        {
            if ( MCUSimControl::SS_IDLE != m_simControl->getSimState() )
            {
                std::cout << QObject::tr("Error: simulation is already in progress.").toStdString() << std::endl;
                return;
            }
            m_simControl->runProgram(true);
            done();
        }
        else if ( "halt" == subcommand )
        {
            m_simControl->stopSimulation();
            done();
        }
        else if ( "irq" == subcommand )
        {
            Mutex mutex(m_simControl);

            switch ( m_simControl->getArch() )
            {
              #ifdef MDS_FEATURE_PICOBLAZE
                case MCUSimBase::ARCH_PICOBLAZE:
                {
                    MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_INTERRUPTS);
                    PicoBlazeInterruptController * intrCtrl = dynamic_cast<PicoBlazeInterruptController*>(subSys);
                    if ( nullptr != intrCtrl )
                    {
                        intrCtrl->irq();
                    }
                    break;
                }
              #endif // MDS_FEATURE_PICOBLAZE
              #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
                case MCUSimBase::ARCH_ADAPTABLE:
                {
                    MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_INTERRUPTS);
                    AdaptableSimInterruptController * intrCtrl = dynamic_cast<AdaptableSimInterruptController*>(subSys);
                    if ( nullptr != intrCtrl )
                    {
                        intrCtrl->irq();
                    }
                    break;
                }
              #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
                default:
                    std::cerr << QObject::tr("Error: this command is not supported for this architecture").toStdString()
                              << std::endl;
                    return;
            }

            done();
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "set" == command )
    {
        Mutex mutex(m_simControl);

        if ( "pc" == subcommand )
        {
            int value;
            std::string argument;
            readWord(argument, input, pos);
            if ( false == readInt(value, argument) )
            {
                return;
            }

            if ( value < 0 )
            {
                std::cout << QObject::tr("Error: program counter cannot be set to a negative value.").toStdString()
                          << std::endl;
                return;
            }

            MCUSimMemory * codeMem = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_MEM_CODE));
            if ( value >= (int) codeMem->size() )
            {
                std::cout << QObject::tr ( "Error: program counter set to the specified value would exceed the size "
                                           "of program memory." ).toStdString()
                          << std::endl;
                return;
            }

            MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_CPU));
            cpu->setProgramCounter((unsigned int) value);
            done();
        }
        else if ( "flag" == subcommand )
        {
            std::string flag;
            std::string value;

            readWord(flag, input, pos);
            readWord(value, input, pos);

            if ( ( "1" != value ) && ( "0" != value ) )
            {
                std::cout << QObject::tr ( "Error: value has to be either 0 or 1." ).toStdString() << std::endl;
                return;
            }

            flagCommand ( flag, ( "1" == value ) );
        }
        else if ( "memory" == subcommand )
        {
            std::string space;
            std::string address;
            std::string value;

            readWord(space, input, pos);
            readWord(address, input, pos);
            readWord(value, input, pos);

            int addrInt;
            int valueInt;

            if ( false == readInt(addrInt, address) )
            {
                return;
            }
            if ( false == readInt(valueInt, value) )
            {
                return;
            }

            if ( addrInt < 0 )
            {
                std::cerr << QObject::tr("Error: address cannot be negative.").toStdString() << std::endl;
                return;
            }
            if ( valueInt < 0 )
            {
                std::cerr << QObject::tr("Error: value cannot be negative.").toStdString() << std::endl;
                return;
            }

            const auto sp = S_MEM_SPACE.find(space);
            if ( S_MEM_SPACE.cend() == sp )
            {
                std::cerr << QObject::tr("Error: invalid specifier: ").toStdString() << space << std::endl;
                return;
            }

            setMemoryCommand(sp->second, (unsigned int) addrInt, (unsigned int) valueInt);
        }
        else if ( "size" == subcommand )
        {
            std::string what;
            std::string value;
            int size;

            readWord(what, input, pos);
            readWord(value, input, pos);

            if ( false == readInt(size, value) )
            {
                return;
            }

            const auto space = S_MEM_SPACE.find(what);
            if ( S_MEM_SPACE.cend() == space )
            {
                std::cerr << QObject::tr("Error: invalid specifier: ").toStdString() << what << std::endl;
                return;
            }

            sizeCommand(space->second, size);
        }
        else if ( "breakpoint" == subcommand )
        {
            std::string fileLine;
            std::string value;

            readWord(fileLine, input, pos);
            readWord(value, input, pos);

            size_t pos = fileLine.find(':');
            if ( std::string::npos == pos )
            {
                std::cout << QObject::tr ( "Error: missing <file:line> pair." ).toStdString() << std::endl;
                return;
            }

            std::string file = fileLine.substr(0, pos);
            int line;
            if ( false == readInt(line, fileLine.substr(pos + 1)) )
            {
                return;
            }

            if ( ( false == value.empty() ) && ( "1" != value ) && ( "0" != value ) )
            {
                std::cout << QObject::tr ( "Error: value has to be either 0 or 1." ).toStdString() << std::endl;
                return;
            }

            breakpointCommand ( file, line, ( "0" != value ) );
        }
        else if ( "config" == subcommand )
        {
            Mutex mutex(m_simControl);

            std::string key;
            std::string value;

            readWord(key, input, pos);
            readWord(value, input, pos);

            configCommand(key, value);
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "get" == command )
    {
        Mutex mutex(m_simControl);

        if ( "pc" == subcommand )
        {
            MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_CPU));
            std::cout << cpu->getProgramCounter() << std::endl;
            done();
        }
        else if ( "flag" == subcommand )
        {
            std::string flag;
            readWord(flag, input, pos);
            flagCommand(flag);
        }
        else if ( "memory" == subcommand )
        {
            std::string space;
            std::string address;
            std::string endAddress;

            readWord(space, input, pos);
            readWord(address, input, pos);
            readWord(endAddress, input, pos);

            const auto sp = S_MEM_SPACE.find(space);
            if ( S_MEM_SPACE.cend() == sp )
            {
                std::cerr << QObject::tr("Error: invalid specifier: ").toStdString() << space << std::endl;
                return;
            }

            getMemoryCommand(sp->second, address, endAddress);
        }
        else if ( "cycles" == subcommand )
        {
            std::cout << m_simControl->getTotalMCycles() << std::endl;
            done();
        }
        else if ( "locations" == subcommand )
        {
            const DbgFile * dbg = m_simControl->getSourceInfo();
            for ( const auto i : dbg->getLineRecords() )
            {
                std::cout << dbg->fileNumber2Name(i.m_fileNumber) << ':' << i.m_lineNumber
                          << " @ " << i.m_address << std::endl;
            }
            done();
        }
        else if ( "size" == subcommand )
        {
            std::string what;
            readWord(what, input, pos);

            const auto space = S_MEM_SPACE.find(what);
            if ( S_MEM_SPACE.cend() == space )
            {
                std::cerr << QObject::tr("Error: invalid specifier: ").toStdString() << what << std::endl;
                return;
            }

            sizeCommand(space->second);
        }
        else if ( "breakpoints" == subcommand )
        {
            const DbgFile * dbg = m_simControl->getSourceInfo();
            for ( const auto & breakpoint : m_simControl->getBreakpoints() )
            {
                std::cout << dbg->fileNumber2Name(breakpoint.second.first) << ':' << breakpoint.second.second
                          << " @ " << breakpoint.first << std::endl;
            }
            done();
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "file" == command )
    {
        Mutex mutex(m_simControl);

        std::string space;
        std::string type;
        std::string file;

        readWord(space, input, pos);
        readWord(type, input, pos);
        readWord(file, input, pos);

        const auto sp = S_MEM_SPACE.find(space);
        if ( S_MEM_SPACE.cend() == sp )
        {
            std::cerr << QObject::tr("Error: invalid specifier: ").toStdString() << space << std::endl;
            return;
        }

        const auto tp = S_FILE_TYPE.find(type);
        if ( S_FILE_TYPE.cend() == tp )
        {
            // Unknown file type.
            std::cerr << QObject::tr("Error: %1 is not a valid file type specification.")
                                    .arg(type.c_str()).toStdString() << std::endl;
            return;
        }

        if ( ( "load" == subcommand ) || ( "save" == subcommand ) )
        {
            fileCommand ( file, sp->second, tp->second, ( "save" == subcommand ) );
        }
        else
        {
            subcmdInvalid = true;
        }
    }
    else if ( "help" == command )
    {
        helpCommand(subcommand);
    }
    else if ( "exit" == command )
    {
        std::cout << QObject::tr("Exiting on user request.").toStdString() << std::endl;

        int code = 0;
        if ( false == subcommand.empty() )
        {
            if ( false == readInt(code, subcommand) )
            {
                return;
            }
        }
        m_simControl->abortAndExit();
        m_simControl->wait();
        exit(code);
    }
    else
    {
        std::cerr << QObject::tr("Error: invalid command: ").toStdString() << command << std::endl;
    }

    if ( true == subcmdInvalid )
    {
        std::cerr << QObject::tr("Error: invalid subcommand: ").toStdString() << subcommand << std::endl;
    }
}

inline void SimCmdLineCommands::fileCommand ( const std::string & file,
                                              MemorySpace space,
                                              FileType type,
                                              bool save )
{
    using namespace boost::filesystem;

    MCUSimSubsys::SubsysId subsysId = MCUSimSubsys::ID_INVALID;
    switch ( space )
    {
        case SP_DATA:
            subsysId = MCUSimSubsys::ID_MEM_DATA;
            break;
        case SP_CODE:
            subsysId = MCUSimSubsys::ID_MEM_CODE;
            break;
        case SP_REGS:
            subsysId = MCUSimSubsys::ID_MEM_REGISTERS;
            break;
        default:
            break;
    }

    MCUSimMemory * memory = nullptr;
    if ( MCUSimSubsys::ID_INVALID != subsysId )
    {
        memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(subsysId));
    }

    if ( nullptr == memory )
    {
        std::cerr << QObject::tr("Error: cannot save the specified memory into a file.").toStdString() << std::endl;
        return;
    }

    DataFile * dataFile;
    switch ( type )
    {
        case FT_INTEL_HEX: dataFile = new HexFile();  break;
        case FT_S_REC:     dataFile = new SrecFile(); break;
        case FT_RAW_BIN:   dataFile = new BinFile();  break;
        default:           dataFile = nullptr;        break;
    }

    std::string filename = path(makeHomeSafe(file)).make_preferred().string();

    try
    {
        if ( true == save )
        {
            memory->storeInDataFile(dataFile);
            dataFile->save(filename, false);
        }
        else
        {
            dataFile->clearAndLoad(filename);
            memory->loadDataFile(dataFile);
        }
    }
    catch ( const DataFileException & e )
    {
        std::cerr << QObject::tr("Error: unable to use file: `%1', reason: ") .arg(filename.c_str()).toStdString()
                  << e.toString() << std::endl;
        return;
    }

    done();
}

inline void SimCmdLineCommands::setMemoryCommand ( MemorySpace space,
                                                   unsigned int address,
                                                   unsigned int value )
{
    MCUSimSubsys::SubsysId subsysId = MCUSimSubsys::ID_INVALID;
    switch ( space )
    {
        case SP_PORTIN:
        case SP_PORTOUT:
        {
            MCUSimPureLogicIO * p = dynamic_cast<MCUSimPureLogicIO*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_PLIO));
            if ( address >= p->getNumberOfPorts() )
            {
                std::cerr << QObject::tr("Error: address is out of range.").toStdString() << std::endl;
                return;
            }

            if ( SP_PORTIN == space )
            {
                p->getInputArray()[address] = value;
            }
            else
            {
                p->getOutputArray()[address] = value;
            }

            done();
            return;
        }
        case SP_DATA:
            subsysId = MCUSimSubsys::ID_MEM_DATA;
            break;
        case SP_CODE:
            subsysId = MCUSimSubsys::ID_MEM_CODE;
            break;
        case SP_STACK:
            subsysId = MCUSimSubsys::ID_STACK;
            break;
        case SP_REGS:
            subsysId = MCUSimSubsys::ID_MEM_REGISTERS;
            break;
    }

    MCUSimMemory * memory = nullptr;
    if ( MCUSimSubsys::ID_INVALID != subsysId )
    {
        memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(subsysId));
    }

    if ( nullptr == memory )
    {
        std::cerr << QObject::tr("Error: the specified memory is not available.").toStdString() << std::endl;
        return;
    }
    else
    {
        if ( address >= memory->size() )
        {
            std::cerr << QObject::tr("Error: address is out of range.").toStdString() << std::endl;
            return;
        }

        memory->directWrite(address, value);
        done();
    }
}

inline void SimCmdLineCommands::getMemoryCommand ( MemorySpace space,
                                                   const std::string & address,
                                                   const std::string & endAddress )
{
    int addr[2];
    if ( false == readInt(addr[0], address) )
    {
        return;
    }

    if ( false == endAddress.empty() )
    {
        if ( false == readInt(addr[1], endAddress) )
        {
            return;
        }
    }
    else
    {
        addr[1] = addr[0];
    }

    if ( ( addr[0] < 0 ) || ( addr[1] < 0 ) )
    {
        std::cerr << QObject::tr("Error: address cannot be negative.").toStdString() << std::endl;
        return;
    }

    MCUSimSubsys::SubsysId subsysId = MCUSimSubsys::ID_INVALID;
    switch ( space )
    {
        case SP_PORTIN:
        case SP_PORTOUT:
        {
            MCUSimPureLogicIO * p = dynamic_cast<MCUSimPureLogicIO*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_PLIO));
            if ( ( addr[0] >= (int) p->getNumberOfPorts() ) || ( addr[1] >= (int) p->getNumberOfPorts() ) )
            {
                std::cerr << QObject::tr("Error: address is out of range.").toStdString() << std::endl;
                return;
            }

            char * array;
            if ( SP_PORTIN == space )
            {
                array = p->getInputArray();
            }
            else
            {
                array = p->getOutputArray();
            }

            for ( unsigned int i = (unsigned int) addr[0]; i <= (unsigned int) addr[1]; i++ )
            {
                std::cout << (int) array[i] << ' ';
            }
            std::cout << std::endl;

            done();
            return;
        }
        case SP_DATA:
            subsysId = MCUSimSubsys::ID_MEM_DATA;
            break;
        case SP_CODE:
            subsysId = MCUSimSubsys::ID_MEM_CODE;
            break;
        case SP_STACK:
            subsysId = MCUSimSubsys::ID_STACK;
            break;
        case SP_REGS:
            subsysId = MCUSimSubsys::ID_MEM_REGISTERS;
            break;
    }

    MCUSimMemory * memory = nullptr;
    if ( MCUSimSubsys::ID_INVALID != subsysId )
    {
        memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(subsysId));
    }

    if ( nullptr == memory )
    {
        std::cerr << QObject::tr("Error: the specified memory is not available.").toStdString() << std::endl;
        return;
    }
    else
    {
        if ( ( addr[0] >= (int) memory->size() ) || ( addr[1] >= (int) memory->size() ) )
        {
            std::cerr << QObject::tr("Error: address is out of range.").toStdString() << std::endl;
            return;
        }

        for ( unsigned int i = (unsigned int) addr[0]; i <= (unsigned int) addr[1]; i++ )
        {
            unsigned int data;
            memory->directRead(i, data);
            std::cout << data << ' ';
        }
        std::cout << std::endl;

        done();
    }
}

inline void SimCmdLineCommands::flagCommand ( const std::string & flag )
{
    switch ( m_simControl->getArch() )
    {
      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSimBase::ARCH_PICOBLAZE:
        {
            MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_FLAGS);
            PicoBlazeStatusFlagsBase * flags = dynamic_cast<PicoBlazeStatusFlagsBase*>(subSys);

            if ( "I" == flag )
            {
                std::cout << flags->m_interrupted << std::endl;
            }
            else if ( "C" == flag )
            {
                std::cout << flags->m_carry << std::endl;
            }
            else if ( "Z" == flag )
            {
                std::cout << flags->m_zero << std::endl;
            }
            else if ( "pC" == flag )
            {
                std::cout << flags->m_preCarry << std::endl;
            }
            else if ( "pZ" == flag )
            {
                std::cout << flags->m_preZero << std::endl;
            }
            else if ( "IE" == flag )
            {
                std::cout << flags->m_inte << std::endl;
            }
            else
            {
                std::cerr << QObject::tr("Error: flag has to be one of: {C, Z, IE, I, pC, pZ}.").toStdString()
                          << std::endl;
                return;
            }
            break;
        }
      #endif // MDS_FEATURE_PICOBLAZE
      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSimBase::ARCH_ADAPTABLE:
        {
            MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_FLAGS);
            AdaptableSimStatusFlagsBase * flags = dynamic_cast<AdaptableSimStatusFlagsBase*>(subSys);

            if ( "I" == flag )
            {
                std::cout << flags->m_interrupted << std::endl;
            }
            else if ( "C" == flag )
            {
                std::cout << flags->m_carry << std::endl;
            }
            else if ( "Z" == flag )
            {
                std::cout << flags->m_zero << std::endl;
            }
            else if ( "pC" == flag )
            {
                std::cout << flags->m_preCarry << std::endl;
            }
            else if ( "pZ" == flag )
            {
                std::cout << flags->m_preZero << std::endl;
            }
            else if ( "IE" == flag )
            {
                std::cout << flags->m_inte << std::endl;
            }
            else if ( "OV" == flag )
            {
                std::cout << flags->m_overflow << std::endl;
            }
            else if ( "N" == flag )
            {
                std::cout << flags->m_negative << std::endl;
            }
            else if ( "HC" == flag )
            {
                std::cout << flags->m_halfCarry << std::endl;
            }
            else if ( "P" == flag )
            {
                std::cout << flags->m_parity << std::endl;
            }
            else if ( "F" == flag )
            {
                std::cout << flags->m_flag << std::endl;
            }
            else if ( "pOV" == flag )
            {
                std::cout << flags->m_preOverflow << std::endl;
            }
            else if ( "pN" == flag )
            {
                std::cout << flags->m_preNegative << std::endl;
            }
            else if ( "pHC" == flag )
            {
                std::cout << flags->m_preHalfCarry << std::endl;
            }
            else if ( "pP" == flag )
            {
                std::cout << flags->m_preParity << std::endl;
            }
            else
            {
                std::cerr << QObject::tr("Error: flag has to be one of: {C, Z, IE, I, pC, pZ, OV, N, HC, P, F, pOV, "
                                         "pN, pHC, pP}.").toStdString() << std::endl;
                return;
            }
            break;
        }
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
        default:
            std::cerr << QObject::tr("Error: this command is not supported for this architecture").toStdString()
                      << std::endl;
            return;
    }

    done();
}

inline void SimCmdLineCommands::flagCommand ( const std::string & flag,
                                              bool value )
{
    switch ( m_simControl->getArch() )
    {
      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSimBase::ARCH_PICOBLAZE:
        {
            MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_FLAGS);
            PicoBlazeStatusFlagsBase * flags = dynamic_cast<PicoBlazeStatusFlagsBase*>(subSys);

            if ( "I" == flag )
            {
                flags->m_interrupted = ( ( true == value ) ? 1 : 0 );
            }
            else if ( "C" == flag )
            {
                flags->m_carry = value;
            }
            else if ( "Z" == flag )
            {
                flags->m_zero = value;
            }
            else if ( "pC" == flag )
            {
                flags->m_preCarry = value;
            }
            else if ( "pZ" == flag )
            {
                flags->m_preZero = value;
            }
            else if ( "IE" == flag )
            {
                flags->m_inte = value;
            }
            else
            {
                std::cerr << QObject::tr("Error: flag has to be one of: {C, Z, IE, I, pC, pZ}.").toStdString()
                          << std::endl;
                return;
            }
            break;
        }
      #endif // MDS_FEATURE_PICOBLAZE
      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSimBase::ARCH_ADAPTABLE:
        {
            MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimSubsys::ID_FLAGS);
            AdaptableSimStatusFlagsBase * flags = dynamic_cast<AdaptableSimStatusFlagsBase*>(subSys);

            if ( "I" == flag )
            {
                flags->m_interrupted = ( ( true == value ) ? 1 : 0 );
            }
            else if ( "C" == flag )
            {
                flags->m_carry = value;
            }
            else if ( "Z" == flag )
            {
                flags->m_zero = value;
            }
            else if ( "pC" == flag )
            {
                flags->m_preCarry = value;
            }
            else if ( "pZ" == flag )
            {
                flags->m_preZero = value;
            }
            else if ( "IE" == flag )
            {
                flags->m_inte = value;
            }
            else if ( "OV" == flag )
            {
                flags->m_overflow = value;
            }
            else if ( "N" == flag )
            {
                flags->m_negative = value;
            }
            else if ( "HC" == flag )
            {
                flags->m_halfCarry = value;
            }
            else if ( "P" == flag )
            {
                flags->m_parity = value;
            }
            else if ( "F" == flag )
            {
                flags->m_flag = value;
            }
            else if ( "pOV" == flag )
            {
                flags->m_preOverflow = value;
            }
            else if ( "pN" == flag )
            {
                flags->m_preNegative = value;
            }
            else if ( "pHC" == flag )
            {
                flags->m_preHalfCarry = value;
            }
            else if ( "pP" == flag )
            {
                flags->m_preParity = value;
            }
            else
            {
                std::cerr << QObject::tr("Error: flag has to be one of: {C, Z, IE, I, pC, pZ, OV, N, HC, P, F, pOV, "
                                         "pN, pHC, pP}.").toStdString() << std::endl;
                return;
            }
            break;
        }
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
        default:
            std::cerr << QObject::tr("Error: this command is not supported for this architecture").toStdString()
                      << std::endl;
            return;
    }

    done();
}

inline void SimCmdLineCommands::sizeCommand ( MemorySpace what,
                                              int size )
{
    MCUSimSubsys::SubsysId subsysId = MCUSimSubsys::ID_INVALID;
    switch ( what )
    {
        case SP_DATA:   subsysId = MCUSimSubsys::ID_MEM_DATA;      break;
        case SP_CODE:   subsysId = MCUSimSubsys::ID_MEM_CODE;      break;
        case SP_STACK:  subsysId = MCUSimSubsys::ID_STACK;         break;
        case SP_REGS:   subsysId = MCUSimSubsys::ID_MEM_REGISTERS; break;
        default:
            break;
    }

    if ( MCUSimSubsys::ID_INVALID == subsysId )
    {
        std::cerr << QObject::tr("Error: unable to change size for the specified memory.").toStdString() << std::endl;
        return;
    }
    if ( ( size < 0 ) || ( size > 0x10000 ) )
    {
        std::cerr << QObject::tr("Error: invalid size value.").toStdString() << std::endl;
        return;
    }

    MCUSimMemory * memory = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(subsysId));
    if ( nullptr == memory )
    {
        std::cerr << QObject::tr("Error: the specified memory space is not available.").toStdString() << std::endl;
        return;
    }

    memory->resize ( (unsigned int) size );
    done();
}

inline void SimCmdLineCommands::sizeCommand ( MemorySpace what )
{
    int size = -1;
    switch ( what )
    {
        case SP_PORTIN:
        case SP_PORTOUT:
        {
            MCUSimPureLogicIO * p = dynamic_cast<MCUSimPureLogicIO*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_PLIO));
            size = (int) p->getNumberOfPorts();
            break;
        }
        case SP_DATA:
        {
            MCUSimMemory * m = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_MEM_DATA));
            size = (int) m->size();
            break;
        }
        case SP_CODE:
        {
            MCUSimMemory * m = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_MEM_CODE));
            size = (int) m->size();
            break;
        }
        case SP_STACK:
        {
            MCUSimMemory * m = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_STACK));
            size = (int) m->size();
            break;
        }
        case SP_REGS:
        {
            MCUSimMemory * m = dynamic_cast<MCUSimMemory*>(m_simControl->getSimSubsys(MCUSimSubsys::ID_MEM_REGISTERS));
            size = (int) m->size();
            break;
        }
    }

    if ( -1 == size )
    {
        std::cerr << QObject::tr("Error: unable to determine size.").toStdString() << std::endl;
        return;
    }

    std::cout << size << std::endl;
    done();
}

inline void SimCmdLineCommands::breakpointCommand ( const std::string & file,
                                                    int line,
                                                    bool value )
{
    const DbgFile * dbg = m_simControl->getSourceInfo();
    std::map<unsigned int, std::pair<int, int>> & breakpoints = m_simControl->getBreakpoints();
    int address = -1;
    int fileNo = -1;

    for ( const auto i : dbg->getLineRecords() )
    {
        if ( ( i.m_lineNumber == line ) && ( dbg->fileNumber2Name(i.m_fileNumber) == file ) )
        {
            fileNo = i.m_fileNumber;
            address =  i.m_address;
            break;
        }
    }

    if ( -1 == address )
    {
        std::cerr << QObject::tr("Error: unusable location ").toStdString() << file << ':' << line << std::endl;
        return;
    }

    const auto iter = breakpoints.find(address);

    if ( true == value )
    {
        if ( breakpoints.cend() != iter )
        {
            std::cerr << QObject::tr("Error: breakpoint is already set at ").toStdString()
                      << file << ':' << line << std::endl;
            return;
        }
        else
        {
            breakpoints.insert ( { (unsigned int) address, { fileNo, line } } );
        }
    }
    else
    {
        if ( breakpoints.cend() == iter )
        {
            std::cerr << QObject::tr("Error: there is no breakpoint set at ").toStdString()
                      << file << ':' << line << std::endl;
            return;
        }
        else
        {
            breakpoints.erase(iter);
        }
    }

    done();
}

inline void SimCmdLineCommands::helpCommand ( const std::string & command )
{
    if ( true == command.empty() )
    {
        std::cout << QObject::tr("Commands:").toStdString() << std::endl;
        std::cout << QObject::tr("  * set <subcommand> [ <args...> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Set something in the simulator.").toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << QObject::tr("  * get  <subcommand> [ <args...> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Get some information from the simulator.").toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << QObject::tr("  * file <subcommand> [ <args...> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Load or save a file with memory dump.").toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << QObject::tr("  * sim <subcommand> [ <args...> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Simulate program.").toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << QObject::tr("  * exit [ <code> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Exit this command line interface.").toStdString() << std::endl;
        std::cout << std::endl;
        std::cout << QObject::tr("  * help [ <command> ]").toStdString() << std::endl;
        std::cout << QObject::tr("      Print a brief help message about the commands.").toStdString() << std::endl;
        std::cout << std::endl;
    }
    else if ( "set" == command )
    {
        std::cout << QObject::tr("Command set.").toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("Set something in the simulator, this command has the following subcommands:")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set pc <address>").toStdString() << std::endl;
        std::cout << QObject::tr("    Set program counter to <address>. For example `set pc 0x3ff' executes "
                                 "unconditional jump at address 0x3ff.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set flag <flag> <value>").toStdString() << std::endl;
        std::cout << QObject::tr("    Set processor flag. <flag> has to be one of {C, Z, IE, I, pC, pZ} (C is Carry, Z"
                                 " is Zero, pC is pre-Carry (Carry before ISR), pZ is pre-Zero (Zero before ISR), I is"
                                 " interrupt, and IE is interrupt enable), <value> has to be either 0 or 1.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set memory <space> <address> <value>").toStdString() << std::endl;
        std::cout << QObject::tr("    Change content of the <space> memory or I/O at <address>. <space> has to be one "
                                 "of {portin, portout, register, data, code, stack} where `code' is program memory, "
                                 "`data' is scratch-pad ram, and others should be obvious). For example `set memory "
                                 "register 0 0x22'  sets register at address 0 (i.e. S0) to value 0x22.")
                                .toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("    When dealing with memory banks, address is always absolute i.e. not respecting "
                                 "boundary of bank size; this can be demonstrated on an example: suppose there are 2 "
                                 "banks, bank size is 16 and memory size is 32, address 1 in the 1st bank is 1 while "
                                 "the address in the other bank is 16+1=17.").toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("    When the provided value has higher bit width than what can be stored at the given"
                                 " location, the value is automatically silently trimmed from the right; for example "
                                 "when the given value is 0x123 while it is supposed to be 8-bit number, the value "
                                 "stored will be 0x23.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set size <space> <size>").toStdString() << std::endl;
        std::cout << QObject::tr("    Resize the <space> memory or I/O to new size of <size>. (Use with care.) For "
                                 "example `set size code 128' reduces size of the program memory to 128.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set breakpoint <file:line> [ <value> ]").toStdString() << std::endl;
        std::cout << QObject::tr("    Set breakpoint at <file:line>, <value> is optional and may be set either to 0 or "
                                 "1, 1 is default, 1 means set and 0 means unset. Please use command `get locations' to"
                                 " see at which locations breakpoints can actually be set. Breakpoints are effective "
                                 "only in run mode or animation mode.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * set config <key> <value>").toStdString() << std::endl;
        std::cout << QObject::tr("    Alter processor or simulator configuration.").toStdString() << std::endl;
        std::cout << QObject::tr("    <key> might be:").toStdString() << std::endl;
        std::cout << QObject::tr("      - `hwbuild' : for which the <value> is an 8-bit number.")
                                .toStdString() << std::endl;
        std::cout << QObject::tr("      - `interrupt_vector' : for which the <key> is an address to program memory.")
                                .toStdString() << std::endl;
    }
    else if ( "get" == command )
    {
        std::cout << QObject::tr("Command get.").toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("Get some information from the simulator, this command has the following subcommands:")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get pc").toStdString() << std::endl;
        std::cout << QObject::tr("    Get current value of the program counter.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get flag <flag>").toStdString() << std::endl;
        std::cout << QObject::tr("    Get processor flag, please see the `set flag' command for details concerning "
                                 "which flags can be retrieved.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get memory <space> <address> [ <end-address> ]").toStdString() << std::endl;
        std::cout << QObject::tr("    Read memory or I/O. When <end-address> is specified, this command will output a "
                                 "space separated list of decimal values read from the memory in range [ <address>, "
                                 "<end-address> ].").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get size <space>").toStdString() << std::endl;
        std::cout << QObject::tr("    Get size of the specified memory or I/O, please see `set size' command for "
                                 "additional details.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get cycles").toStdString() << std::endl;
        std::cout << QObject::tr("    Get total number of machine cycles executed on the simulated processor so far. "
                                 "This value is automatically set to zero when the simulator is reseted using the "
                                 "`sim reset' command.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get breakpoints").toStdString() << std::endl;
        std::cout << QObject::tr("    List breakpoints set by user using the `set breakpoint' command.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * get locations").toStdString() << std::endl;
        std::cout << QObject::tr("    List source locations (i.e. files and line numbers) where it is possible to set "
                                 "a breakpoint.").toStdString() << std::endl << std::endl;
    }
    else if ( "file" == command )
    {
        std::cout << QObject::tr("Command file.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("Load or save a file with memory dump; this commands recognizes the same file types at"
                                 " the `--code-file-type' command line option, and memory spaces are the same as with "
                                 "the `set memory' command.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("This command has the following subcommands:").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * file load <space> <type> <file>").toStdString() << std::endl;
        std::cout << QObject::tr("    Load contents of the specified file (<file>) into the specified memory (<space>),"
                                 " <type> is type of file, <file> is file name.")
                                .toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("  * file save <space> <type> <file>").toStdString() << std::endl;
        std::cout << QObject::tr("    Save contents of the specified memory (<space>) into the specified file (<file>).")
                                .toStdString() << std::endl << std::endl;
    }
    else if ( "sim" == command )
    {
        std::cout << QObject::tr("Command sim.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("Simulate program.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim step [ <steps> ]").toStdString() << std::endl;
        std::cout << QObject::tr("    Step, optionally execute <steps> steps.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim run").toStdString() << std::endl;
        std::cout << QObject::tr("    Run program. Program animation will run in a separate thread while this tool "
                                 "continues to listen and answer to commands. It is safe to use all other commands "
                                 "while the simulator is running, e.g. `set memory ...', etc.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim animate").toStdString() << std::endl;
        std::cout << QObject::tr("    Animate program. Program animation will run in a separate thread while this tool "
                                 "continues to listen and answer to commands. It is safe to use all other commands "
                                 "while the simulator is running, e.g. `set memory ...', etc.")
                                .toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim halt").toStdString() << std::endl;
        std::cout << QObject::tr("    Halt program animation or run.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim reset").toStdString() << std::endl;
        std::cout << QObject::tr("    Reset simulated processor.").toStdString() << std::endl << std::endl;

        std::cout << QObject::tr("  * sim irq").toStdString() << std::endl;
        std::cout << QObject::tr("    Invoke an interrupt request.").toStdString() << std::endl << std::endl;
    }
    else if ( "exit" == command )
    {
        std::cout << QObject::tr("exit [ <code> ]").toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("Exit this command line interface with exit code <code>; if the <code> is not "
                                 "specified, default exit code with value of 0 will be used.")
                                .toStdString() << std::endl << std::endl;
    }
    else if ( "help" == command )
    {
        std::cout << QObject::tr("help [ <command> ]").toStdString() << std::endl << std::endl;
        std::cout << QObject::tr("Print a brief help message about the commands; if optional <command> is specified, "
                                 "this command prints a message concerning specifically the given <command>.")
                                .toStdString() << std::endl << std::endl;
    }
    else
    {
        std::cerr << QObject::tr("Error: command `%1' not recognized.").arg(command.c_str()).toStdString() << std::endl;
    }
}

inline void SimCmdLineCommands::configCommand ( const std::string & key,
                                                const std::string & value )
{
    switch ( m_simControl->getArch() )
    {
      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSimBase::ARCH_PICOBLAZE:
        {
            MCUSimSubsys * subSys = m_simControl->getSimSubsys(MCUSimCPU::ID_CPU);
            PicoBlazeInstructionSet * cpu = dynamic_cast<PicoBlazeInstructionSet*>(subSys);
            if ( nullptr == cpu )
            {
                return;
            }

            int valueInt;
            if ( false == readInt(valueInt, value) )
            {
                return;
            }

            if ( valueInt < 0 )
            {
                std::cerr << QObject::tr("Error: value cannot be negative").toStdString() << std::endl;
                return;
            }

            if ( "hwbuild" == key )
            {
                cpu->m_config.m_hwbuild = (uint8_t) valueInt;
            }
            else if ( "interrupt_vector" == key )
            {
                cpu->m_config.m_interruptVector = (unsigned int) valueInt;
            }
            else
            {
                std::cerr << QObject::tr("Error: invalid key: %1").arg(key.c_str()).toStdString() << std::endl;
                return;
            }

            break;
        }
      #endif // MDS_FEATURE_PICOBLAZE
        default:
            std::cerr << QObject::tr("Error: this command is not supported for this architecture").toStdString()
                      << std::endl;
            return;
    }

    done();
}

bool SimCmdLineCommands::readInt ( int & out,
                                   const std::string & in )
{
    using namespace boost;

    int base = 0;
    int start = 0;
    int end = (int) in.size();

    if ( true == regex_match(in, m_reBinaryPrefix) )
    {
        start = 2;
        base = 2;
    }
    else if ( true == regex_match(in, m_reBinarySuffix) )
    {
        base = 2;
        end--;
    }
    else if ( true == regex_match(in, m_reOctalPrefix) )
    {
        start = 1;
        base = 8;
    }
    else if ( true == regex_match(in, m_reOctalSuffix) )
    {
        base = 8;
        end--;
    }
    else if ( true == regex_match(in, m_reDecimalPrefix) )
    {
        base = 10;
    }
    else if ( true == regex_match(in, m_reDecimalSuffix) )
    {
        base = 10;
        end--;
    }
    else if ( true == regex_match(in, m_reHexadecimalPrefix) )
    {
        start = 2;
        base = 16;
    }
    else if ( true == regex_match(in, m_reHexadecimalSuffix) )
    {
        base = 16;
        end--;
    }
    else
    {
        std::cerr << QObject::tr("Error: not a valid number: ").toStdString() << in << std::endl;
        return false;
    }

    long number = strtol(in.substr(start, end).c_str(), nullptr, base);
    if ( ( number > INT_MAX ) || ( number < INT_MIN ) )
    {
        std::cerr << QObject::tr("Error: number exceeds allowed range: ").toStdString() << in << std::endl;
        return false;
    }

    out = (int) number;
    return true;
}

void SimCmdLineCommands::readWord ( std::string & word,
                                    const std::string & input,
                                    unsigned int & pos)
{
    word.clear();

    while ( pos < input.size() )
    {
        char in = input[pos++];
        if ( ( ' ' == in ) || ( '\t' == in ) )
        {
            if ( true == word.empty() )
            {
                continue;
            }
            else
            {
                break;
            }
        }

        word += in;
    }
}

SimCmdLineCommands::Mutex::Mutex ( MCUSimControl * simControl ) : m_simControl ( simControl )
{
    m_simState = m_simControl->getSimState();
    if ( MCUSimControl::SS_IDLE != m_simState )
    {
        m_simControl->stopSimulation();
    }
}

SimCmdLineCommands::Mutex::~Mutex()
{
    switch ( m_simState )
    {
        case MCUSimControl::SS_IDLE:
            break;
        case MCUSimControl::SS_RUN:
            m_simControl->runProgram(true);
            break;
        case MCUSimControl::SS_ANIMATION:
            m_simControl->animateProgram(true);
            break;
    }
}

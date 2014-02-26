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
 * @ingroup testsMCUSimPicoBlaze
 * @file main.cxx
 */
// =============================================================================

// PicoBlaze simulator.
#include "PicoBlazeSim.h"
#include "PicoBlazeConfig.h"
#include "PicoBlazeProgramMemory.h"

// Tool for working with Intel 16 Hex files.
#include "HexFile.h"

// Standard header files.
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iostream>

void printUsage ( const char * argv0 )
{
    std::cout << "Usage: " << argv0 << " <kcpsm_family> <number_of_steps> <file.hex>" << std::endl;
    std::cout << std::endl;
    std::cout << "       <kcpsm_family>" << std::endl;
    std::cout << "             ├─ 0 : kcpsm1 CPLD" << std::endl;
    std::cout << "             ├─ 1 : kcpsm1" << std::endl;
    std::cout << "             ├─ 2 : kcpsm2" << std::endl;
    std::cout << "             ├─ 3 : kcpsm3" << std::endl;
    std::cout << "             ╰─ 6 : kcpsm6" << std::endl;
    std::cout << std::endl;

    exit(1);
}

int main ( int argc, char ** argv )
{
    HexFile * programFile;
    PicoBlazeSim * simulator;
    MCUSimEventLogger * log;
    unsigned long long int numberOfSteps;
    MCUSimBase::Family kcpsmFamily;

    // Read and verify validity of the CLI arguments.
    if ( 4 != argc )
    {
        printUsage(argv[0]);
    }
    if ( 1 != strlen(argv[1]) )
    {
        printUsage(argv[0]);
    }
    switch ( argv[1][0] )
    {
        case '0':
            kcpsmFamily = MCUSimBase::FAMILY_KCPSM1CPLD;
            break;
        case '1':
            kcpsmFamily = MCUSimBase::FAMILY_KCPSM1;
            break;
        case '2':
            kcpsmFamily = MCUSimBase::FAMILY_KCPSM2;
            break;
        case '3':
            kcpsmFamily = MCUSimBase::FAMILY_KCPSM3;
            break;
        case '6':
            kcpsmFamily = MCUSimBase::FAMILY_KCPSM6;
            break;
        default:
            printUsage(argv[0]);
    }
    for ( int i = (int) strlen(argv[2]) - 1; i >= 0; i-- )
    {
        if ( 0 == isdigit(argv[2][i]) )
        {
            printUsage(argv[0]);
        }
    }
    numberOfSteps = strtoull(argv[2], 0, 10);

    // Attempt to open the given Intel 8/16 HEX file.
    try
    {
        programFile = new HexFile(argv[3]);
    }
    catch ( const DataFileException & e )
    {
        std::cerr << "Instance of DataFileException thrown:" << std::endl;
        std::cerr << e.toString() << std::endl;
        return 2;
    }

    // Create and configure simulator.
    simulator = new PicoBlazeSim;
    log = simulator->getLog();
    dynamic_cast<PicoBlazeConfig&>(simulator->getConfig()).configure(kcpsmFamily);

    // Put simulator into the default state after power up.
    simulator->reset(MCUSim::RSTMD_NEW_CONFIG);
    simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    simulator->reset(MCUSim::RSTMD_MCU_RESET);

    // Load program memory from the given Intel 8/16 HEX file.
    MCUSimSubsys * programMemSubsys = simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE);
    PicoBlazeProgramMemory * programMem = dynamic_cast<PicoBlazeProgramMemory*>(programMemSubsys);
    programMem->loadDataFile(programFile);
    delete programFile;

    // Run simulation.
    for ( unsigned long long int i = 0; i < numberOfSteps; i++ )
    {
        simulator->executeInstruction();
        log->clear();
    }

    return 0;
}

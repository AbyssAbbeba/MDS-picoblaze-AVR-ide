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
 * @ingroup SimControl
 * @file MCUSimControl.cxx
 */
// =============================================================================

#include "MCUSimControl.h"

#include "../../mds.h"

#include "MCUSimObserver.h"

#include "DbgFileCDB.h"
#include "DbgFileAvraLst.h"
#include "DbgFileAvrCoff.h"
#include "DbgFileNative.h"

#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"
#include "ObjectDataFile.h"

#ifdef MDS_FEATURE_AVR8
    #include "AVR8Sim.h"
    #include "AVR8ProgramMemory.h"
    #include "McuDeviceSpecAVR8.h"
#endif // MDS_FEATURE_AVR8

#ifdef MDS_FEATURE_PIC8
    #include "PIC8Sim.h"
    #include "PIC8ProgramMemory.h"
    #include "McuDeviceSpecPIC8.h"
#endif // MDS_FEATURE_PIC8

#ifdef MDS_FEATURE_PICOBLAZE
    #include "PicoBlazeSim.h"
    #include "PicoBlazeProgramMemory.h"
#endif // MDS_FEATURE_PICOBLAZE

#ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    #include "AdaptableSim.h"
    #include "AdjSimProcDef.h"
    #include "AdaptableSimProgramMemory.h"
#endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

#include "McuSimCfgMgr.h"
#include "McuDeviceSpec.h"

#include <ctime>
#include <cstring>

#include <QDebug>
#include <QCoreApplication>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

MCUSimControl::MCUSimControl ( const char * deviceName,
                               const AdjSimProcDef * procDef )
                             : m_simulator(nullptr),
                               m_dbgFile(nullptr)
{
    m_abort = false;
    m_running = false;

    m_breakPointsEnabled = true;
    changeDevice(deviceName, procDef);
}

MCUSimControl::~MCUSimControl()
{
    if ( nullptr != m_simulator )
    {
        delete m_simulator;
    }

    if ( nullptr != m_dbgFile )
    {
        delete m_dbgFile;
    }
}

bool MCUSimControl::startSimulation ( const std::string & filename,
                                      CompilerID compilerId,
                                      DataFileType dataFileType )
{
    m_breakPointsSet = false;
    if ( nullptr != m_dbgFile )
    {
        delete m_dbgFile;
        m_dbgFile = nullptr;
    }

    DataFile * dataFile = nullptr;
    std::string dbgFileExt;
    std::string dataFileExt;

    switch ( compilerId )
    {
        case COMPILER_NATIVE:
        {
            dbgFileExt = ".dbg";
            m_dbgFile = new DbgFileNative();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFile = new HexFile();
                    dataFileExt = ".ihex";
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        case COMPILER_SDCC:
        {
            dbgFileExt = ".cdb";
            m_dbgFile = new DbgFileCDB();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFileExt = ".ihx";
                    dataFile = new HexFile();
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        case COMPILER_GCC: // TODO: Not implemeted yet!
        {
            m_messages.push_back(QObject::tr("Compiler not implemeted yet!").toStdString());
            return false;
            break;
        }

        case COMPILER_AVRA: // TODO: Not implemeted yet!
        {
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFileExt = ".hex";
                    dbgFileExt = ".lst";
                    dataFile = new HexFile();
                    m_dbgFile = new DbgFileAvraLst();
                    break;
                }
                case DBGFILEID_COFF:
                {
                    dataFileExt = ".cof";
                    dbgFileExt = ".cof";
                    ObjectDataFile * objectDataFile = new ObjectDataFile();
                    DbgFileAvrCoff * dbgFileAvrCoff = new DbgFileAvrCoff();
                    dbgFileAvrCoff->assignCodeMemDataContainer(objectDataFile);
                    dataFile = objectDataFile;
                    m_dbgFile = dbgFileAvrCoff;
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        default:
        {
            m_messages.push_back(QObject::tr("Compiler not supported.").toStdString());
            return false;
        }
    }

    if ( ( nullptr == dataFile ) || ( nullptr == m_dbgFile) )
    {
        // TODO: implement a proper error handling here
        m_messages.push_back(QObject::tr("error: ( nullptr == dataFile ) || ( nullptr == m_dbgFile)").toStdString());
        return false;
    }

    try
    {
        m_dbgFile->openFile(filename + dbgFileExt);
    }
    catch ( const DbgFile::Exception & e )
    {
        m_messages.push_back(QObject::tr("Failed to load the debug file: %1").arg(e.toString().c_str()).toStdString());
        return false;
    }

    // Load data file for the program memory
    try
    {
        dataFile->clearAndLoad(filename + dataFileExt);
    }
    catch ( const DataFileException & e )
    {
        // TODO: implement a proper error handling here
        m_messages.push_back(QObject::tr("Failed to load program memory from the given file.").toStdString());
        delete dataFile;
        return false;
    }

    // Reset the simulator
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    resetProgram();

    // Start simulator
    switch ( m_architecture )
    {
      #ifdef MDS_FEATURE_AVR8
        case MCUSim::ARCH_AVR8:
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_AVR8

      #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PIC8

      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PICOBLAZE

      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSim::ARCH_ADAPTABLE:
            dynamic_cast<AdaptableSimProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

        default:
            // TODO: implement a proper error handling here
            m_messages.push_back(QObject::tr("Unknown device architecture.").toStdString());
            return false;
    }

    //
    m_simulatorLog->clear();
    allObservers_setReadOnly(false);

    delete dataFile;
    return true;
}

bool MCUSimControl::startSimulation ( DbgFile * dbgFile,
                                      DataFile * dataFile )
{
    m_breakPointsSet = false;
    m_dbgFile = dbgFile;

    // Reset the simulator
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    resetProgram();

    // Start simulator
    switch ( m_architecture )
    {
      #ifdef MDS_FEATURE_AVR8
        case MCUSim::ARCH_AVR8:
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_AVR8

      #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #ifdef MDS_FEATURE_PIC8

      #endif // MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PICOBLAZE

      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSim::ARCH_ADAPTABLE:
            dynamic_cast<AdaptableSimProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

        default:
            // TODO: implement a proper error handling here
            m_messages.push_back(QObject::tr("Unknown device architecture.").toStdString());
            return false;
    }

    //
    m_simulatorLog->clear();
    allObservers_setReadOnly(false);

    delete dataFile;
    return true;
}

bool MCUSimControl::startSimulation ( const std::string & dbgFileName,
                                      const std::string & dataFileName,
                                      MCUSimControl::CompilerID compilerId,
                                      MCUSimControl::DataFileType dataFileType )
{
    m_breakPointsSet = false;
    if ( nullptr != m_dbgFile )
    {
        delete m_dbgFile;
        m_dbgFile = nullptr;
    }

    DataFile * dataFile = nullptr;

    switch ( compilerId )
    {
        case COMPILER_NATIVE:
        {
            m_dbgFile = new DbgFileNative();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFile = new HexFile();
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        case COMPILER_SDCC:
        {
            m_dbgFile = new DbgFileCDB();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFile = new HexFile();
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        case COMPILER_GCC: // TODO: Not implemeted yet!
        {
            m_messages.push_back(QObject::tr("Compiler not implemeted yet!").toStdString());
            return false;
        }

        case COMPILER_AVRA: // TODO: Not implemeted yet!
        {
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                {
                    dataFile = new HexFile();
                    m_dbgFile = new DbgFileAvraLst();
                    break;
                }
                case DBGFILEID_COFF:
                {
                    ObjectDataFile * objectDataFile = new ObjectDataFile();
                    DbgFileAvrCoff * dbgFileAvrCoff = new DbgFileAvrCoff();
                    dbgFileAvrCoff->assignCodeMemDataContainer(objectDataFile);
                    dataFile = objectDataFile;
                    m_dbgFile = dbgFileAvrCoff;
                    break;
                }
                default:
                {
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
                }
            }
            break;
        }

        default:
        {
            m_messages.push_back(QObject::tr("Compiler not supported.").toStdString());
            return false;
        }
    }

    if ( ( nullptr == dataFile ) || ( nullptr == m_dbgFile) )
    {
        // TODO: implement a proper error handling here
        m_messages.push_back(QObject::tr("error: ( nullptr == dataFile ) || ( nullptr == m_dbgFile).").toStdString());
        return false;
    }

    try
    {
        m_dbgFile->openFile(dbgFileName);
    }
    catch ( const DbgFile::Exception & e )
    {
        m_messages.push_back(QObject::tr("Failed to load the debug file: %1").arg(e.toString().c_str()).toStdString());
        return false;
    }

    // Load data file for the program memory
    try
    {
        dataFile->clearAndLoad(dataFileName);
    }
    catch ( const DataFileException & e )
    {
        // TODO: implement a proper error handling here
        m_messages.push_back(QObject::tr("Failed to load program memory from the given file.").toStdString());
        delete dataFile;
        return false;
    }

    // Reset the simulator
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    resetProgram();

    // Start simulator
    switch ( m_architecture )
    {
      #ifdef MDS_FEATURE_AVR8
        case MCUSim::ARCH_AVR8:
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_AVR8

      #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #ifdef MDS_FEATURE_PIC8

      #endif // MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PICOBLAZE

      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSim::ARCH_ADAPTABLE:
            dynamic_cast<AdaptableSimProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))->loadDataFile(dataFile);
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR

        default:
            // TODO: implement a proper error handling here
            m_messages.push_back(QObject::tr("Unknown device architecture.").toStdString());
            delete dataFile;
            return false;
    }

    //
    m_simulatorLog->clear();
    allObservers_setReadOnly(false);

    delete dataFile;
    return true;
}

void MCUSimControl::getLineNumber ( std::vector<std::pair<const std::string *, unsigned int>> & lines ) const
{
    lines.clear();

    if ( false == initialized() )
    {
        return;
    }

    std::vector<unsigned int> recordNumbers;

    MCUSimCPU * cpu = dynamic_cast<MCUSimCPU*>(m_simulator->getSubsys(MCUSimSubsys::ID_CPU));
    if ( nullptr == cpu )
    {
        return;
    }

    m_dbgFile->getLineByAddr ( cpu->getProgramCounter(), recordNumbers);
    if ( true == recordNumbers.empty() )
    {
        return;
    }

    for ( unsigned int idx : recordNumbers )
    {
        const DbgFile::LineRecord & lineRecord = m_dbgFile->getLineRecords()[idx];
        const std::string * filename = &( m_dbgFile->fileNumber2Name(lineRecord.m_fileNumber) );
        unsigned int lineNumber = lineRecord.m_lineNumber;

        lines.push_back({filename, lineNumber});
    }
}

const DbgFile * MCUSimControl::getSourceInfo()
{
    if ( false == initialized() )
    {
        return nullptr;
    }
    return m_dbgFile;
}

unsigned long long MCUSimControl::getTotalMCycles() const
{
    return m_totalMCycles;
}

void MCUSimControl::stopSimulation()
{
    m_abort = true;
    allObservers_setReadOnly(true);
}

void MCUSimControl::stepProgram()
{
    if ( nullptr == m_simulator )
    {
        return;
    }

    m_simulatorLog->setFilter(MCUSimEventLogger::FLAG_ALL);
    m_totalMCycles += m_simulator->executeInstruction();
    dispatchEvents();
    emit(updateRequest(0x3));
}

void MCUSimControl::animateProgram()
{
    if ( nullptr == m_simulator )
    {
        return;
    }

    if ( true == m_running )
    {
        m_abort = true;
        return;
    }

    m_abort = false;
    m_running = true;

    m_simulatorLog->setFilter(MCUSimEventLogger::FLAG_ALL);

    while ( true )
    {
        if ( true == m_abort )
        {
            m_abort = false;
            m_running = false;
            return;
        }

        m_totalMCycles += m_simulator->executeInstruction();
        dispatchEvents();
        emit(updateRequest(0x3));
        QCoreApplication::instance()->processEvents();

        if ( true == m_breakPointsEnabled )
        {
            m_abort |= checkBreakpoint();
        }
    }
}

void MCUSimControl::runProgram()
{
    constexpr unsigned int MAX_REFRESH_FREQ_HZ = 25;
    constexpr unsigned int MIN_REFRESH_FREQ_I  = 250000;

    if ( nullptr == m_simulator )
    {
        return;
    }

    if ( true == m_running )
    {
        m_abort = true;
        return;
    }

    m_abort = false;
    m_running = true;

    m_simulatorLog->setFilter(MCUSimEventLogger::FLAG_HI_PRIO);

    clock_t t = clock();
    unsigned int auxCounter = 0;

    while ( true )
    {
        if ( true == m_abort )
        {
            m_abort = false;
            m_running = false;
            emit(updateRequest(0x7));
            return;
        }

        m_totalMCycles += m_simulator->executeInstruction();

        if ( false == m_simulatorLog->empty() )
        {
            dispatchEvents();
        }

        if ( ++auxCounter > MIN_REFRESH_FREQ_I )
        {
            const clock_t t2 = clock();
            if ( ( t2 - t ) * MAX_REFRESH_FREQ_HZ / CLOCKS_PER_SEC )
            {
                t = t2;
                auxCounter = 0;

                emit(updateRequest(0x1));
                QCoreApplication::instance()->processEvents();
            }
        }

        if ( true == m_breakPointsEnabled )
        {
            m_abort |= checkBreakpoint();
        }
    }
}

void MCUSimControl::resetProgram()
{
    if ( nullptr == m_simulator )
    {
        return;
    }

    m_totalMCycles = 0;
    m_simulator->reset(MCUSim::RSTMD_MCU_RESET);
    allObservers_deviceReset();
    m_lastBrkPntStop.clear();
}

bool MCUSimControl::changeDevice ( const char * deviceName,
                                   const AdjSimProcDef * procDef )
{
    if ( nullptr != m_simulator )
    {
        delete m_simulator;
    }

    if ( false ) {}
    #ifdef MDS_FEATURE_PICOBLAZE
    else if (
           ( 0 == strcmp("kcpsm1cpld", deviceName) )
               ||
           ( 0 == strcmp("kcpsm1", deviceName) )
               ||
           ( 0 == strcmp("kcpsm2", deviceName) )
               ||
           ( 0 == strcmp("kcpsm3", deviceName) )
               ||
           ( 0 == strcmp("kcpsm6", deviceName) )
       )
    {
        m_architecture = MCUSim::ARCH_PICOBLAZE;
    }
    #endif // MDS_FEATURE_PICOBLAZE
    #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    else if ( 0 == strcmp("AdaptableSim", deviceName) )
    {
        m_architecture = MCUSim::ARCH_ADAPTABLE;
    }
    #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
    else
    {
        m_deviceSpec = McuSimCfgMgr::getInstance()->getDeviceSpec(deviceName);
        if ( nullptr == m_deviceSpec )
        {
            m_messages.push_back ( QObject::tr ( "Failed to retrieve the device configuration specification." )
                                               . toStdString() );
            return false;
        }

        m_architecture = m_deviceSpec->m_arch;
    }

    switch ( m_architecture )
    {
      #ifdef MDS_FEATURE_AVR8
        case MCUSim::ARCH_AVR8:
            m_simulator = new AVR8Sim();
            break;
      #endif // MDS_FEATURE_AVR8
      #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
            m_simulator = new PIC8Sim();
            break;
      #endif // MDS_FEATURE_PIC8
      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
            m_simulator = new PicoBlazeSim();
            break;
      #endif // MDS_FEATURE_PICOBLAZE
      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSim::ARCH_ADAPTABLE:
            m_simulator = new AdaptableSim();
            break;
      #endif // MDS_FEATURE_ADAPTABLE_SIMULATOR
        default:
            m_messages.push_back(QObject::tr("Unknown device architecture." ).toStdString());
            return false;
    }

    m_simulatorLog = m_simulator->getLog();
    McuSimCfgMgr::getInstance()->setupSimulator(deviceName, m_simulator->getConfig(), procDef);
    m_simulator->reset(MCUSim::RSTMD_NEW_CONFIG);
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);

    allObservers_deviceChanged();
    return true;
}

const char * MCUSimControl::getDeviceName() const
{
    if ( nullptr == m_simulator )
    {
        return "";
    }
    else
    {
        return m_deviceSpec->m_name.toStdString().c_str();
    }
}

MCUSim::Arch MCUSimControl::getArch() const
{
    if ( nullptr == m_simulator )
    {
        return MCUSim::ARCH_INVALID;
    }
    else
    {
        return m_architecture;
    }
}

MCUSimSubsys * MCUSimControl::getSimSubsys ( MCUSimSubsys::SubsysId id )
{
    if ( nullptr == m_simulator )
    {
        return nullptr;
    }
    else
    {
        return m_simulator->getSubsys(id);
    }
}

MCUSim * MCUSimControl::directAccess()
{
    return m_simulator;
}

const McuDeviceSpec * MCUSimControl::getDeviceSpec() const
{
    if ( nullptr == m_simulator )
    {
        return nullptr;
    }
    else
    {
        return m_deviceSpec;
    }
}

bool MCUSimControl::initialized() const
{
    return ( nullptr != m_simulator );
}

void MCUSimControl::registerObserver ( MCUSimObserver * observer,
                                       MCUSimSubsys::SubsysId simSubsysToObserve,
                                       const std::vector<int> & subsysEventsToObserve )
{
    uint64_t events = 0;
    for ( const auto ev : subsysEventsToObserve )
    {
        events |= ( 1 << ev );
    }

    registerObserver(observer, simSubsysToObserve, events);

    unregisterSpecificObserver(MCUSimSubsys::SubsysId(simSubsysToObserve), observer);
    m_observers[simSubsysToObserve].push_back({observer, events});

    observer->setControlUnit(this);
}

inline void MCUSimControl::registerObserver ( MCUSimObserver * observer,
                                              MCUSimSubsys::SubsysId simSubsysToObserve,
                                              uint64_t events )
{
    unregisterSpecificObserver(MCUSimSubsys::SubsysId(simSubsysToObserve), observer);
    m_observers[simSubsysToObserve].push_back({observer, events});

    observer->setControlUnit(this);
}

inline bool MCUSimControl::unregisterSpecificObserver ( MCUSimSubsys::SubsysId subsysId,
                                                        const MCUSimObserver * observer )
{
    int size = m_observers[subsysId].size();
    bool result = false;

    for ( int i = 0; i < size; i ++ ) {
        if ( m_observers[subsysId][i].first == observer ) {
            m_observers[subsysId].erase( m_observers[subsysId].begin() + i );
            result = true;
            size--;
        }
    }

    return result;
}

bool MCUSimControl::unregisterObserver ( const MCUSimObserver * observer )
{
    bool result = false;

    for ( int i = 0; i < MCUSimSubsys::ID__MAX__; i++ )
    {
        result |= unregisterSpecificObserver(MCUSimSubsys::SubsysId(i), observer);
    }

    return result;
}

void MCUSimControl::dispatchEvents()
{
    int subsysId, eventId, locationOrReason, detail;

    while ( 0 != m_simulatorLog->getEvent(subsysId, eventId, locationOrReason, detail) )
    {
        #ifndef NDEBUG
            if ( (subsysId >= MCUSimSubsys::ID__MAX__) || (subsysId < 0) )
            {
                m_messages.push_back(QObject::tr("Invalid subsysId." ).toStdString());
                continue;
            }
        #endif // NDEBUG

        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[subsysId].begin(); it != m_observers[subsysId].end(); ++it )
        {
            if ( (1 << eventId) & it->second )
            {
                it->first->handleEvent(subsysId, eventId, locationOrReason, detail);
            }
        }
    }
}

void MCUSimControl::allObservers_deviceChanged()
{
    for ( int i = 0; i < MCUSimSubsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); ++it )
        {
            it->first->deviceChanged();
        }
    }
}

void MCUSimControl::allObservers_deviceReset()
{
    for ( int i = 0; i < MCUSimSubsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); ++it )
        {
            it->first->deviceReset();
        }
    }
}

void MCUSimControl::allObservers_setReadOnly ( bool readOnly )
{
    for ( int i = 0; i < MCUSimSubsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); ++it )
        {
            it->first->setReadOnly(readOnly);
        }
    }
}

void MCUSimControl::setBreakPoints ( const std::vector<std::pair<std::string, std::set<unsigned int>>> & fileLinePairs )
{
    using namespace boost::filesystem;

    m_breakPointsSet = false;

    if ( false == initialized() )
    {
        return;
    }

    const std::vector<std::string> & files = m_dbgFile->getFileNames();

    /*
    // K čemu tohle kurva je, fakt nevim, vole... Na co bych tady kurva převáděl cesty k souborům do kanonickýho tvaru?
    // Tohle má simulovat mikroporcesory, ne šoustat negry do prdele.

    std::vector<std::string> filesBr;

    for ( size_t j = 0; j < fileLinePairs.size(); j++ )
    {
        try
        {
            filesBr.push_back(canonical(path(fileLinePairs[j].first)).make_preferred().string());
        }
        catch ( const boost::filesystem::filesystem_error & e )
        {
            qDebug() << "!!! ERROR: MCUSimControl::setBreakPoints(...): \"" << fileLinePairs[j].first.c_str() << "\" is not valid path.";
        }
    }
    */

    m_breakpoints.clear();
    m_breakpoints.resize(files.size());

    for ( size_t i = 0; i < m_breakpoints.size(); i++ )
    {
        for ( size_t j = 0; j < fileLinePairs.size(); j++ )
        {
            if ( files[i] == /*filesBr[j]*/ fileLinePairs[j].first )
            {
                if ( false == fileLinePairs[j].second.empty() )
                {
                    m_breakPointsSet = true;
                }
                m_breakpoints[i] = fileLinePairs[j].second;
                break;
            }
        }
    }
}

inline bool MCUSimControl::checkBreakpoint()
{
    if ( false == m_breakPointsSet )
    {
        return false;
    }

    bool stop = false;
    std::vector<unsigned int> recordNumbers;

    m_lastBrkPntStop.open();
    m_dbgFile->getLineByAddr(static_cast<MCUSimCPU*>(m_simulator->getSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter(),
                             recordNumbers);

    if ( true == recordNumbers.empty() )
    {
        m_lastBrkPntStop.close();
        return false;
    }

    for ( unsigned int idx : recordNumbers )
    {
        const DbgFile::LineRecord & lineRecord = m_dbgFile->getLineRecords()[idx];
        const std::set<unsigned int> & brkPntSet = m_breakpoints[lineRecord.m_fileNumber];

        if ( brkPntSet.cend() != brkPntSet.find(lineRecord.m_lineNumber) )
        {
            if ( false == m_lastBrkPntStop.check(lineRecord.m_fileNumber, lineRecord.m_lineNumber) )
            {
                stop = true;
            }
        }
    }

    if ( true == stop )
    {
        emit(breakpointReached());
    }

    m_lastBrkPntStop.close();
    return stop;
}

void MCUSimControl::enableBreakPoints ( bool enabled )
{
    m_breakPointsEnabled = enabled;
}

bool MCUSimControl::breakPointsEnabled() const
{
    return m_breakPointsEnabled;
}

const std::vector<std::string> & MCUSimControl::getMessages() const
{
    return m_messages;
}

void MCUSimControl::clearMessages()
{
    m_messages.clear();
}

bool MCUSimControl::getListOfSFR ( std::vector<SFRRegDesc> & sfr )
{
    if ( nullptr == m_simulator )
    {
        return false;
    }

    sfr.clear();

    switch ( m_architecture )
    {
        #ifdef MDS_FEATURE_AVR8
        case MCUSim::ARCH_AVR8:
        {
            const McuDeviceSpecAVR8 * devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_deviceSpec);

            for ( unsigned int i = 0; i < devSpec->m_dataMemory.m_ioRegSize; i++ )
            {
                if ( 0 == devSpec->m_dataMemory.m_ioRegDesc[i].m_name.size() )
                {
                    continue;
                }

                sfr.push_back(SFRRegDesc());

                sfr.back().m_address = i + devSpec->m_dataMemory.m_regFileSize;
                sfr.back().m_regName = devSpec->m_dataMemory.m_ioRegDesc[i].m_name;
                std::string m_regNameTip = devSpec->m_dataMemory.m_ioRegDesc[i].m_desc;
                sfr.back().m_mask = uint8_t ( ( ( devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff00 ) >> 8 )
                                                |
                                              ( ( devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff0000 ) >> 16 ) );

                for ( int j = 0; j < 8; j++ )
                {
                    sfr.back().m_bitNames[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_name;
                    sfr.back().m_toolsTips[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_ttip;
                    sfr.back().m_statusTips[j] = devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_stip;
                }
            }

            break;
        }
        #endif // MDS_FEATURE_AVR8

        #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
        {
            const McuDeviceSpecPIC8 * devSpec = dynamic_cast<const McuDeviceSpecPIC8*>(m_deviceSpec);

            for ( unsigned int i = 0; i < devSpec->m_dataMemory.m_size; i++ )
            {
                if ( 0 == devSpec->m_dataMemory.m_regDescription[i].m_name.size() )
                {
                    continue;
                }

                sfr.push_back(SFRRegDesc());

                sfr.back().m_address = devSpec->m_dataMemory.m_regDescription[i].m_addresses[0];
                sfr.back().m_regName = devSpec->m_dataMemory.m_regDescription[i].m_name;
                std::string m_regNameTip = devSpec->m_dataMemory.m_regDescription[i].m_desc;
                sfr.back().m_mask = uint8_t ( ( ( devSpec->m_dataMemory.m_initValues[i] & 0xff00 ) >> 8 )
                                                |
                                              ( ( devSpec->m_dataMemory.m_initValues[i] & 0xff0000 ) >> 16 ) );

                for ( int j = 0; j < 8; j++ )
                {
                    sfr.back().m_bitNames[j] = devSpec->m_dataMemory.m_regDescription[i].m_bit[j].m_name;
                    sfr.back().m_toolsTips[j] = devSpec->m_dataMemory.m_regDescription[i].m_bit[j].m_ttip;
                    sfr.back().m_statusTips[j] = devSpec->m_dataMemory.m_regDescription[i].m_bit[j].m_stip;
                }
            }

            break;
        }
        #endif // MDS_FEATURE_PIC8

        #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
        {
            // There are no SFR (Special Function Registers) on KCPSM (PicoBlaze)
            break;
        }
        #endif // MDS_FEATURE_PICOBLAZE

        default:
        {
            return false;
        }
    }

    return true;
}

inline void MCUSimControl::BrkPntStop::open()
{
    for ( auto & point : m_brkPnts )
    {
        point.m_checked = false;
    }
}

inline void MCUSimControl::BrkPntStop::close()
{
    bool reconstruct = false;

    for ( const auto & point : m_brkPnts )
    {
        if ( false == point.m_checked )
        {
            reconstruct = true;
            break;
        }
    }

    std::list<BrkPnt> newBrkPnts;

    if ( true == reconstruct )
    {
        for ( const auto & point : m_brkPnts )
        {
            if ( true == point.m_checked )
            {
                newBrkPnts.push_back(point);
            }
        }

        m_brkPnts = newBrkPnts;
    }
}

inline void MCUSimControl::BrkPntStop::clear()
{
    m_brkPnts.clear();
}

inline bool MCUSimControl::BrkPntStop::check ( const int file,
                                               const int line )
{
    for ( auto & point : m_brkPnts )
    {
        if ( ( file == point.m_fileNumber ) && ( line == point.m_lineNumber ) )
        {
            point.m_checked = true;
            return true;
        }
    }

    m_brkPnts.push_back ( { line, file, true } );
    return false;
}

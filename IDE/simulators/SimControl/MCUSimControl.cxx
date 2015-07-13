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
#include <iostream>

#include <QDebug>
#include <QCoreApplication>

// Boost Filesystem library.
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

MCUSimControl::MCUSimControl ( const std::string & deviceName,
                               const AdjSimProcDef * procDef,
                               bool textMode )
                             : m_simulator ( nullptr ),
                               m_dbgFile ( nullptr ),
                               m_textMode ( textMode )
{
    m_abort = false;
    m_running = false;
    m_simState = SS_IDLE;

    m_quotasEnabled = true;
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
    std::string dbgFileExt;
    std::string dataFileExt;

    switch ( compilerId )
    {
        case COMPILER_NATIVE:
            dbgFileExt = ".dbg";
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFileExt = ".ihex";
                    break;
                default:
                    break;
            }
            break;

        case COMPILER_SDCC:
            dbgFileExt = ".cdb";
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFileExt = ".ihx";
                    break;
                default:
                    break;
            }
            break;

        case COMPILER_GCC: // TODO: Not implemeted yet!
            return false;

        case COMPILER_AVRA: // TODO: Not implemeted yet!
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFileExt = ".hex";
                    dbgFileExt = ".lst";
                    break;
                case DBGFILEID_COFF:
                    dataFileExt = ".cof";
                    dbgFileExt = ".cof";
                    break;
                default:
                    break;
            }
            break;
    }

    return startSimulation ( ( filename + dbgFileExt ), ( filename + dataFileExt ), compilerId, dataFileType );
}

bool MCUSimControl::startSimulation ( const std::string & dbgFileName,
                                      const std::string & dataFileName,
                                      MCUSimControl::CompilerID compilerId,
                                      MCUSimControl::DataFileType dataFileType )
{
    DbgFile * dbgFile =  nullptr;
    DataFile * dataFile = nullptr;

    switch ( compilerId )
    {
        case COMPILER_NATIVE:
            dbgFile = new DbgFileNative();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFile = new HexFile();
                    break;
                default:
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
            }
            break;

        case COMPILER_SDCC:
            dbgFile = new DbgFileCDB();
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFile = new HexFile();
                    break;
                default:
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
            }
            break;

        case COMPILER_GCC: // TODO: Not implemeted yet!
            m_messages.push_back(QObject::tr("Compiler not implemeted yet!").toStdString());
            return false;

        case COMPILER_AVRA: // TODO: Not implemeted yet!
            switch ( dataFileType )
            {
                case DBGFILEID_HEX:
                    dataFile = new HexFile();
                    dbgFile = new DbgFileAvraLst();
                    break;
                case DBGFILEID_COFF:
                {
                    ObjectDataFile * objectDataFile = new ObjectDataFile();
                    DbgFileAvrCoff * dbgFileAvrCoff = new DbgFileAvrCoff();
                    dbgFileAvrCoff->assignCodeMemDataContainer(objectDataFile);
                    dataFile = objectDataFile;
                    dbgFile = dbgFileAvrCoff;
                    break;
                }
                default:
                    m_messages.push_back(QObject::tr("File format not supported.").toStdString());
                    return false;
            }
            break;

        default:
            m_messages.push_back(QObject::tr("Compiler not supported.").toStdString());
            return false;
    }

    if ( ( nullptr == dataFile ) || ( nullptr == dbgFile) )
    {
        // TODO: implement a proper error handling here
        m_messages.push_back(QObject::tr("error: ( nullptr == dataFile ) || ( nullptr == dbgFile).").toStdString());
        return false;
    }

    try
    {
        dbgFile->openFile(dbgFileName);
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

    return startSimulation ( dbgFile, dataFile );
}

bool MCUSimControl::startSimulation ( DbgFile * dbgFile,
                                      DataFile * dataFile )
{
    if ( nullptr != m_dbgFile )
    {
        delete m_dbgFile;
    }

    m_quotasSet = false;
    for ( int i = 0; i < QTP__MAX__; i++ )
    {
        m_quotas[i] = -1;
    }

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
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))
                ->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_AVR8

      #ifdef MDS_FEATURE_PIC8
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))
                ->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PIC8

      #ifdef MDS_FEATURE_PICOBLAZE
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))
                ->loadDataFile(dataFile);
            break;
      #endif // MDS_FEATURE_PICOBLAZE

      #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
        case MCUSim::ARCH_ADAPTABLE:
            dynamic_cast<AdaptableSimProgramMemory*>(m_simulator->getSubsys(MCUSimSubsys::ID_MEM_CODE))
                ->loadDataFile(dataFile);
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

void MCUSimControl::getLineNumber ( std::vector<std::pair<const std::string *, unsigned int>> & lines ) const
{
    lines.clear();

    if ( false == initialized() )
    {
        return;
    }

    std::vector<unsigned int> recordNumbers;

    m_dbgFile->getLineByAddr ( m_simCPU->getProgramCounter(), recordNumbers);
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
    emit ( updateRequest ( UR_TIME_AND_PC | UR_SIM_CURSOR ) );
}

void MCUSimControl::animateProgram ( bool thread )
{
    if ( true == thread )
    {
        m_threadCmd.m_animate = true;
        return;
    }

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
    m_simState = SS_ANIMATION;

    m_simulatorLog->setFilter(MCUSimEventLogger::FLAG_ALL);

    while ( true )
    {
        if ( true == m_abort )
        {
            m_abort = false;
            m_running = false;
            m_simState = SS_IDLE;
            return;
        }

        m_totalMCycles += m_simulator->executeInstruction();
        dispatchEvents();
        emit ( updateRequest ( UR_TIME_AND_PC | UR_SIM_CURSOR ) );
        QCoreApplication::instance()->processEvents();

        if ( true == m_breakPointsEnabled )
        {
            m_abort |= checkBreakpoint();
        }

        if ( true == m_quotasEnabled )
        {
            m_abort |= checkQuotas();
        }
    }
}

void MCUSimControl::runProgram ( bool thread )
{
    constexpr unsigned int MAX_REFRESH_FREQ_HZ = 20;

    if ( true == thread )
    {
        m_threadCmd.m_run = true;
        return;
    }

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
    m_simState = SS_RUN;

    m_simulatorLog->setFilter(MCUSimEventLogger::FLAG_HI_PRIO);

    clock_t t = clock();
    unsigned int cycleCounter = 0;
    unsigned int refreshAfterNCycles = 0x10000;

    while ( true )
    {
        if ( true == m_abort )
        {
            m_abort = false;
            m_running = false;
            m_simState = SS_IDLE;
            emit ( updateRequest ( UR_TIME_AND_PC | UR_SIM_CURSOR | UR_MEMORY_REFRESH ) );
            return;
        }

        m_totalMCycles += m_simulator->executeInstruction();

        if ( false == m_simulatorLog->empty() )
        {
            dispatchEvents();
        }

        if ( ++cycleCounter > refreshAfterNCycles )
        {
            const clock_t t2 = clock();
            float ratio = ( 1.0 * ( t2 - t ) * MAX_REFRESH_FREQ_HZ / CLOCKS_PER_SEC );

            if ( ratio > 1 )
            {
                if ( ratio > 2 )
                {
                    refreshAfterNCycles /= ratio;
                    if ( 0 == refreshAfterNCycles )
                    {
                        refreshAfterNCycles = 1;
                    }
                }

                t = t2;
                cycleCounter = 0;

                emit(updateRequest(UR_TIME_AND_PC));
                QCoreApplication::instance()->processEvents();
            }
            else
            {
                refreshAfterNCycles *= ( 1 / ratio );
            }
        }

        if ( true == m_breakPointsEnabled )
        {
            m_abort |= checkBreakpoint();
        }

        if ( true == m_quotasEnabled )
        {
            m_abort |= checkQuotas();
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

bool MCUSimControl::changeDevice ( const std::string & deviceName,
                                   const AdjSimProcDef * procDef )
{
    if ( nullptr != m_simulator )
    {
        delete m_simulator;
        m_simulator = nullptr;
    }

    if ( false ) {}
    #ifdef MDS_FEATURE_PICOBLAZE
    else if (
           ( "kcpsm1cpld" == deviceName )
               ||
           ( "kcpsm1" == deviceName )
               ||
           ( "kcpsm2" == deviceName )
               ||
           ( "kcpsm3" == deviceName )
               ||
           ( "kcpsm6" == deviceName )
       )
    {
        m_architecture = MCUSim::ARCH_PICOBLAZE;
    }
    #endif // MDS_FEATURE_PICOBLAZE
    #ifdef MDS_FEATURE_ADAPTABLE_SIMULATOR
    else if ( "AdaptableSim" == deviceName )
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

    m_simCPU = dynamic_cast<MCUSimCPU*>(m_simulator->getSubsys(MCUSimSubsys::ID_CPU));

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

MCUSimBase::Arch MCUSimControl::getArch() const
{
    if ( nullptr == m_simulator )
    {
        return MCUSimBase::ARCH_INVALID;
    }
    else
    {
        return m_architecture;
    }
}

MCUSimBase::Family MCUSimControl::getFamily() const
{
    if ( nullptr == m_simulator )
    {
        return MCUSimBase::FAMILY_INVALID;
    }
    else
    {
        return m_simulator->family();
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

void MCUSimControl::registerObserver ( MCUSimObserver * observer,
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

        for ( auto observer : m_observers[subsysId] )
        {
            if ( (1 << eventId) & observer.second )
            {
                observer.first->handleEvent(subsysId, eventId, locationOrReason, detail);
            }
        }

        for ( auto observer : m_observers[MCUSimSubsys::ID_INVALID] )
        {
            observer.first->handleEvent(subsysId, eventId, locationOrReason, detail);
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
    m_breakpoints.clear();
    m_breakPointsSet = false;

    if ( false == initialized() )
    {
        return;
    }

    std::vector<std::set<unsigned int>> breakpoints;

    {
        const std::vector<std::string> & files = m_dbgFile->getFileNames();

        breakpoints.resize(files.size());
        for ( size_t i = 0; i < breakpoints.size(); i++ )
        {
            for ( size_t j = 0; j < fileLinePairs.size(); j++ )
            {
                if ( files[i] == fileLinePairs[j].first )
                {
                    if ( false == fileLinePairs[j].second.empty() )
                    {
                        m_breakPointsSet = true;
                    }
                    breakpoints[i] = fileLinePairs[j].second;
                    break;
                }
            }
        }
    }

    {
        const std::vector<DbgFile::LineRecord> & lineRecords = m_dbgFile->getLineRecords();
        for ( const DbgFile::LineRecord & record : lineRecords )
        {
            const std::set<unsigned int> & breakPointSet = breakpoints[record.m_fileNumber];
            if ( breakPointSet.cend() != breakPointSet.find(record.m_lineNumber) )
            {
                m_breakpoints.insert ( { (unsigned int) record.m_address,
                                         { record.m_fileNumber, record.m_lineNumber } } );
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

    m_lastBrkPntStop.open();

    bool stop = false;
    const auto it = m_breakpoints.find(m_simCPU->getProgramCounter());

    if ( m_breakpoints.cend() != it )
    {
        stop = ! m_lastBrkPntStop.check(it->second.first, it->second.second);
    }

    if ( true == stop )
    {
        if ( true == m_textMode )
        {
            std::cout << ">>> breakpoint @ " << it->first << std::endl;
        }
        else
        {
            emit(breakpointReached());
        }
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

inline bool MCUSimControl::checkQuotas()
{
    if ( false == m_quotasSet )
    {
        return false;
    }

    if ( ( -1 != m_quotas[QTP_CYCLES] ) && ( (int) m_totalMCycles >= m_quotas[QTP_CYCLES] ) )
    {
        emit(quotaReached(QTP_CYCLES));
        return true;
    }

//     QTP_INTERRUPTS,
//     QTP_SUBROUTINES,
//     QTP_RETURNS,
//     QTP_INT_RETURNS,
//     QTP_BREAKPOINTS

    return false;
}

void MCUSimControl::enableQuotas ( bool enabled )
{
    m_quotasEnabled = enabled;
}

bool MCUSimControl::quotasEnabled() const
{
    return m_quotasEnabled;
}

void MCUSimControl::setQuota ( MCUSimControl::QuotaType type,
                               int value )
{
    m_quotas[type] = value;

    m_quotasSet = false;
    for ( int i = 0; i < QTP__MAX__; i++ )
    {
        if ( -1 != m_quotas[i] )
        {
            m_quotasSet = true;
            break;
        }
    }
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

void MCUSimControl::run()
{
    while ( true )
    {
        if ( true == m_threadCmd.m_exit )
        {
            return;
        }
        else if ( true == m_threadCmd.m_run )
        {
            m_threadCmd.m_run = false;
            runProgram(false);
        }
        else if ( true == m_threadCmd.m_animate )
        {
            m_threadCmd.m_animate = false;
            animateProgram(false);
        }

        msleep(1);
    }
}

MCUSimControl::ThreadCmd::ThreadCmd()
{
    m_run = false;
    m_exit = false;
    m_animate = false;
}

void MCUSimControl::abortAndExit()
{
    m_abort = true;
    m_threadCmd.m_exit = true;
}

std::map<unsigned int, std::pair<int, int>> & MCUSimControl::getBreakpoints()
{
//     m_breakPointsSet = true;
    return m_breakpoints;
}

MCUSimControl::SimulatorState MCUSimControl::getSimState() const
{
    return m_simState;
}
// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup SimControl
 * @file MCUSimControl.cxx
 */
// =============================================================================

#include "MCUSimControl.h"

#include "MCUSimObserver.h"

#include "DbgFileCDB.h"
#include "DbgFileAvraLst.h"
#include "DbgFileAvrCoff.h"
#include "DbgFileNative.h"

#include "HexFile.h"
#include "BinFile.h"
#include "SrecFile.h"
#include "ObjectDataFile.h"

#include "AVR8Sim.h"
#include "AVR8ProgramMemory.h"

#include "PIC8Sim.h"
#include "PIC8ProgramMemory.h"

#include "PicoBlazeSim.h"
#include "PicoBlazeProgramMemory.h"

#include "McuSimCfgMgr.h"
#include "McuDeviceSpec.h"
#include "McuDeviceSpecAVR8.h"
#include "McuDeviceSpecPIC8.h"

#include <cstring>

#include <QDebug>

MCUSimControl::MCUSimControl ( const char * deviceName )
                             : m_simulator(NULL),
                               m_dbgFile(NULL)
{
    changeDevice(deviceName);
}

MCUSimControl::~MCUSimControl()
{
    if ( NULL != m_simulator )
    {
        delete m_simulator;
    }

    if ( NULL != m_dbgFile )
    {
        delete m_dbgFile;
    }
}

bool MCUSimControl::start ( const std::string & filename,
                            CompilerID compilerId,
                            DataFileType dataFileType )
{
    if ( NULL != m_dbgFile )
    {
        delete m_dbgFile;
        m_dbgFile = NULL;
    }

    DataFile * dataFile = NULL;
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
                    dataFileExt = ".hex";
                    break;
                }
                default:
                {
                    qDebug("File format not supported.");
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
                    qDebug("File format not supported.");
                    return false;
                }
            }
            break;
        }

        case COMPILER_GCC: // TODO: Not implemeted yet!
        {
            qDebug("Not implemeted yet!");
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
                    qDebug("File format not supported.");
                    return false;
                }
            }
            break;
        }

        default:
        {
            qDebug("Compiler not supported.");
            return false;
        }
    }

    if ( ( NULL == dataFile ) || ( NULL == m_dbgFile) )
    {
        // TODO: implement a proper error handling here
        qDebug("error: ( NULL == dataFile ) || ( NULL == m_dbgFile)");
        return false;
    }

    try
    {
        m_dbgFile->openFile(filename + dbgFileExt);
    }
    catch ( DbgFile::Exception & e )
    {
        qDebug ( "Failed to load the debug file: %s", e.toString().c_str() );
        return false;
    }

    // Load data file for the program memory
    try
    {
        dataFile->clearAndLoad(filename + dataFileExt);
    }
    catch ( DataFile::Exception & e )
    {
        // TODO: implement a proper error handling here
        qDebug("Failed to load program memory from the given file.");
        delete dataFile;
        return false;
    }

    // Reset the simulator
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    reset();

    // Start simulator
    switch ( m_architecture )
    {
        case MCUSim::ARCH_AVR8:
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        default:
            // TODO: implement a proper error handling here
            qDebug("Unknown device architecture.");
            return false;
    }

    //
    m_simulatorLog->clear();
    allObservers_setReadOnly(false);

    delete dataFile;
    return true;
}

bool MCUSimControl::start ( const std::string & dbgFileName,
                            const std::string & dataFileName,
                            MCUSimControl::CompilerID compilerId,
                            MCUSimControl::DataFileType dataFileType )
{
    if ( NULL != m_dbgFile )
    {
        delete m_dbgFile;
        m_dbgFile = NULL;
    }

    DataFile * dataFile = NULL;

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
                    qDebug("File format not supported.");
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
                    qDebug("File format not supported.");
                    return false;
                }
            }
            break;
        }

        case COMPILER_GCC: // TODO: Not implemeted yet!
        {
            qDebug("Not implemeted yet!");
            return false;
            break;
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
                    qDebug("File format not supported.");
                    return false;
                }
            }
            break;
        }

        default:
        {
            qDebug("Compiler not supported.");
            return false;
        }
    }

    if ( ( NULL == dataFile ) || ( NULL == m_dbgFile) )
    {
        // TODO: implement a proper error handling here
        qDebug("error: ( NULL == dataFile ) || ( NULL == m_dbgFile)");
        return false;
    }

    try
    {
        m_dbgFile->openFile(dbgFileName);
    }
    catch ( DbgFile::Exception & e )
    {
        qDebug ( "Failed to load the debug file: %s", e.toString().c_str() );
        return false;
    }

    // Load data file for the program memory
    try
    {
        dataFile->clearAndLoad(dataFileName);
    }
    catch ( DataFile::Exception & e )
    {
        // TODO: implement a proper error handling here
        qDebug("Failed to load program memory from the given file.");
        delete dataFile;
        return false;
    }

    // Reset the simulator
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);
    reset();

    // Start simulator
    switch ( m_architecture )
    {
        case MCUSim::ARCH_AVR8:
            dynamic_cast<AVR8ProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        case MCUSim::ARCH_PIC8:
            dynamic_cast<PIC8ProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        case MCUSim::ARCH_PICOBLAZE:
            dynamic_cast<PicoBlazeProgramMemory*>(m_simulator->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(dataFile);
            break;
        default:
            // TODO: implement a proper error handling here
            qDebug("Unknown device architecture.");
            return false;
    }

    //
    m_simulatorLog->clear();
    allObservers_setReadOnly(false);

    delete dataFile;
    return true;
}

int MCUSimControl::getLineNumber ( std::string * fileName )
{
    if ( false == initialized() )
    {
        if ( NULL != fileName )
        {
            *fileName = "";
        }
        return -1;
    }

    unsigned int pc = m_simCpu->getProgramCounter();
    int idx = m_dbgFile->getLineByAddr(pc);
    if ( -1 == idx )
    {
        if ( NULL != fileName )
        {
            *fileName = "";
        }
        return -1;
    }
    else
    {
        int fileNumber = m_dbgFile->getLineRecords().at(idx).m_fileNumber;
        if ( NULL != fileName )
        {
            *fileName = m_dbgFile->fileNumber2Name(fileNumber);
        }
        return m_dbgFile->getLineRecords().at(idx).m_lineNumber;
    }
}

const DbgFile * MCUSimControl::getSourceInfo()
{
    if ( false == initialized() )
    {
        return NULL;
    }
    return m_dbgFile;
}

void MCUSimControl::stop()
{
    allObservers_setReadOnly(true);
}

void MCUSimControl::step()
{
    if ( NULL == m_simulator )
    {
        return;
    }

    m_simulator->executeInstruction();
    dispatchEvents();

    m_fileName.clear();
    int lineNumber = getLineNumber(&m_fileName);
    emit(lineNumberChanged(lineNumber, m_fileName));
}

void MCUSimControl::stepOver()
{
    qDebug("MCUSimControl::stepOver is not implemented yet!");
}

void MCUSimControl::animate()
{
    qDebug("MCUSimControl::animate is not implemented yet!");
}

void MCUSimControl::run()
{
    qDebug("MCUSimControl::run is not implemented yet!");
}

void MCUSimControl::reset()
{
    if ( NULL == m_simulator )
    {
        return;
    }

    m_simulator->reset(MCUSim::RSTMD_MCU_RESET);
    allObservers_deviceReset();

    std::string fileName;
    int lineNumber = getLineNumber(&fileName);
    emit(lineNumberChanged(lineNumber, fileName));
}

bool MCUSimControl::changeDevice ( const char * deviceName )
{
    if ( NULL != m_simulator )
    {
        delete m_simulator;
    }

    if ( 0 == strcmp("kcpsm3", deviceName) )
    {
        m_architecture = MCUSim::ARCH_PICOBLAZE;;
    }
    else
    {
        m_deviceSpec = McuSimCfgMgr::getInstance()->getDeviceSpec(deviceName);
        if ( NULL == m_deviceSpec )
        {
            qDebug("Failed to retrieve the device configuration specification.");
            return false;
        }

        m_architecture = m_deviceSpec->m_arch;
    }

    switch ( m_architecture )
    {
        case MCUSim::ARCH_AVR8:
            m_simulator = new AVR8Sim();
            break;
        case MCUSim::ARCH_PIC8:
            m_simulator = new PIC8Sim();
            break;
        case MCUSim::ARCH_PICOBLAZE:
            m_simulator = new PicoBlazeSim();
            break;
        default:
            qDebug("Unknown device architecture.");
            return false;

    }

    m_simulatorLog = m_simulator->getLog();
    m_simCpu = static_cast<MCUSim::CPU*>(m_simulator->getSubsys(MCUSim::Subsys::ID_CPU));

    McuSimCfgMgr::getInstance()->setupSimulator(deviceName, m_simulator->getConfig());
    m_simulator->reset(MCUSim::RSTMD_NEW_CONFIG);
    m_simulator->reset(MCUSim::RSTMD_INITIAL_VALUES);

    allObservers_deviceChanged();
    return true;
}

const char * MCUSimControl::getDeviceName() const
{
    if ( NULL == m_simulator )
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
    if ( NULL == m_simulator )
    {
        return MCUSim::ARCH_INVALID;
    }
    else
    {
        return m_architecture;
    }
}

MCUSim::Subsys * MCUSimControl::getSimSubsys ( MCUSim::Subsys::SubsysId id )
{
    if ( NULL == m_simulator )
    {
        return NULL;
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
    if ( NULL == m_simulator )
    {
        return NULL;
    }
    else
    {
        return m_deviceSpec;
    }
}

bool MCUSimControl::initialized() const
{
    return ( NULL != m_simulator );
}

void MCUSimControl::registerObserver ( MCUSimObserver * observer,
                                       MCUSim::Subsys::SubsysId simSubsysToObserve,
                                       const std::vector<int> & subsysEventsToObserve )
{
    uint64_t events = 0;
    for ( std::vector<int>::const_iterator it = subsysEventsToObserve.begin();
          it != subsysEventsToObserve.end();
          it++ )
    {
        events |= ( 1 << *it );
    }

    registerObserver(observer, simSubsysToObserve, events);

    unregisterSpecificObserver(MCUSim::Subsys::SubsysId(simSubsysToObserve), observer);
    m_observers[simSubsysToObserve].push_back(std::make_pair(observer, events));

    observer->setControlUnit(this);
}

inline void MCUSimControl::registerObserver ( MCUSimObserver * observer,
                                              MCUSim::Subsys::SubsysId simSubsysToObserve,
                                              uint64_t events )
{
    unregisterSpecificObserver(MCUSim::Subsys::SubsysId(simSubsysToObserve), observer);
    m_observers[simSubsysToObserve].push_back(std::make_pair(observer, events));

    observer->setControlUnit(this);
}

inline bool MCUSimControl::unregisterSpecificObserver ( MCUSim::Subsys::SubsysId subsysId,
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

    for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ )
    {
        result |= unregisterSpecificObserver(MCUSim::Subsys::SubsysId(i), observer);
    }

    return result;
}

void MCUSimControl::dispatchEvents()
{
    int subsysId, eventId, locationOrReason, detail;

    while ( 0 != m_simulatorLog->getEvent(subsysId, eventId, locationOrReason, detail))
    {
        if ( (subsysId >= MCUSim::Subsys::ID__MAX__) || (subsysId < 0) )
        {
            qDebug("Invalid subsysId");
            continue;
        }

        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[subsysId].begin(); it != m_observers[subsysId].end(); it++ )
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
    for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ )
        {
            it->first->deviceChanged();
        }
    }
}

void MCUSimControl::allObservers_deviceReset()
{
    for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ )
        {
            it->first->deviceReset();
        }
    }
}

void MCUSimControl::allObservers_setReadOnly ( bool readOnly )
{
    for ( int i = 0; i < MCUSim::Subsys::ID__MAX__; i++ )
    {
        std::vector<std::pair<MCUSimObserver*, uint64_t> >::iterator it;
        for ( it = m_observers[i].begin(); it != m_observers[i].end(); it++ )
        {
            it->first->setReadOnly(readOnly);
        }
    }
}

bool MCUSimControl::getListOfSFR ( std::vector<SFRRegDesc> & sfr )
{
    if ( NULL == m_simulator )
    {
        return false;
    }

    sfr.clear();

    switch ( m_architecture )
    {
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
        case MCUSim::ARCH_PICOBLAZE:
        {
            // There are no SFR (Special Function Registers) on KCPSM3 (PicoBlaze)
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}

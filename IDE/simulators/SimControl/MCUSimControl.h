// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimControl
 * @file MCUSimControl.h
 */
// =============================================================================

#ifndef MCUSIMCONTROL_H
#define MCUSIMCONTROL_H

// Forward declarations
class MCUSimObserver;
class McuDeviceSpec;
class DbgFile;
class DataFile;

#include "../MCUSim/MCUSim.h"
#include <vector>
#include <string>
#include <utility>
#include <cstdint>
#include <QObject>

/**
 * @class MCUSimControl
 * @ingroup SimControl
 * @brief
 */
class MCUSimControl : public QObject
{
    Q_OBJECT

    ////    Public Datatypes    ////
    public:
        /**
          * @brief
          */
        enum CompilerID
        {
            COMPILER_NATIVE,    ///< Native compiler, i.e. our own compiler - Moravia Microsystems Compiler
            COMPILER_SDCC,      ///< Small Device C Compiler - open-source C compiler for microcontrollers
            COMPILER_GCC,       ///< GNU Compiler Collection - open-source set of various compilers
            COMPILER_AVRA       ///< AVRA assembler - open-source assembler for AVR
        };

        /**
         * @brief
         */
        enum DataFileType
        {
            DBGFILEID_HEX,      ///< Intel HEX
            DBGFILEID_COFF,     ///< Common Object File Format (an archaic format, today widely replaced with ELF, etc.)
            DBGFILEID_SREC      // TODO: Not fully supported yet!
        };

        /**
         * @brief
         */
        struct SFRRegDesc
        {
            int m_address;
            std::string m_regName;      ///<
            std::string m_regNameTip;   ///<
            std::string m_bitNames[8];  ///<
            std::string m_toolsTips[8]; ///<
            std::string m_statusTips[8];///<
            uint8_t m_mask;             ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] deviceName
         */
        MCUSimControl ( const char * deviceName );

        /**
         * @brief Destructor
         */
        virtual ~MCUSimControl();

    private:
        /**
         * @brief Forbidden constructor
         */
        MCUSimControl() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief This method can be called multiple times to register an observer to multiple subsystems
         * @param[in,out] observer
         * @param[in] simSubsysToObserve
         * @param[in] subsysEventsToObserve
         */
        void registerObserver ( MCUSimObserver * observer,
                                MCUSim::Subsys::SubsysId simSubsysToObserve,
                                const std::vector<int> & subsysEventsToObserve );

        /**
         * @brief
         * @param[in,out] observer
         * @param[in] simSubsysToObserve
         * @param[in] events
         */
        void registerObserver ( MCUSimObserver * observer,
                                MCUSim::Subsys::SubsysId simSubsysToObserve,
                                uint64_t events = 0xFFFFFFFFFFFFFFFFULL );

        /**
         * @brief
         * @param[in] observer
         */
        bool unregisterObserver ( const MCUSimObserver * observer );

        /**
         * @brief
         * @param[out] filename
         * @return
         */
        int getLineNumber ( std::string * filename = NULL );

        /**
         * @brief
         * @return
         */
        const DbgFile * getSourceInfo();

        /**
         * @brief
         * @return
         */
        bool initialized() const;

        /**
         * @brief
         * @return
         */
        const char * getDeviceName() const;

        /**
         * @brief
         * @return
         */
        MCUSim::Arch getArch() const;

        /**
         * @brief
         * @param[in] id
         * @return
         */
        MCUSim::Subsys * getSimSubsys ( MCUSim::Subsys::SubsysId id );

        /**
         * @brief
         * @return
         */
        MCUSim * directAccess();

        /**
         * @brief
         * @return
         */
        const McuDeviceSpec * getDeviceSpec() const;

        /**
         * @brief
         * @param[out] sfr
         * @return
         */
        bool getListOfSFR ( std::vector<SFRRegDesc> & sfr );

        /**
         * @brief
         * @param[in] fileNames
         * @param[in] lineNumbers
         */
        void setBreakPoints ( const std::vector<std::string> & fileNames,
                              const std::vector<std::vector<unsigned int>> & lineNumbers );

        /**
         * @brief
         * @param[in] fileNames
         */
        void enableBreakPoints ( bool enabled );

        /**
         * @brief
         * @return
         */
        bool breakPointsEnabled() const;

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        MCUSim::Arch m_architecture;

        /**
         * @brief
         */
        MCUSim * m_simulator;

        /**
         * @brief
         */
        DbgFile * m_dbgFile;

        /**
         * @brief
         */
        MCUSim::CPU * m_simCpu;

        /**
         * @brief
         */
        MCUSim::EventLogger * m_simulatorLog;

        /**
         * @brief
         */
        const McuDeviceSpec * m_deviceSpec;

        /**
         * @brief
         */
        std::vector<std::pair<MCUSimObserver*, uint64_t> > m_observers [ MCUSim::Subsys::ID__MAX__ ];

    ////    Private Operations    ////
    private:
        /**
         * @brief
         */
        void dispatchEvents();

        /**
         * @brief
         */
        void allObservers_deviceChanged();

        /**
         * @brief
         */
        void allObservers_deviceReset();

        /**
         * @brief
         * @param[in] readOnly
         */
        void allObservers_setReadOnly ( bool readOnly );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] subsysId
         * @param[in] observer
         * @return
         */
        inline bool unregisterSpecificObserver ( MCUSim::Subsys::SubsysId subsysId,
                                                 const MCUSimObserver * observer );

    ////    Qt Signals    ////
    signals:
        /**
         * @brief
         * @param[out] lineNumber
         * @param[out] fileName
         */
        void lineNumberChanged ( int lineNumber,
                                 const std::string & fileName );

    ////    Qt Public Slots    ////
    public slots:
        /**
         * @brief
         * @param[in] fileName
         * @param[in] compilerId
         * @param[in] dataFileType
         * @return
         */
        bool start ( const std::string & fileName,
                     CompilerID compilerId,
                     DataFileType dataFileType = DBGFILEID_HEX );

        /**
         * @brief
         * @param[in] dbgFile
         * @param[in] dataFile
         * @warning dbgFile and dataFile are automatically deleted by this class when it does no longer need them.
         * @return
         */
        bool start ( DbgFile * dbgFile,
                     DataFile * dataFile );

        /**
         * @brief
         * @param[in] dbgFile
         * @param[in] dataFile
         * @param[in] compilerId
         * @param[in] dataFileType
         * @return
         */
        bool start ( const std::string & dbgFileName,
                     const std::string & dataFileName,
                     CompilerID compilerId,
                     DataFileType dataFileType = DBGFILEID_HEX );

        /**
         * @brief
         */
        void stop();

        /**
         * @brief
         * @return
         */
        bool isStarted() const;

        /**
         * @brief
         */
        void step();

        /**
         * @brief
         */
        void stepOver();

        /**
         * @brief
         */
        void animate();

        /**
         * @brief
         */
        void run();

        /**
         * @brief
         */
        void reset();

        /**
         * @brief
         * @param[in] deviceName
         * @return
         */
        bool changeDevice ( const char * deviceName );

    ////    Private Attributes    ////
    private:
        /// @brief
        std::string m_fileName;
};

#endif // MCUSIMCONTROL_H

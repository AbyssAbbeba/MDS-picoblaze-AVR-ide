// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimControl
 * @file MCUSimControl.h
 */
// =============================================================================

#ifndef MCUSIMCONTROL_H
#define MCUSIMCONTROL_H

// Forward declarations
class DbgFile;
class DataFile;
class McuDeviceSpec;
class MCUSimObserver;

#include "../MCUSim/MCUSim.h"

#include <set>
#include <list>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include <QThread>

/**
 * @class MCUSimControl
 * @ingroup SimControl
 * @brief
 */
class MCUSimControl : public QThread
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

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        class BrkPntStop
        {
            ////    Private Datatypes    ////
            private:
                /// @brief
                struct BrkPnt
                {
                    int m_lineNumber; ///<
                    int m_fileNumber; ///<
                    bool m_checked;   ///<
                };

            ////    Public Operations    ////
            public:
                /// @brief
                inline void open();

                /// @brief
                inline void close();

                /// @brief
                inline void clear();

                /**
                 * @brief
                 * @param[in] file
                 * @param[in] line
                 * @return
                 */
                inline bool check ( const int file,
                                    const int line );

            private:
                /// @brief
                std::list<BrkPnt> m_brkPnts;
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

    ////    Public Operations    ////
    public:
        /**
         * @brief This method can be called multiple times to register an observer to multiple subsystems
         * @param[in,out] observer
         * @param[in] simSubsysToObserve
         * @param[in] subsysEventsToObserve
         */
        void registerObserver ( MCUSimObserver * observer,
                                MCUSimSubsys::SubsysId simSubsysToObserve,
                                const std::vector<int> & subsysEventsToObserve );

        /**
         * @brief
         * @param[in,out] observer
         * @param[in] simSubsysToObserve
         * @param[in] events
         */
        void registerObserver ( MCUSimObserver * observer,
                                MCUSimSubsys::SubsysId simSubsysToObserve,
                                uint64_t events = 0xFFFFFFFFFFFFFFFFULL );

        /**
         * @brief
         * @param[in] observer
         */
        bool unregisterObserver ( const MCUSimObserver * observer );

        /**
         * @brief
         * @param[out] lines
         */
        void getLineNumber ( std::vector<std::pair<const std::string *, unsigned int>> & lines ) const;

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
        MCUSimSubsys * getSimSubsys ( MCUSimSubsys::SubsysId id );

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
         * @param[in] fileLinePairs
         */
        void setBreakPoints ( const std::vector<std::pair<std::string, std::set<unsigned int>>> & fileLinePairs );

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

        /**
         * @brief
         * @return
         */
        const std::vector<std::string> & getMessages() const;

        /**
         * @brief
         */
        void clearMessages();

        /**
         * @brief Get total number of executed machine cycles.
         * @return
         */
        unsigned long long getTotalMCycles() const;

    ////    Private Operations    ////
    private:
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

        /**
         * @brief
         */
        void dispatchEvents();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] subsysId
         * @param[in] observer
         * @return
         */
        inline bool unregisterSpecificObserver ( MCUSimSubsys::SubsysId subsysId,
                                                 const MCUSimObserver * observer );
        /**
         * @brief
         * @return
         */
        inline bool checkBreakpoint();

    ////    Qt Public Slots    ////
    public slots:
        /**
         * @brief
         * @param[in] fileName
         * @param[in] compilerId
         * @param[in] dataFileType
         * @return
         */
        bool startSimulation ( const std::string & fileName,
                               CompilerID compilerId,
                               DataFileType dataFileType = DBGFILEID_HEX );

        /**
         * @brief
         * @param[in] dbgFile
         * @param[in] dataFile
         * @warning dbgFile and dataFile are automatically deleted by this class when it does no longer need them.
         * @return
         */
        bool startSimulation ( DbgFile * dbgFile,
                               DataFile * dataFile );

        /**
         * @brief
         * @param[in] dbgFile
         * @param[in] dataFile
         * @param[in] compilerId
         * @param[in] dataFileType
         * @return
         */
        bool startSimulation ( const std::string & dbgFileName,
                               const std::string & dataFileName,
                               CompilerID compilerId,
                               DataFileType dataFileType = DBGFILEID_HEX );

        /**
         * @brief
         */
        void stopSimulation();

        /**
         * @brief
         */
        void stepProgram();

        /**
         * @brief
         */
        void animateProgram();

        /**
         * @brief
         */
        void runProgram();

        /**
         * @brief
         */
        void resetProgram();

        /**
         * @brief
         * @param[in] deviceName
         * @return
         */
        bool changeDevice ( const char * deviceName );

    ////    Qt Signals    ////
    signals:
        /**
         * @brief
         * @param[in]
         */
        void updateRequest(int);

        /**
         * @brief
         */
        void breakpointReached();

    ////    Private Attributes    ////
    private:
        /// @brief
        MCUSim::Arch m_architecture;

        /// @brief
        MCUSim * m_simulator;

        /// @brief
        DbgFile * m_dbgFile;

        /// @brief
        MCUSimEventLogger * m_simulatorLog;

        /// @brief
        const McuDeviceSpec * m_deviceSpec;

        /// @brief
        std::vector<std::pair<MCUSimObserver*, uint64_t> > m_observers [ MCUSimSubsys::ID__MAX__ ];

        /// @brief
        std::vector<std::string> m_messages;

        /// @brief
        bool m_breakPointsEnabled;

        /// @brief
        bool m_breakPointsSet;

        /// @brief
        std::vector<std::set<unsigned int>> m_breakpoints;

        /// @brief
        unsigned long long m_totalMCycles;

        /// @brief
        BrkPntStop m_lastBrkPntStop;

        /// @brief
        bool m_running;

        /// @brief
        bool m_abort;
};

#endif // MCUSIMCONTROL_H

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
 * @ingroup PIC8
 * @file PIC8ISP.h
 */
// =============================================================================

#ifndef PIC8ISP_H
#define PIC8ISP_H

// Forward declarations
class PIC8ProgramMemory;
class PIC8DataEEPROM;
class PIC8IO;
class PIC8ConfigWord;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PIC8
 * @class PIC8ISP
 */
class PIC8ISP : public MCUSimSubsys
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int SEQUENCE_LENGTH = 22;

        /// @brief
        static const unsigned int COMMAND_LENGTH = 6;

        /// @brief
        static const unsigned int CONFIG_MEM_START_ADDRESS = 0x2000;
        static const unsigned int CONFIG_MEM_END_ADDRESS = 0x2007;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum CommandSet
        {
            COMMANDSET_PIC16C55X,
            COMMANDSET_PIC16F62X,
            COMMANDSET_PIC16C6XX_7XX_9XX,
            COMMANDSET_PIC16F87X,
            COMMANDSET_PIC16F84A,
            COMMANDSET_PIC16F8X,

            COMMANDSET__MAX__
        };

        enum Command
        {
            COMMAND_NONE,

            COMMAND_LOAD_CONFIGURATION,
            COMMAND_LOAD_DATA_FOR_PROGRAM_MEMORY,
            COMMAND_READ_DATA_FROM_PROGRAM_MEMORY,
            COMMAND_INCREMENT_ADDRESS,
            COMMAND_BEGIN_ERASE_PROGRAMMING_CYCLE,
            COMMAND_BEGIN_PROGRAMMING_ONLY_CYCLE,
            COMMAND_LOAD_DATA_FOR_DATA_MEMORY,
            COMMAND_READ_DATA_FROM_DATA_MEMORY,
            COMMAND_BULK_ERASE_PROGRAM_MEMORY,
            COMMAND_BULK_ERASE_DATA_MEMORY,
            COMMAND_BULK_ERASE_SETUP1,
            COMMAND_BULK_ERASE_SETUP2,

            COMMAND__MAX__
        };

        /**
         * @brief
         */
        enum Event
        {
            EVENT_ISP_INVALID_COMMAND,             ///<
            EVENT_ISP_UNSUPPORTED_COMMAND,         ///<
            EVENT_ISP_COMMAND_ACCEPTED,            ///<
            EVENT_ISP_ADDRESS_CHANGED,             ///<
            EVENT_ISP_COMMAND_PROCESSED,           ///<
            EVENT_ISP_OPERATION_NOT_COMPLETED_YET, ///<
            EVENT_ISP_OPERATION_COMPLETED,         ///<
            EVENT_ISP_ERROR_BUFFER_EMPTY,          ///<

            EVENT_ISP__MAX__
        };

        /**
         * @brief
         */
        struct Config
        {
            ///
            bool m_enabled;

            ///
            float m_delayBetweenCmdAndData;

            ///
            float m_progTime;

            ///
            CommandSet m_commandSet;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        PIC8ISP();

        /**
         * @brief
         */
        ~PIC8ISP();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in,out] programMemory
         * @param[in,out] io
         * @param[in,out] configWord
         * @return
         */
        PIC8ISP * link ( MCUSimEventLogger * eventLogger,
                         PIC8ProgramMemory   * programMemory,
                         PIC8DataEEPROM      * dataEEPROM,
                         PIC8IO              * io,
                         PIC8ConfigWord      * configWord );

        /**
         * @brief
         * @param[in] mode
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief
         * @param[in] mode
         */
        void timeStep ( float timeStep );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void loadConfig();

        /**
         * @brief
         */
        inline void mcuReset();

        /**
         * @brief
         */
        inline void incrPosition();

        /**
         * @brief
         * @return
         */
        inline bool transmitBit();

        /**
         * @brief
         * @param[in] bit
         */
        inline void receiveBit ( bool bit );

        /**
         * @brief
         * @param[in] command
         */
        inline void preProcessCommand ( int command );

        /**
         * @brief
         */
        inline void postProcessCommand();

        /**
         * @brief
         */
        inline void clearInBuffer();

        /**
         * @brief
         */
        inline void clearOutBuffer();

        /**
         * @brief
         */
        inline void postProcessCommandBeginEraseProgrammingCycle();

        /**
         * @brief
         */
        inline void postProcessCommandBeginProgrammingOnlyCycle();

        /**
         * @brief
         * @return
         */
        inline int getDataPartFromInBuffer() const;

        /**
         * @brief
         * @param[in] data
         */
        inline void setDataPartInOutputBuffer ( int data );

        /// @name ISP commands
        //@{
            /**
             * @brief
             * @param[in] command
             */
            inline void commandLoadConfiguration ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandLoadDataForProgramMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandReadDataFromProgramMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandIncrementAddress ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBeginEraseProgrammingCycle ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBeginProgrammingOnlyCycle ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandLoadDataForDataMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandReadDataFromDataMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBulkEraseProgramMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBulkEraseDataMemory ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBulkEraseSetup1 ( int command );

            /**
             * @brief
             * @param[in] command
             */
            inline void commandBulkEraseSetup2 ( int command );
        //@}

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        Config m_config;

    ////    Private Attributes    ////
    private:
        /// @name PIC8 simulator subsystems
        //@{
            ///
            PIC8ProgramMemory * m_programMemory;

            ///
            PIC8DataEEPROM * m_dataEEPROM;

            ///
            PIC8IO * m_io;

            ///
            PIC8ConfigWord * m_configWord;
        //@}

        /**
         * @brief
         */
        bool m_clk;

        /**
         * @brief
         */
        bool m_commandAccepted;

        /**
         * @brief
         */
        unsigned int m_position;

        /**
         * @brief
         */
        unsigned int m_address;

        /**
         * @brief
         */
        float m_delay;

        /**
         * @brief
         */
        Command m_lastCommand;

        /**
         * @brief
         */
        bool m_inBuffer [ SEQUENCE_LENGTH ];

        /**
         * @brief
         */
        bool m_outBuffer [ SEQUENCE_LENGTH ];

        /**
         * @brief
         */
        int m_bufferForDataMem;

        /**
         * @brief
         */
        int m_bufferForProgMem;
};

#endif // PIC8ISP_H

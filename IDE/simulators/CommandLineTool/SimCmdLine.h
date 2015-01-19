// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup SimCmdLine
 * @file SimCmdLine.h
 */
// =============================================================================

#ifndef SIMCMDLINE_H
#define SIMCMDLINE_H

// Forward declarations.
class DbgFile;
class DataFile;
class AdjSimProcDef;
class MCUSimControl;

// Standard headers.
#include <string>

// Boost regular expressions library.
#include "boost/regex.hpp"

/**
 * @brief
 * @ingroup SimCmdLine
 * @class SimCmdLine
 */
class SimCmdLine
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const char * VERSION;

        /// @brief
        static const boost::regex m_reOctalPrefix;
        static const boost::regex m_reOctalSuffix;
        static const boost::regex m_reBinaryPrefix;
        static const boost::regex m_reBinarySuffix;
        static const boost::regex m_reDecimalPrefix;
        static const boost::regex m_reDecimalSuffix;
        static const boost::regex m_reHexadecimalPrefix;
        static const boost::regex m_reHexadecimalSuffix;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ExitCode
        {
            EXIT_CODE_SUCCESS    = 0, ///<
            EXIT_ERROR_SIMULATOR = 1, ///<
            EXIT_ERROR_CLI       = 2  ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] argc
         * @param[in] argv
         * @return
         */
        ExitCode main ( int argc,
                        char ** argv );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[out] word
         * @param[in] input
         * @param[in,out] pos
         */
        void readWord ( std::string & word,
                        const std::string & input,
                        unsigned int & pos);

        /**
         * @brief
         * @param[out] out
         * @param[in] in
         * @return
         */
        bool readInt ( int & out,
                       const std::string & in );

        /**
         * @brief
         * @param[in] executable
         */
        void printUsage ( const char * executable );

        /**
         * @brief
         */
        void printIntro();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] executable
         */
        inline void printHelp ( const char * executable );

        /**
         * @brief
         * @param[in] argc
         * @param[in] argv
         * @return
         */
        inline ExitCode parseCLI ( int argc,
                                   char ** argv );

        /**
         * @brief
         * @return
         */
        inline ExitCode initializeSim();

        /**
         * @brief
         * @return
         */
        inline ExitCode commandLoop();

        /**
         * @brief
         * @param[in] command
         * @param[in] subcommand
         * @param[in] input
         * @param[in,out] pos
         */
        inline void processCommand ( const std::string & command,
                                     const std::string & subcommand,
                                     const std::string & input,
                                     unsigned int & pos );

    ////    Private Attributes    ////
    private:
        /// @name Basic settings
        //@{
            std::string m_procDefFile;
            std::string m_device;
            std::string m_debugFile;
            std::string m_codeFile;
            std::string m_codeFileType;
            bool m_silent;
        //@}

        /// @name Simulator objects
        //@{
            DbgFile * m_dbgFile;
            DataFile * m_dataFile;
            AdjSimProcDef * m_procDef;
            MCUSimControl * m_simControl;
        //@}
};

#endif // SIMCMDLINE_H

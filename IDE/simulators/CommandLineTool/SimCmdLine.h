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

#include "SimCmdLineBase.h"

/**
 * @brief
 * @ingroup SimCmdLine
 * @class SimCmdLine
 */
class SimCmdLine : public SimCmdLineBase
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const char * VERSION;

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
         */
        inline void setUpEventObserver();

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

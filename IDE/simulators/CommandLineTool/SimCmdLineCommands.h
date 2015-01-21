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
 * @file SimCmdLineCommands.h
 */
// =============================================================================

#ifndef SIMCMDLINECOMMANDS_H
#define SIMCMDLINECOMMANDS_H

// Forward declarations.
class MCUSimControl;

// Standard headers.
#include <map>
#include <string>

#include "SimCmdLineBase.h"
#include "SimCmdLineObserver.h"

// Boost regular expressions library.
#include "boost/regex.hpp"

/**
 * @brief
 * @ingroup SimCmdLine
 * @class SimCmdLineCommands
 */
class SimCmdLineCommands : public SimCmdLineBase
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum MemorySpace
        {
            SP_PORTIN,  ///<
            SP_PORTOUT, ///<
            SP_DATA,    ///<
            SP_CODE,    ///<
            SP_STACK,   ///<
            SP_REGS     ///<
        };

    ////    Private Static Constants    ////
    private:
        ///
        static const std::map<std::string, MemorySpace> S_MEM_SPACE;

        /// @name
        //@{
            ///
            static const boost::regex m_reOctalPrefix;

            ///
            static const boost::regex m_reOctalSuffix;

            ///
            static const boost::regex m_reBinaryPrefix;

            ///
            static const boost::regex m_reBinarySuffix;

            ///
            static const boost::regex m_reDecimalPrefix;

            ///
            static const boost::regex m_reDecimalSuffix;

            ///
            static const boost::regex m_reHexadecimalPrefix;

            ///
            static const boost::regex m_reHexadecimalSuffix;
        //@}

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] simControl
         */
        SimCmdLineCommands ( MCUSimControl * simControl ) : m_simControl ( simControl ) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        ExitCode commandLoop();

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

    ////    Inline Private Operations    ////
    private:
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

        /**
         * @brief
         * @param[in] subcommand
         */
        inline void helpCommand ( const std::string & subcommand );

        /**
         * @brief
         * @param[in] flag
         */
        inline void flagCommand ( const std::string & flag );

        /**
         * @brief
         * @param[in] flag
         * @param[in] value
         */
        inline void flagCommand ( const std::string & flag,
                                  bool value );

        /**
         * @brief
         * @param[in] file
         * @param[in] space
         * @param[in] type
         * @param[in] save
         */
        inline void fileCommand ( const std::string & file,
                                  MemorySpace space,
                                  FileType type,
                                  bool save );

        /**
         * @brief
         * @param[in] space
         * @param[in] address
         * @param[in] value
         */
        inline void setMemoryCommand ( MemorySpace space,
                                       unsigned int address,
                                       unsigned int value );

        /**
         * @brief
         * @param[in] space
         * @param[in] address
         * @param[in] endAddress
         */
        inline void getMemoryCommand ( MemorySpace space,
                                       const std::string & address,
                                       const std::string & endAddress );

        /**
         * @brief
         * @param[in] what
         * @param[in] size
         */
        inline void sizeCommand ( MemorySpace what,
                                  int size );

        /**
         * @brief
         * @param[in] what
         */
        inline void sizeCommand ( MemorySpace what );

        /**
         * @brief
         * @param[in] file
         * @param[in] line
         * @param[in] value
         */
        inline void breakpointCommand ( const std::string & file,
                                        int line,
                                        bool value );

        /**
         * @brief
         */
        inline void done();

    ////    Private Attributes    ////
    private:
        ///
        MCUSimControl * const m_simControl;

        ///
        SimCmdLineObserver m_eventObserver;
};

#endif // SIMCMDLINECOMMANDS_H

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup MoraviaScript
 * @file MScriptCore.h
 */
// =============================================================================

#ifndef MSCRIPTCORE_H
#define MSCRIPTCORE_H

// Forward declarations.
class MScriptStrategy;
class MScriptStatement;

// MScript language interpreter header files.
#include "MScriptParserInterface.h"

// Standard header files.
#include <vector>
#include <string>

/**
 * @brief
 * @class MScriptCore
 * @ingroup MoraviaScript
 */
class MScriptCore : private MScriptParserInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptCore ( MScriptStrategy * strategy,
                      const std::string & script = "" );

        /**
         * @brief
         */
        virtual ~MScriptCore();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] script
         * @return
         */
        bool loadScript ( const std::string & script );

        /**
         * @brief
         */
        void unloadScript();

        /**
         * @brief
         * @return
         */
        bool executeStep();

        /**
         * @brief
         * @return
         */
        bool executeRun();

        /**
         * @brief
         * @return
         */
        std::vector<std::string> & getMessages();

        /**
         * @brief
         */
        void clearMessages();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        virtual void syntaxAnalysisComplete ( MScriptStatement * codeTree );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptStrategy * const m_strategy;

        /// @brief
        MScriptStatement * m_codeTree;

        /// @brief
        std::vector<std::string> m_messages;
};

#endif // MSCRIPTCORE_H

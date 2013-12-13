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
 * @ingroup MoraviaScript
 * @file MScript.h
 */
// =============================================================================

#ifndef MSCRIPT_H
#define MSCRIPT_H

// Forward declarations.
class MScriptCore;
class MScriptStrategy;

// MScript language interpreter header files.
#include "MScriptBase.h"

// Standard header files.
#include <vector>
#include <string>
#include <cstdio>

/**
 * @brief
 * @class MScript
 * @ingroup MoraviaScript
 */
class MScript
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] strategy
         * @param[in] scriptCode
         */
        MScript ( MScriptStrategy * strategy,
                  const std::string & scriptCode = "" );

        /**
         * @brief
         * @param[in,out] strategy
         * @param[in,out] sourceFile
         */
        MScript ( MScriptStrategy * strategy,
                  FILE * sourceFile,
                  const std::string & fileName );

        /**
         * @brief
         */
        virtual ~MScript();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] scriptCode
         * @return
         */
        bool loadScript ( const std::string & scriptCode );

        /**
         * @brief
         * @param[in,out] sourceFile
         * @param[in] fileName
         * @return
         */
        bool loadScript ( FILE * sourceFile,
                          const std::string & fileName );

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

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptCore * const m_core;
};

#endif // MSCRIPT_H

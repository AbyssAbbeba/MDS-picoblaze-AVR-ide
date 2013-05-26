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
 * @ingroup Compiler
 * @file CompilerMsgIntfFile.h
 */
// =============================================================================

#ifndef COMPILERMSGINTFFILE_H
#define COMPILERMSGINTFFILE_H

#include "CompilerMsgInterface.h"

// Standard header files.
#include <fstream>

class CompilerMsgIntfFile : public CompilerMsgInterface
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         */
        void openFile ( const std::string & filename );

        /**
         * @brief
         */
        void closeFile();

        /**
         * @brief
         */
        bool isFileOpened();

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        void message ( const std::string & text,
                       CompilerBase::MessageType type = CompilerBase::MT_GENERAL );

    ////    Private Attributes    ////
    private:
        /// @brief
        std::ofstream m_ofstream;
};

#endif // COMPILERMSGINTFFILE_H

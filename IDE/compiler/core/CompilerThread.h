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
 * @file CompilerThread.h
 */
// =============================================================================


#ifndef COMPILERTHREAD_H
#define COMPILERTHREAD_H

// Forward declarations
class CompilerCore;

// Compiler header files.
#include "CompilerBase.h"
#include "CompilerMsgInterface.h"
#include "CompilerOptions.h"

// Standard header files.
#include <string>

// Qt header files.
#include <QThread>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerThread
 */
class CompilerThread : public QThread,
                       protected CompilerMsgInterface
{
    Q_OBJECT

    ////    Protected Datatypes    ////
    protected:
        /**
         * @brief
         */
        struct JobSpec
        {
            ///
            CompilerBase::LangId m_lang;

            ///
            CompilerBase::TargetArch m_arch;

            ///
            CompilerOptions * m_opts;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerThread();

        /**
         * @brief
         */
        virtual ~CompilerThread();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] lang
         * @param[in] arch
         * @param[in] opts
         * @param[in] filename
         */
        void compile ( CompilerBase::LangId lang,
                       CompilerBase::TargetArch arch,
                       CompilerOptions * const opts );

    ////    Qt signals    ////
    signals:
        /**
         * @brief
         * @param[in] success
         */
        void compilationFinished ( bool success );

        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        void compilationMessage ( const std::string & text,
                                  CompilerBase::MessageType type );

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         */
        void run();

        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        void message ( const std::string & text,
                       CompilerBase::MessageType type = CompilerBase::MT_GENERAL );

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        CompilerCore * const m_compilerCore;

        /**
         * @brief
         */
        JobSpec m_jobSpec;
};

#endif // COMPILERTHREAD_H

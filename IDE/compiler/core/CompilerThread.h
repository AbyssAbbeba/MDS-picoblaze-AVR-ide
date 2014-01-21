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
 * @ingroup Compiler
 * @file CompilerThread.h
 */
// =============================================================================


#ifndef COMPILERTHREAD_H
#define COMPILERTHREAD_H

// Forward declarations
class CompilerCore;
class DbgFile;
class DataFile;

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

            ///
            bool m_genSimData;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] baseIncludeDir
         */
        CompilerThread ( const std::string & baseIncludeDir );

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
         * @param[in] genSimData
         */
        void compile ( CompilerBase::LangId lang,
                       CompilerBase::TargetArch arch,
                       CompilerOptions * const opts,
                       bool genSimData = false );

    ////    Qt signals    ////
    signals:
        /**
         * @brief
         * @param[out] success
         */
        void compilationFinished ( bool success );

        /**
         * @brief
         * @param[out] dbgFile
         * @param[out] dataFile
         */
        void simDataGenerated ( DbgFile * dbgFile,
                                DataFile * dataFile );

        /**
         * @brief
         * @param[out] text
         * @param[out] type
         */
        void compilationMessage ( const std::string & text,
                                  CompilerBase::MessageType type );

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         */
        virtual void run() override;

        /**
         * @brief
         * @param[in] text
         * @param[in] type
         */
        virtual void message ( const std::string & text,
                               CompilerBase::MessageType type = CompilerBase::MT_GENERAL ) override;

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

// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file CompilerThread.cxx
 */
// =============================================================================

// Compiler header files
#include "CompilerThread.h"
#include "CompilerCore.h"

CompilerThread::CompilerThread() : m_compilerCore ( new CompilerCore(this) )
{
}

CompilerThread::~CompilerThread()
{
    wait();

    delete m_compilerCore;
}

void CompilerThread::run()
{
    bool success = m_compilerCore->compile ( m_jobSpec.m_lang,
                                             m_jobSpec.m_arch,
                                             m_jobSpec.m_opts );

    emit(compilationFinished(success));
}

void CompilerThread::compile ( CompilerBase::LangId lang,
                               CompilerBase::TargetArch arch,
                               CompilerOptions * const opts )
{
    m_jobSpec.m_lang = lang;
    m_jobSpec.m_arch = arch;
    m_jobSpec.m_opts = opts;

    start();
}

void CompilerThread::message ( const std::string & text,
                               CompilerBase::MessageType type )
{
    emit(compilationMessage(text, type));
}

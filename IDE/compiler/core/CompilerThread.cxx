// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerThread.cxx
 */
// =============================================================================

// Compiler header files
#include "CompilerThread.h"
#include "CompilerCore.h"

CompilerThread::CompilerThread ( const std::string & baseIncludeDir )
                               : m_compilerCore ( new CompilerCore ( this ) )
{
    m_compilerCore->setBaseIncludeDir(baseIncludeDir);
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
                                             m_jobSpec.m_opts,
                                             m_jobSpec.m_genSimData );

    if ( true == m_jobSpec.m_genSimData && true == success )
    {
        emit(simDataGenerated(m_compilerCore->getSimDbg(), m_compilerCore->getSimData()));
    }
    emit(compilationFinished(success));
}

void CompilerThread::compile ( CompilerBase::LangId lang,
                               CompilerBase::TargetArch arch,
                               CompilerOptions * const opts,
                               bool genSimData )
{
    m_jobSpec.m_lang = lang;
    m_jobSpec.m_arch = arch;
    m_jobSpec.m_opts = opts;
    m_jobSpec.m_genSimData = genSimData;

    start();
}

void CompilerThread::message ( const std::string & text,
                               CompilerBase::MessageType type )
{
    emit(compilationMessage(text, type));
}

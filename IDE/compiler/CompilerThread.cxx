// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerThread.cxx
 */
// =============================================================================

// Compiler header files
#include "Compiler.h"
#include "CompilerThread.h"

CompilerThread::CompilerThread ( const std::string & baseIncludeDir )
                               :
                                 m_compiler ( new Compiler(this, baseIncludeDir) )
{
}

CompilerThread::~CompilerThread()
{
    wait();

    delete m_compiler;
}

void CompilerThread::run()
{
    bool success = m_compiler->compile ( m_jobSpec.m_lang,
                                         m_jobSpec.m_arch,
                                         m_jobSpec.m_opts,
                                         m_jobSpec.m_genSimData );

    if ( true == m_jobSpec.m_genSimData && true == success )
    {
        emit(simDataGenerated(m_compiler->getSimDbg(), m_compiler->getSimData()));
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

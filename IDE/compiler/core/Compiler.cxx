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
 * @file Compiler.cxx
 */
// =============================================================================

#include "Compiler.h"

// Compiler header files.
#include "CompilerCore.h"
#include "CompilerMsgInterface.h"

Compiler::Compiler ( CompilerMsgInterface * msgInterface,
                     std::string baseIncludeDir )
                   : m_compilerCore ( new CompilerCore(msgInterface) )
{
    setBaseIncludeDir(baseIncludeDir);
}

Compiler::~Compiler()
{
    delete m_compilerCore;
}

bool Compiler::compile ( CompilerBase::LangId lang,
                         CompilerBase::TargetArch arch,
                         CompilerOptions * const opts,
                         bool genSimData )
{
    return m_compilerCore -> compile ( lang, arch, opts, genSimData );
}

void Compiler::setBaseIncludeDir ( const std::string & directory )
{
    m_compilerCore->setBaseIncludeDir(directory);
}

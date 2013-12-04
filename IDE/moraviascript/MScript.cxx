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
 * @ingroup MoraviaScript
 * @file MScript.cxx
 */
// =============================================================================

#include "MScript.h"

// MScript language interpreter header files.
#include "MScriptCore.h"
#include "MScriptStrategy.h"

MScript::MScript ( MScriptStrategy * strategy,
                   const std::string & scriptCode )
                 :
                   m_core ( new MScriptCore(strategy, scriptCode) )
{
}

MScript::MScript ( MScriptStrategy * strategy,
                   FILE * sourceFile,
                   const std::string & fileName )
                 :
                   m_core ( new MScriptCore(strategy, sourceFile, fileName) )
{
}

MScript::~MScript()
{
    delete m_core;
}

bool MScript::loadScript ( const std::string & scriptCode )
{
    return m_core->loadScript(scriptCode);
}

bool MScript::loadScript ( FILE * sourceFile,
                           const std::string & fileName )
{
    return m_core->loadScript(sourceFile, fileName);
}

void MScript::unloadScript()
{
    m_core->unloadScript();
}

bool MScript::executeStep()
{
    m_core->step();
    return true;
}

bool MScript::executeRun()
{
    while ( true == m_core->step() );
    return true;
}

std::vector<std::string> & MScript::getMessages()
{
    return m_core->m_messages;
}

void MScript::clearMessages()
{
    m_core->m_messages.clear();
}

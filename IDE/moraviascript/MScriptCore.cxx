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
 * @ingroup MoraviaScript
 * @file MScriptCore.cxx
 */
// =============================================================================

#include "MScriptCore.h"

MScriptCore::MScriptCore ( MScriptStrategy * strategy,
                           const std::string & script = "" )
                         : m_strategy ( strategy )
{
    m_codeTree = NULL;
    loadScript(script);
}

MScriptCore::~MScriptCore()
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }
}

bool MScriptCore::loadScript ( const std::string & script )
{
    unloadScript();
}

void MScriptCore::unloadScript()
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
        m_codeTree = NULL;
    }
    m_messages.clear();
}

bool MScriptCore::executeStep()
{

}

bool MScriptCore::executeRun()
{

}

std::vector<std::string> & MScriptCore::getMessages()
{
    return m_messages;
}

void MScriptCore::clearMessages()
{
    m_messages.clear();
}

void MScriptCore::syntaxAnalysisComplete ( MScriptStatement * codeTree )
{
    if ( NULL != m_codeTree )
    {
        m_codeTree->completeDelete();
    }
    m_codeTree = codeTree;
}

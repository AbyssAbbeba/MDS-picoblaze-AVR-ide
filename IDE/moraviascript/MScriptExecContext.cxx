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
 * @file MScriptExecContext.cxx
 */
// =============================================================================

#include "MScriptExecContext.h"

void MScriptExecContext::init ( const MScriptStatement * rootNode )
{
    clear();
    m_programPointer.push_back(ProgPtr(rootNode, FLAG_NORMAL));
}

void MScriptExecContext::clear()
{
    m_programPointer.clear();
}

void MScriptExecContext::popNext()
{
    if ( false == m_programPointer.empty() )
    {
        m_programPointer.pop_back();
    }
}

const MScriptStatement * MScriptExecContext::getNextNode() const
{
    return m_programPointer.back().first;
}

MScriptExecContext::ExecFlags MScriptExecContext::getNextFlags() const
{
    return m_programPointer.back().second;
}

void MScriptExecContext::addNext ( const MScriptStatement * node,
                                   MScriptExecContext::ExecFlags flags )
{
    if ( NULL != node )
    {
        m_programPointer.push_back(ProgPtr(node, flags));
    }
}

void MScriptExecContext::setNextFlags ( MScriptExecContext::ExecFlags flags )
{
    if ( false == m_programPointer.empty() )
    {
        m_programPointer.back().second = flags;
    }
}

void MScriptExecContext::replaceNext ( const MScriptStatement * node,
                                       MScriptExecContext::ExecFlags flags )
{
    popNext();
    addNext(node, flags);
}

void MScriptExecContext::cutOffBranch ( unsigned int level )
{
    while ( 0 != level )
    {
        m_programPointer.pop_back();
        level--;
    }
}

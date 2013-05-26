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
 * @file CompilerMsgFilter.cxx
 */
// =============================================================================

// Compiler compiler header files.
#include "CompilerMsgFilter.h"
#include "CompilerOptions.h"
#include "CompilerCore.h"

CompilerMsgFilter::CompilerMsgFilter ( const CompilerCore * compilerCore,
                                       CompilerMsgInterface * msgInterface )
                                     : m_compilerCore ( compilerCore ),
                                       m_msgInterface ( msgInterface )
{
}

void CompilerMsgFilter::message ( const std::string & text,
                                  CompilerBase::MessageType type )
{
    if ( NULL == m_msgInterface )
    {
        return;
    }

    CompilerOptions::Verbosity verbosity = m_compilerCore->getCompilationOptions()->m_verbosity;

    switch ( type )
    {
        case CompilerBase::MT_GENERAL:
            if ( 0 == ( CompilerOptions::V_GENERAL  & verbosity ) )
            {
                return;
            }
            break;
        case CompilerBase::MT_ERROR:
            if ( 0 == ( CompilerOptions::V_ERRORS  & verbosity ) )
            {
                return;
            }
            break;
        case CompilerBase::MT_WARNING:
            if ( 0 == ( CompilerOptions::V_WARNINGS  & verbosity ) )
            {
                return;
            }
            break;
        case CompilerBase::MT_REMARK:
            if ( 0 == ( CompilerOptions::V_REMARKS  & verbosity ) )
            {
                return;
            }
            break;
        case CompilerBase::MT_INVALID:
            return;
    }

    m_msgInterface->message(text, type);
}

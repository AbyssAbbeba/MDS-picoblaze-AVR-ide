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
 * @file CompilerMsgFilter.cxx
 */
// =============================================================================

// Compiler compiler header files.
#include "CompilerMsgFilter.h"
#include "CompilerOptions.h"
#include "CompilerCore.h"

// Used for i18n only
#include <QObject>

CompilerMsgFilter::CompilerMsgFilter ( const CompilerCore * compilerCore,
                                       CompilerMsgInterface * msgInterface,
                                       int messageLimit )
                                     :
                                       m_messageLimit ( messageLimit ),
                                       m_compilerCore ( compilerCore ),
                                       m_msgInterface ( msgInterface )
{
    clear();
}

void CompilerMsgFilter::message ( const std::string & text,
                                  CompilerBase::MessageType type )
{
    if ( nullptr == m_msgInterface )
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

    if ( -1 != m_messageLimit )
    {
        if ( m_messageLimit == m_msgCounter )
        {
            m_msgInterface->message ( QObject::tr ( "maximum number of messages reached, suppressing compiler message "
                                                    "generation" )
                                                  . toStdString(),
                                      CompilerBase::MT_WARNING );
            m_msgCounter++;
            return;
        }
        else if ( m_messageLimit < m_msgCounter )
        {
            return;
        }
        m_msgCounter++;
    }
    m_msgInterface->message(text, type);
}

void CompilerMsgFilter::clear()
{
    m_msgCounter = 0;
}

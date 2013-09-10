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
 * @file MScriptVarTable.cxx
 */
// =============================================================================

#include "MScriptVarTable.h"

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptValue.h"
#include "MScriptInterpretInterface.h"

// Used for i18n only
#include <QObject>

void MScriptVarTable::pushScope()
{
    m_varTables.push_back(VarTable());
}

void MScriptVarTable::popScope()
{
    m_varTables.pop_back();
}

bool MScriptVarTable::remove ( const std::string & variable,
                               const MScriptSrcLocation & location )
{
    bool result = false;

    for ( std::vector<VarTable>::reverse_iterator varTab = m_varTables.rbegin();
          varTab != m_varTables.rend();
          varTab++ )
    {
        VarTable::iterator var = varTab->find(variable);

        if ( varTab->end() != var )
        {
            if ( false == result )
            {
                varTab->erase(var);
                result = true;
            }
            else
            {
                const std::string eclipsedVarLoc = var->second.m_location.toString();
                m_interpret->interpreterMessage ( location,
                                                  MScriptBase::MT_WARNING,
                                                  QObject::tr ( "variable with the same name (`%1') has also been "
                                                                "declared in upper scope at %2 (this variable is no "
                                                                "longer eclipsed)" ) . arg ( variable.c_str() )
                                                                                     . arg ( eclipsedVarLoc.c_str() )
                                                                                     . toStdString() );
                break;
            }
        }
    }

    if ( false == result )
    {
        m_interpret->interpreterMessage ( location,
                                          MScriptBase::MT_ERROR,
                                          QObject::tr ( "variable `%1' has not been declared (cannot delete "
                                                        "nonexistent variable)" ) . arg ( variable.c_str() )
                                                                                  . toStdString() );
    }

    return result;
}

bool MScriptVarTable::declare ( const std::string & variable,
                                const MScriptSrcLocation & location,
                                MScriptVariable::Flags flags,
                                unsigned int dimensions )
{
}

bool MScriptVarTable::assign ( const std::string & variable,
                               const MScriptSrcLocation & location,
                               const MScriptValue & value,
                               const Index & index )
{
}

const MScriptValue * MScriptVarTable::get  ( const std::string & variable,
                                             const MScriptSrcLocation & location,
                                             const Index & index )
{
}

bool MScriptVarTable::exists ( const std::string & variable ) const
{
}

bool MScriptVarTable::isDefined ( const std::string & variable,
                                  const Index & index ) const
{
}

MScriptVariable::Flags MScriptVarTable::getFlags ( const std::string & variable ) const
{
}

std::ostream & operator << ( std::ostream & out,
                             const MScriptVarTable & table )
{
    static const char * const sepLine0 = "==================================================";
    static const char * const sepLine1 = "--------------------------------------------------";

    int stackFrameNo = -1;
    for ( std::vector<MScriptVarTable::VarTable>::const_iterator varTab = table.m_varTables.cbegin();
          varTab != table.m_varTables.cend();
          varTab++ )
    {
        stackFrameNo++;
        out << sepLine0 << std::endl;
        out << "Scope level #" << stackFrameNo << ":" << std::endl;
        out << sepLine0 << std::endl;

        for ( MScriptVarTable::VarTable::const_iterator var = varTab->cbegin();
              var != varTab->cend();
              var++ )
        {
            out << "name: " << var->first << std::endl;
            out << "value(s): " << var->second << std::endl;
            out << sepLine1 << std::endl;
        }

        out << std::endl;
    }

    return out;
}

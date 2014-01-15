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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeMacros.cxx
 */
// =============================================================================

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeMacros.h"
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeCodeListing.h"

// Common compiler header files.
#include "CompilerStatementTypes.h"

// Standard headers.
#include <cstdio>
#include <cstdint>
#include <fstream>

AsmPicoBlazeMacros::Macro::Macro()
{
    m_definition = nullptr;
    m_id = -1;
}

AsmPicoBlazeMacros::AsmPicoBlazeMacros ( CompilerSemanticInterface * compilerCore,
                                         CompilerOptions * opts,
                                         AsmPicoBlazeSymbolTable * symbolTable,
                                         AsmPicoBlazeCodeListing * codeListing )
                                       : m_compilerCore ( compilerCore ),
                                         m_opts ( opts ),
                                         m_symbolTable ( symbolTable ),
                                         m_codeListing ( codeListing )
{
    clear();
}

AsmPicoBlazeMacros::~AsmPicoBlazeMacros()
{
    clear();
}

void AsmPicoBlazeMacros::setExpEnabled ( bool enabled )
{
    m_expEnabled = enabled;
}

void AsmPicoBlazeMacros::define ( CompilerSourceLocation location,
                                  const std::string & name,
                                  const CompilerExpr * parameters,
                                  CompilerStatement * macroDef )
{
    if ( m_table.cend() != m_table.find(name) )
    {
        if ( nullptr != m_table[name].m_definition )
        {
            m_table[name].m_definition->completeDelete();
        }

        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_WARNING,
                                            QObject::tr ( "redefinition of macro `%1', original definition is at: " )
                                                        . arg ( name.c_str() )
                                                        . toStdString()
                                                        + m_compilerCore->locationToStr(m_table[name].m_location) );
    }

    if ( macroDef->last() == macroDef )
    {
        macroDef->completeDelete();
        macroDef = new CompilerStatement();
    }
    else
    {
        // Remove `ENDM' statement.
        delete macroDef->last();
    }

    incrMacroCounter(macroDef);
    m_table[name] = Macro(location, m_idCounter++, macroDef);

    for ( const CompilerExpr * param = parameters;
          param != nullptr;
          param = param->m_next )
    {
        const char * paramName = param->m_lValue.m_data.m_symbol;
        if ( true == m_symbolTable->isDefined(paramName) )
        {
            const CompilerSourceLocation & symLocation = m_symbolTable->getValue(paramName)->location();
            if ( true == symLocation.isSet() )
            {
                m_compilerCore -> semanticMessage ( param->location(),
                                                    CompilerBase::MT_WARNING,
                                                    QObject::tr ( "macro parameter `%1' eclipses global symbol `%1', "
                                                                  "defined at: " )
                                                                . arg ( paramName )
                                                                . toStdString()
                                                                + m_compilerCore->locationToStr(symLocation) );
            }
            else
            {
                m_compilerCore -> semanticMessage ( param->location(),
                                                    CompilerBase::MT_WARNING,
                                                    QObject::tr ( "macro parameter `%1' eclipses global symbol `%1'" )
                                                                . arg ( paramName )
                                                                . toStdString() );
            }
        }
        m_table[name].m_parameters.push_back(paramName);
    }
}

bool AsmPicoBlazeMacros::isFromMacro ( const CompilerStatement * node ) const
{
    return ( 0 != ( 0xffff & node->m_userData ) );
}

void AsmPicoBlazeMacros::incrMacroCounter ( CompilerStatement * macro ) const
{
    if ( nullptr == macro )
    {
        return;
    }

    for ( CompilerStatement * node = macro;
          nullptr != node;
          node = node->next() )
    {
        uint16_t macroCounter = ( 0xffff & node->m_userData );
        macroCounter++;
        node->m_userData = ( ~0xffff & node->m_userData ) | ( 0xffff & macroCounter );
        incrMacroCounter(node->branch());
    }
}

CompilerStatement * AsmPicoBlazeMacros::expand ( const CompilerSourceLocation & location,
                                                 const std::string & name,
                                                 const CompilerExpr * arguments )
{
    if ( false == m_expEnabled )
    {
        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_REMARK,
                                            QObject::tr ( "macro expansion has been disabled, macro `%1' will not be "
                                                          "expanded" )
                                                        . arg ( name.c_str() )
                                                        . toStdString() );
        return nullptr;
    }

    if ( m_table.end() == m_table.find(name) )
    {
        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("macro not defined: ").toStdString() + "\"" + name + "\"" );
        return new CompilerStatement();
    }

    Macro & macro = m_table[name];
    CompilerStatement * result = macro.m_definition->copyEntireChain();

    macro.m_usageCounter++;

    m_expCounter++;
    m_codeListing->expandMacro ( location,
                                 macro.m_definition,
                                 result );

    // Substitute macro parameters with expansion arguments.
    int numberOfParams = (int) macro.m_parameters.size();
    int argNo = -1;
    for ( const CompilerExpr * arg = arguments;
          nullptr != arg;
          arg = arg->m_next )
    {
        argNo++;

        if ( numberOfParams <= argNo )
        {
            m_compilerCore -> semanticMessage ( arg->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr ( "too many arguments given to macro `%1', expecting at "
                                                              "most %2 argument(s)")
                                                           . arg ( name.c_str() )
                                                           . arg ( numberOfParams )
                                                           . toStdString() );
            break;
        }

        symbolSubst ( macro.m_parameters[argNo],
                      arg,
                      result );
    }

    // Substitute remaining parameters with "blank values".
    CompilerExpr blankExpr;
    blankExpr.m_location = location;
    for ( int i = argNo + 1; i < numberOfParams; i++ )
    {
        const std::string & param = macro.m_parameters[i];
        symbolSubst ( param, &blankExpr, result );
        m_compilerCore -> semanticMessage ( location,
                                            CompilerBase::MT_REMARK,
                                            QObject::tr ( "parameter `%1' substituted for blank value" )
                                                       . arg ( param.c_str() )
                                                       . toStdString() );
    }

    return result;
}

bool AsmPicoBlazeMacros::mangleName ( const CompilerSourceLocation & location,
                                      std::vector<std::string> * localSymbols,
                                      const std::string & local,
                                      const std::string & macroName,
                                      CompilerStatement * node )
{
    if ( nullptr == node )
    {
        return false;
    }

    bool result = false;
    bool found = false;

    for ( const auto & symbol : *localSymbols )
    {
        if ( symbol == local )
        {
            found = true;
            m_compilerCore -> semanticMessage ( location,
                                                CompilerBase::MT_WARNING,
                                                QObject::tr ( "symbol `%1' already declared as local" )
                                                            . arg(symbol.c_str()).toStdString() );
        }
    }

    if ( false == found )
    {
        char usageCounter[10];
        sprintf(usageCounter, "%d", m_table[macroName].m_usageCounter);
        const CompilerExpr mangledName(MANGLE_PREFIX + macroName + "." + local + "." + usageCounter);

        result |= symbolSubst ( local, &mangledName, node );
        localSymbols->push_back(local);

        delete [] mangledName.lVal().m_data.m_symbol;
    }

    return result;
}

bool AsmPicoBlazeMacros::symbolSubst ( const std::string & parameter,
                                       const CompilerExpr * argument,
                                       CompilerStatement * target )
{
    if ( nullptr == target )
    {
        return false;
    }

    bool result = false;

    for ( CompilerStatement * node = target;
          nullptr != node;
          node = node->next() )
    {
        if ( 0 != m_symbolTable->substitute ( parameter, argument, node->args() ) )
        {
            result = true;
        }
        result |= symbolSubst ( parameter, argument, node->branch() );
    }

    return result;
}


void AsmPicoBlazeMacros::output()
{
    if ( m_opts->m_macroTable.empty() )
    {
        return;
    }

    std::ofstream file ( m_opts->m_macroTable, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to open " ).toStdString()
                                                        + "\"" + m_opts -> m_macroTable  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> semanticMessage ( CompilerSourceLocation(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "Unable to write to " ).toStdString()
                                                        + "\"" + m_opts -> m_macroTable  + "\"" );
        return;
    }
}

void AsmPicoBlazeMacros::clear()
{
    m_idCounter = 0;
    m_expCounter = 0;
    m_expEnabled = true;

    for ( const auto & mac : m_table )
    {
        mac.second.m_definition->completeDelete();
    }

    m_table.clear();
}

void AsmPicoBlazeMacros::printSymLocation ( std::ostream & out,
                                           const CompilerSourceLocation & location ) const
{
    if ( false == location.isSet() )
    {
        return;
    }

    out << m_compilerCore->locationToStr(location);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmPicoBlazeMacros * macros )
{
    for ( auto mac = macros->m_table.cbegin();
          mac != macros->m_table.cend();
          mac++ )
    {
        out << mac->first;

        std::string parameters;
        for ( const auto & param : mac->second.m_parameters )
        {
            if ( false == parameters.empty() )
            {
                parameters += ", ";
            }
            parameters += param;
        }
        out << " ( ";
        out << parameters;
        out << " ) ";

        for ( unsigned int i = ( mac->first.size() + parameters.size() ); i < 50; i++ )
        {
            if ( i % 2 )
            {
                out << " ";
            }
            else
            {
                out << ".";
            }
        }

        if ( 0 != mac->second.m_usageCounter)
        {
            char usageCounter[13];
            sprintf(usageCounter, "%3d × USED ", mac->second.m_usageCounter);
            out << usageCounter;
        }
        else
        {
            out << "  NOT USED ";
        }

        macros->printSymLocation(out, mac->second.m_location);

        out << std::endl;
    }

    return out;
}

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
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3Macros.cxx
 */
// =============================================================================

// KCPSM3 assembler semantic analyzer header files.
#include "AsmKcpsm3Macros.h"
#include "AsmKcpsm3SymbolTable.h"
#include "AsmKcpsm3CodeListing.h"

// Standard headers.
#include <cstdio>
#include <fstream>

AsmKcpsm3Macros::Macro::Macro()
{
    m_id = -1;
}

AsmKcpsm3Macros::AsmKcpsm3Macros ( CompilerSemanticInterface * compilerCore,
                                   CompilerOptions * opts,
                                   AsmKcpsm3SymbolTable * symbolTable,
                                   AsmKcpsm3CodeListing * codeListing )
                                 : m_compilerCore ( compilerCore ),
                                   m_opts ( opts ),
                                   m_symbolTable ( symbolTable ),
                                   m_codeListing ( codeListing )
{
    m_idCounter = 0;
    m_expCounter = 0;
}

AsmKcpsm3Macros::~AsmKcpsm3Macros()
{
}

void AsmKcpsm3Macros::define ( CompilerBase::SourceLocation location,
                               const std::string & name,
                               const CompilerExpr * parameters,
                               CompilerStatement * macroDef )
{
    if ( m_table.cend() != m_table.find(name) )
    {
        if ( NULL != m_table[name].m_definition )
        {
            delete m_table[name].m_definition;
        }

        m_compilerCore -> compilerMessage ( location,
                                            CompilerBase::MT_WARNING,
                                            QObject::tr("redefinition of macro ").toStdString() + "\"" + name + "\"" + QObject::tr("original definition is at ").toStdString() + m_compilerCore->locationToStr(m_table[name].m_location) );
    }

    m_table[name] = Macro(location, m_idCounter++);
    m_table[name].m_definition = macroDef;

    for ( const CompilerExpr * param = parameters;
          param != NULL;
          param = param->m_next )
    {
        m_table[name].m_parameters.push_back(param->m_lValue.m_data.m_symbol);
    }
}

CompilerStatement * AsmKcpsm3Macros::expand ( CompilerBase::SourceLocation location,
                                              const std::string & name,
                                              const CompilerExpr * arguments )
{
    if ( m_table.end() == m_table.find(name) )
    {
        m_compilerCore -> compilerMessage ( location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("macro not defined: ").toStdString() + "\"" + name + "\"" );
        return new CompilerStatement();
    }

    Macro & macro = m_table[name];
    CompilerStatement * result = macro.m_definition->copyEntireChain();

    macro.m_usageCounter++;

    int numberOfParams = (int) macro.m_parameters.size();
    int argNo = -1;
    for ( const CompilerExpr * arg = arguments;
          arg != NULL;
          arg = arg->m_next )
    {
        argNo++;

        if ( numberOfParams <= argNo )
        {
            m_compilerCore -> compilerMessage ( arg->m_location,
                                                CompilerBase::MT_ERROR,
                                                QObject::tr("too many arguments given, expecting at most %1 arguments").arg(numberOfParams).toStdString() );
            break;
        }

        paramSubst ( macro.m_parameters[argNo],
                     arg,
                     result );
    }

    // Substitute remaining parameters with "blank values".
    const CompilerExpr blankExpr;
    for ( int i = argNo; i < numberOfParams; i++ )
    {
        paramSubst ( macro.m_parameters[i], &blankExpr, result );
    }

    m_expCounter++;
    m_codeListing->expandMacro ( location,
                                 macro.m_definition,
                                 result );

    return result;
}

void AsmKcpsm3Macros::paramSubst ( const std::string & parameter,
                                   const CompilerExpr * argument,
                                   CompilerStatement * target )
{
    if ( NULL == target )
    {
        return;
    }

    paramSubst ( parameter, argument, target->next() );
    paramSubst ( parameter, argument, target->branch() );

    m_symbolTable->substitute ( parameter, argument, target->args() );
}


void AsmKcpsm3Macros::output()
{
    if ( m_opts->m_macroTable.empty() )
    {
        return;
    }

    if ( true == m_opts->m_makeBackupFiles )
    {
        rename(m_opts->m_macroTable.c_str(), (m_opts->m_macroTable + "~").c_str());
    }

    std::ofstream file ( m_opts->m_macroTable, ( std::fstream::out | std::fstream::trunc ) );

    if ( false == file.is_open() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to open ").toStdString() + "\"" + m_opts -> m_macroTable  + "\"" );
        return;
    }

    file << this;

    if ( true == file.bad() )
    {
        m_compilerCore -> compilerMessage ( CompilerBase::MT_ERROR,
                                            QObject::tr("Unable to write to ").toStdString() + "\"" + m_opts -> m_macroTable  + "\"" );
        return;
    }
}

void AsmKcpsm3Macros::clear()
{
    m_table.clear();
}

void AsmKcpsm3Macros::printSymLocation ( std::ostream & out,
                                        const CompilerBase::SourceLocation & location ) const
{
    if ( false == location.isSet() )
    {
        return;
    }

    out << m_compilerCore->locationToStr(location);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmKcpsm3Macros * macros )
{
    for ( std::map<std::string,AsmKcpsm3Macros::Macro>::const_iterator mac = macros->m_table.cbegin();
          mac != macros->m_table.cend();
          mac++ )
    {
        out << mac->first;

        std::string parameters;
        for ( std::vector<std::string>::const_iterator param = mac->second.m_parameters.cbegin();
              param != mac->second.m_parameters.cend();
              param++ )
        {
            if ( false == parameters.empty() )
            {
                parameters += ", ";
            }
            parameters += *param;
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

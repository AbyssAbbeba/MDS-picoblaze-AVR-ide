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
 * @file AsmPicoBlazeTreeDecoder.cxx
 */
// =============================================================================

#include "AsmPicoBlazeTreeDecoder.h"

// Common compiler header files.
#include "AsmDgbFileGen.h"
#include "CompilerExpr.h"
#include "CompilerOptions.h"
#include "CompilerStatement.h"
#include "AsmMachineCodeGen.h"
#include "CompilerSourceLocation.h"
#include "CompilerSemanticInterface.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeMacros.h"
#include "AsmPicoBlazeMemoryPtr.h"
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeCodeListing.h"
#include "AsmPicoBlazeSpecialMacros.h"
#include "AsmPicoBlazeInstructionSet.h"

// Standard headers.
#include <vector>

#define HANDLE_ACTION(arg)                  \
    switch ( arg )                          \
    {                                       \
        case CA_NO_ACTION:    break;        \
        case CA_CONTINUE:     continue;     \
        case CA_RETURN_TRUE:  return true;  \
        case CA_RETURN_FALSE: return false; \
    }

AsmPicoBlazeTreeDecoder::AsmPicoBlazeTreeDecoder ( CompilerSemanticInterface            * compilerCore,
                                                   CompilerOptions                      * opts,
                                                   AsmDgbFileGen                        * dgbFile,
                                                   AsmMachineCodeGen                    * machineCode,
                                                   AsmPicoBlazeMacros                   * macros,
                                                   AsmPicoBlazeMemoryPtr                * memoryPtr,
                                                   AsmPicoBlazeSymbolTable              * symbolTable,
                                                   AsmPicoBlazeCodeListing              * codeListing,
                                                   AsmPicoBlazeSpecialMacros            * specialMacros,
                                                   AsmPicoBlazeInstructionSet           * instructionSet,
                                                   AsmPicoBlazeSemanticAnalyzer::Device & device )
                                                 :
                                                   m_compilerCore   ( compilerCore   ),
                                                   m_opts           ( opts           ),
                                                   m_dgbFile        ( dgbFile        ),
                                                   m_machineCode    ( machineCode    ),
                                                   m_macros         ( macros         ),
                                                   m_memoryPtr      ( memoryPtr      ),
                                                   m_symbolTable    ( symbolTable    ),
                                                   m_codeListing    ( codeListing    ),
                                                   m_specialMacros  ( specialMacros  ),
                                                   m_instructionSet ( instructionSet ),
                                                   m_device         ( device         )
{
}

bool AsmPicoBlazeTreeDecoder::phase1 ( CompilerStatement * codeTree,
                                       const CompilerSourceLocation * origLocation,
                                       const std::string * macroName )
{
    using namespace CompilerStatementTypes;

    std::vector<std::string> localSymbols;

    m_forceNext = NULL;

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        if ( NULL != m_forceNext )
        {
            node = m_forceNext->next();
            if ( NULL == node )
            {
                break;
            }
            m_forceNext = NULL;
        }

        const CompilerSourceLocation * location = origLocation;
        if ( NULL == location )
        {
            location = &(node->location());
        }

        if ( true == m_instructionSet->isInstruction ( node ) )
        {
            m_instructionSet->encapsulate(node, m_memoryPtr->m_code);
            m_memoryPtr->m_code++;
            continue;
        }

        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_EQU:
            case ASMPICOBLAZE_DIR_REG:
            case ASMPICOBLAZE_DIR_CODE:
            case ASMPICOBLAZE_DIR_PORT:
            case ASMPICOBLAZE_DIR_DATA:
                dir_EQU_etc(node, location);
                break;

            case ASMPICOBLAZE_DIR_AUTOREG:
            case ASMPICOBLAZE_DIR_AUTOSPR:
                dir_AUTOxxx(node, location);
                break;

            case ASMPICOBLAZE_LOCAL:
                dir_LOCAL ( node, location, macroName, localSymbols );
                break;

            case ASMPICOBLAZE_DIR_END:
                break;

            case ASMPICOBLAZE_MACRO:       HANDLE_ACTION( macro       ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_RTFOR:   HANDLE_ACTION( dir_RTFOR   ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_REPT:    HANDLE_ACTION( dir_REPT    ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_WHILE:   HANDLE_ACTION( dir_WHILE   ( node, location ) ); break;
            case ASMPICOBLAZE_RT_COND:     HANDLE_ACTION( dir_RTCOND  ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_DEVICE:  HANDLE_ACTION( dir_DEVICE  ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_RTWHILE: HANDLE_ACTION( dir_RTWHILE ( node, location ) ); break;
            case ASMPICOBLAZE_DIR_EXITM:   HANDLE_ACTION( dir_EXITM   ( node ) );           break;

            case ASMPICOBLAZE_DIR_DB:       dir_DB       ( node, location ); continue;
            case ASMPICOBLAZE_DIR_ORG:      dir_ORG      ( node );           continue;
            case ASMPICOBLAZE_DIR_SKIP:     dir_SKIP     ( node );           continue;
            case ASMPICOBLAZE_COND_ASM:     dir_IF       ( node );           break;
            case ASMPICOBLAZE_DIR_LIST:     dir_LIST     ( node );           break;
            case ASMPICOBLAZE_DIR_NOLIST:   dir_NOLIST   ( node );           break;
            case ASMPICOBLAZE_DIR_MACRO:    dir_MACRO    ( node );           break;
            case ASMPICOBLAZE_DIR_TITLE:    dir_TITLE    ( node );           break;
            case ASMPICOBLAZE_INCLUDE:      dir_INCLUDE  ( node );           break;
            case ASMPICOBLAZE_DIR_UNDEFINE: dir_UNDEFINE ( node );           break;
            case ASMPICOBLAZE_LABEL:        label        ( node, location ); break;
            case ASMPICOBLAZE_DIR_SET:      dir_SET      ( node, location ); break;
            case ASMPICOBLAZE_DIR_LIMIT:    dir_LIMIT    ( node, location ); break;
            case ASMPICOBLAZE_DIR_ERROR:    dir_ERROR    ( node, location ); break;
            case ASMPICOBLAZE_DIR_MESSG:    dir_MESSG    ( node, location ); break;
            case ASMPICOBLAZE_DIR_DEFINE:   dir_DEFINE   ( node, location ); break;
            case ASMPICOBLAZE_DIR_WARNING:  dir_WARNING  ( node, location ); break;

            case ASMPICOBLAZE_DIR_EXPAND:   dir_EXPAND   (); break;
            case ASMPICOBLAZE_DIR_NOEXPAND: dir_NOEXPAND (); break;

            default:
                // For now keep the nodes which were not understood yet.
                continue;
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }

    return true;
}

void AsmPicoBlazeTreeDecoder::phase2 ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    m_symbolTable->maskNonLabels();

    for ( CompilerStatement * node = codeTree->next();
          NULL != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case EMPTY_STATEMENT:
                // Do not interpret empty statements and let them be deleted (later in this method).
                break;

            case ASMPICOBLAZE_DIR_ORG:
                m_machineCode->setOrigin((uint32_t)m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_SKIP:
                m_machineCode->incrementAddr((unsigned int)m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_DB:
                for ( CompilerExpr * arg = node->args();
                      NULL != arg;
                      arg = arg->next() )
                {
                    long long code = arg->lVal().m_data.m_integer;
                    int address = m_machineCode->setCode ( (uint32_t) code );
                    m_codeListing -> setCode ( node->location(), (int) code, address );
                    m_dgbFile -> setCode ( node->location(), (int) code, address );
                    m_memoryPtr -> tryReserve ( node->location(), AsmPicoBlazeMemoryPtr::MS_CODE, address );
                }
                break;

            default: // Only instructions are expected here, anything else will be ignored and deleted.
            {
                int opcode = m_instructionSet->resolveOPcode(node);
                if ( -1 == opcode )
                {
                    m_compilerCore->compilerMessage ( node->location(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "instruction not supported on the this device: " )
                                                                  . toStdString()
                                                                  + m_instructionSet->getInstructionName(node) );
                }
                else
                {
                    int address = m_machineCode -> setCode ( opcode );
                    m_codeListing -> setCode ( node->location(), opcode, address );
                    m_dgbFile -> setCode ( node->location(), opcode, address );
                    m_memoryPtr->tryReserve ( node->location(), AsmPicoBlazeMemoryPtr::MS_CODE, address );
                }
                break;
            }
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_IF ( CompilerStatement * rootNode )
{
    using namespace CompilerStatementTypes;

    CompilerStatement * ifTree = rootNode->branch();

    if ( NULL == ifTree )
    {
        return;
    }

    unsigned int conditionVal = 0;
    for ( CompilerStatement * node = ifTree;
          NULL != node;
          node = node->next() )
    {
        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_IF:
            case ASMPICOBLAZE_DIR_ELSEIF:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                break;

            case ASMPICOBLAZE_DIR_IFN:
            case ASMPICOBLAZE_DIR_ELSEIFN:
                conditionVal = m_symbolTable->resolveExpr(node->args());
                conditionVal = ( ( 0 == conditionVal ) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFDEF:
            case ASMPICOBLAZE_DIR_ELSEIFDEF:
                if ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type )
                {
                    conditionVal = 0;
                }
                else if ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
                {
                    conditionVal = 1;
                }
                else
                {
                    conditionVal = 0;
                }
                break;

            case ASMPICOBLAZE_DIR_IFNDEF:
            case ASMPICOBLAZE_DIR_ELSEIFNDEF:
                if ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type )
                {
                    conditionVal = 1;
                }
                else if ( true == m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
                {
                    conditionVal = 0;
                }
                else
                {
                    conditionVal = 1;
                }
                break;

            case ASMPICOBLAZE_DIR_IFB:
            case ASMPICOBLAZE_DIR_ELSEIFB:
                conditionVal = ( ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type ) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFNB:
            case ASMPICOBLAZE_DIR_ELSEIFNB:
                conditionVal = ( ( CompilerValue::TYPE_EMPTY == node->args()->lVal().m_type ) ? 0 : 1 );
                break;

            case ASMPICOBLAZE_DIR_ELSE:
                conditionVal = 1;
                break;

            case ASMPICOBLAZE_DIR_ENDIF:
                break;
        }

        m_codeListing->setValue(node->location(), conditionVal);

        if ( 0 != conditionVal )
        {
            rootNode->insertLink(node->branch());
            node->m_branch = NULL;
        }
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_AUTOxxx ( CompilerStatement * node,
                                                   const CompilerSourceLocation * location )
{
    using namespace CompilerStatementTypes;

    int addr;
    AsmPicoBlazeSymbolTable::SymbolType symbolType;

    if ( ASMPICOBLAZE_DIR_AUTOSPR == node->type() )
    {
        if ( NULL != node->args()->next() )
        {
            m_memoryPtr->m_data = m_symbolTable->resolveExpr(node->args()->next());
        }

        addr = m_memoryPtr->m_data;
        m_memoryPtr->m_data++;
        symbolType = AsmPicoBlazeSymbolTable::STYPE_DATA;

        m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_DATA, addr );
    }
    else
    {
        if ( NULL != node->args()->next() )
        {
            m_memoryPtr->m_reg = m_symbolTable->resolveExpr(node->args()->next());
        }

        addr = m_memoryPtr->m_reg;
        m_memoryPtr->m_reg++;
        symbolType = AsmPicoBlazeSymbolTable::STYPE_REGISTER;

        m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_REG, addr );
    }

    CompilerExpr value(addr);
    m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                 &value,
                                 location,
                                 symbolType,
                                 true );
    m_codeListing->setValue(node->location(), addr);
}

inline void AsmPicoBlazeTreeDecoder::dir_DB ( CompilerStatement * node,
                                              const CompilerSourceLocation * location )
{
    std::vector<unsigned char> dbData;

    for ( CompilerExpr * arg = node->args();
          NULL != arg;
          arg = arg->next() )
    {
        if ( CompilerValue::TYPE_ARRAY == arg->lVal().m_type )
        {
            const CompilerValue::Data::CharArray & charArray = arg->lVal().m_data.m_array;
            for ( int i = 0; i < charArray.m_size; i++ )
            {
                dbData.push_back ( charArray.m_data[i] );
            }
        }
        else
        {
            int value = m_symbolTable->resolveExpr(arg);
            if ( ( true == isInstWord3B() ) && ( value & 0xff0000 ) )
            {
                dbData.push_back ( ( value >> 16 ) & 0xff );
            }
            if ( value & 0xff00 )
            {
                dbData.push_back ( ( value >> 8 ) & 0xff );
            }
            dbData.push_back ( value & 0xff );
        }
    }

    node->args()->completeDelete();
    node->m_args = NULL;

    if ( true == isInstWord3B() )
    {
        m_memoryPtr->m_code += ( dbData.size() / 3 );
        m_memoryPtr->m_code += ( 0 == ( dbData.size() % 3 ) ? 0 : 1);
    }
    else
    {
        m_memoryPtr->m_code += ( dbData.size() / 2 );
        m_memoryPtr->m_code += ( 0 == ( dbData.size() % 2 ) ? 0 : 1);
    }

    for ( size_t i = 0; i < dbData.size(); i++ )
    {
        unsigned int code = 0;

        if ( true == isInstWord3B() )
        {
            code |= ( dbData[i] << 16 );
            i++;
        }

        if ( i < dbData.size() )
        {
            code |= ( dbData[i] << 8 );
        }
        i++;

        if ( i < dbData.size() )
        {
            code |= dbData[i];
        }

        if ( ( true == isInstWord3B() ) && ( 0 != ( code & ~0x3ffff ) ) )
        {
            m_compilerCore -> compilerMessage ( *location,
                                                CompilerBase::MT_WARNING,
                                                QObject::tr ( "instruction word is only 18 bits wide, value"
                                                              " `%1' trimmed to `%2'" )
                                                            . arg ( code )
                                                            . arg ( code & 0x3ffff )
                                                            . toStdString() );
            code &= 0x3ffff;
        }

        if ( NULL == node->m_args )
        {
            node->m_args = new CompilerExpr((int)code);
        }
        else
        {
            node->m_args->appendLink(new CompilerExpr((int)code));
        }
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_REPT ( CompilerStatement * node,
                                           const CompilerSourceLocation * location )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_CONTINUE;
    }

    unsigned int times = m_symbolTable->resolveExpr(node->args());
    if ( 0 == times )
    {
        return CA_CONTINUE;
    }
    if ( times >= MAX_REPEAT_ITERATIONS )
    {
        m_compilerCore -> compilerMessage ( *location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("maximum number of REPEAT directive iterations (%1) reached")
                                                       .arg(MAX_REPEAT_ITERATIONS).toStdString() );
    }

    CompilerSourceLocation lastLocation = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDR' directive

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(node->branch()->copyEntireChain());
    m_codeListing->repeatCode(lastLocation, body, true);
    lastLocation = body->lastLeaf()->location();
    if ( false == phase1(body/*, location*/) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    for ( unsigned int i = 1; i < times; i++ )
    {
        CompilerStatement * exp = new CompilerStatement();
        exp->appendLink(node->branch()->copyEntireChain());
        m_codeListing->repeatCode(lastLocation, exp, false);
        lastLocation = exp->lastLeaf()->location();
        if ( false == phase1(exp/*, location*/) )
        {
            body->completeDelete();
            exp->completeDelete();
            return CA_RETURN_FALSE;
        }
        body->appendLink(exp);
    }

    node = node->prev();
    delete node->next();
    m_forceNext = body->last();
    node->insertLink(body);

    return CA_CONTINUE;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_WHILE ( CompilerStatement * node,
                                            const CompilerSourceLocation * location )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_NO_ACTION;
    }

    CompilerSourceLocation lastLocation = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDW' directive

    unsigned int i;
    CompilerStatement * body = NULL;
    for ( i = 0; i < MAX_WHILE_ITERATIONS; i++ )
    {
        if ( 0 == m_symbolTable->resolveExpr(node->args()) )
        {
            return CA_NO_ACTION;
        }

        if ( NULL == body )
        {
            body = new CompilerStatement();
            body->appendLink(node->branch()->copyEntireChain());
            m_codeListing->repeatCode(lastLocation, body, true);
            lastLocation = body->lastLeaf()->location();
            if ( false == phase1(body/*, location*/) )
            {
                body->completeDelete();
                return CA_RETURN_FALSE;
            }
        }
        else
        {
            CompilerStatement * exp = new CompilerStatement();
            exp->appendLink(node->branch()->copyEntireChain());
            m_codeListing->repeatCode(lastLocation, exp, false);
            lastLocation = exp->lastLeaf()->location();
            if ( false == phase1(exp/*, location*/) )
            {
                body->completeDelete();
                exp->completeDelete();
                return CA_RETURN_FALSE;
            }
            body->appendLink(exp);
        }
    }

    if ( MAX_WHILE_ITERATIONS == i )
    {
        m_compilerCore -> compilerMessage ( *location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "maximum number of WHILE directive iterations "
                                                          "(%1) reached" )
                                                        . arg ( MAX_WHILE_ITERATIONS )
                                                        . toStdString() );
    }
    if ( NULL != body)
    {
        node = node->prev();
        delete node->next();
        m_forceNext = body->last();
        node->insertLink(body);
        return CA_CONTINUE;
    }
    else
    {
        return CA_NO_ACTION;
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::macro ( CompilerStatement * node,
                                        const CompilerSourceLocation * location )
{
    if ( -1 != m_opts->m_maxMacroExp && node->m_userData >= m_opts->m_maxMacroExp )
    {
        m_compilerCore -> compilerMessage ( node->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "maximum macro expansion level (%1) reached " )
                                                        . arg ( m_opts->m_maxMacroExp )
                                                        . toStdString() );
        return CA_NO_ACTION;
    }

    const std::string nameOfMacro = node->args()->lVal().m_data.m_symbol;

    CompilerStatement * macro = m_macros -> expand ( *location,
                                                     node->location(),
                                                     nameOfMacro,
                                                     node->args()->next() );

    if ( CompilerStatementTypes::EMPTY_STATEMENT == macro->type() )
    {
        macro->completeDelete();
        return CA_NO_ACTION;
    }

    //
    macro->prependLink(new CompilerStatement());
    macro = macro->first();
    if ( false == phase1(macro, location, &nameOfMacro) )
    {
        macro->completeDelete();
        return CA_RETURN_FALSE;
    }

    node = node->prev();
    delete node->next();

    m_forceNext = macro->last();
    node->insertLink(macro);

    return CA_CONTINUE;
}

inline void AsmPicoBlazeTreeDecoder::dir_MACRO ( CompilerStatement * node )
{
    m_macros -> define ( node->location(),
                         node->args()->lVal().m_data.m_symbol,
                         node->args()->m_next,
                         node->branch() );
    node->m_branch = NULL;
}

inline void AsmPicoBlazeTreeDecoder::dir_TITLE ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_codeListing->setTitle(arg);
}

inline void AsmPicoBlazeTreeDecoder::dir_MESSG ( CompilerStatement * node,
                                                 const CompilerSourceLocation * location )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->compilerMessage(*location, CompilerBase::MT_REMARK, arg);
}

inline void AsmPicoBlazeTreeDecoder::dir_WARNING ( CompilerStatement * node,
                                                   const CompilerSourceLocation * location )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->compilerMessage(*location, CompilerBase::MT_WARNING, arg);
}

inline void AsmPicoBlazeTreeDecoder::dir_ERROR ( CompilerStatement * node,
                                                 const CompilerSourceLocation * location )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_compilerCore->compilerMessage(*location, CompilerBase::MT_ERROR, arg);
}

inline void AsmPicoBlazeTreeDecoder::dir_LIST ( CompilerStatement * node )
{
    m_codeListing->setNoList(node->location(), false);
}

inline void AsmPicoBlazeTreeDecoder::dir_NOLIST ( CompilerStatement * node )
{
    m_codeListing->setNoList(node->location(), true);
}

inline void AsmPicoBlazeTreeDecoder::dir_EXPAND()
{
    m_macros->setExpEnabled(true);
}

inline void AsmPicoBlazeTreeDecoder::dir_NOEXPAND()
{
    m_macros->setExpEnabled(false);
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_EXITM ( CompilerStatement * node )
{
    if ( false == m_macros->isFromMacro(node) )
    {
        m_compilerCore->compilerMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "directive EXITM' cannot apper outside macro definition")
                                                      .toStdString() );
        return CA_NO_ACTION;
    }

    node->m_prev = NULL;
    node->prev()->m_next = NULL;
    node->completeDelete();
    return CA_RETURN_TRUE;
}

inline void AsmPicoBlazeTreeDecoder::dir_INCLUDE ( CompilerStatement * node )
{
    m_codeListing -> setInclusion ( node->location(),
                                    (int) (node->args()->m_lValue.m_data.m_integer) );
}

inline void AsmPicoBlazeTreeDecoder::dir_LOCAL ( CompilerStatement * node,
                                                 const CompilerSourceLocation * location,
                                                 const std::string * macroName,
                                                 std::vector<std::string> & localSymbols )
{
    if ( false == m_macros->isFromMacro(node) )
    {
        m_compilerCore->compilerMessage ( node->location(),
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "directive `LOCAL' cannot apper outside macro "
                                                        "definition" )
                                                      . toStdString() );
    }

    const std::string local = node->args()->lVal().m_data.m_symbol;
    if ( false == m_macros -> mangleName ( *location,
                                           &localSymbols,
                                           local,
                                           *macroName,
                                           node->next() ) )
    {
        m_compilerCore->compilerMessage ( *location,
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "symbol `%1' declared as local but never used, "
                                                        "declaration ignored"  )
                                                       . arg ( local.c_str() )
                                                       . toStdString() );
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_SET ( CompilerStatement * node,
                                               const CompilerSourceLocation * location )
{
    int value;
    std::string name = node->args()->lVal().m_data.m_symbol;

    if ( true == m_symbolTable->isDefined(name) )
    {
        value = m_symbolTable -> assignValue ( name,
                                               node->args()->next(),
                                               location,
                                               AsmPicoBlazeSymbolTable::STYPE_NUMBER,
                                               true );
    }
    else
    {
        value = m_symbolTable -> addSymbol ( name,
                                             node->args()->next(),
                                             location,
                                             AsmPicoBlazeSymbolTable::STYPE_NUMBER,
                                             true,
                                             true );
    }

    m_codeListing->setValue(node->location(), value);
}

inline void AsmPicoBlazeTreeDecoder::dir_DEFINE ( CompilerStatement * node,
                                                  const CompilerSourceLocation * location )
{
    int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             node->args()->next(),
                                             location,
                                             AsmPicoBlazeSymbolTable::STYPE_EXPRESSION,
                                             false );

    m_codeListing->setValue(node->location(), value);
}

inline void AsmPicoBlazeTreeDecoder::dir_UNDEFINE ( CompilerStatement * node )
{
    m_symbolTable -> removeSymbol ( node->args()->lVal().m_data.m_symbol,
                                    node->location() );
}

inline void AsmPicoBlazeTreeDecoder::label ( CompilerStatement * node,
                                             const CompilerSourceLocation * location )
{
    CompilerExpr e(m_memoryPtr->m_code);
    m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                 &e,
                                 location,
                                 AsmPicoBlazeSymbolTable::STYPE_LABEL,
                                 true );
}

inline void AsmPicoBlazeTreeDecoder::dir_ORG ( CompilerStatement * node )
{
    m_memoryPtr->m_code = m_symbolTable->resolveExpr(node->args());
    m_codeListing->setValue(node->location(), m_memoryPtr->m_code);
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(m_memoryPtr->m_code);
}

inline void AsmPicoBlazeTreeDecoder::dir_SKIP ( CompilerStatement * node )
{
    int skip = m_symbolTable->resolveExpr(node->args());
    m_memoryPtr->m_code += skip;
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(skip);
}

inline void AsmPicoBlazeTreeDecoder::dir_LIMIT ( CompilerStatement * node,
                                                 const CompilerSourceLocation * location )
{
    const char * limSel = node->args()->lVal().m_data.m_symbol;
    int limVal = m_symbolTable->resolveExpr ( node->args()->next() );

    if ( -1 == limVal )
    {
        m_compilerCore->compilerMessage ( *location,
                                          CompilerBase::MT_REMARK,
                                          QObject::tr("limit value -1 means unlimited").toStdString() );
    }
    else if ( -1 > limVal )
    {
        m_compilerCore->compilerMessage ( *location,
                                          CompilerBase::MT_ERROR,
                                          QObject::tr("limit value %1 is not valid").arg(limVal).toStdString() );
        return;
    }

    if ( 0 == strcmp("R", limSel) )
    {
        m_opts->m_processorlimits.m_regFileSize = limVal;
    }
    else if ( 0 == strcmp("D", limSel) )
    {
        m_opts->m_processorlimits.m_iDataMemSize = limVal;
    }
    else if ( 0 == strcmp("C", limSel) )
    {
        m_opts->m_processorlimits.m_iCodeMemSize = limVal;
    }
}

inline void AsmPicoBlazeTreeDecoder::dir_EQU_etc ( CompilerStatement * node,
                                                   const CompilerSourceLocation * location )
{
    using namespace CompilerStatementTypes;

    AsmPicoBlazeSymbolTable::SymbolType symbolType;

    switch ( (int) node->type() )
    {
        case ASMPICOBLAZE_DIR_CODE:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_LABEL;
            break;
        case ASMPICOBLAZE_DIR_DATA:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_DATA;
            break;
        case ASMPICOBLAZE_DIR_PORT:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_PORT;
            break;
        case ASMPICOBLAZE_DIR_REG:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            break;
        case ASMPICOBLAZE_DIR_EQU:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_NUMBER;
            break;
        default:
            symbolType = AsmPicoBlazeSymbolTable::STYPE_UNSPECIFIED;
            break;
    }

    int value = m_symbolTable -> addSymbol ( node->args()->lVal().m_data.m_symbol,
                                             node->args()->m_next,
                                             location,
                                             symbolType,
                                             true );

    m_codeListing->setValue(node->location(), value);

    if ( ( value >= 0 ) && ( -1 != location->m_fileNumber ) )
    {
        if ( node->type() == ASMPICOBLAZE_DIR_REG )
        {
            m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_REG, (unsigned int) value );
        }
        else if ( node->type() == ASMPICOBLAZE_DIR_DATA )
        {
            m_memoryPtr -> tryReserve ( *location, AsmPicoBlazeMemoryPtr::MS_DATA, (unsigned int) value );
        }
    }
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTCOND ( CompilerStatement * node,
                                             const CompilerSourceLocation * location )
{
    if ( false == checkKcpsm3AndHigher(location, "run-time conditions") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeCondition(node));

    if ( false == phase1(body, location) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTWHILE ( CompilerStatement * node,
                                              const CompilerSourceLocation * location )
{
    if ( false == checkKcpsm3AndHigher(location, "run-time while loops") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeWhile(node));

    if ( false == phase1(body, location) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_RTFOR ( CompilerStatement * node,
                                            const CompilerSourceLocation * location )
{
    if ( false == checkKcpsm3AndHigher(location, "run-time for loops") )
    {
        return CA_NO_ACTION;
    }

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(m_specialMacros->runTimeFor(node));

    if ( false == phase1(body, location) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    m_specialMacros->runTimeForLeave();

    m_forceNext = body->last();
    node->insertLink(body);

    return CA_NO_ACTION;
}

inline AsmPicoBlazeTreeDecoder::CourseOfAction
       AsmPicoBlazeTreeDecoder::dir_DEVICE ( CompilerStatement * node,
                                             const CompilerSourceLocation * location )
{
    std::string deviceName = node->args()->lVal().m_data.m_symbol;
    for ( size_t i = 0; i < deviceName.size(); i++ )
    {
        deviceName[i] = (char) tolower(deviceName[i]);
    }

    CompilerBase::DevSpecLoaderFlag loaderFlag;
    CompilerStatement * devSpecCode = m_compilerCore->loadDevSpecCode(deviceName, &loaderFlag);

    if ( NULL == devSpecCode )
    {
        if ( CompilerBase::DSLF_DOES_NOT_EXIST == loaderFlag )
        {
            m_compilerCore->compilerMessage ( *location,
                                              CompilerBase::MT_ERROR,
                                              QObject::tr("Device not supported: ").toStdString()
                                              + "\"" + deviceName + "\"" );
            return CA_RETURN_FALSE;
        }
    }
    else
    {
        node->insertLink(devSpecCode->next());
        delete devSpecCode;
    }

    return CA_NO_ACTION;
}

inline bool AsmPicoBlazeTreeDecoder::isInstWord3B() const
{
    switch ( m_device )
    {
        case AsmPicoBlazeSemanticAnalyzer::DEV_UNSPEC:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1CPLD:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM2:
            return false;

        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM3:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM6:
        default:
            return true;
    }
}

bool AsmPicoBlazeTreeDecoder::checkKcpsm3AndHigher ( const CompilerSourceLocation * location,
                                                     const char * functionality )
{
    switch ( m_device )
    {
        case AsmPicoBlazeSemanticAnalyzer::DEV_UNSPEC:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1CPLD:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM1:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM2:
            m_compilerCore->compilerMessage(*location,
                                            CompilerBase::MT_ERROR,
                                            QObject::tr("assembler feature '%1' is supported only on KCPSM3 and higher")
                                                       .arg(functionality).toStdString() );
            return false;

        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM3:
        case AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM6:
        default:
            return true;
    }
}

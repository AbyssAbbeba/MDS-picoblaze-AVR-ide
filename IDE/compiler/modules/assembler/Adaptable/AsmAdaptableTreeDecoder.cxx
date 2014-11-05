// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableAsm
 * @file AsmAdaptableTreeDecoder.cxx
 */
// =============================================================================

#include "AsmAdaptableTreeDecoder.h"

// Common compiler header files.
#include "AsmMacros.h"
#include "AsmMemoryPtr.h"
#include "AsmDgbFileGen.h"
#include "AsmSymbolTable.h"
#include "AsmCodeListing.h"
#include "CompilerStatementTypes.h"

// Support for processor definition files used Adaptable Simulator
#include "AdjSimProcDef.h"

// Header files of libMCUDataFiles.
#include "HexFile.h"

// Standard headers.
#include <vector>
#include <iostream>
#include <algorithm>

#define HANDLE_ACTION(arg)                  \
    switch ( arg )                          \
    {                                       \
        case CA_NO_ACTION:    break;        \
        case CA_CONTINUE:     continue;     \
        case CA_RETURN_TRUE:  return true;  \
        case CA_RETURN_FALSE: return false; \
    }

AsmAdaptableTreeDecoder::AsmAdaptableTreeDecoder ( AsmAdaptableSemanticAnalyzer * semanticAnalyzer )
                                                 : m_semanticAnalyzer ( semanticAnalyzer )
{
    m_mergeAddr = -1;
    m_failjmp   = nullptr;
    m_forceNext = nullptr;
}

AsmAdaptableTreeDecoder::~AsmAdaptableTreeDecoder()
{
    if ( nullptr != m_failjmp )
    {
        delete m_failjmp;
    }
}

bool AsmAdaptableTreeDecoder::phase1 ( CompilerStatement * codeTree,
                                       const std::string * macroName )
{
    using namespace CompilerStatementTypes;
    std::vector<std::string> localSymbols;

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = ( ( nullptr == m_forceNext ) ? node : m_forceNext ) -> next() )
    {
        m_forceNext = nullptr;

        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_EQU:
            case ASMPICOBLAZE_DIR_REG:
            case ASMPICOBLAZE_DIR_CODE:
            case ASMPICOBLAZE_DIR_PORT:
            case ASMPICOBLAZE_DIR_PORTIN:
            case ASMPICOBLAZE_DIR_PORTOUT:
            case ASMPICOBLAZE_DIR_DATA:
                dir_EQU_etc(node);
                break;

            case ASMPICOBLAZE_DIR_AUTOREG:
            case ASMPICOBLAZE_DIR_AUTOSPR:
                dir_AUTOxxx(node);
                break;

            case ASMPICOBLAZE_LOCAL:
                dir_LOCAL ( node, macroName, localSymbols );
                break;

            case ASMPICOBLAZE_DIR_END:
                break;

            case ASMPICOBLAZE_MACRO:       HANDLE_ACTION ( macro       ( node ) ); break;
            case ASMPICOBLAZE_DIR_REPT:    HANDLE_ACTION ( dir_REPT    ( node ) ); break;
            case ASMPICOBLAZE_DIR_WHILE:   HANDLE_ACTION ( dir_WHILE   ( node ) ); break;
            case ASMPICOBLAZE_DIR_EXITM:   HANDLE_ACTION ( dir_EXITM   ( node ) ); break;
            case ASMPICOBLAZE_DIR_DB:      HANDLE_ACTION ( dir_DB      ( node ) ); break;

            case ASMPICOBLAZE_DIR_ORG:      dir_ORG      ( node ); continue;
            case ASMPICOBLAZE_DIR_SKIP:     dir_SKIP     ( node ); continue;
            case ASMPICOBLAZE_DIR_DEVICE:   dir_DEVICE   ( node ); break;
            case ASMPICOBLAZE_COND_ASM:     dir_IF       ( node ); break;
            case ASMPICOBLAZE_DIR_LIST:     dir_LIST     ( node ); break;
            case ASMPICOBLAZE_DIR_NOLIST:   dir_NOLIST   ( node ); break;
            case ASMPICOBLAZE_DIR_MACRO:    dir_MACRO    ( node ); break;
            case ASMPICOBLAZE_DIR_TITLE:    dir_TITLE    ( node ); break;
            case ASMPICOBLAZE_INCLUDE:      dir_INCLUDE  ( node ); break;
            case ASMPICOBLAZE_DIR_UNDEFINE: dir_UNDEFINE ( node ); break;
            case ASMPICOBLAZE_LABEL:        label        ( node ); break;
            case ASMPICOBLAZE_DIR_SET:      dir_SET      ( node ); break;
            case ASMPICOBLAZE_DIR_LIMIT:    dir_LIMIT    ( node ); break;
            case ASMPICOBLAZE_DIR_ERROR:    dir_ERROR    ( node ); break;
            case ASMPICOBLAZE_DIR_MESSG:    dir_MESSG    ( node ); break;
            case ASMPICOBLAZE_DIR_DEFINE:   dir_DEFINE   ( node ); break;
            case ASMPICOBLAZE_DIR_WARNING:  dir_WARNING  ( node ); break;
            case ASMPICOBLAZE_DIR_FAILJMP:  dir_FAILJMP  ( node ); break;
            case ASMPICOBLAZE_DIR_ORGSPR:   dir_ORGSPR   ( node ); break;
            case ASMPICOBLAZE_DIR_INITSPR:  dir_INITSPR  ( node ); break;
            case ASMPICOBLAZE_DIR_MERGESPR: dir_MERGESPR ( node ); break;

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

void AsmAdaptableTreeDecoder::phase2 ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    m_semanticAnalyzer->m_symbolTable->maskNonLabels();

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case EMPTY_STATEMENT:
                // Do not interpret empty statements and let them be deleted (later in this method).
                break;

            case ASMPICOBLAZE_DIR_ORG:
                m_semanticAnalyzer->m_machineCode->setOrigin(
                    (uint32_t)m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_SKIP:
                m_semanticAnalyzer->m_machineCode->incrementAddr(
                    (unsigned int)m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args()));
                break;

            case ASMPICOBLAZE_DIR_DB:
                dir_DB_phase2(node);
                for ( CompilerExpr * arg = node->args();
                      nullptr != arg;
                      arg = arg->next() )
                {
                    long long code = arg->lVal().m_data.m_integer;
                    int address = m_semanticAnalyzer->m_machineCode->setCode ( (uint32_t) code );
                    m_semanticAnalyzer->m_codeListing->setCode ( node->location(), (int) code, address );
                    m_semanticAnalyzer->m_dgbFile->setCode ( node->location(), (int) code, address );
                    m_semanticAnalyzer->m_memoryPtr->tryReserve ( node->location(), AsmMemoryPtr::MS_CODE, address );
                }
                break;

            default:
                break;
        }

        // Remove the inspected node.
        node = node->prev();
        delete node->next();
    }
}

void AsmAdaptableTreeDecoder::phase3()
{
    if ( false == m_sprInit.empty() )
    {
        if ( -1 == m_mergeAddr )
        {
            if ( true == m_semanticAnalyzer->m_opts->m_second.empty() )
            {
                m_semanticAnalyzer->m_compilerCore->semanticMessage ( CompilerSourceLocation(),
                                                                      CompilerBase::MT_WARNING,
                                                                      QObject::tr ( "no target file for SPR "
                                                                                    "initialization specified" )
                                                                      . toStdString() );
            }
            else
            {
                HexFile hexFile;
                for ( const auto & i : m_sprInit )
                {
                    hexFile.set ( i.m_address, (uint8_t) i.m_octet );
                }

                try
                {
                    hexFile.save(m_semanticAnalyzer->m_opts->m_second, m_semanticAnalyzer->m_opts->m_makeBackupFiles);
                }
                catch ( const DataFileException & e )
                {
                    std::cerr << e.toString() << std::endl;
                    m_semanticAnalyzer->m_compilerCore->semanticMessage ( CompilerSourceLocation(),
                                                                          CompilerBase::MT_ERROR,
                                                                          QObject::tr ( "unable to save file `%1'" )
                                                                                      . arg ( m_semanticAnalyzer->
                                                                                              m_opts->
                                                                                              m_second.c_str() )
                                                                                      . toStdString() );
                }
            }
        }
        else
        {
            int size = -1;
            for ( const auto & i : m_sprInit )
            {
                if ( (int) i.m_address > size )
                {
                    size = (int) i.m_address;
                }
            }

            size++;
            size = ( size / 2 ) + ( size % 2 );

            const CompilerSourceLocation * location[size];
            unsigned int data[size];

            for ( int i = 0; i < size; i++ )
            {
                data[i] = 0;
                location[i] = nullptr;
            }

            unsigned int shift, addr, mask;
            for ( const auto & i : m_sprInit )
            {
                shift = ( ( i.m_address % 2 ) ? 8 : 0 );
                addr  = ( i.m_address / 2 );
                mask  = ( 0xff << shift );

                data[addr] &= ~mask;
                data[addr] |= ( i.m_octet << shift );

                location[addr] = &( i.m_location );
            }

            addr = m_mergeAddr;
            const CompilerSourceLocation * loc;
            CompilerSourceLocation emptyLocation;
            for ( int i = 0; i < size; i++, addr++ )
            {
                if ( nullptr == location[i] )
                {
                    loc = &emptyLocation;
                }
                else
                {
                    loc = location[i];
                }

                m_semanticAnalyzer->m_machineCode->setCode(addr, data[i]);
                m_semanticAnalyzer->m_dgbFile->setCode(*loc, data[i], addr);
                m_semanticAnalyzer->m_memoryPtr->tryReserve(*loc, AsmMemoryPtr::MS_CODE, addr);
            }
            for ( int i = size; i < ( m_semanticAnalyzer->m_opts->m_processorlimits.m_iDataMemSize / 2 ); i++, addr++ )
            {
                m_semanticAnalyzer->m_machineCode->setCode(addr, 0);
                m_semanticAnalyzer->m_dgbFile->setCode(m_mergeSprLoc, 0, addr);
                m_semanticAnalyzer->m_memoryPtr->tryReserve(m_mergeSprLoc, AsmMemoryPtr::MS_CODE, addr);
            }
        }

        m_sprInit.clear();
    }

    if ( nullptr != m_failjmp )
    {
        if ( true == m_semanticAnalyzer->m_opts->m_strict )
        {
            if ( AsmSymbolTable::STYPE_LABEL != m_semanticAnalyzer->m_symbolTable->getType(m_failjmp->args()) )
            {
                m_semanticAnalyzer->m_compilerCore->semanticMessage ( m_failjmp->location(),
                                                                      CompilerBase::MT_WARNING,
                                                                      QObject::tr("jump address is not specified by a"
                                                                                  " label").toStdString(),
                                                                      true );
            }
        }

        int memSize = m_semanticAnalyzer->m_opts->m_processorlimits.m_iCodeMemSize;
        if ( -1 == memSize )
        {
            m_semanticAnalyzer->m_compilerCore->semanticMessage ( m_failjmp->location(),
                                                                  CompilerBase::MT_ERROR,
                                                                  QObject::tr ( "no user defined limit for the program "
                                                                                "memory size, fail jump cannot be used")
                                                                              . toStdString() );
        }
        else
        {
            int opcode = m_semanticAnalyzer->m_symbolTable->resolveExpr(m_failjmp->args());

            for ( int addr = 0; addr < memSize; addr++ )
            {
                if ( false == m_semanticAnalyzer->m_memoryPtr->isReserved ( AsmMemoryPtr::MS_CODE, (unsigned int) addr ) )
                {
                    m_semanticAnalyzer->m_machineCode->setCode(addr, opcode);
                    m_semanticAnalyzer->m_dgbFile->setCode(m_failjmp->location(), opcode, addr);
                }
            }
        }

        delete m_failjmp;
        m_failjmp = nullptr;
    }
}

inline bool AsmAdaptableTreeDecoder::isInstruction ( const std::string & mnemonic )
{
    for ( const AdjSimProcDef::Instruction & inst : m_semanticAnalyzer->m_device.m_instructionSet )
    {
        if ( mnemonic == inst.m_mnemonic )
        {
            return true;
        }
    }

    return false;
}

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::instruction2db ( const std::string & mnemonic,
                                                 CompilerStatement * node )
{
    CompilerStatement * db = resolveInstruction(mnemonic, node);
    if ( nullptr == db )
    {
        return CA_RETURN_FALSE;
    }
    else
    {
        node->insertLink(db);
        return CA_NO_ACTION;
    }
}

inline CompilerStatement * AsmAdaptableTreeDecoder::resolveInstruction ( const std::string & mnemonic,
                                                                         const CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    std::vector<char> operandTypes;
    for ( CompilerExpr * opr = node->args()->next();
          nullptr != opr;
          opr = opr->next() )
    {
        switch ( opr->oper() )
        {
            case CompilerExpr::OPER_HASH:
                operandTypes.push_back('#');
                break;
            case CompilerExpr::OPER_AT:
                operandTypes.push_back('@');
                break;
            default:
                operandTypes.push_back(' ');
                break;
        }

        opr->m_operator = CompilerExpr::OPER_NONE;
    }

    for ( const AdjSimProcDef::Instruction & inst : m_semanticAnalyzer->m_device.m_instructionSet )
    {
        if ( ( mnemonic == inst.m_mnemonic ) && ( inst.m_operands.size() == operandTypes.size() ) )
        {
            bool match = true;
            for ( unsigned int i = 0; i < operandTypes.size(); i++ )
            {
                int operandNumber = -1;
                if ( i < inst.m_operands.size() )
                {
                    switch ( inst.m_operands[i].m_number )
                    {
                        case AdjSimProcDef::Instruction::Operand::N_FIRST:  operandNumber =  0; break;
                        case AdjSimProcDef::Instruction::Operand::N_SECOND: operandNumber =  1; break;
                        case AdjSimProcDef::Instruction::Operand::N_THIRD:  operandNumber =  2; break;
                        case AdjSimProcDef::Instruction::Operand::N_HIDDEN: operandNumber = -1; break;
                    }
                }

                char type = ' ';
                switch ( inst.m_operands[i].m_type )
                {
                    case AdjSimProcDef::Instruction::Operand::T_IMMEDIATE:
                        type = '#';
                        break;
                    case AdjSimProcDef::Instruction::Operand::T_REG_INDR:
                    case AdjSimProcDef::Instruction::Operand::T_DATA_INDR:
                        type = '@';
                        break;
                    case AdjSimProcDef::Instruction::Operand::T_REG_DIR:
                    case AdjSimProcDef::Instruction::Operand::T_DATA_DIR:
                    case AdjSimProcDef::Instruction::Operand::T_PROGRAM:
                    case AdjSimProcDef::Instruction::Operand::T_PORT:
                        type = ' ';
                        break;
                }

                if ( ( -1 != operandNumber ) && ( type != operandTypes[operandNumber] ) )
                {
                    match = false;
                    break;
                }
            }

            if ( true == match )
            {
                int code = 0;
                int mask = 0;
                for ( unsigned int i = 0; i < inst.m_opCode.size(); i++ )
                {
                    switch ( inst.m_opCode[i] )
                    {
                        case AdjSimProcDef::Instruction::OCB_ZERO:
                            mask |= ( 1 << i );
                            break;
                        case AdjSimProcDef::Instruction::OCB_ONE:
                            code |= ( 1 << i );
                            mask |= ( 1 << i );
                            break;
                        case AdjSimProcDef::Instruction::OCB_DONT_CARE:
                            break;
                    }
                }

                CompilerStatement * db = new CompilerStatement ( node->location(),
                                                                 ASMPICOBLAZE_DIR_DB,
                                                                 new CompilerExpr(code, node->location()) );

                for ( unsigned int i = 0; i < inst.m_operands.size(); i++ )
                {
                    if ( i >= operandTypes.size() )
                    {
                        break;
                    }

                    int operandNumber = -1;
                    switch ( inst.m_operands[i].m_number )
                    {
                        case AdjSimProcDef::Instruction::Operand::N_FIRST:  operandNumber =  0; break;
                        case AdjSimProcDef::Instruction::Operand::N_SECOND: operandNumber =  1; break;
                        case AdjSimProcDef::Instruction::Operand::N_THIRD:  operandNumber =  2; break;

                        case AdjSimProcDef::Instruction::Operand::N_HIDDEN:
                            continue;
                    }

                    CompilerExpr * value = node->args()->at(operandNumber + 1);

                    for ( int fromBit = 0; fromBit < (int) (inst.m_operands[i].m_OPCodePermutation.size()); fromBit++ )
                    {
                        int toBit = inst.m_operands[i].m_OPCodePermutation[fromBit];

                        // ( ( value & ( 1 << fromBit ) ) >> fromBit ) << toBit
                        CompilerExpr * operand = new CompilerExpr (
                                                     new CompilerExpr (
                                                         new CompilerExpr ( value->copyChainLink(),
                                                                            CompilerExpr::OPER_BAND,
                                                                            new CompilerExpr ( 1,
                                                                                               CompilerExpr::OPER_SHL,
                                                                                               fromBit,
                                                                                               value->location() ) ),
                                                         CompilerExpr::OPER_SHR,
                                                         fromBit,
                                                         value->location() ),
                                                     CompilerExpr::OPER_SHL,
                                                     toBit,
                                                     value->location() );

                        CompilerExpr * arg = db->args();
                        while ( true )
                        {
                            if ( CompilerExpr::OPER_NONE == arg->oper() )
                            {
                                arg->m_operator = CompilerExpr::OPER_BOR;
                                arg->m_rValue.m_type = CompilerValue::TYPE_EXPR;
                                arg->m_rValue.m_data.m_expr = new CompilerExpr(operand, value->location());
                                break;
                            }
                            else
                            {
                                arg = arg->m_rValue.m_data.m_expr;
                            }
                        }
                    }
                }

                m_semanticAnalyzer->m_symbolTable->resolveSymbols ( db->args(),
                                                                    m_semanticAnalyzer->m_memoryPtr->m_code );

                return db;
            }
        }
    }

    m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                          CompilerBase::MT_ERROR,
                                                          QObject::tr ( "instruction %1 cannot be used with these "
                                                                        "operands" )
                                                                      . arg ( mnemonic.c_str() )
                                                                      . toStdString() );
    return nullptr;
}

inline void AsmAdaptableTreeDecoder::dir_IF ( CompilerStatement * rootNode )
{
    using namespace CompilerStatementTypes;

    CompilerStatement * ifTree = rootNode->branch();

    if ( nullptr == ifTree )
    {
        return;
    }

    unsigned int conditionVal = 0;
    for ( CompilerStatement * node = ifTree;
          nullptr != node;
          node = node->next() )
    {
        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_IF:
            case ASMPICOBLAZE_DIR_ELSEIF:
                conditionVal = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
                break;

            case ASMPICOBLAZE_DIR_IFN:
            case ASMPICOBLAZE_DIR_ELSEIFN:
                conditionVal = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
                conditionVal = ( ( 0 == conditionVal ) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFDEF:
            case ASMPICOBLAZE_DIR_ELSEIFDEF:
                if ( true == isBlank(node->args()) )
                {
                    conditionVal = 0;
                }
                else if ( true == m_semanticAnalyzer->m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
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
                if ( true == isBlank(node->args()) )
                {
                    conditionVal = 1;
                }
                else if ( true == m_semanticAnalyzer->m_symbolTable->isDefined(node->args()->lVal().m_data.m_symbol) )
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
                conditionVal = ( isBlank(node->args()) ? 1 : 0 );
                break;

            case ASMPICOBLAZE_DIR_IFNB:
            case ASMPICOBLAZE_DIR_ELSEIFNB:
                conditionVal = ( isBlank(node->args()) ? 0 : 1 );
                break;

            case ASMPICOBLAZE_DIR_ELSE:
                conditionVal = 1;
                break;

            case ASMPICOBLAZE_DIR_ENDIF:
                return;
        }

        m_semanticAnalyzer->m_codeListing->setValue(node->location(), conditionVal);

        if ( 0 != conditionVal )
        {
            rootNode->insertLink(node->branch());
            node->m_branch = nullptr;
            return;
        }
    }
}

inline void AsmAdaptableTreeDecoder::dir_AUTOxxx ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    int addr;
    AsmSymbolTable::SymbolType symbolType;

    if ( ASMPICOBLAZE_DIR_AUTOSPR == node->type() )
    {
        int size = 1;

        CompilerExpr * arg = node->args()->next();
        if ( nullptr != arg )
        {
            if ( nullptr != arg->next() )
            {
                size = m_semanticAnalyzer->m_symbolTable->resolveExpr(arg->next());

                if (
                       ( size <= 0 )
                           ||
                       (
                           ( -1 != m_semanticAnalyzer->m_opts->m_processorlimits.m_iDataMemSize )
                               &&
                           ( size >= m_semanticAnalyzer->m_opts->m_processorlimits.m_iDataMemSize )
                       )
                   )
                {
                    m_semanticAnalyzer->m_compilerCore->semanticMessage ( arg->next()->location(),
                                                                          CompilerBase::MT_ERROR,
                                                                          QObject::tr("invalid size: %1")
                                                                                     .arg(size).toStdString() );
                    size = 1;
                }
            }
            else
            {
                m_semanticAnalyzer->m_memoryPtr->m_data = m_semanticAnalyzer->m_symbolTable->resolveExpr(arg);
            }
        }

        addr = m_semanticAnalyzer->m_memoryPtr->m_data;
        m_semanticAnalyzer->m_memoryPtr->m_data += size;
        symbolType = AsmSymbolTable::STYPE_DATA;

        for ( int i = 0; i < size; i++ )
        {
            m_semanticAnalyzer->m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_DATA, ( addr + i ) );
        }
    }
    else
    {
        if ( nullptr != node->args()->next() )
        {
            m_semanticAnalyzer->m_memoryPtr->m_reg=m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args()->next());
        }

        addr = m_semanticAnalyzer->m_memoryPtr->m_reg;
        m_semanticAnalyzer->m_memoryPtr->m_reg++;
        symbolType = AsmSymbolTable::STYPE_REGISTER;

        m_semanticAnalyzer->m_memoryPtr -> tryReserve ( node->location(), AsmMemoryPtr::MS_REG, addr );
    }

    CompilerExpr value(addr);
    m_semanticAnalyzer->m_symbolTable->addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                   &value,
                                                   &( node->location() ),
                                                   symbolType,
                                                   true );

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), addr);
}

inline void AsmAdaptableTreeDecoder::dir_ORGSPR ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_memoryPtr->m_data = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
}

inline void AsmAdaptableTreeDecoder::dir_INITSPR ( CompilerStatement * node )
{
    std::vector<unsigned char> initData;

    for ( CompilerExpr * arg = node->args()->next();
          nullptr != arg;
          arg = arg->next() )
    {
        if ( CompilerValue::TYPE_ARRAY == arg->lVal().m_type )
        {
            const CompilerValue::Data::CharArray & charArray = arg->lVal().m_data.m_array;
            for ( int i = 0; i < charArray.m_size; i++ )
            {
                initData.push_back ( charArray.m_data[i] );
            }
        }
        else
        {
            initData.push_back ( 0xff & m_semanticAnalyzer->m_symbolTable->resolveExpr(arg) );
        }
    }

    int size = (int) initData.size();
    int addr = m_semanticAnalyzer->m_memoryPtr->m_data;

    CompilerExpr value(addr);
    m_semanticAnalyzer->m_symbolTable->addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                   &value,
                                                   &( node->location() ),
                                                   AsmSymbolTable::STYPE_DATA,
                                                   true );

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), addr);

    m_semanticAnalyzer->m_memoryPtr->m_data += size;
    for ( int i = 0; i < size; i++ )
    {
        m_semanticAnalyzer->m_memoryPtr->tryReserve( node->location(), AsmMemoryPtr::MS_DATA, addr);
        m_sprInit.push_back ( { node->location(), (unsigned int) addr, initData[i] } );
        addr++;
    }
}

inline void AsmAdaptableTreeDecoder::dir_MERGESPR ( CompilerStatement * node )
{
    m_mergeSprLoc = node->location();
    m_mergeAddr = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());

    if ( m_mergeAddr <= 0 )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "cannot merge DATA memory (SPR) with the "
                                                                            "CODE memory at address: " )
                                                                            . arg ( m_mergeAddr )
                                                                            . toStdString() );
    }

    if (
           ( -1 != m_semanticAnalyzer->m_opts->m_processorlimits.m_iCodeMemSize )
               &&
           ( m_mergeAddr >= m_semanticAnalyzer->m_opts->m_processorlimits.m_iCodeMemSize )
       )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "address is crossing CODE memory size "
                                                                            "boundary: %1" )
                                                                          . arg ( m_mergeAddr )
                                                                          . toStdString() );
        m_mergeAddr = -1;
    }
}

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::dir_DB ( const CompilerStatement * node )
{
    if ( false == m_semanticAnalyzer->m_deviceSet )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                            CompilerBase::MT_ERROR,
                                                            QObject::tr ( "device specification not loaded, cannot "
                                                                          "continue from here" )
                                                                        . toStdString() );
        return CA_RETURN_FALSE;
    }

    int wordSize = 1;
    switch ( m_semanticAnalyzer->m_device.m_memory.m_program.m_word )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B: wordSize = 1; break;
        case AdjSimProcDef::Memory::Program::WORD_2B: wordSize = 2; break;
        case AdjSimProcDef::Memory::Program::WORD_3B: wordSize = 3; break;
    }

    for ( CompilerExpr * arg = node->args();
          nullptr != arg;
          arg = arg->next() )
    {
        if ( CompilerValue::TYPE_ARRAY == arg->lVal().m_type )
        {
            int charArraySize = arg->lVal().m_data.m_array.m_size;

            m_semanticAnalyzer->m_memoryPtr->m_code += ( charArraySize / wordSize );
            m_semanticAnalyzer->m_memoryPtr->m_code += ( 0 == ( charArraySize % wordSize ) ? 0 : 1);
        }
        else
        {
            m_semanticAnalyzer->m_symbolTable->resolveSymbols(arg, m_semanticAnalyzer->m_memoryPtr->m_code);
            m_semanticAnalyzer->m_memoryPtr->m_code++;
        }
    }

    return CA_CONTINUE;
}

inline void AsmAdaptableTreeDecoder::dir_DB_phase2 ( CompilerStatement * node )
{
    std::vector<unsigned char> dbData;
    int wordSize = 1;

    switch ( m_semanticAnalyzer->m_device.m_memory.m_program.m_word )
    {
        case AdjSimProcDef::Memory::Program::WORD_1B: wordSize = 1; break;
        case AdjSimProcDef::Memory::Program::WORD_2B: wordSize = 2; break;
        case AdjSimProcDef::Memory::Program::WORD_3B: wordSize = 3; break;
    }

    for ( CompilerExpr * arg = node->args();
          nullptr != arg;
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
            int value = m_semanticAnalyzer->m_symbolTable->resolveExpr(arg);
            if ( ( wordSize >= 3 ) && ( value & 0xff0000 ) )
            {
                dbData.push_back ( ( value >> 16 ) & 0xff );
            }
            if ( wordSize >= 2 )
            {
                dbData.push_back ( ( value >> 8 ) & 0xff );
            }
            dbData.push_back ( value & 0xff );
        }
    }

    node->args()->completeDelete();
    node->m_args = nullptr;

    m_semanticAnalyzer->m_memoryPtr->m_code += ( dbData.size() / wordSize );
    m_semanticAnalyzer->m_memoryPtr->m_code += ( 0 == ( dbData.size() % wordSize ) ? 0 : 1);

    for ( size_t i = 0; i < dbData.size(); i++ )
    {
        unsigned int code = 0;

        if ( wordSize >= 3 )
        {
            code |= ( dbData[i] << 16 );
            i++;
        }

        if ( ( wordSize >= 2 ) && ( i < dbData.size() ) )
        {
            code |= ( dbData[i] << 8 );
            i++;
        }

        if ( i < dbData.size() )
        {
            code |= dbData[i];
        }

        if ( nullptr == node->m_args )
        {
            node->m_args = new CompilerExpr((int)code);
        }
        else
        {
            node->m_args->appendLink(new CompilerExpr((int)code));
        }
    }
}

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::dir_REPT ( CompilerStatement * node )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_CONTINUE;
    }

    unsigned int times = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
    if ( 0 == times )
    {
        return CA_CONTINUE;
    }
    if ( times >= MAX_REPEAT_ITERATIONS )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "maximum number of REPEAT directive "
                                                                            "iterations (%1) reached" )
                                                                         . arg(MAX_REPEAT_ITERATIONS)
                                                                         . toStdString(),
                                                                true );
    }

    CompilerSourceLocation lastLocation = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDR' directive

    CompilerStatement * body = new CompilerStatement();
    body->appendLink(node->branch()->copyEntireChain());
    m_semanticAnalyzer->m_codeListing->repeatCode(lastLocation, body, true);
    lastLocation = body->lastLeaf()->location();
    if ( false == phase1(body) )
    {
        body->completeDelete();
        return CA_RETURN_FALSE;
    }

    for ( unsigned int i = 1; i < times; i++ )
    {
        CompilerStatement * exp = new CompilerStatement();
        exp->appendLink(node->branch()->copyEntireChain());
        m_semanticAnalyzer->m_codeListing->repeatCode(lastLocation, exp, false);
        lastLocation = exp->lastLeaf()->location();
        if ( false == phase1(exp) )
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

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::dir_WHILE ( CompilerStatement * node )
{
    if ( node->branch()->last() == node->branch() )
    {
        return CA_NO_ACTION;
    }

    CompilerSourceLocation lastLocation = node->branch()->last()->location();
    delete node->branch()->last(); // remove `ENDW' directive

    unsigned int i;
    CompilerStatement * body = nullptr;
    for ( i = 0; i < MAX_WHILE_ITERATIONS; i++ )
    {
        if ( 0 == m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args()) )
        {
            break;
        }

        if ( nullptr == body )
        {
            body = (new CompilerStatement())->appendLink(node->branch()->copyEntireChain())->first();
            m_semanticAnalyzer->m_codeListing->repeatCode(lastLocation, body, true);
            lastLocation = body->lastLeaf()->location();
            if ( false == phase1(body) )
            {
                body->completeDelete();
                return CA_RETURN_FALSE;
            }
        }
        else
        {
            CompilerStatement * exp = (new CompilerStatement())->appendLink(node->branch()->copyEntireChain())->first();
            m_semanticAnalyzer->m_codeListing->repeatCode(lastLocation, exp, false);
            lastLocation = exp->lastLeaf()->location();
            if ( false == phase1(exp) )
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
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "maximum number of WHILE directive "
                                                                            "iterations (%1) reached" )
                                                                          . arg ( MAX_WHILE_ITERATIONS )
                                                                          . toStdString(),
                                                              true );
    }

    if ( nullptr != body)
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

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::macro ( CompilerStatement * node )
{
    CompilerExpr * name = node->args();
    while ( CompilerValue::TYPE_EXPR == name->lVal().m_type )
    {
        name = name->lVal().m_data.m_expr;
    }

    const std::string nameOfMacro = name->lVal().m_data.m_symbol;

    if ( true == isInstruction(nameOfMacro) )
    {
        return instruction2db(nameOfMacro, node);
    }

    if ( ( -1 != m_semanticAnalyzer->m_opts->m_maxMacroExp )
             &&
         ( (int) ( 0xffff & node->m_userData ) >= m_semanticAnalyzer->m_opts->m_maxMacroExp ) )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "maximum macro expansion level (%1) "
                                                                            "reached " )
                                                                          . arg ( m_semanticAnalyzer->
                                                                                  m_opts->
                                                                                  m_maxMacroExp )
                                                                          . toStdString(),
                                                              true );
        return CA_NO_ACTION;
    }

    // Check macro arguments.
    for ( CompilerExpr * arg = node->args()->next(); nullptr != arg; arg = arg->next() )
    {
        if ( ( CompilerExpr::OPER_HASH == arg->oper() ) || ( CompilerExpr::OPER_AT == arg->oper() ) )
        {
            m_semanticAnalyzer->m_compilerCore->semanticMessage ( arg->location(),
                                                                  CompilerBase::MT_ERROR,
                                                                  QObject::tr("invalid argument").toStdString() );
            return CA_RETURN_FALSE;
        }
    }

    CompilerStatement * macro = m_semanticAnalyzer->m_macros->expand(node->location(),nameOfMacro,node->args()->next());

    if ( CompilerStatementTypes::EMPTY_STATEMENT == macro->type() )
    {
        macro->completeDelete();
        return CA_NO_ACTION;
    }

    //
    macro = (new CompilerStatement())->appendLink(macro)->first();

    if ( false == phase1(macro, &nameOfMacro) )
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

inline void AsmAdaptableTreeDecoder::dir_MACRO ( CompilerStatement * node )
{
    if ( true == isInstruction(node->args()->lVal().m_data.m_symbol) )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->args()->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr("macro cannot have the same name as an "
                                                                          "instruction mnemonic").toStdString());
        return;
    }

    m_semanticAnalyzer->m_macros -> define ( node->location(),
                                             node->args()->lVal().m_data.m_symbol,
                                             node->args()->m_next,
                                             node->branch() );
    node->m_branch = nullptr;
}

inline void AsmAdaptableTreeDecoder::dir_TITLE ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_semanticAnalyzer->m_codeListing->setTitle(arg);
}

inline void AsmAdaptableTreeDecoder::dir_MESSG ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_semanticAnalyzer->m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_REMARK, arg, true);
}

inline void AsmAdaptableTreeDecoder::dir_WARNING ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_semanticAnalyzer->m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_WARNING, arg, true);
}

inline void AsmAdaptableTreeDecoder::dir_ERROR ( CompilerStatement * node )
{
    const CompilerValue::Data::CharArray & argCharArray = node->args()->lVal().m_data.m_array;
    std::string arg ( (char*) argCharArray.m_data, argCharArray.m_size );
    m_semanticAnalyzer->m_compilerCore->semanticMessage(node->location(), CompilerBase::MT_ERROR, arg, true);
}

inline void AsmAdaptableTreeDecoder::dir_LIST ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_codeListing->setNoList(node->location(), false);
}

inline void AsmAdaptableTreeDecoder::dir_NOLIST ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_codeListing->setNoList(node->location(), true);
}

inline void AsmAdaptableTreeDecoder::dir_EXPAND()
{
    m_semanticAnalyzer->m_macros->setExpEnabled(true);
}

inline void AsmAdaptableTreeDecoder::dir_NOEXPAND()
{
    m_semanticAnalyzer->m_macros->setExpEnabled(false);
}

inline AsmAdaptableTreeDecoder::CourseOfAction
       AsmAdaptableTreeDecoder::dir_EXITM ( CompilerStatement * node )
{
    if ( false == m_semanticAnalyzer->m_macros->isFromMacro(node) )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "directive `EXITM' cannot appear outside "
                                                                            "macro definition")
                                                                          .toStdString(),
                                                              true );
        return CA_NO_ACTION;
    }

    node->prev()->m_next = nullptr;
    node->m_prev = nullptr;
    node->completeDelete();
    return CA_RETURN_TRUE;
}

inline void AsmAdaptableTreeDecoder::dir_INCLUDE ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_codeListing->setInclusion(node->location(), (int)(node->args()->m_lValue.m_data.m_integer));
}

inline void AsmAdaptableTreeDecoder::dir_LOCAL ( CompilerStatement * node,
                                                 const std::string * macroName,
                                                 std::vector<std::string> & localSymbols )
{
    if ( false == m_semanticAnalyzer->m_macros->isFromMacro(node) )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "directive `LOCAL' cannot appear outside "
                                                                            "macro definition" )
                                                                          . toStdString(),
                                                              true );
    }

    const std::string local = node->args()->lVal().m_data.m_symbol;
    if ( false == m_semanticAnalyzer->m_macros -> mangleName ( node->location(),
                                           &localSymbols,
                                           local,
                                           *macroName,
                                           node->next() ) )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_WARNING,
                                                              QObject::tr ( "symbol `%1' declared as local but never "
                                                                            "used, declaration ignored"  )
                                                                          . arg ( local.c_str() )
                                                                          . toStdString() );
    }
}

inline void AsmAdaptableTreeDecoder::dir_SET ( CompilerStatement * node )
{
    const CompilerValue * nameVal;
    for ( nameVal = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == nameVal->m_type;
          nameVal = &( nameVal->m_data.m_expr->lVal() ) );

    std::string name = nameVal->m_data.m_symbol;
    int value;

    if ( true == m_semanticAnalyzer->m_symbolTable->isDefined(name) )
    {
        value = m_semanticAnalyzer->m_symbolTable->assignValue ( name,
                                                                 node->args()->next(),
                                                                 &( node->location() ),
                                                                 AsmSymbolTable::STYPE_NUMBER,
                                                                 true );
    }
    else
    {
        value = m_semanticAnalyzer->m_symbolTable->addSymbol ( name,
                                                               node->args()->next(),
                                                               &( node->location() ),
                                                               AsmSymbolTable::STYPE_NUMBER,
                                                               true,
                                                               true );
    }

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), value);
}

inline void AsmAdaptableTreeDecoder::dir_DEFINE ( CompilerStatement * node )
{
    int value = m_semanticAnalyzer->m_symbolTable->addSymbol ( node->args()->lVal().m_data.m_symbol,
                                                               node->args()->next(),
                                                               &( node->location() ),
                                                               AsmSymbolTable::STYPE_EXPRESSION,
                                                               false );

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), value);
}

inline void AsmAdaptableTreeDecoder::dir_UNDEFINE ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_symbolTable->removeSymbol ( node->args()->lVal().m_data.m_symbol, node->location() );
}

inline void AsmAdaptableTreeDecoder::label ( CompilerStatement * node )
{
    CompilerExpr * label = node->args();
    while ( CompilerValue::TYPE_EXPR == label->lVal().m_type )
    {
        label = label->lVal().m_data.m_expr;
    }

    CompilerExpr e(m_semanticAnalyzer->m_memoryPtr->m_code);
    int value = m_semanticAnalyzer->m_symbolTable->addSymbol ( label->lVal().m_data.m_symbol,
                                                               &e,
                                                               &( node->location() ),
                                                               AsmSymbolTable::STYPE_LABEL,
                                                               true );

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), value);
}

inline void AsmAdaptableTreeDecoder::dir_ORG ( CompilerStatement * node )
{
    m_semanticAnalyzer->m_memoryPtr->m_code = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
    m_semanticAnalyzer->m_codeListing->setValue(node->location(), m_semanticAnalyzer->m_memoryPtr->m_code);
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(m_semanticAnalyzer->m_memoryPtr->m_code);
}

inline void AsmAdaptableTreeDecoder::dir_SKIP ( CompilerStatement * node )
{
    int skip = m_semanticAnalyzer->m_symbolTable->resolveExpr(node->args());
    m_semanticAnalyzer->m_memoryPtr->m_code += skip;
    node->args()->completeDelete();
    node->m_args = new CompilerExpr(skip);
}

inline void AsmAdaptableTreeDecoder::dir_FAILJMP ( CompilerStatement * node )
{
    if ( nullptr != m_failjmp )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr ( "only one fail jump may be specified in the"
                                                                            " code") . toStdString(),
                                                              true );
    }

    m_failjmp = resolveInstruction(m_semanticAnalyzer->m_device.m_failjmp, node);
}

inline void AsmAdaptableTreeDecoder::dir_LIMIT ( CompilerStatement * node )
{
    const char * limSel = node->args()->lVal().m_data.m_symbol;
    int limVal = m_semanticAnalyzer->m_symbolTable->resolveExpr ( node->args()->next() );

    if ( -1 == limVal )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_REMARK,
                                                              QObject::tr("limit value -1 means unlimited")
                                                                         .toStdString(),
                                                              true );
    }
    else if ( -1 > limVal )
    {
        m_semanticAnalyzer->m_compilerCore->semanticMessage ( node->location(),
                                                              CompilerBase::MT_ERROR,
                                                              QObject::tr("limit value %1 is not valid")
                                                                         .arg(limVal)
                                                                         .toStdString(),
                                                              true );
        return;
    }

    if ( 0 == strcmp("R", limSel) )
    {
        m_semanticAnalyzer->m_opts->m_processorlimits.m_regFileSize = limVal;
    }
    else if ( 0 == strcmp("D", limSel) )
    {
        m_semanticAnalyzer->m_opts->m_processorlimits.m_iDataMemSize = limVal;
    }
    else if ( 0 == strcmp("C", limSel) )
    {
        m_semanticAnalyzer->m_opts->m_processorlimits.m_iCodeMemSize = limVal;
    }
}

inline void AsmAdaptableTreeDecoder::dir_EQU_etc ( CompilerStatement * node )
{
    using namespace CompilerStatementTypes;

    AsmSymbolTable::SymbolType symbolType;

    switch ( (int) node->type() )
    {
        case ASMPICOBLAZE_DIR_CODE:
            symbolType = AsmSymbolTable::STYPE_LABEL;
            break;
        case ASMPICOBLAZE_DIR_DATA:
            symbolType = AsmSymbolTable::STYPE_DATA;
            break;
        case ASMPICOBLAZE_DIR_PORT:
            symbolType = AsmSymbolTable::STYPE_PORT;
            break;
        case ASMPICOBLAZE_DIR_PORTIN:
            symbolType = AsmSymbolTable::STYPE_PORTIN;
            break;
        case ASMPICOBLAZE_DIR_PORTOUT:
            symbolType = AsmSymbolTable::STYPE_PORTOUT;
            break;
        case ASMPICOBLAZE_DIR_REG:
            symbolType = AsmSymbolTable::STYPE_REGISTER;
            break;
        case ASMPICOBLAZE_DIR_EQU:
            symbolType = AsmSymbolTable::STYPE_NUMBER;
            break;
        default:
            symbolType = AsmSymbolTable::STYPE_UNSPECIFIED;
            break;
    }

    const CompilerValue * nameVal;
    for ( nameVal = &( node->args()->lVal() );
          CompilerValue::TYPE_EXPR == nameVal->m_type;
          nameVal = &( nameVal->m_data.m_expr->lVal() ) );

    std::string name = nameVal->m_data.m_symbol;

    int value = m_semanticAnalyzer->m_symbolTable->addSymbol ( name,
                                                               node->args()->m_next,
                                                               &( node->location() ),
                                                               symbolType,
                                                               true );

    m_semanticAnalyzer->m_codeListing->setValue(node->location(), value);

    if ( ( value >= 0 ) && ( -1 != node->location().m_fileNumber ) )
    {
        if ( node->type() == ASMPICOBLAZE_DIR_REG )
        {
            m_semanticAnalyzer->m_memoryPtr->tryReserve(node->location(), AsmMemoryPtr::MS_REG, (unsigned int) value);
        }
        else if ( node->type() == ASMPICOBLAZE_DIR_DATA )
        {
            m_semanticAnalyzer->m_memoryPtr->tryReserve(node->location(), AsmMemoryPtr::MS_DATA, (unsigned int) value);
        }
    }
}

inline void AsmAdaptableTreeDecoder::dir_DEVICE ( CompilerStatement * node )
{
    m_semanticAnalyzer->setDevice(node->args()->lVal().m_data.m_symbol);
}

bool AsmAdaptableTreeDecoder::isBlank ( const CompilerExpr * expr ) const
{
    if ( CompilerExpr::OPER_NONE != expr->oper() )
    {
        return false;
    }

    if ( CompilerValue::TYPE_EMPTY == expr->lVal().m_type )
    {
        return true;
    }
    else if ( CompilerValue::TYPE_EXPR == expr->lVal().m_type )
    {
        return isBlank(expr->lVal().m_data.m_expr);
    }
    else
    {
        return false;
    }
}

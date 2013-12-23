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
 * @file AsmPicoBlazeInstructionSet.cxx
 */
// =============================================================================

#include "AsmPicoBlazeInstructionSet.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSymbolTable.h"
#include "AsmPicoBlazeInstructionSetN.h"
#include "AsmPicoBlazeInstructionSet6.h"

// Common compiler header files.
#include "CompilerStatement.h"
#include "CompilerOptions.h"

AsmPicoBlazeInstructionSet::~AsmPicoBlazeInstructionSet()
{
    if ( NULL != m_strategy )
    {
        delete m_strategy;
    }
}

unsigned int AsmPicoBlazeInstructionSet::checkLimit ( LimitType type,
                                                      const CompilerSourceLocation & location,
                                                      unsigned int value ) const
{
    int limit = -1;
    switch ( type )
    {
        case LIM_C:
            limit = m_opts->m_processorlimits.m_iCodeMemSize;
            break;
        case LIM_R:
            limit = m_opts->m_processorlimits.m_regFileSize;
            break;
        case LIM_D:
            limit = m_opts->m_processorlimits.m_iDataMemSize;
            break;
    }

    if ( -1 == limit )
    {
        // There is no limit set.
        return value;
    }

    if ( int(value) < limit )
    {
        // Limit is not exceeded.
        return value;
    }

    // Limit exceeded, print error message.
    std::string msg;
    switch ( type )
    {
        case LIM_C:
            msg = QObject::tr ( "user defined memory limit for program memory exceeded ( %1 > %2 )" )
                              . arg ( value )
                              . arg ( limit - 1 )
                              . toStdString();
            break;
        case LIM_R:
            msg = QObject::tr ( "user defined memory limit for register file exceeded ( %1 > %2 )" )
                              . arg ( value )
                              . arg ( limit - 1 )
                              . toStdString();
            break;
        case LIM_D:
            msg = QObject::tr ( "user defined memory limit for Scratch Pad RAM exceeded ( %1 > %2 )" )
                              . arg ( value )
                              . arg ( limit - 1 )
                              . toStdString();
            break;
    }
    m_compilerCore -> semanticMessage ( location, CompilerBase::MT_ERROR, msg );

    return 0;
}

int AsmPicoBlazeInstructionSet::resolveOPcode ( const CompilerStatement * stmt )
{
    if ( NULL == m_strategy )
    {
        *m_device = AsmPicoBlazeSemanticAnalyzer::DEV_KCPSM6;
        setStrategy(new AsmPicoBlazeInstructionSet6());
        m_compilerCore->semanticMessage ( CompilerSourceLocation(),
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "exact device not specified, using KCPSM6 by default" )
                                                      .toStdString() );
    }
    return m_strategy->resolveOPcode(stmt);
}

inline void AsmPicoBlazeInstructionSet::detAccSymTypes ( const CompilerStatement * stmt,
                                                         int * acceptableTypes ) const
{
    using namespace CompilerStatementTypes;

    acceptableTypes[0] = (int) ( AsmPicoBlazeSymbolTable::STYPE_NUMBER | AsmPicoBlazeSymbolTable::STYPE_EXPRESSION );
    acceptableTypes[1] = (int) ( AsmPicoBlazeSymbolTable::STYPE_NUMBER | AsmPicoBlazeSymbolTable::STYPE_EXPRESSION );

    switch ( (int) stmt->type() )
    {
        case ASMPICOBLAZE_INS_JUMP_AAA:
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:
        case ASMPICOBLAZE_INS_JUMP_C_AAA:
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:
        case ASMPICOBLAZE_INS_CALL_AAA:
        case ASMPICOBLAZE_INS_CALL_Z_AAA:
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:
        case ASMPICOBLAZE_INS_CALL_C_AAA:
        case ASMPICOBLAZE_INS_CALL_NC_AAA:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_LABEL;
            break;

        case ASMPICOBLAZE_INS_RETURN:
        case ASMPICOBLAZE_INS_RETURN_Z:
        case ASMPICOBLAZE_INS_RETURN_NZ:
        case ASMPICOBLAZE_INS_RETURN_C:
        case ASMPICOBLAZE_INS_RETURN_NC:
        case ASMPICOBLAZE_INS_RETURNI_ENA:
        case ASMPICOBLAZE_INS_RETURNI_DIS:
        case ASMPICOBLAZE_INS_ENABLE_INT:
        case ASMPICOBLAZE_INS_DISABLE_INT:
        case ASMPICOBLAZE_INS_REGBANK_A:
        case ASMPICOBLAZE_INS_REGBANK_B:
            break;

        case ASMPICOBLAZE_INS_TEST_SX_KK:
        case ASMPICOBLAZE_INS_ADD_SX_KK:
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:
        case ASMPICOBLAZE_INS_SUB_SX_KK:
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:
        case ASMPICOBLAZE_INS_COMPARE_SX_KK:
        case ASMPICOBLAZE_INS_LOAD_SX_KK:
        case ASMPICOBLAZE_INS_AND_SX_KK:
        case ASMPICOBLAZE_INS_OR_SX_KK:
        case ASMPICOBLAZE_INS_XOR_SX_KK:
        case ASMPICOBLAZE_INS_LD_RET_SX_KK:
        case ASMPICOBLAZE_INS_TESTCY_SX_KK:
        case ASMPICOBLAZE_INS_COMPARECY_SX_KK:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            break;

        case ASMPICOBLAZE_INS_ADD_SX_SY:
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:
        case ASMPICOBLAZE_INS_SUB_SX_SY:
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:
        case ASMPICOBLAZE_INS_COMPARE_SX_SY:
        case ASMPICOBLAZE_INS_LOAD_SX_SY:
        case ASMPICOBLAZE_INS_AND_SX_SY:
        case ASMPICOBLAZE_INS_OR_SX_SY:
        case ASMPICOBLAZE_INS_XOR_SX_SY:
        case ASMPICOBLAZE_INS_TEST_SX_SY:
        case ASMPICOBLAZE_INS_STORE_SX_SY:
        case ASMPICOBLAZE_INS_FETCH_SX_SY:
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:
        case ASMPICOBLAZE_INS_INPUT_SX_SY:
        case ASMPICOBLAZE_INS_JUMP_SX_SY:
        case ASMPICOBLAZE_INS_CALL_SX_SY:
        case ASMPICOBLAZE_INS_COMPARECY_SX_SY:
        case ASMPICOBLAZE_INS_TESTCY_SX_SY:
        case ASMPICOBLAZE_INS_STAR_SX_SY:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            break;

        case ASMPICOBLAZE_INS_SR0_SX:
        case ASMPICOBLAZE_INS_SR1_SX:
        case ASMPICOBLAZE_INS_SRX_SX:
        case ASMPICOBLAZE_INS_SRA_SX:
        case ASMPICOBLAZE_INS_RR_SX:
        case ASMPICOBLAZE_INS_SL0_SX:
        case ASMPICOBLAZE_INS_SL1_SX:
        case ASMPICOBLAZE_INS_SLA_SX:
        case ASMPICOBLAZE_INS_SLX_SX:
        case ASMPICOBLAZE_INS_RL_SX:
        case ASMPICOBLAZE_INS_HWBUILD_SX:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            break;

        case ASMPICOBLAZE_INS_STORE_SX_SS:
        case ASMPICOBLAZE_INS_FETCH_SX_SS:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmPicoBlazeSymbolTable::STYPE_DATA;
            break;

        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:
        case ASMPICOBLAZE_INS_INPUT_SX_PP:
            acceptableTypes[0] |= AsmPicoBlazeSymbolTable::STYPE_REGISTER;
            acceptableTypes[1] |= AsmPicoBlazeSymbolTable::STYPE_PORT;
            break;

        case ASMPICOBLAZE_INS_OUTPUTK_KK_P:
            acceptableTypes[1] |= AsmPicoBlazeSymbolTable::STYPE_PORT;
            break;

        default:
            break;
    }
}

std::string AsmPicoBlazeInstructionSet::getInstructionName ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        case ASMPICOBLAZE_INS_JUMP_AAA:         return "JUMP aaa";
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:       return "JUMP Z, aaa";
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:      return "JUMP NZ, aaa";
        case ASMPICOBLAZE_INS_JUMP_C_AAA:       return "JUMP C, aaa";
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:      return "JUMP NC, aaa";
        case ASMPICOBLAZE_INS_CALL_AAA:         return "CALL aaa";
        case ASMPICOBLAZE_INS_CALL_Z_AAA:       return "CALL Z, aaa";
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:      return "CALL NZ, aaa";
        case ASMPICOBLAZE_INS_CALL_C_AAA:       return "CALL C, aaa";
        case ASMPICOBLAZE_INS_CALL_NC_AAA:      return "CALL NC, aaa";
        case ASMPICOBLAZE_INS_RETURN:           return "RETURN";
        case ASMPICOBLAZE_INS_RETURN_Z:         return "RETURN Z";
        case ASMPICOBLAZE_INS_RETURN_NZ:        return "RETURN NZ";
        case ASMPICOBLAZE_INS_RETURN_C:         return "RETURN C";
        case ASMPICOBLAZE_INS_RETURN_NC:        return "RETURN NC";
        case ASMPICOBLAZE_INS_ADD_SX_KK:        return "ADD sX, kk";
        case ASMPICOBLAZE_INS_ADD_SX_SY:        return "ADD sX, sY";
        case ASMPICOBLAZE_INS_ADDCY_SX_KK:      return "ADDCY sX, kk";
        case ASMPICOBLAZE_INS_ADDCY_SX_SY:      return "ADDCY sX, sY";
        case ASMPICOBLAZE_INS_SUB_SX_KK:        return "ADD sX, kk";
        case ASMPICOBLAZE_INS_SUB_SX_SY:        return "SUB sX, sY";
        case ASMPICOBLAZE_INS_SUBCY_SX_KK:      return "SUBCY sX, kk";
        case ASMPICOBLAZE_INS_SUBCY_SX_SY:      return "SUBCY sX, sY";
        case ASMPICOBLAZE_INS_COMPARE_SX_KK:    return "COMPARE sX, kk";
        case ASMPICOBLAZE_INS_RETURNI_ENA:      return "RETURN ENABLE";
        case ASMPICOBLAZE_INS_RETURNI_DIS:      return "RETURN DISABLE";
        case ASMPICOBLAZE_INS_COMPARE_SX_SY:    return "COMPARE sX, sY";
        case ASMPICOBLAZE_INS_ENABLE_INT:       return "ENABLE INTERRUPT";
        case ASMPICOBLAZE_INS_DISABLE_INT:      return "DISABLE INTERRUPT";
        case ASMPICOBLAZE_INS_LOAD_SX_KK:       return "LOAD sX, kk";
        case ASMPICOBLAZE_INS_LOAD_SX_SY:       return "LOAD sX, sY";
        case ASMPICOBLAZE_INS_AND_SX_KK:        return "AND sX, kk";
        case ASMPICOBLAZE_INS_AND_SX_SY:        return "AND sX, sY";
        case ASMPICOBLAZE_INS_OR_SX_KK:         return "OR sX, kk";
        case ASMPICOBLAZE_INS_OR_SX_SY:         return "OR sX, sY";
        case ASMPICOBLAZE_INS_XOR_SX_KK:        return "XOR sX, kk";
        case ASMPICOBLAZE_INS_XOR_SX_SY:        return "YOR sX, sY";
        case ASMPICOBLAZE_INS_TEST_SX_KK:       return "TEST sX, kk";
        case ASMPICOBLAZE_INS_TEST_SX_SY:       return "TEST sX, sY";
        case ASMPICOBLAZE_INS_STORE_SX_SS:      return "STORE sX, ss";
        case ASMPICOBLAZE_INS_STORE_SX_SY:      return "STORE sX, sY";
        case ASMPICOBLAZE_INS_FETCH_SX_SS:      return "FETCH sX, ss";
        case ASMPICOBLAZE_INS_FETCH_SX_SY:      return "FETCH sX, sY";
        case ASMPICOBLAZE_INS_SR0_SX:           return "SR0 sX";
        case ASMPICOBLAZE_INS_SR1_SX:           return "SR1 sX";
        case ASMPICOBLAZE_INS_SRX_SX:           return "SRX sX";
        case ASMPICOBLAZE_INS_SRA_SX:           return "SRA sX";
        case ASMPICOBLAZE_INS_RR_SX:            return "RR sX";
        case ASMPICOBLAZE_INS_SL0_SX:           return "SL0 sX";
        case ASMPICOBLAZE_INS_SL1_SX:           return "SL1 sX";
        case ASMPICOBLAZE_INS_SLA_SX:           return "SLA sX";
        case ASMPICOBLAZE_INS_SLX_SX:           return "SLX sX";
        case ASMPICOBLAZE_INS_RL_SX:            return "RL sX";
        case ASMPICOBLAZE_INS_OUTPUT_SX_PP:     return "OUTPUT sX, pp";
        case ASMPICOBLAZE_INS_OUTPUT_SX_SY:     return "OUTPUT sX, sY";
        case ASMPICOBLAZE_INS_INPUT_SX_PP:      return "INPUT sX, pp";
        case ASMPICOBLAZE_INS_INPUT_SX_SY:      return "INPUT sX, sY";
        case ASMPICOBLAZE_INS_JUMP_SX_SY:       return "JUMP &(sX, sY)";
        case ASMPICOBLAZE_INS_CALL_SX_SY:       return "CALL &(sX, sY)";
        case ASMPICOBLAZE_INS_LD_RET_SX_KK:     return "LOAD & RETURN sX, kk";
        case ASMPICOBLAZE_INS_HWBUILD_SX:       return "HWBUILD sX";
        case ASMPICOBLAZE_INS_STAR_SX_SY:       return "STAR sX. sY";
        case ASMPICOBLAZE_INS_TESTCY_SX_SY:     return "TESTCY sX, sY";
        case ASMPICOBLAZE_INS_TESTCY_SX_KK:     return "TESTCY sX, kk";
        case ASMPICOBLAZE_INS_COMPARECY_SX_SY:  return "COMPARECY sX, sY";
        case ASMPICOBLAZE_INS_COMPARECY_SX_KK:  return "COMPARECY sX, kk";
        case ASMPICOBLAZE_INS_REGBANK_A:        return "REGBANK A";
        case ASMPICOBLAZE_INS_REGBANK_B:        return "REGBANK B";
        case ASMPICOBLAZE_INS_OUTPUTK_KK_P:     return "OUTPUTK kk, p";
        default:                                return "<unknown>";
    }
}

inline std::string AsmPicoBlazeInstructionSet::getSymbolTypes ( int types ) const
{
    std::string result;

    if ( types & AsmPicoBlazeSymbolTable::STYPE_NUMBER )
    {
        result += "number";
    }

    if ( types & AsmPicoBlazeSymbolTable::STYPE_REGISTER )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "register";
    }

    if ( types & AsmPicoBlazeSymbolTable::STYPE_LABEL )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "label";
    }

    if ( types & AsmPicoBlazeSymbolTable::STYPE_PORT )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "port";
    }

    if ( types & AsmPicoBlazeSymbolTable::STYPE_DATA )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "data";
    }

    if ( types & AsmPicoBlazeSymbolTable::STYPE_EXPRESSION )
    {
        if ( false == result.empty() )
        {
            result += ", or ";
        }
        result += "expression";
    }

    if ( true == result.empty() )
    {
        result = "<no type>";
    }

    return result;
}

void AsmPicoBlazeInstructionSet::encapsulate ( CompilerStatement * stmt,
                                               int codePointer ) const
{
    int acceptableTypes[2];
    detAccSymTypes(stmt, acceptableTypes);

    int i = 0;
    for ( CompilerExpr * arg = stmt->args();
          NULL != arg;
          arg = arg->next() )
    {
        int symbolType = (int) m_symbolTable->getType(arg);
        if ( 0 == ( acceptableTypes[i] & symbolType ) )
        {
            m_compilerCore->semanticMessage(arg->location(),
                                            CompilerBase::MT_ERROR,
                                            QObject::tr ( "instruction `%1' requires operand #%2 to be of type(s): %3"
                                                          "; while the given operand is of type: %4" )
                                                        . arg ( getInstructionName(stmt).c_str() )
                                                        . arg ( i + oprIdxShift(stmt) )
                                                        . arg ( getSymbolTypes(acceptableTypes[i]).c_str() )
                                                        . arg ( getSymbolTypes(symbolType).c_str() )
                                                        . toStdString() );
        }

        m_symbolTable->resolveSymbols(arg, codePointer);
        i++;
    }
}

inline int AsmPicoBlazeInstructionSet::oprIdxShift ( const CompilerStatement * stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        case ASMPICOBLAZE_INS_JUMP_Z_AAA:
        case ASMPICOBLAZE_INS_JUMP_NZ_AAA:
        case ASMPICOBLAZE_INS_JUMP_C_AAA:
        case ASMPICOBLAZE_INS_JUMP_NC_AAA:
        case ASMPICOBLAZE_INS_CALL_Z_AAA:
        case ASMPICOBLAZE_INS_CALL_NZ_AAA:
        case ASMPICOBLAZE_INS_CALL_C_AAA:
        case ASMPICOBLAZE_INS_CALL_NC_AAA:
            return 2;
        default:
            return 1;
    }
}

bool AsmPicoBlazeInstructionSet::isInstruction ( const CompilerStatement * const stmt ) const
{
    using namespace CompilerStatementTypes;

    switch ( (int) stmt->type() )
    {
        case ASMPICOBLAZE_INS_JUMP_AAA:    case ASMPICOBLAZE_INS_JUMP_Z_AAA:  case ASMPICOBLAZE_INS_JUMP_NZ_AAA:
        case ASMPICOBLAZE_INS_JUMP_C_AAA:  case ASMPICOBLAZE_INS_JUMP_NC_AAA: case ASMPICOBLAZE_INS_CALL_AAA:
        case ASMPICOBLAZE_INS_CALL_Z_AAA:  case ASMPICOBLAZE_INS_CALL_NZ_AAA: case ASMPICOBLAZE_INS_CALL_C_AAA:
        case ASMPICOBLAZE_INS_CALL_NC_AAA: case ASMPICOBLAZE_INS_RETURN:      case ASMPICOBLAZE_INS_RETURN_Z:
        case ASMPICOBLAZE_INS_RETURN_NZ:   case ASMPICOBLAZE_INS_RETURN_C:    case ASMPICOBLAZE_INS_RETURN_NC:
        case ASMPICOBLAZE_INS_ADD_SX_KK:   case ASMPICOBLAZE_INS_ADD_SX_SY:   case ASMPICOBLAZE_INS_ADDCY_SX_KK:
        case ASMPICOBLAZE_INS_ADDCY_SX_SY: case ASMPICOBLAZE_INS_SUB_SX_KK:   case ASMPICOBLAZE_INS_SUB_SX_SY:
        case ASMPICOBLAZE_INS_SUBCY_SX_KK: case ASMPICOBLAZE_INS_SUBCY_SX_SY: case ASMPICOBLAZE_INS_COMPARE_SX_KK:
        case ASMPICOBLAZE_INS_RETURNI_ENA: case ASMPICOBLAZE_INS_RETURNI_DIS: case ASMPICOBLAZE_INS_COMPARE_SX_SY:
        case ASMPICOBLAZE_INS_ENABLE_INT:  case ASMPICOBLAZE_INS_DISABLE_INT: case ASMPICOBLAZE_INS_LOAD_SX_KK:
        case ASMPICOBLAZE_INS_LOAD_SX_SY:  case ASMPICOBLAZE_INS_AND_SX_KK:   case ASMPICOBLAZE_INS_AND_SX_SY:
        case ASMPICOBLAZE_INS_OR_SX_KK:    case ASMPICOBLAZE_INS_OR_SX_SY:    case ASMPICOBLAZE_INS_XOR_SX_KK:
        case ASMPICOBLAZE_INS_XOR_SX_SY:   case ASMPICOBLAZE_INS_TEST_SX_KK:  case ASMPICOBLAZE_INS_TEST_SX_SY:
        case ASMPICOBLAZE_INS_STORE_SX_SS: case ASMPICOBLAZE_INS_STORE_SX_SY: case ASMPICOBLAZE_INS_FETCH_SX_SS:
        case ASMPICOBLAZE_INS_FETCH_SX_SY: case ASMPICOBLAZE_INS_SR0_SX:      case ASMPICOBLAZE_INS_COMPARECY_SX_SY:
        case ASMPICOBLAZE_INS_SRX_SX:      case ASMPICOBLAZE_INS_SRA_SX:      case ASMPICOBLAZE_INS_COMPARECY_SX_KK:
        case ASMPICOBLAZE_INS_SL0_SX:      case ASMPICOBLAZE_INS_SL1_SX:      case ASMPICOBLAZE_INS_OUTPUT_SX_SY:
        case ASMPICOBLAZE_INS_SLA_SX:      case ASMPICOBLAZE_INS_RL_SX:       case ASMPICOBLAZE_INS_INPUT_SX_PP:
        case ASMPICOBLAZE_INS_INPUT_SX_SY: case ASMPICOBLAZE_INS_SLX_SX:      case ASMPICOBLAZE_INS_OUTPUT_SX_PP:
        case ASMPICOBLAZE_INS_JUMP_SX_SY:  case ASMPICOBLAZE_INS_CALL_SX_SY:  case ASMPICOBLAZE_INS_LD_RET_SX_KK:
        case ASMPICOBLAZE_INS_HWBUILD_SX:  case ASMPICOBLAZE_INS_STAR_SX_SY:  case ASMPICOBLAZE_INS_TESTCY_SX_SY:
        case ASMPICOBLAZE_INS_RR_SX:       case ASMPICOBLAZE_INS_SR1_SX:      case ASMPICOBLAZE_INS_TESTCY_SX_KK:
        case ASMPICOBLAZE_INS_REGBANK_A:   case ASMPICOBLAZE_INS_REGBANK_B:   case ASMPICOBLAZE_INS_OUTPUTK_KK_P:
            return true;
        default:
            return false;
    }
}

void AsmPicoBlazeInstructionSet::setStrategy ( AsmPicoBlazeInstructionSetN * strategy )
{
    if ( NULL != m_strategy )
    {
        delete m_strategy;
    }

    m_strategy = strategy;
    m_strategy->init(this, m_symbolTable);
}

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
 * @ingroup AdaptableSim
 * @file AdaptableSimInstruction.cxx
 */
// =============================================================================

#include "AdaptableSimInstruction.h"

std::ostream & operator << ( std::ostream & out,
                             const AdaptableSimInstruction & instruction )
{
    out << std::dec;
    out << "ID:   "            << instruction.m_id    << std::endl;
    out << "Time [positive]: " << instruction.m_timeP << std::endl;
    out << "Time [negative]: " << instruction.m_timeN << std::endl;

    out << std::hex;
    out << "Code: 0x"          << instruction.m_code      << std::endl;
    out << "Mask: 0x"          << instruction.m_mask      << std::endl;
    out << "Operation: "       << instruction.m_operation << std::endl;
    out << "Next [positive]: " << instruction.m_nextP     << std::endl;
    out << "Next [negative]: " << instruction.m_nextN     << std::endl;

    out << "Operands:" << std::endl;
    for ( unsigned int i = 0; i < AdaptableSimInstruction::OPERANDS_MAX; i++ )
    {
        out << "    #" << i << ": ";

        if ( true == instruction.m_permutation[i].empty() )
        {
            out << "(empty)" << std::endl;
            continue;
        }

        bool first = true;
        for ( int j = ( instruction.m_permutation[i].size() - 1 ); j >= 0; j-- )
        {
            if ( false == first )
            {
                out << ", ";
            }
            first = false;
            out << (int) instruction.m_permutation[i][j];
        }
        out << " ( lsb -> msb )" << std::endl;
    }

    out << "Fixed operands:" << std::endl;
    out << instruction.m_fixedOperands << std::endl;

    out << "Operation parameters:" << std::endl;
//     TODO: uncomment this when debugging is done.
//     out << instruction.m_parameters << std::endl;

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::OperParam operParam )
{
    out << std::boolalpha;
    out << "    Even parity:         " << operParam.evenParity()       << std::endl;
    out << "    Next negative:       " << operParam.nextN()            << std::endl;
    out << "    Next positive:       " << operParam.nextP()            << std::endl;
    out << "    Discart result:      " << operParam.discartResult()    << std::endl;
    out << "    Ignore carry flag:   " << operParam.ignoreCarry()      << std::endl;
    out << "    Result in 3rd oper.: " << operParam.separateResult()   << std::endl;

    out << "    Execution condition: " << operParam.condition()        << std::endl;
    out << "    Condition flag: "      << operParam.conditionFlag()    << std::endl;

    out << "    Processor flags:" << std::endl;
    for ( int i = 0; i < 8; i++ )
    {
        out << "        " << (AdaptableSimInstruction::OperParam::ProcFlag) i << " - "
            << operParam.flagAttr(AdaptableSimInstruction::OperParam::ProcFlag(i)) << std::endl;
    }

    out << "    Operand address space and addressing modes:" << std::endl;
    for ( unsigned int i = 0; i < AdaptableSimInstruction::OPERANDS_MAX; i++ )
    {
        out << "        Operand #" << i << ": " << operParam.addressingMode(i) << std::endl;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::OperParam::Condition condition )
{
    switch ( condition )
    {
        case AdaptableSimInstruction::OperParam::C_NONE:     out << "NONE";     break;
        case AdaptableSimInstruction::OperParam::C_POSITIVE: out << "POSITIVE"; break;
        case AdaptableSimInstruction::OperParam::C_NEGATIVE: out << "NEGATIVE"; break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::OperParam::FlagAttr attribute )
{
    switch ( attribute )
    {
        case AdaptableSimInstruction::OperParam::P_KEEP:   out << "KEEP";       break;
        case AdaptableSimInstruction::OperParam::P_SET:    out << "SET";        break;
        case AdaptableSimInstruction::OperParam::P_CLEAR:  out << "CLEAR";      break;
        case AdaptableSimInstruction::OperParam::P_AFFECT: out << "AFFECT";     break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::OperParam::ProcFlag flag )
{
    switch ( flag )
    {
        case AdaptableSimInstruction::OperParam::F_ZERO:        out << "ZERO";          break;
        case AdaptableSimInstruction::OperParam::F_CARRY:       out << "CARRY";         break;
        case AdaptableSimInstruction::OperParam::F_OVERFLOW:    out << "OVERFLOW";      break;
        case AdaptableSimInstruction::OperParam::F_NEGATIVE:    out << "NEGATIVE";      break;
        case AdaptableSimInstruction::OperParam::F_HALF_CARRY:  out << "HALF_CARRY";    break;
        case AdaptableSimInstruction::OperParam::F_INTR_ENABLE: out << "INTR_ENABLE";   break;
        case AdaptableSimInstruction::OperParam::F_PARITY:      out << "PARITY";        break;
        case AdaptableSimInstruction::OperParam::F_FLAG:        out << "FLAG";          break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::OperParam::AddressingMode mode )
{
    switch ( mode )
    {
        case AdaptableSimInstruction::OperParam::A_IMMEDIATE: out << "IMMEDIATE";       break;
        case AdaptableSimInstruction::OperParam::A_REG_DIR:   out << "REG_DIR";         break;
        case AdaptableSimInstruction::OperParam::A_REG_INDR:  out << "REG_INDR";        break;
        case AdaptableSimInstruction::OperParam::A_DATA_DIR:  out << "DATA_DIR";        break;
        case AdaptableSimInstruction::OperParam::A_DATA_INDR: out << "DATA_INDR";       break;
        case AdaptableSimInstruction::OperParam::A_PROGRAM:   out << "PROGRAM";         break;
        case AdaptableSimInstruction::OperParam::A_PORT:      out << "PORT";            break;
        case AdaptableSimInstruction::OperParam::A_REG_DATA:  out << "REG_DATA";        break;
    }

    return out;
}

std::ostream & operator << ( std::ostream & out,
                             AdaptableSimInstruction::FixedOperands fixedOperands )
{
    out << std::boolalpha << std::hex;

    for ( unsigned int i = 0; i < AdaptableSimInstruction::OPERANDS_MAX; i++ )
    {
        out << "        Operand #" << i << std::endl;
        out << "            use   = "   << fixedOperands.use(i)   << std::endl;
        out << "            value = 0x" << fixedOperands.value(i) << std::endl;
    }

    return out;
}

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
 * @ingroup DisasmPicoBlaze
 * @file DAsmPicoBlazeKcpsm6.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeKcpsm6.h"

// Disassembler header files.
#include "DAsmPicoBlazeOPCodes.h"

bool DAsmPicoBlazeKcpsm6::phase1 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    switch ( ( code & 0x3f000 ) >> 12 )
    {
        case KCPSM6_REGBANK:
        case KCPSM6_DIS_ENA_INT:
        case KCPSM6_RETURNI:
        case KCPSM6_RETURN:
        case KCPSM6_RETURN_Z:
        case KCPSM6_RETURN_NZ:
        case KCPSM6_RETURN_C:
        case KCPSM6_RETURN_NC:
            break;

        case KCPSM6_LOAD_SX_KK:
        case KCPSM6_AND_SX_KK:
        case KCPSM6_OR_SX_KK:
        case KCPSM6_XOR_SX_KK:
        case KCPSM6_ADD_SX_KK:
        case KCPSM6_ADDCY_SX_KK:
        case KCPSM6_SUB_SX_KK:
        case KCPSM6_SUBCY_SX_KK:
        case KCPSM6_TEST_SX_KK:
        case KCPSM6_TESTCY_SX_KK:
        case KCPSM6_COMPARE_SX_KK:
        case KCPSM6_COMPARECY_SX_KK:
        case KCPSM6_LD_RET_SX_KK:
            m_addresses[CONST].insert ( code & 0xff );
        case KCPSM6_SHIFT_ROTATE:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM6_LOAD_SX_SY:
        case KCPSM6_STAR_SX_SY:
        case KCPSM6_AND_SX_SY:
        case KCPSM6_OR_SX_SY:
        case KCPSM6_XOR_SX_SY:
        case KCPSM6_ADD_SX_SY:
        case KCPSM6_ADDCY_SX_SY:
        case KCPSM6_SUB_SX_SY:
        case KCPSM6_SUBCY_SX_SY:
        case KCPSM6_TEST_SX_SY:
        case KCPSM6_TESTCY_SX_SY:
        case KCPSM6_COMPARE_SX_SY:
        case KCPSM6_COMPARECY_SX_SY:
        case KCPSM6_INPUT_SX_SY:
        case KCPSM6_OUTPUT_SX_SY:
        case KCPSM6_JUMP_SX_SY:
        case KCPSM6_CALL_SX_SY:
        case KCPSM6_FETCH_SX_SY:
        case KCPSM6_STORE_SX_SY:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[REG].insert ( ( code & 0x0f0 ) >> 4 );
            break;

        case KCPSM6_INPUT_SX_PP:
        case KCPSM6_OUTPUT_SX_PP:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[PORT].insert ( code & 0xff );
            break;
        case KCPSM6_OUTPUTK_KK_P:
            m_addresses[CONST].insert ( ( code & 0xff0 ) >> 4 );
            m_addresses[PORT].insert ( code & 0xf );
            break;

        case KCPSM6_STORE_SX_SS:
        case KCPSM6_FETCH_SX_SS:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[DATA].insert ( code & 0xff );
            break;

        case KCPSM6_JUMP_AAA:
        case KCPSM6_JUMP_Z_AAA:
        case KCPSM6_JUMP_NZ_AAA:
        case KCPSM6_JUMP_C_AAA:
        case KCPSM6_JUMP_NC_AAA:
        case KCPSM6_CALL_AAA:
        case KCPSM6_CALL_Z_AAA:
        case KCPSM6_CALL_NZ_AAA:
        case KCPSM6_CALL_C_AAA:
        case KCPSM6_CALL_NC_AAA:
            m_addresses[CODE].insert ( code & 0xfff );
            break;

        default:
            return false;
    }

    m_usedCode.insert(addr);
    return true;
}

void DAsmPicoBlazeKcpsm6::phase2 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    m_lastAddr++;
    if ( addr != m_lastAddr )
    {
        m_code.push_back(std::string());
        std::string & line = m_code.back();

        m_lastAddr = addr;

        indent(line, 16);
        appendStr(line, "ORG");

        indent(line, 32);
        appendStr(line, num2str(addr));
    }

    m_code.push_back(std::string());
    std::string & line = m_code.back();

    if ( m_addresses[CODE].end() != m_addresses[CODE].find(addr) )
    {
        label(line, addr, true);
    }

    indent(line, 16);

    switch ( ( code & 0xff000 ) >> 12 )
    {
        case KCPSM6_LOAD_SX_KK:         sx_kk(line, code, "LOAD");      break;
        case KCPSM6_AND_SX_KK:          sx_kk(line, code, "AND");       break;
        case KCPSM6_OR_SX_KK:           sx_kk(line, code, "OR");        break;
        case KCPSM6_XOR_SX_KK:          sx_kk(line, code, "XOR");       break;
        case KCPSM6_ADD_SX_KK:          sx_kk(line, code, "ADD");       break;
        case KCPSM6_ADDCY_SX_KK:        sx_kk(line, code, "ADDCY");     break;
        case KCPSM6_SUB_SX_KK:          sx_kk(line, code, "SUB");       break;
        case KCPSM6_SUBCY_SX_KK:        sx_kk(line, code, "SUBCY");     break;
        case KCPSM6_TEST_SX_KK:         sx_kk(line, code, "TEST");      break;
        case KCPSM6_TESTCY_SX_KK:       sx_kk(line, code, "TESTCY");    break;
        case KCPSM6_COMPARE_SX_KK:      sx_kk(line, code, "COMPARE");   break;
        case KCPSM6_COMPARECY_SX_KK:    sx_kk(line, code, "COMPARECY"); break;
        case KCPSM6_LD_RET_SX_KK:       sx_kk(line, code, "LD & RET");  break;

        case KCPSM6_SHIFT_ROTATE:       shift_rotate(line, code);       break;

        case KCPSM6_LOAD_SX_SY:         sx_sy(line, code, "LOAD");      break;
        case KCPSM6_STAR_SX_SY:         sx_sy(line, code, "STAR");      break;
        case KCPSM6_AND_SX_SY:          sx_sy(line, code, "AND");       break;
        case KCPSM6_OR_SX_SY:           sx_sy(line, code, "OR");        break;
        case KCPSM6_XOR_SX_SY:          sx_sy(line, code, "XOR");       break;
        case KCPSM6_ADD_SX_SY:          sx_sy(line, code, "ADD");       break;
        case KCPSM6_ADDCY_SX_SY:        sx_sy(line, code, "ADDCY");     break;
        case KCPSM6_SUB_SX_SY:          sx_sy(line, code, "SUB");       break;
        case KCPSM6_SUBCY_SX_SY:        sx_sy(line, code, "SUBCY");     break;
        case KCPSM6_TEST_SX_SY:         sx_sy(line, code, "TEST");      break;
        case KCPSM6_TESTCY_SX_SY:       sx_sy(line, code, "TESTCY");    break;
        case KCPSM6_COMPARE_SX_SY:      sx_sy(line, code, "COMPARE");   break;
        case KCPSM6_COMPARECY_SX_SY:    sx_sy(line, code, "COMPARECY"); break;

        case KCPSM6_INPUT_SX_SY:        sx_at_sy(line, code, "INPUT");  break;
        case KCPSM6_OUTPUT_SX_SY:       sx_at_sy(line, code, "OUTPUT"); break;
        case KCPSM6_STORE_SX_SY:        sx_at_sy(line, code, "STORE");  break;
        case KCPSM6_FETCH_SX_SY:        sx_at_sy(line, code, "FETCH");  break;

        case KCPSM6_JUMP_SX_SY:         at_sx_sy(line, code, "JUMP");   break;
        case KCPSM6_CALL_SX_SY:         at_sx_sy(line, code, "CALL");   break;


        case KCPSM6_INPUT_SX_PP:        sx_pp(line, code, "INPUT");     break;
        case KCPSM6_OUTPUT_SX_PP:       sx_pp(line, code, "OUTPUT");    break;

        case KCPSM6_OUTPUTK_KK_P:       kk_p(line, code, "OUTPUTK");    break;

        case KCPSM6_STORE_SX_SS:        sx_ss(line, code, "STORE");     break;
        case KCPSM6_FETCH_SX_SS:        sx_ss(line, code, "FETCH");     break;

        case KCPSM6_JUMP_AAA:           aaa(line, code, "JUMP");        break;
        case KCPSM6_JUMP_Z_AAA:         aaa(line, code, "JUMP", "Z");   break;
        case KCPSM6_JUMP_NZ_AAA:        aaa(line, code, "JUMP", "NZ");  break;
        case KCPSM6_JUMP_C_AAA:         aaa(line, code, "JUMP", "C");   break;
        case KCPSM6_JUMP_NC_AAA:        aaa(line, code, "JUMP", "NC");  break;
        case KCPSM6_CALL_AAA:           aaa(line, code, "CALL");        break;
        case KCPSM6_CALL_Z_AAA:         aaa(line, code, "CALL", "Z");   break;
        case KCPSM6_CALL_NZ_AAA:        aaa(line, code, "CALL", "NZ");  break;
        case KCPSM6_CALL_C_AAA:         aaa(line, code, "CALL", "C");   break;
        case KCPSM6_CALL_NC_AAA:        aaa(line, code, "CALL", "NC");  break;

        default:                        otherOpCodes(line, code);       break;
    }
}

void DAsmPicoBlazeKcpsm6::shift_rotate ( std::string & line,
                                         unsigned int code )
{
    const char * inst = NULL;
    switch ( code & 0xff )
    {
        case 0x06: inst = "SL0";      break;
        case 0x07: inst = "SL1";      break;
        case 0x04: inst = "SLX";      break;
        case 0x00: inst = "SLA";      break;
        case 0x02: inst = "RL";       break;
        case 0x0E: inst = "SR0";      break;
        case 0x0F: inst = "SR1";      break;
        case 0x0A: inst = "SRX";      break;
        case 0x08: inst = "SRA";      break;
        case 0x0C: inst = "RR";       break;
        case 0x80: inst = "HWBUILD";  break;
    }

    if ( NULL == inst )
    {
        appendStr(line, "DB");
        indent(line, 32);
        appendStr(line, num2str(code));
    }
    else
    {
        appendStr(line, inst);
        indent(line, 32);
        reg(line, ( ( code & 0xf00 ) >> 8 ) );
    }
}

void DAsmPicoBlazeKcpsm6::otherOpCodes ( std::string & line,
                                         unsigned int code )
{
    switch ( code )
    {
        case 0x37000:
            appendStr(line, "REGBANK");
            indent(line, 32);
            appendStr(line, "A");
            break;
        case 0x37001:
            appendStr(line, "REGBANK");
            indent(line, 32);
            appendStr(line, "B");
            break;
        case 0x28000:
            appendStr(line, "DISABLE INTERRUPT");
            break;
        case 0x28001:
            appendStr(line, "ENABLE INTERRUPT");
            break;
        case 0x29000:
            appendStr(line, "RETURNI DISABLE");
            break;
        case 0x29001:
            appendStr(line, "RETURNI ENABLE");
            break;
        case 0x25000:
            appendStr(line, "RETURN");
            break;
        case 0x31000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "Z");
            break;
        case 0x35000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "NZ");
            break;
        case 0x39000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "C");
            break;
        case 0x3D000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "NC");
            break;
        default:
            appendStr(line, "DB");
            indent(line, 32);
            appendStr(line, num2str(code));
            break;
    }
}

void DAsmPicoBlazeKcpsm6::sx_sy ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    reg(line, ( ( code & 0xf0 ) >> 4 ) );
}

void DAsmPicoBlazeKcpsm6::sx_at_sy ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", @";
    reg(line, ( ( code & 0xf0 ) >> 4 ) );
}

void DAsmPicoBlazeKcpsm6::sx_pp ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    port(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm6::sx_ss ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    data(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm6::sx_kk ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", #";
    imm(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm6::at_sx_sy ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    line += "@ (";
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += " , ";
    reg(line, ( ( code & 0xf0 ) >> 4 ) );
    line += " )";
}

void DAsmPicoBlazeKcpsm6::aaa ( std::string & line,
                                unsigned int code,
                                const char * inst,
                                const char * cnd )
{
    appendStr(line, inst);
    indent(line, 32);
    if ( NULL != cnd )
    {
        appendStr(line, cnd);
        line += ", ";
    }
    label(line, ( code & 0xfff ) );
}

void DAsmPicoBlazeKcpsm6::kk_p ( std::string & line,
                                 unsigned int code,
                                 const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    line += "#";
    imm(line, ( ( code & 0xff0 ) >> 4 ) );
    line += ", ";
    port(line, ( code & 0xf ) );
}

void DAsmPicoBlazeKcpsm6::reg ( std::string & line,
                                unsigned int addr )
{
    if ( Config::STG_REG & m_config.m_symbolsToGenerate )
    {
        appendStr ( line, m_symbols[REG].find(addr)->second );
    }
    else
    {
        appendStr ( line, num2str(addr) );
    }
}

void DAsmPicoBlazeKcpsm6::port ( std::string & line,
                                 unsigned int addr )
{
    if ( Config::STG_PORT & m_config.m_symbolsToGenerate )
    {
        appendStr ( line, m_symbols[PORT].find(addr)->second );
    }
    else
    {
        appendStr ( line, num2str(addr) );
    }
}

void DAsmPicoBlazeKcpsm6::data ( std::string & line,
                                 unsigned int addr )
{
    if ( Config::STG_DATA & m_config.m_symbolsToGenerate )
    {
        appendStr ( line, m_symbols[DATA].find(addr)->second );
    }
    else
    {
        appendStr ( line, num2str(addr) );
    }
}

void DAsmPicoBlazeKcpsm6::imm ( std::string & line,
                                unsigned int addr )
{
    if ( Config::STG_CONST & m_config.m_symbolsToGenerate )
    {
        appendStr ( line, m_symbols[CONST].find(addr)->second );
    }
    else
    {
        appendStr ( line, num2str(addr) );
    }
}

void DAsmPicoBlazeKcpsm6::label ( std::string & line,
                                  unsigned int addr,
                                  bool definition )
{
    if ( Config::STG_CODE & m_config.m_symbolsToGenerate )
    {
        appendStr ( line, m_symbols[CODE].find(addr)->second );
        if ( true == definition )
        {
            line += ':';
        }
    }
}

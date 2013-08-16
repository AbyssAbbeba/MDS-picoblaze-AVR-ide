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
 * @file DAsmPicoBlazeKcpsm2.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeKcpsm2.h"

// Disassembler header files.
#include "DAsmPicoBlazeOPCodes.h"

bool DAsmPicoBlazeKcpsm2::phase1 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    switch ( ( code & 0x3f000 ) >> 13 )
    {
        case KCPSM2_LOAD_SX_KK:
        case KCPSM2_AND_SX_KK:
        case KCPSM2_OR_SX_KK:
        case KCPSM2_XOR_SX_KK:
        case KCPSM2_ADD_SX_KK:
        case KCPSM2_ADDCY_SX_KK:
        case KCPSM2_SUB_SX_KK:
        case KCPSM2_SUBCY_SX_KK:
            m_addresses[REG].insert ( ( code & 0x1f00 ) >> 8 );
            m_addresses[CONST].insert ( code & 0xff );
            break;

        case KCPSM2_LOAD_SX_SY:
        case KCPSM2_AND_SX_SY:
        case KCPSM2_OR_SX_SY:
        case KCPSM2_XOR_SX_SY:
        case KCPSM2_ADD_SX_SY:
        case KCPSM2_ADDCY_SX_SY:
        case KCPSM2_SUB_SX_SY:
        case KCPSM2_SUBCY_SX_SY:
        case KCPSM2_INPUT_SX_SY:
        case KCPSM2_OUTPUT_SX_SY:
            m_addresses[REG].insert ( ( code & 0x1f00 ) >> 8 );
            m_addresses[REG].insert ( ( code & 0x00f8 ) >> 3 );
            break;

        case KCPSM2_INPUT_SX_PP:
        case KCPSM2_OUTPUT_SX_PP:
            m_addresses[REG].insert ( ( code & 0x1f00 ) >> 8 );
            m_addresses[PORT].insert ( code & 0xff );
            break;

        case KCPSM2_SHIFT_ROTATE:
            m_addresses[REG].insert ( ( code & 0x1f00 ) >> 8 );
            break;

        case KCPSM2_JUMP:
        case KCPSM2_CALL:
            m_addresses[CODE].insert ( code & 0x3ff );
            break;

        case KCPSM2_DIS_ENA_INT:
        case KCPSM2_RETURN:
        case KCPSM2_RETURNI:
            break;

        default:
            return false;
    }

    m_usedCode.insert(addr);
    return true;
}

void DAsmPicoBlazeKcpsm2::phase2 ( unsigned int code,
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

    switch ( ( code & 0x3f000 ) >> 13 )
    {
        case KCPSM2_LOAD_SX_KK:         sx_kk(line, code, "LOAD");      break;
        case KCPSM2_AND_SX_KK:          sx_kk(line, code, "AND");       break;
        case KCPSM2_OR_SX_KK:           sx_kk(line, code, "OR");        break;
        case KCPSM2_XOR_SX_KK:          sx_kk(line, code, "XOR");       break;
        case KCPSM2_ADD_SX_KK:          sx_kk(line, code, "ADD");       break;
        case KCPSM2_ADDCY_SX_KK:        sx_kk(line, code, "ADDCY");     break;
        case KCPSM2_SUB_SX_KK:          sx_kk(line, code, "SUB");       break;
        case KCPSM2_SUBCY_SX_KK:        sx_kk(line, code, "SUBCY");     break;

        case KCPSM2_LOAD_SX_SY:         sx_sy(line, code, "LOAD");      break;
        case KCPSM2_AND_SX_SY:          sx_sy(line, code, "AND");       break;
        case KCPSM2_OR_SX_SY:           sx_sy(line, code, "OR");        break;
        case KCPSM2_XOR_SX_SY:          sx_sy(line, code, "XOR");       break;
        case KCPSM2_ADD_SX_SY:          sx_sy(line, code, "ADD");       break;
        case KCPSM2_ADDCY_SX_SY:        sx_sy(line, code, "ADDCY");     break;
        case KCPSM2_SUB_SX_SY:          sx_sy(line, code, "SUB");       break;
        case KCPSM2_SUBCY_SX_SY:        sx_sy(line, code, "SUBCY");     break;
        case KCPSM2_INPUT_SX_SY:        sx_sy(line, code, "INPUT");     break;
        case KCPSM2_OUTPUT_SX_SY:       sx_sy(line, code, "OUTPUT");    break;

        case KCPSM2_INPUT_SX_PP:        sx_pp(line, code, "INPUT");     break;
        case KCPSM2_OUTPUT_SX_PP:       sx_pp(line, code, "OUTPUT");    break;

        case KCPSM2_SHIFT_ROTATE:       shift_rotate(line, code);       break;

        case KCPSM2_JUMP:               inst_JUMP(line, code);          break;
        case KCPSM2_CALL:               inst_CALL(line, code);          break;

        default:                        otherOpCodes(line, code);       break;
    }
}

void DAsmPicoBlazeKcpsm2::inst_JUMP ( std::string & line,
                                      unsigned int code )
{
    switch ( ( code & 0x1c00 ) >> 10 )
    {
        case 0b100: aaa(line, code, "JUMP", "Z");  break;
        case 0b101: aaa(line, code, "JUMP", "NZ"); break;
        case 0b110: aaa(line, code, "JUMP", "C");  break;
        case 0b111: aaa(line, code, "JUMP", "NC"); break;
        case 0b000: aaa(line, code, "JUMP");       break;
    }
}

void DAsmPicoBlazeKcpsm2::inst_CALL ( std::string & line,
                                      unsigned int code )
{
    switch ( ( code & 0x1c00 ) >> 10 )
    {
        case 0b100: aaa(line, code, "CALL", "Z");  break;
        case 0b101: aaa(line, code, "CALL", "NZ"); break;
        case 0b110: aaa(line, code, "CALL", "C");  break;
        case 0b111: aaa(line, code, "CALL", "NC"); break;
        case 0b000: aaa(line, code, "CALL");       break;
    }
}

void DAsmPicoBlazeKcpsm2::shift_rotate ( std::string & line,
                                         unsigned int code )
{
    const char * inst = NULL;
    switch ( code & 0xff )
    {
        case 0b0110: inst = "SL0";      break;
        case 0b0111: inst = "SL1";      break;
        case 0b0100: inst = "SLX";      break;
        case 0b0000: inst = "SLA";      break;
        case 0b0010: inst = "RL";       break;
        case 0b1110: inst = "SR0";      break;
        case 0b1111: inst = "SR1";      break;
        case 0b1010: inst = "SRX";      break;
        case 0b1000: inst = "SRA";      break;
        case 0b1100: inst = "RR";       break;
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
        reg(line, ( ( code & 0x1f00 ) >> 8 ) );
    }
}

void DAsmPicoBlazeKcpsm2::otherOpCodes ( std::string & line,
                                         unsigned int code )
{
    switch ( code )
    {
        case 0x3C000:
            appendStr(line, "DISABLE INTERRUPT");
            break;
        case 0x3C001:
            appendStr(line, "ENABLE INTERRUPT");
            break;
        case 0x2C000:
            appendStr(line, "RETURNI DISABLE");
            break;
        case 0x2C001:
            appendStr(line, "RETURNI ENABLE");
            break;
        case 0x24000:
            appendStr(line, "RETURN");
            break;
        case 0x25000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "Z");
            break;
        case 0x25400:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "NZ");
            break;
        case 0x25800:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "C");
            break;
        case 0x25C00:
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

void DAsmPicoBlazeKcpsm2::sx_sy ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x1f00 ) >> 8 ) );
    line += ", ";
    reg(line, ( ( code & 0x00f8 ) >> 3 ) );
}

void DAsmPicoBlazeKcpsm2::sx_at_sy ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x1f00 ) >> 8 ) );
    line += ", @";
    reg(line, ( ( code & 0x00f8 ) >> 3 ) );
}

void DAsmPicoBlazeKcpsm2::sx_pp ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x1f00 ) >> 8 ) );
    line += ", ";
    port(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm2::sx_kk ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x1f00 ) >> 8 ) );
    line += ", #";
    imm(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm2::aaa ( std::string & line,
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
    label(line, ( code & 0x3ff ) );
}

void DAsmPicoBlazeKcpsm2::reg ( std::string & line,
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

void DAsmPicoBlazeKcpsm2::port ( std::string & line,
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

void DAsmPicoBlazeKcpsm2::imm ( std::string & line,
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

void DAsmPicoBlazeKcpsm2::label ( std::string & line,
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

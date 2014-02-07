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
 * @ingroup DisasmPicoBlaze
 * @file DAsmPicoBlazeKcpsm3.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeKcpsm3.h"

// Disassembler header files.
#include "DAsmPicoBlazeOPCodes.h"

bool DAsmPicoBlazeKcpsm3::phase1 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    switch ( ( code & 0x3f000 ) >> 13 )
    {
        case KCPSM3_LOAD:
        case KCPSM3_AND:
        case KCPSM3_OR:
        case KCPSM3_XOR:
        case KCPSM3_TEST:
        case KCPSM3_COMPARE:
        case KCPSM3_ADD:
        case KCPSM3_ADDCY:
        case KCPSM3_SUB:
        case KCPSM3_SUBCY:
            if ( 0x1000 & code )
            {
                // <INSTRUCTION> sX, sY
                m_addresses[REG].insert ( ( code & 0xf0 ) >> 4 );
            }
            else
            {
                // <INSTRUCTION> sX, kk
                m_addresses[CONST].insert ( code & 0xff );
            }
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM3_INPUT:
        case KCPSM3_OUTPUT:
            if ( 0x1000 & code )
            {
                // <INSTRUCTION> sX, sY
                m_addresses[REG].insert ( ( code & 0xf0 ) >> 4 );
            }
            else
            {
                // <INSTRUCTION> sX, pp
                m_addresses[PORT].insert ( code & 0xff );
            }
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM3_FETCH:
        case KCPSM3_STORE:
            if ( 0x1000 & code )
            {
                // <INSTRUCTION> sX, sY
                m_addresses[REG].insert ( ( code & 0xf0 ) >> 4 );
            }
            else
            {
                // <INSTRUCTION> sX, ss
                m_addresses[DATA].insert ( code & 0xff );
            }
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM3_SHIFT_ROTATE:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM3_CALL:
        case KCPSM3_JUMP:
            m_addresses[CODE].insert ( code & 0x3ff );
            break;

        case KCPSM3_RETURN:
        case KCPSM3_RETURNI:
        case KCPSM3_DIS_ENA_INT:
            break;

        default:
            return false;
    }

    m_usedCode.insert(addr);
    return true;
}

void DAsmPicoBlazeKcpsm3::phase2 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    m_lastAddr++;
    if ( addr != m_lastAddr )
    {
        m_code.push_back(std::string());
        std::string & ln = m_code.back();

        m_lastAddr = addr;

        indent(ln, 16);
        appendStr(ln, "ORG");

        indent(ln, 32);
        appendStr(ln, num2str(addr));
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
        case KCPSM3_LOAD:               sx_sy_kk(line, code, "LOAD");   break;
        case KCPSM3_AND:                sx_sy_kk(line, code, "AND");    break;
        case KCPSM3_OR:                 sx_sy_kk(line, code, "OR");     break;
        case KCPSM3_XOR:                sx_sy_kk(line, code, "XOR");    break;
        case KCPSM3_TEST:               sx_sy_kk(line, code, "TEST");   break;
        case KCPSM3_COMPARE:            sx_sy_kk(line, code, "COMPARE");break;
        case KCPSM3_ADD:                sx_sy_kk(line, code, "ADD");    break;
        case KCPSM3_ADDCY:              sx_sy_kk(line, code, "ADDCY");  break;
        case KCPSM3_SUB:                sx_sy_kk(line, code, "SUB");    break;
        case KCPSM3_SUBCY:              sx_sy_kk(line, code, "SUBCY");  break;

        case KCPSM3_INPUT:              sx_sy_pp(line, code, "INPUT");  break;
        case KCPSM3_OUTPUT:             sx_sy_pp(line, code, "OUTPUT"); break;

        case KCPSM3_FETCH:              sx_sy_ss(line, code, "FETCH");  break;
        case KCPSM3_STORE:              sx_sy_ss(line, code, "STORE");  break;

        case KCPSM3_SHIFT_ROTATE:       shift_rotate(line, code);       break;

        case KCPSM3_CALL:               inst_CALL(line, code);          break;
        case KCPSM3_JUMP:               inst_JUMP(line, code);          break;

        default:                        otherOpCodes(line, code);       break;
    }
}

void DAsmPicoBlazeKcpsm3::sx_sy_kk ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    if ( 0x1000 & code )
    {
        // <INSTRUCTION> sX, sY
        reg(line, ( ( code & 0x0f0 ) >> 4 ) );
    }
    else
    {
        // <INSTRUCTION> sX, kk
        line += "#";
        imm(line, ( code & 0xff ) );
    }
}

void DAsmPicoBlazeKcpsm3::sx_sy_ss ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    if ( 0x1000 & code )
    {
        // <INSTRUCTION> sX, sY
        line += "@";
        reg(line, ( ( code & 0x0f0 ) >> 4 ) );
    }
    else
    {
        // <INSTRUCTION> sX, ss
        data(line, ( code & 0xff ) );
    }
}

void DAsmPicoBlazeKcpsm3::sx_sy_pp ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    if ( 0x1000 & code )
    {
        // <INSTRUCTION> sX, sY
        line += "@";
        reg(line, ( ( code & 0x0f0 ) >> 4 ) );
    }
    else
    {
        // <INSTRUCTION> sX, pp
        port(line, ( code & 0xff ) );
    }
}

void DAsmPicoBlazeKcpsm3::inst_JUMP ( std::string & line,
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

void DAsmPicoBlazeKcpsm3::inst_CALL ( std::string & line,
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

void DAsmPicoBlazeKcpsm3::shift_rotate ( std::string & line,
                                         unsigned int code )
{
    const char * inst = nullptr;
    switch ( 0x10ff & code )
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

    if ( nullptr == inst )
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

void DAsmPicoBlazeKcpsm3::otherOpCodes ( std::string & line,
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
        case 0x38000:
            appendStr(line, "RETURNI DISABLE");
            break;
        case 0x38001:
            appendStr(line, "RETURNI ENABLE");
            break;
        case 0x2A000:
            appendStr(line, "RETURN");
            break;
        case 0x2B000:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "Z");
            break;
        case 0x2B400:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "NZ");
            break;
        case 0x2B800:
            appendStr(line, "RETURN");
            indent(line, 32);
            appendStr(line, "C");
            break;
        case 0x2BC00:
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

void DAsmPicoBlazeKcpsm3::aaa ( std::string & line,
                                unsigned int code,
                                const char * inst,
                                const char * cnd )
{
    appendStr(line, inst);
    indent(line, 32);
    if ( nullptr != cnd )
    {
        appendStr(line, cnd);
        line += ", ";
    }
    label(line, ( code & 0x3ff ) );
}

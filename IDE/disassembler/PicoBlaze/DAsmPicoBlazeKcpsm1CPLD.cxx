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
 * @file DAsmPicoBlazeKcpsm1CPLD.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeKcpsm1CPLD.h"

// Disassembler header files.
#include "DAsmPicoBlazeOPCodes.h"

bool DAsmPicoBlazeKcpsm1CPLD::phase1 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    switch ( ( code & 0xf800 ) >> 11 )
    {
        case KCPSM1CPLD_LOAD_SX_KK:
        case KCPSM1CPLD_AND_SX_KK:
        case KCPSM1CPLD_OR_SX_KK:
        case KCPSM1CPLD_XOR_SX_KK:
        case KCPSM1CPLD_ADD_SX_KK:
        case KCPSM1CPLD_ADDCY_SX_KK:
        case KCPSM1CPLD_SUB_SX_KK:
        case KCPSM1CPLD_SUBCY_SX_KK:
            m_addresses[REG].insert ( ( code & 0x700 ) >> 8 );
            m_addresses[CONST].insert ( code & 0xff );
            break;

        case KCPSM1CPLD_LOAD_SX_SY:
        case KCPSM1CPLD_AND_SX_SY:
        case KCPSM1CPLD_OR_SX_SY:
        case KCPSM1CPLD_XOR_SX_SY:
        case KCPSM1CPLD_ADD_SX_SY:
        case KCPSM1CPLD_ADDCY_SX_SY:
        case KCPSM1CPLD_SUB_SX_SY:
        case KCPSM1CPLD_SUBCY_SX_SY:
        case KCPSM1CPLD_INPUT_SX_SY:
        case KCPSM1CPLD_OUTPUT_SX_SY:
            m_addresses[REG].insert ( ( code & 0x700 ) >> 8 );
            m_addresses[REG].insert ( ( code & 0x0e0 ) >> 5 );
            break;

        case KCPSM1CPLD_INPUT_SX_PP:
        case KCPSM1CPLD_OUTPUT_SX_PP:
            m_addresses[REG].insert ( ( code & 0x700 ) >> 8 );
            m_addresses[PORT].insert ( code & 0xff );
            break;

        case KCPSM1CPLD_SHIFT_ROTATE:
            m_addresses[REG].insert ( ( code & 0x700 ) >> 8 );
            break;

        case KCPSM1CPLD_JUMP:
        case KCPSM1CPLD_CALL:
            m_addresses[CODE].insert ( code & 0xff );
            break;

        case KCPSM1CPLD_RETURN:
        case KCPSM1CPLD_RETURNI:
        case KCPSM1CPLD_INTERRUPT:
            break;

        default:
            return false;
    }

    m_usedCode.insert(addr);
    return true;
}

void DAsmPicoBlazeKcpsm1CPLD::phase2 ( unsigned int code,
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

    switch ( ( code & 0xf800 ) >> 11 )
    {
        case KCPSM1CPLD_LOAD_SX_KK:   sx_kk(line, code, "LOAD");      break;
        case KCPSM1CPLD_AND_SX_KK:    sx_kk(line, code, "AND");       break;
        case KCPSM1CPLD_OR_SX_KK:     sx_kk(line, code, "OR");        break;
        case KCPSM1CPLD_XOR_SX_KK:    sx_kk(line, code, "XOR");       break;
        case KCPSM1CPLD_ADD_SX_KK:    sx_kk(line, code, "ADD");       break;
        case KCPSM1CPLD_ADDCY_SX_KK:  sx_kk(line, code, "ADDCY");     break;
        case KCPSM1CPLD_SUB_SX_KK:    sx_kk(line, code, "SUB");       break;
        case KCPSM1CPLD_SUBCY_SX_KK:  sx_kk(line, code, "SUBCY");     break;

        case KCPSM1CPLD_LOAD_SX_SY:   sx_sy(line, code, "LOAD");      break;
        case KCPSM1CPLD_AND_SX_SY:    sx_sy(line, code, "AND");       break;
        case KCPSM1CPLD_OR_SX_SY:     sx_sy(line, code, "OR");        break;
        case KCPSM1CPLD_XOR_SX_SY:    sx_sy(line, code, "XOR");       break;
        case KCPSM1CPLD_ADD_SX_SY:    sx_sy(line, code, "ADD");       break;
        case KCPSM1CPLD_ADDCY_SX_SY:  sx_sy(line, code, "ADDCY");     break;
        case KCPSM1CPLD_SUB_SX_SY:    sx_sy(line, code, "SUB");       break;
        case KCPSM1CPLD_SUBCY_SX_SY:  sx_sy(line, code, "SUBCY");     break;
        case KCPSM1CPLD_INPUT_SX_SY:  sx_at_sy(line, code, "INPUT");  break;
        case KCPSM1CPLD_OUTPUT_SX_SY: sx_at_sy(line, code, "OUTPUT"); break;

        case KCPSM1CPLD_INPUT_SX_PP:  sx_pp(line, code, "INPUT");     break;
        case KCPSM1CPLD_OUTPUT_SX_PP: sx_pp(line, code, "OUTPUT");    break;

        case KCPSM1CPLD_SHIFT_ROTATE: shift_rotate(line, code);       break;

        case KCPSM1CPLD_JUMP:         inst_JUMP(line, code);          break;
        case KCPSM1CPLD_CALL:         inst_CALL(line, code);          break;
        case KCPSM1CPLD_RETURN:       inst_RETURN(line, code);        break;
        case KCPSM1CPLD_RETURNI:      inst_RETURNI(line, code);       break;
        case KCPSM1CPLD_INTERRUPT:    inst_INTERRUPT(line, code);     break;

        default:
            appendStr(line, "DB");
            indent(line, 32);
            appendStr(line, num2str(code));
            break;
    }
}

void DAsmPicoBlazeKcpsm1CPLD::inst_JUMP ( std::string & line,
                                          unsigned int code )
{
    switch ( ( code & 0x0700 ) >> 8 )
    {
        case 0b100: aaa(line, code, "JUMP", "Z");  break;
        case 0b101: aaa(line, code, "JUMP", "NZ"); break;
        case 0b110: aaa(line, code, "JUMP", "C");  break;
        case 0b111: aaa(line, code, "JUMP", "NC"); break;
        case 0b000: aaa(line, code, "JUMP");       break;
        default:
            appendStr(line, "DB");
            indent(line, 32);
            appendStr(line, num2str(code));
            break;
    }
}

void DAsmPicoBlazeKcpsm1CPLD::inst_CALL ( std::string & line,
                                          unsigned int code )
{
    switch ( ( code & 0x0700 ) >> 8 )
    {
        case 0b100: aaa(line, code, "CALL", "Z");  break;
        case 0b101: aaa(line, code, "CALL", "NZ"); break;
        case 0b110: aaa(line, code, "CALL", "C");  break;
        case 0b111: aaa(line, code, "CALL", "NC"); break;
        case 0b000: aaa(line, code, "CALL");       break;
        default:
            appendStr(line, "DB");
            indent(line, 32);
            appendStr(line, num2str(code));
            break;
    }
}

void DAsmPicoBlazeKcpsm1CPLD::inst_RETURN ( std::string & line,
                                            unsigned int code )
{
    const char * cnd = nullptr;
    switch ( ( code & 0x0700 ) >> 8 )
    {
        case 0b100: cnd = "Z";  break;
        case 0b101: cnd = "NZ"; break;
        case 0b110: cnd = "C";  break;
        case 0b111: cnd = "NC"; break;
    }

    appendStr(line, "RETURN");
    indent(line, 32);
    if ( nullptr != cnd )
    {
        appendStr(line, cnd);
        line += ", ";
    }
}

void DAsmPicoBlazeKcpsm1CPLD::inst_RETURNI ( std::string & line,
                                             unsigned int code )
{
    if ( 0xb000 == code )
    {
        // RETURNI ENABLE
        appendStr(line, "RETURNI ENABLE");
    }
    else if ( 0xb001 == code )
    {
        // RETURNI DISABLE
        appendStr(line, "RETURNI DISABLE");
    }
    else
    {
        appendStr(line, "DB");
        indent(line, 32);
        appendStr(line, num2str(code));
    }
}

void DAsmPicoBlazeKcpsm1CPLD::inst_INTERRUPT ( std::string & line,
                                               unsigned int code )
{
    if ( 0xf000 == code )
    {
        // INTERRUPT ENABLE
        appendStr(line, "INTERRUPT ENABLE");
    }
    else if ( 0xf001 == code )
    {
        // INTERRUPT DISABLE
        appendStr(line, "INTERRUPT DISABLE");
    }
    else
    {
        appendStr(line, "DB");
        indent(line, 32);
        appendStr(line, num2str(code));
    }
}

void DAsmPicoBlazeKcpsm1CPLD::shift_rotate ( std::string & line,
                                             unsigned int code )
{
    const char * inst = nullptr;
    if ( 0 == ( code & 0xff00 ) )
    {
        switch ( code & 0xff )
        {
            case 0b0000: inst = "SLA";      break;
            case 0b0010: inst = "SLX";      break;
            case 0b0100: inst = "RL";       break;
            case 0b0110: inst = "SL0";      break;
            case 0b0111: inst = "SL1";      break;
            case 0b1000: inst = "SRA";      break;
            case 0b1010: inst = "SRX";      break;
            case 0b1100: inst = "RR";       break;
            case 0b1110: inst = "SR0";      break;
            case 0b1111: inst = "SR1";      break;
        }
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
        reg(line, ( ( code & 0x700 ) >> 8 ) );
    }
}

void DAsmPicoBlazeKcpsm1CPLD::sx_sy ( std::string & line,
                                      unsigned int code,
                                      const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x700 ) >> 8 ) );
    line += ", ";
    reg(line, ( ( code & 0x0e0 ) >> 5 ) );
}

void DAsmPicoBlazeKcpsm1CPLD::sx_at_sy ( std::string & line,
                                         unsigned int code,
                                         const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x700 ) >> 8 ) );
    line += ", @";
    reg(line, ( ( code & 0x0e0 ) >> 5 ) );
}

void DAsmPicoBlazeKcpsm1CPLD::sx_pp ( std::string & line,
                                      unsigned int code,
                                      const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x700 ) >> 8 ) );
    line += ", ";
    port(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm1CPLD::sx_kk ( std::string & line,
                                      unsigned int code,
                                      const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0x700 ) >> 8 ) );
    line += ", #";
    imm(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm1CPLD::aaa ( std::string & line,
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
    label(line, ( code & 0xff ) );
}

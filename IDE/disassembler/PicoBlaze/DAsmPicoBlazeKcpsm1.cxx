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
 * @file DAsmPicoBlazeKcpsm1.cxx
 */
// =============================================================================

#include "DAsmPicoBlazeKcpsm1.h"

// Disassembler header files.
#include "DAsmPicoBlazeOPCodes.h"

bool DAsmPicoBlazeKcpsm1::phase1 ( unsigned int code,
                                   unsigned int addr )
{
    using namespace DAsmPicoBlazeOPCodes;

    switch ( ( code & 0xf000 ) >> 12 )
    {
        case KCPSM1_LOAD_SX_KK:
        case KCPSM1_AND_SX_KK:
        case KCPSM1_OR_SX_KK:
        case KCPSM1_XOR_SX_KK:
        case KCPSM1_ADD_SX_KK:
        case KCPSM1_ADDCY_SX_KK:
        case KCPSM1_SUB_SX_KK:
        case KCPSM1_SUBCY_SX_KK:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[CONST].insert ( code & 0x0ff );
            break;

        case KCPSM1_1100:
        case KCPSM1_INPUT_SX_SY:
        case KCPSM1_OUTPUT_SX_SY:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[REG].insert ( ( code & 0x0f0 ) >> 4 );
            break;

        case KCPSM1_INPUT_SX_PP:
        case KCPSM1_OUTPUT_SX_PP:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            m_addresses[PORT].insert ( code & 0x0ff );
            break;

        case KCPSM1_1101:
            m_addresses[REG].insert ( ( code & 0xf00 ) >> 8 );
            break;

        case KCPSM1_1000:
        case KCPSM1_1001:
            if ( code & 0x100 )
            {
                m_addresses[CODE].insert ( code & 0xff );
            }
            break;

        default:
            return false;
    }

    m_usedCode.insert(addr);
    return true;
}

void DAsmPicoBlazeKcpsm1::phase2 ( unsigned int code,
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

    switch ( ( code & 0xf000 ) >> 12 )
    {
        case KCPSM1_LOAD_SX_KK:          sx_kk(line, code, "LOAD");      break;
        case KCPSM1_AND_SX_KK:           sx_kk(line, code, "AND");       break;
        case KCPSM1_OR_SX_KK:            sx_kk(line, code, "OR");        break;
        case KCPSM1_XOR_SX_KK:           sx_kk(line, code, "XOR");       break;
        case KCPSM1_ADD_SX_KK:           sx_kk(line, code, "ADD");       break;
        case KCPSM1_ADDCY_SX_KK:         sx_kk(line, code, "ADDCY");     break;
        case KCPSM1_SUB_SX_KK:           sx_kk(line, code, "SUB");       break;
        case KCPSM1_SUBCY_SX_KK:         sx_kk(line, code, "SUBCY");     break;

        case KCPSM1_1100:
            switch ( code & 0xf )
            {
                case KCPSM1_LOAD_SX_KK:  sx_sy(line, code, "LOAD");      break;
                case KCPSM1_AND_SX_KK:   sx_sy(line, code, "AND");       break;
                case KCPSM1_OR_SX_KK:    sx_sy(line, code, "OR");        break;
                case KCPSM1_XOR_SX_KK:   sx_sy(line, code, "XOR");       break;
                case KCPSM1_ADD_SX_KK:   sx_sy(line, code, "ADD");       break;
                case KCPSM1_ADDCY_SX_KK: sx_sy(line, code, "ADDCY");     break;
                case KCPSM1_SUB_SX_KK:   sx_sy(line, code, "SUB");       break;
                case KCPSM1_SUBCY_SX_KK: sx_sy(line, code, "SUBCY");     break;
            }

        case KCPSM1_INPUT_SX_SY:         sx_at_sy(line, code, "INPUT");  break;
        case KCPSM1_OUTPUT_SX_SY:        sx_at_sy(line, code, "OUTPUT"); break;

        case KCPSM1_INPUT_SX_PP:         sx_pp(line, code, "INPUT");     break;
        case KCPSM1_OUTPUT_SX_PP:        sx_pp(line, code, "OUTPUT");    break;

        case KCPSM1_1101:                shift_rotate(line, code);       break;

        case KCPSM1_1000:
        case KCPSM1_1001:                inst_1000_and_1001(line, code); break;

        default:
            appendStr(line, "DB");
            indent(line, 32);
            appendStr(line, num2str(code));
            break;
    }
}

void DAsmPicoBlazeKcpsm1::inst_1000_and_1001 ( std::string & line,
                                               unsigned int code )
{
    if ( 0x100 & code )
    {
        if ( 0x200 & code )
        {
            inst_CALL(line, code);
        }
        else
        {
            inst_JUMP(line, code);
        }
    }
    else
    {
        switch ( code )
        {
            case 0x8030:
                appendStr(line, "ENABLE INTERRUPT");
                break;

            case 0x8010:
                appendStr(line, "DISABLE INTERRUPT");
                break;

            case 0x80D0:
                appendStr(line, "RETURNI DISABLE");
                break;

            case 0x80F0:
                appendStr(line, "RETURNI ENABLE");
                break;

            default:
                if ( 0x80 == ( code & 0xff ) )
                {
                    inst_RETURN(line, code);
                }
                else
                {
                    appendStr(line, "DB");
                    indent(line, 32);
                    appendStr(line, num2str(code));
                }
                break;
        }
    }

}
inline void DAsmPicoBlazeKcpsm1::inst_JUMP ( std::string & line,
                                             unsigned int code )
{
    switch ( ( code & 0x1c00 ) >> 10 )
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

inline void DAsmPicoBlazeKcpsm1::inst_CALL ( std::string & line,
                                             unsigned int code )
{
    switch ( ( code & 0x1c00 ) >> 10 )
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

inline void DAsmPicoBlazeKcpsm1::inst_RETURN ( std::string & line,
                                               unsigned int code )
{
    const char * cnd = NULL;
    switch ( ( code & 0x1c00 ) >> 10 )
    {
        case 0b100: cnd = "Z";  break;
        case 0b101: cnd = "NZ"; break;
        case 0b110: cnd = "C";  break;
        case 0b111: cnd = "NC"; break;
    }

    appendStr(line, "RETURN");
    indent(line, 32);
    if ( NULL != cnd )
    {
        appendStr(line, cnd);
        line += ", ";
    }
}

void DAsmPicoBlazeKcpsm1::shift_rotate ( std::string & line,
                                         unsigned int code )
{
    const char * inst = NULL;
    if ( 0 == ( code & 0xff00 ) )
    {
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

void DAsmPicoBlazeKcpsm1::sx_sy ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    reg(line, ( ( code & 0x0f0 ) >> 4 ) );
}

void DAsmPicoBlazeKcpsm1::sx_at_sy ( std::string & line,
                                     unsigned int code,
                                     const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", @";
    reg(line, ( ( code & 0x0f0 ) >> 4 ) );
}

void DAsmPicoBlazeKcpsm1::sx_pp ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", ";
    port(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm1::sx_kk ( std::string & line,
                                  unsigned int code,
                                  const char * inst )
{
    appendStr(line, inst);
    indent(line, 32);
    reg(line, ( ( code & 0xf00 ) >> 8 ) );
    line += ", #";
    imm(line, ( code & 0xff ) );
}

void DAsmPicoBlazeKcpsm1::aaa ( std::string & line,
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
    label(line, ( code & 0xff ) );
}

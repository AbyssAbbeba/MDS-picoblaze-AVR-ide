// =============================================================================
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
 * @ingroup AsmTranslator
 * @file AsmTranslatorKcpsmPBIDE.cxx
 */
// =============================================================================

#include "AsmTranslatorKcpsmPBIDE.h"

// Standard header files.
#include <cstdio>
#include <cctype>
#include <iterator>

constexpr boost::regex::flag_type flags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reAtMark      = boost::regex ( "^@", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reComment     = boost::regex ( "^;.*$", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reWhiteSpace  = boost::regex ( "^[[:space:]]+", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reWord        = boost::regex ( "[_[:alnum:]]+", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reAndReturn   = boost::regex ( "^&[[:space:]]*return", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reInstruction = boost::regex ( "^[_[:alpha:]][_[:alnum:]]+", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reOperandSep  = boost::regex ( "^[[:space:]]*,[[:space:]]*", flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reLdAndRet    = boost::regex ( "load[[:space:]]*&[[:space:]]*return",
                                                                             flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reLabel       = boost::regex ( "^[_[:alpha:]][_[:alnum:]]*[[:space:]]*:",
                                                                             flags );
const boost::regex AsmTranslatorKcpsmPBIDE::m_reOperand     = boost::regex ( "^([_[:alnum:]]+)"
                                                                             "|(\\([[:space:]]*[_[:alnum:]]+"
                                                                             "[[:space:]]*\\))", flags );

AsmTranslatorKcpsmPBIDE::AsmTranslatorKcpsmPBIDE()
{
    m_instFlag = false;

    for ( int i = 0; i < 16; i++ )
    {
        char reg[3];
        sprintf(reg, "s%x", i);
        m_registers.insert(reg);
    }
    for ( int i = 0; i < 32; i++ )
    {
        char reg[4];
        sprintf(reg, "s%02x", i);
        m_registers.insert(reg);
    }
}

bool AsmTranslatorKcpsmPBIDE::process ( std::string & line,
                                        unsigned int lineNumber,
                                        bool secondPass )
{
    LineFields lineFields(line);
    boost::smatch match;
    std::string::const_iterator begin = line.cbegin();
    std::string::const_iterator lineStartIt = begin;

    // Find white space at the beginning of the line.
    boost::regex_search(begin, line.cend(), match, m_reWhiteSpace);
    if ( true == match[0].matched )
    {
        begin = match[0].second;
    }

    // Find label.
    boost::regex_search(begin, line.cend(), match, m_reLabel);
    if ( true == match[0].matched )
    {
        begin = match[0].second;
        lineFields.m_label[0] = std::distance(lineStartIt, match[0].first);
        lineFields.m_label[1] = std::distance(lineStartIt, match[0].second);

        // Find white space.
        boost::regex_search(begin, line.cend(), match, m_reWhiteSpace);
        if ( true == match[0].matched )
        {
            begin = match[0].second;
        }
    }

    // Find instruction and its operands.
    boost::regex_search(begin, line.cend(), match, m_reInstruction);
    if ( true == match[0].matched )
    {
        begin = match[0].second;
        lineFields.m_instruction[0] = std::distance(lineStartIt, match[0].first);
        lineFields.m_instruction[1] = std::distance(lineStartIt, match[0].second);

        // Find white space.
        boost::regex_search(begin, line.cend(), match, m_reWhiteSpace);
        if ( true == match[0].matched )
        {
            begin = match[0].second;
        }

        /*
         * Special cases.
         */
        if ( ( "call" == lineFields.getInstruction() ) || ( "jump" == lineFields.getInstruction() ) )
        {
            boost::regex_search(begin, line.cend(), match, m_reAtMark);
            if ( true == match[0].matched )
            {
                m_instFlag = true;
                if ( true == secondPass )
                {
                    m_lineMap->insert({lineNumber + m_prologue.size(), lineNumber});

                    begin = match[0].second;
                    boost::regex_search(begin, line.cend(), match, m_reWord);
                    if ( true == match[0].matched )
                    {
                        lineFields.m_operands.push_back({std::distance(lineStartIt, match[0].first),
                                                         std::distance(lineStartIt, match[0].second)});

                        begin = match[0].second;
                        boost::regex_search(begin, line.cend(), match, m_reWord);
                        if ( false == match[0].matched )
                        {
                            return true;
                        }

                        lineFields.m_operands.push_back({std::distance(lineStartIt, match[0].first),
                                                         std::distance(lineStartIt, match[0].second)});


                        for ( int i = 0; i < 2; i++ )
                        {
                            lineFields.replaceOpr ( changeLetterCase ( lineFields.getOperand ( i, true ),
                                                                       m_config->m_letterCase [
                                                                           AsmTranslatorConfig::F_SYMBOL
                                                                       ] ),
                                                    i );
                        }
                    }

                    lineFields.replaceInst ( changeLetterCase ( lineFields.getInstruction(),
                                                                m_config->m_letterCase [
                                                                    AsmTranslatorConfig::F_INSTRUCTION
                                                                ] ) );
                }

                line = autoIndent(&line, indSz());
                return true;
            }
        }
        else if ( "load" == lineFields.getInstruction() )
        {
            boost::regex_search(begin, line.cend(), match, m_reAndReturn);
            if ( true == match[0].matched )
            {
                begin = match[0].second;
                lineFields.m_instruction[1] = std::distance(lineStartIt, match[0].second);

                boost::regex_search(begin, line.cend(), match, m_reWhiteSpace);
                if ( true == match[0].matched )
                {
                    begin = match[0].second;
                }
            }
        }

        // Find operands.
        while ( true == boost::regex_search(begin, line.cend(), match, m_reOperand) )
        {
            if ( false == match[0].matched )
            {
                break;
            }

            begin = match[0].second;
            lineFields.m_operands.push_back ( { std::distance(lineStartIt, match[0].first),
                                                std::distance(lineStartIt, match[0].second) } );

            // Find operand separator.
            boost::regex_search(begin, line.cend(), match, m_reOperandSep);
            if ( true == match[0].matched )
            {
                begin = match[0].second;
            }
            else
            {
                break;
            }
        }

        // Find white space.
        boost::regex_search(begin, line.cend(), match, m_reWhiteSpace);
        if ( true == match[0].matched )
        {
            begin = match[0].second;
        }
    }

    // Find comment.
    boost::regex_search(begin, line.cend(), match, m_reComment);
    if ( true == match[0].matched )
    {
        begin = match[0].second;
        lineFields.m_comment[0] = std::distance(lineStartIt, match[0].first);
        lineFields.m_comment[1] = std::distance(lineStartIt, match[0].second);
    }

    if ( ( false == secondPass ) && ( line.cend() != begin ) )
    {
        m_messages->push_back ( { lineNumber,
                                  QObject::tr ( "Error: line not understood: `%1'." )
                                              . arg ( line.c_str() )
                                              . toStdString() } );
        return false;
    }

    if ( true == secondPass )
    {
        return processInstructions ( lineFields, lineNumber );
    }
    else
    {
        return processDirectives ( lineFields, lineNumber );
    }
}

inline bool AsmTranslatorKcpsmPBIDE::processDirectives ( LineFields & lineFields,
                                                         unsigned int lineNumber )
{
    // Fix strangely formed labels, i.e. `label   :' -> `label:'.
    {
        std::string lbl = lineFields.getLabel();
        if ( false == lbl.empty() )
        {
            while ( true )
            {
                size_t pos = lbl.find(' ');
                if ( std::string::npos == pos )
                {
                    pos = lbl.find('\t');
                }
                if ( std::string::npos == pos )
                {
                    break;
                }
                lbl.replace(pos, 1, "");
            }

            lineFields.replaceLabel ( changeLetterCase ( lbl,
                                                         m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] ) );
        }
    }

    std::string directive = lineFields.getInstruction();
    if ( true == directive.empty() )
    {
        return true;
    }

    if ( "constant" == directive )
    {
        fixRadix(lineFields, 1);
        std::string substitute = changeLetterCase ( lineFields.getOperand(0, true),
                                                    m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] )
                                 + changeLetterCase ( " equ ",
                                                      m_config->m_letterCase[AsmTranslatorConfig::F_DIRECTIVE] )
                                 + changeLetterCase ( lineFields.getOperand(1, true),
                                                      m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] );

        if ( true == m_instFlag )
        {
            substitute += lineFields.getComment();
            m_prologue.push_back(autoIndent(&substitute, indSz(), true));
            lineFields.replaceAll("; >>>>> (line moved to the beginning) <<<<<");
            m_lineMap->insert({m_prologue.size(), lineNumber});
            m_messages->push_back ( { lineNumber,
                                      QObject::tr ( "Warning: directive `constant' should be used prior to "
                                                    "any instructions." )
                                                  . toStdString() } );
        }
        else
        {
            lineFields.replaceInstOpr(substitute);
        }
    }
    else if ( "namereg" == directive )
    {
        m_registers.insert(lineFields.getOperand(1));
        lineFields.replaceInstOpr ( changeLetterCase ( lineFields.getOperand(1, true),
                                                       m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] )
                                    + changeLetterCase ( " reg ",
                                                         m_config->m_letterCase[AsmTranslatorConfig::F_DIRECTIVE] )
                                    + changeLetterCase ( lineFields.getOperand(0, true),
                                                         m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] ) );
    }
    else if ( "address" == directive )
    {
        fixRadix(lineFields, 0);
        lineFields.replaceInst ( changeLetterCase ( "org", m_config->m_letterCase[AsmTranslatorConfig::F_DIRECTIVE] ) );
        lineFields.replaceOpr ( changeLetterCase ( lineFields.getOperand(0, true),
                                                   m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] ),
                                0 );
        lineFields.replaceAll(autoIndent(lineFields.m_line, indSz()));
        return true;
    }
    else
    {
        m_instFlag = true;
        return true;
    }

    lineFields.replaceAll(autoIndent(lineFields.m_line, indSz(), true));
    return true;
}

inline bool AsmTranslatorKcpsmPBIDE::processInstructions ( LineFields & lineFields,
                                                           unsigned int lineNumber )
{
    m_lineMap->insert({lineNumber + m_prologue.size(), lineNumber});

    {
        std::string op0 = lineFields.getOperand(0);
        if ( ( "equ" == op0 ) || ( "reg" == op0 ) )
        {
            return true;
        }
    }

    std::string instruction = lineFields.getInstruction();
    if ( true == instruction.empty() )
    {
        lineFields.replaceAll(autoIndent(lineFields.m_line, indSz()));
        return true;
    }

    if ( ( "load"      == instruction ) || ( "star"    == instruction ) ||
         ( "and"       == instruction ) || ( "or"      == instruction ) ||
         ( "xor"       == instruction ) || ( "add"     == instruction ) ||
         ( "addcy"     == instruction ) || ( "sub"     == instruction ) ||
         ( "subcy"     == instruction ) || ( "test"    == instruction ) ||
         ( "testcy"    == instruction ) || ( "compare" == instruction ) ||
         ( "comparecy" == instruction ) || ( "cmp"     == instruction ) ||
         ( "cmpc"      == instruction ) || ( "testc"   == instruction ) ||
         ( "addc"      == instruction ) || ( "subc"    == instruction ) )
    {
        fixRadix(lineFields, 1);

        if ( m_registers.end() == m_registers.find(lineFields.getOperand(1)) )
        {
            lineFields.replaceOpr( '#' + lineFields.getOperand(1, true), 1);
        }

        if ( "comparecy" == instruction )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInst("cmpcy");
            }
        }
        else if ( "compare" == instruction )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInst("cmp");
            }
        }
        else if ( "load" == instruction )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInst("ld");
            }
        }
        else if ( "cmpc" == instruction )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInst("cmpcy");
            }
            else
            {
                lineFields.replaceInst("comparecy");
            }
        }
        else if ( "testc" == instruction )
        {
            lineFields.replaceInst("testcy");
        }
        else if ( "addc" == instruction )
        {
            lineFields.replaceInst("addcy");
        }
        else if ( "subc" == instruction )
        {
            lineFields.replaceInst("subcy");
        }
    }
    else if ( ( "sl0" == instruction ) || ( "sl1" == instruction ) ||
              ( "sla" == instruction ) || ( "slx" == instruction ) ||
              ( "rl"  == instruction ) || ( "sr0" == instruction ) ||
              ( "sr1" == instruction ) || ( "sra" == instruction ) ||
              ( "srx" == instruction ) || ( "rr"  == instruction ) )
    {
        // Don't do anything.
    }
    else if ( "regbank" == instruction )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst("rb");
        }
    }
    else if ( ( "input" == instruction ) || ( "output" == instruction ) ||
              ( "in"    == instruction ) || ( "out"    == instruction ) ||
              ( "store" == instruction ) || ( "fetch"  == instruction ) )
    {
        std::string opr1 = lineFields.getOperand(1, true);
        if ( '(' == opr1[0] )
        {
            boost::smatch match;
            boost::regex_search(opr1.cbegin(), opr1.cend(), match, m_reWord);
            if ( true == match[0].matched )
            {
                int first  = std::distance ( opr1.cbegin(), match[0].first  );
                int second = std::distance ( opr1.cbegin(), match[0].second );
                lineFields.replaceOpr ( opr1.substr(first, second - first), 1 );
                fixRadix(lineFields, 1);
                lineFields.replaceOpr ( '@' + lineFields.getOperand(1, true), 1 );
            }
        }
        else
        {
            fixRadix(lineFields, 1);
        }

        if ( true == m_config->m_shortInstructions )
        {
            if ( ( 'i' == instruction[0] ) && ( 'p' == instruction[2] ) )
            {
                // Instruction `input'.
                lineFields.replaceInst("in");
            }
            else if ( ( 'o' == instruction[0] ) && ( 'p' == instruction[3] ) )
            {
                // Instruction `output'.
                lineFields.replaceInst("out");
            }
            else if ( 's' == instruction[0] )
            {
                // Instruction store'.
                lineFields.replaceInst("st");
            }
            else if ( 'f' == instruction[0] )
            {
                // Instruction `fetch'.
                lineFields.replaceInst("ft");
            }
        }
        else
        {
            if ( "in" == instruction )
            {
                // Instruction `input'.
                lineFields.replaceInst("input");
            }
            else if ( "out" == instruction )
            {
                // Instruction `output'.
                lineFields.replaceInst("output");
            }
        }
    }
    else if ( "outputk" == instruction )
    {
        fixRadix(lineFields, 0);
        fixRadix(lineFields, 1);
        lineFields.replaceOpr('#' + lineFields.getOperand(0, true), 0);
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst("outk");
        }
    }
    else if ( ( "disable" == instruction ) || ( "dint" == instruction ) )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInstOpr("dis");
        }
        else
        {
            lineFields.replaceInstOpr("disable interrupt");
        }
    }
    else if ( ( "enable" == instruction ) || ( "ent" == instruction ) )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInstOpr("ena");
        }
        else
        {
            lineFields.replaceInstOpr("enable interrupt");
        }
    }
    else if ( ( "returni" == instruction ) || ( "reti" == instruction ) )
    {
        if ( "enable" == lineFields.getOperand(0) )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInstOpr("retie");
            }
            else
            {
                lineFields.replaceInstOpr("returni enable");
            }
        }
        else if ( "disable" == lineFields.getOperand(0) )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInstOpr("retid");
            }
            else
            {
                lineFields.replaceInstOpr("returni disable");
            }
        }
        else
        {
            m_messages->push_back ( { lineNumber,
                                      QObject::tr ( "Error: instruction not understood, `reti enable' or"
                                                    " `reti disable' was expeced." )
                                                  . toStdString() } );
            lineFields.replaceAll(autoIndent(lineFields.m_line, indSz()));
            return false;
        }
    }
    else if ( "jump" == instruction )
    {
        // Don't do anything.
    }
    else if ( "call" == instruction )
    {
        // Don't do anything.
    }
    else if ( "ret" == instruction )
    {
        // Don't do anything.
    }
    else if ( "return" == instruction )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInstOpr("ret");
        }
    }
    else if ( "hwbuild" == instruction )
    {
        // Don't do anything.
    }
    else if ( ( "loadret" == instruction ) || ( true == boost::regex_match(instruction, m_reLdAndRet ) ) )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst("ldret");
        }
        else
        {
            lineFields.replaceInst("load&return");
        }
        fixRadix(lineFields, 1);
        lineFields.replaceOpr('#' + lineFields.getOperand(1, true), 1);
    }
    else
    {
        return true;
    }

    lineFields.replaceInst ( changeLetterCase ( lineFields.getInstruction(),
                                                m_config->m_letterCase[AsmTranslatorConfig::F_INSTRUCTION] ) );

    for ( int  i = 0; true == lineFields.hasOperand(i); i++ )
    {
        lineFields.replaceOpr ( changeLetterCase ( lineFields.getOperand ( i, true ),
                                                   m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] ),
                                i );
    }

    lineFields.replaceAll(autoIndent(lineFields.m_line, indSz()));
    return true;
}

void AsmTranslatorKcpsmPBIDE::fixRadix ( LineFields & lineFields,
                                         int i )
{
    if ( true == lineFields.hasOperand(i) )
    {
        std::string operand = lineFields.getOperand(i, true);
        if ( '\\' == operand[0] )
        {
            lineFields.replaceOpr ( '0' + lineFields.getOperand(i, true), i );
        }
        else if ( '0' == operand[0] )
        {
            while ( '0' == operand[0] )
            {
                operand.replace(0, 1, "");
            }

            if ( true == operand.empty() )
            {
                operand = "0";
            }
            else if ( 0 == isdigit(operand[0]) )
            {
                operand = "0" + operand;
            }

            lineFields.replaceOpr ( operand, i );
        }
    }
}

inline unsigned int AsmTranslatorKcpsmPBIDE::indSz() const
{
    return ( ( true == m_config->m_shortInstructions ) ? 0x313 : 0x323 );
}

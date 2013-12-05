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
 * @ingroup AsmTranslator
 * @file AsmTranslatorKcpsmXil.cxx
 */
// =============================================================================

#include "AsmTranslatorKcpsmXil.h"

// Standard header files.
#include <cstdio>
#include <cctype>
#include <iterator>

AsmTranslatorKcpsmXil::AsmTranslatorKcpsmXil()
{
    m_instFlag = false;

    const boost::regex::flag_type flags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );

    m_reWhiteSpace  = boost::regex ( "^[[:space:]]+", flags );
    m_reLabel       = boost::regex ( "^[_[:alpha:]][_[:alnum:]]*:", flags );
    m_reInstruction = boost::regex ( "^[_[:alpha:]][_[:alnum:]]+", flags );
    m_reWord        = boost::regex ( "[_[:alnum:]]+", flags );
    m_reOperand     = boost::regex ( "^([_[:alnum:]]+)|(\\([[:space:]]*[_[:alnum:]]+[[:space:]]*\\))", flags );
    m_reOperandSep  = boost::regex ( "^[[:space:]]*,[[:space:]]*", flags );
    m_reComment     = boost::regex ( "^;.*$", flags );
    m_reAtMark      = boost::regex ( "^@", flags );
    m_reAndReturn   = boost::regex ( "^&[[:space:]]*return", flags );
    m_reLdAndRet    = boost::regex ( "load[[:space:]]*&[[:space:]]*return", flags );

    for ( int i = 0; i < 16; i++ )
    {
        char reg[3];
        sprintf(reg, "s%x", i);
        m_registers.insert(reg);
    }
    for ( int i = 0; i < 32; i++ )
    {
        char reg[4];
        sprintf(reg, "s%0x", i);
        m_registers.insert(reg);
    }
}

bool AsmTranslatorKcpsmXil::process ( std::vector<std::string> & messages,
                                      std::string & line,
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
            lineFields.m_operands.push_back ( std::make_pair ( std::distance(lineStartIt, match[0].first),
                                                               std::distance(lineStartIt, match[0].second) ) );

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
        messages.push_back ( QObject::tr ( "Warning: line not understood: `%1'." )
                                         . arg (line.c_str())
                                         . toStdString() );
        return false;
    }

    if ( true == secondPass )
    {
        return processInstructions ( messages, lineFields );
    }
    else
    {
        return processDirectives ( messages, lineFields );
    }
}

inline bool AsmTranslatorKcpsmXil::processDirectives ( std::vector<std::string> & messages,
                                                       LineFields & lineFields )
{
    std::string directive = lineFields.getInstruction();
    if ( true == directive.empty() )
    {
        return true;
    }

    // Convert numbers to numbers with radix specifier.
    for ( unsigned int i = 0; i < 2; i++ )
    {
        if ( ( true == lineFields.hasOperand(i) ) && ( true == ishex(lineFields.getOperand(i)) ) )
        {
            lineFields.replaceOpr ( "0x" + lineFields.getOperand(i), i );
        }
    }

    if ( "constant" == directive )
    {
        std::string substitute = lineFields.getOperand(0) + " EQU " + lineFields.getOperand(1);

        if ( true == m_instFlag )
        {
            lineFields.replaceInstOpr("");
            m_prologue.push_back(substitute);
            messages.push_back ( QObject::tr ( "Warning: directive `constant' should be used prior to any "
                                               "instructions." )
                                             . toStdString() );
        }
        else
        {
            lineFields.replaceInstOpr(substitute);
        }
    }
    else if ( "namereg" == directive )
    {
        m_registers.insert(lineFields.getOperand(1));
        lineFields.replaceInstOpr(lineFields.getOperand(1) + " REG " + lineFields.getOperand(0));
    }
    else if ( "address" == directive )
    {
        lineFields.replaceInst("ORG");
    }
    else
    {
        m_instFlag = true;
    }

    return true;
}

inline bool AsmTranslatorKcpsmXil::processInstructions ( std::vector<std::string> & messages,
                                                         LineFields & lineFields )
{
    std::string instruction = lineFields.getInstruction();
    if ( true == instruction.empty() )
    {
        return true;
    }

    if ( ( "load"      == instruction ) || ( "star"    == instruction ) ||
         ( "and"       == instruction ) || ( "or"      == instruction ) ||
         ( "xor"       == instruction ) || ( "add"     == instruction ) ||
         ( "addcy"     == instruction ) || ( "sub"     == instruction ) ||
         ( "subcy"     == instruction ) || ( "test"    == instruction ) ||
         ( "testcy"    == instruction ) || ( "compare" == instruction ) ||
         ( "comparecy" == instruction ) )
    {
        if ( m_registers.end() == m_registers.find(lineFields.getOperand(1)) )
        {
            lineFields.replaceOpr( "#" + lineFields.getOperand(1), 1);
        }

        if ( "comparecy" == instruction )
        {
            lineFields.replaceInst("cmpcy");
        }
        else if ( "compare" == instruction )
        {
            lineFields.replaceInst("cmp");
        }
        else if ( "load" == instruction )
        {
            lineFields.replaceInst("ld");
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
        lineFields.replaceInst("rb");
    }
    else if ( ( "input" == instruction ) || ( "output" == instruction ) ||
              ( "store" == instruction ) || ( "fetch"  == instruction ) )
    {
        std::string opr1 = lineFields.getOperand(1);
        if ( '(' == opr1[0] )
        {
            boost::smatch match;
            boost::regex_search(opr1.cbegin(), opr1.cend(), match, m_reWord);
            if ( true == match[0].matched )
            {
                int first  = std::distance ( opr1.cbegin(), match[0].first  );
                int second = std::distance ( opr1.cbegin(), match[0].second );
                std::string opr1subst = opr1.substr(first, second - first);

                if ( ( false == opr1subst.empty() ) && ( true == ishex(opr1subst) ) )
                {
                    opr1subst = "0x" + opr1subst;
                }
                lineFields.replaceOpr ( "@" + opr1subst, 1 );
            }
        }

        if ( 'i' == instruction[0] )
        {
            // Instruction `input'.
            lineFields.replaceInst("in");
        }
        else if ( 'o' == instruction[0] )
        {
            // Instruction `output'.
            lineFields.replaceInst("out");
        }
    }
    else if ( "outputk" == instruction )
    {
        lineFields.replaceOpr("#" + lineFields.getOperand(0), 0);
        lineFields.replaceInst("outk");
    }
    else if ( "disable" == instruction )
    {
        lineFields.replaceInstOpr("dis");
    }
    else if ( "enable" == instruction )
    {
        lineFields.replaceInstOpr("ena");
    }
    else if ( "returni" == instruction )
    {
        if ( "enable" == lineFields.getOperand(0) )
        {
            lineFields.replaceInstOpr("retie");
        }
        else if ( "disable" == lineFields.getOperand(0) )
        {
            lineFields.replaceInstOpr("retid");
        }
        else
        {
            messages.push_back ( QObject::tr ( "Error: instruction not understood, `returni enable' or `returni "
                                               "disable' was expeced." )
                                             . toStdString() );
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
    else if ( "return" == instruction )
    {
        lineFields.replaceInstOpr("ret");
    }
    else if ( "hwbuild" == instruction )
    {
        // Don't do anything.
    }
    else if ( true == boost::regex_match(instruction, m_reLdAndRet ) )
    {
        lineFields.replaceInst("ld&ret");
    }

    return true;
}

inline bool AsmTranslatorKcpsmXil::ishex ( const std::string & str ) const
{
    for ( auto c : str )
    {
        if ( 0 == isxdigit(c) )
        {
            return false;
        }
    }

    return true;
}

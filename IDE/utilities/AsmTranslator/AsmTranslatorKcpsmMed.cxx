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
 * @file AsmTranslatorKcpsmMed.cxx
 */
// =============================================================================

#include "AsmTranslatorKcpsmMed.h"

// Standard header files.
#include <cstdio>
#include <cctype>
#include <iterator>

AsmTranslatorKcpsmMed::AsmTranslatorKcpsmMed()
{
    m_instFlag = false;

    const boost::regex::flag_type flags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );

    m_reAtMark      = boost::regex ( "^@", flags );
    m_reComment     = boost::regex ( "^((;)|(//)).*$", flags );
    m_reOperand     = boost::regex ( "^[^,;]+", flags );
    m_reWord        = boost::regex ( "[_[:alnum:]]+", flags );
    m_reWhiteSpace  = boost::regex ( "^[[:space:]]+", flags );
    m_reAndReturn   = boost::regex ( "^&[[:space:]]*return", flags );
    m_reOperandSep  = boost::regex ( "^[[:space:]]*,[[:space:]]*", flags );
    m_reInstruction = boost::regex ( "^\\.?[_[:alpha:]][_[:alnum:]]*", flags );
    m_reLdAndRet    = boost::regex ( "load[[:space:]]*&[[:space:]]*return", flags );
    m_reLabel       = boost::regex ( "^[_[:alpha:]][_[:alnum:]]*[[:space:]]*:", flags );
    m_reUNumber     = boost::regex ( "(^|[[:space:]])(0[xb])?[_[:xdigit:]]+([[:space:]]|$)", flags );
    m_reSymDef      = boost::regex ( "^[_[:alpha:]][_[:alnum:]]*[[:space:]]+\\.[_[:alnum:]]*", flags );

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

bool AsmTranslatorKcpsmMed::process ( std::vector<std::string> & messages,
                                      std::string & line,
                                      bool secondPass )
{
    if ( false == secondPass )
    {
        // Fix underscores in numbers, i.e. `0x12_3_4_56' -> `0x123456'.
        std::string::const_iterator begin = line.cbegin();
        while ( true )
        {
            boost::smatch match;
            boost::regex_search(begin, line.cend(), match, m_reUNumber);
            if ( true == match[0].matched )
            {
                size_t endPos = std::distance(line.cbegin(), match[0].second);
                size_t startPos = line.find('_', std::distance(line.cbegin(), match[0].first));

                if ( ( std::string::npos == startPos ) || ( startPos >= endPos ) )
                {
                    begin = match[0].second;
                    continue;
                }

                for ( size_t pos = startPos;
                      ( std::string::npos != pos ) && ( pos < endPos );
                      pos = line.find('_', pos) )
                {
                    line.replace(pos, 1, "");
                }
            }
            else
            {
                break;
            }
        }
    }

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

    //
    boost::regex * labelRE;
    boost::regex_search(begin, line.cend(), match, m_reSymDef);
    if ( true == match[0].matched )
    {
        labelRE = &m_reWord;
    }
    else
    {
        labelRE = &m_reLabel;
    }

    // Find label.
    boost::regex_search(begin, line.cend(), match, *labelRE);
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

        if ( '/' == line[lineFields.m_comment[0]] )
        {
            line.replace(lineFields.m_comment[0], 2, ";");
            begin = match[0].second - 1;
            lineFields.m_comment[1]--;
        }
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

inline bool AsmTranslatorKcpsmMed::processDirectives ( std::vector<std::string> & messages,
                                                       LineFields & lineFields )
{
    std::string directive = lineFields.getInstruction();
    if ( true == directive.empty() )
    {
        return true;
    }

    // Fix stupidly formed labels, i.e. `stupid   :' -> ``stupid:'.
    {
        std::string lbl = lineFields.getLabel();
        if ( false == lbl.empty() )
        {
            bool modified = false;
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
                modified = true;
            }
            if ( true == modified )
            {
                lineFields.replaceLabel(lbl);
            }
        }
    }

    if ( "constant" == directive )
    {
        fixRadix(lineFields, 1);
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
        fixRadix(lineFields, 0);
        lineFields.replaceInst("ORG");
    }
    else if ( "inst" == directive )
    {
        lineFields.replaceInst("db");
        fixRadix(lineFields, 0);
    }
    else if ( ".end" == directive )
    {
        fixRadix(lineFields, 0);
        lineFields.replaceInstOpr("limit c, 1 + " + lineFields.getOperand(0));
    }
    else if ( ".if" == directive )
    {
        lineFields.replaceInst("#if");
    }
    else if ( ".fi" == directive )
    {
        lineFields.replaceInst("#endif");
    }
    else if ( ".def" == directive )
    {
        lineFields.replaceInst("define");
        std::string opr = lineFields.getOperand(0);
        for ( size_t pos = opr.find('@');
              std::string::npos != pos;
              pos = opr.find('@', pos) )
        {
            // NOTE: this works only for `@d' where `d' is a single decimal digit, it won't work for more digits than 1.
            opr.replace(pos, 1, "{");
            opr.insert(pos + 2, "}");
        }
        lineFields.replaceOpr(opr, 0);
    }
    else
    {
        m_instFlag = true;
    }

    return true;
}

inline bool AsmTranslatorKcpsmMed::processInstructions ( std::vector<std::string> & messages,
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
         ( "comparecy" == instruction ) || ( "cmpc"    == instruction ) ||
         ( "move"      == instruction ) || ( "addc"    == instruction ) ||
         ( "subc"      == instruction ) || ( "tstc"    == instruction ) ||
         ( "comp"      == instruction ) )
    {
        fixRadix(lineFields, 1);

        if ( m_registers.end() == m_registers.find(lineFields.getOperand(1)) )
        {
            lineFields.replaceOpr( "#" + lineFields.getOperand(1), 1);
        }

        if ( ( "comparecy" == instruction ) || ( "cmpc" == instruction ) )
        {
            lineFields.replaceInst("cmpcy");
        }
        else if ( ( "compare" == instruction ) || ( "comp" == instruction ) )
        {
            lineFields.replaceInst("cmp");
        }
        else if ( ( "load" == instruction ) || ( "move" == instruction ) )
        {
            lineFields.replaceInst("ld");
        }
        else if ( "addc" == instruction )
        {
            lineFields.replaceInst("addcy");
        }
        else if ( "subc" == instruction )
        {
            lineFields.replaceInst("subcy");
        }
        else if ( "tstc" == instruction )
        {
            lineFields.replaceInst("testcy");
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
              ( "in"    == instruction ) || ( "out"    == instruction ) ||
              ( "st"    == instruction ) || ( "ld"     == instruction ) ||
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
                lineFields.replaceOpr ( "@" + opr1.substr(first, second - first), 1 );
            }
        }
        else
        {
            fixRadix(lineFields, 1);
        }

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
        else if ( ( 's' == instruction[0] ) && ( 'o' == instruction[2] ) )
        {
            // Instruction store'.
            lineFields.replaceInst("st");
        }
        else if ( ( 'f' == instruction[0] ) || ( 'l' == instruction[0] ) )
        {
            // Instruction `fetch' or `ld'.
            lineFields.replaceInst("ft");
        }
    }
    else if ( ( "outputk" == instruction ) || ( "outk" == instruction ) )
    {
        fixRadix(lineFields, 0);
        fixRadix(lineFields, 1);
        lineFields.replaceOpr("#" + lineFields.getOperand(0), 0);
        lineFields.replaceInst("outk");
    }
    else if ( ( "disable" == instruction ) || ( "dint" == instruction ) )
    {
        lineFields.replaceInstOpr("dis");
    }
    else if ( ( "enable" == instruction ) || ( "eint" == instruction ) )
    {
        lineFields.replaceInstOpr("ena");
    }
    else if ( ( "returni" == instruction ) || ( "reti" == instruction ) )
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
    else if ( "bank" == instruction )
    {
        // Don't do anything.
        lineFields.replaceInstOpr("rb");
    }
    else if ( "core" == instruction )
    {
        // Don't do anything.
        lineFields.replaceInstOpr("hwbuild");
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
    else if ( "skip" == instruction )
    {
        // Translate nonsense pseudo-instruction (skip) into something meaningful.
        if ( true == lineFields.hasOperand(1) )
        {
            lineFields.replaceOpr("$ + " + lineFields.getOperand(1), 1);
            lineFields.replaceInst("jump");
        }
        else if ( true == lineFields.hasOperand(0) )
        {
            lineFields.replaceOpr("$ + " + lineFields.getOperand(0), 0);
            lineFields.replaceInst("jump");
        }
        else
        {
            lineFields.replaceInst("jump $ + 1");
        }
    }
    else if ( ( "ret" == instruction ) || ( true == boost::regex_match(instruction, m_reLdAndRet ) ) )
    {
        if ( true == lineFields.hasOperand(1) )
        {
            lineFields.replaceInst("ldret");
            fixRadix(lineFields, 1);
            lineFields.replaceOpr("#" + lineFields.getOperand(1), 1);
        }
    }

    return true;
}

void AsmTranslatorKcpsmMed::fixRadix ( LineFields & /*lineFields*/,
                                       int /*i*/ )
{
}

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
 * @file AsmTranslatorKcpsmXil.cxx
 */
// =============================================================================

#include "AsmTranslatorKcpsmXil.h"

// Standard header files.
#include <cstdio>
#include <cctype>
#include <utility>
#include <iterator>

AsmTranslatorKcpsmXil::AsmTranslatorKcpsmXil()
{
    m_instFlag = false;

    const boost::regex::flag_type flags = ( boost::regex::extended | boost::regex::icase | boost::regex::optimize );

    m_reWhiteSpace  = boost::regex ( "^[[:space:]]+", flags );
    m_reLabel       = boost::regex ( "^[_[:alnum:]]*[[:space:]]*:", flags );
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
        sprintf(reg, "s%02x", i);
        m_registers.insert(reg);
    }

    m_defaultSymbols.insert ( std::make_pair ( "NUL", "'\\0'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "BEL", "'\\a'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "BS",  "'\\b'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "HT",  "'\\t'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "LF",  "'\\n'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "VT",  "'\\v'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "CR",  "'\\r'" ) );
    m_defaultSymbols.insert ( std::make_pair ( "ESC", "'\\e'" ) );
}

bool AsmTranslatorKcpsmXil::process ( std::vector<std::pair<unsigned int, std::string> > & messages,
                                      std::string & line,
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
                    begin = match[0].second;
                    boost::regex_search(begin, line.cend(), match, m_reWord);
                    if ( true == match[0].matched )
                    {
                        lineFields.m_operands.push_back(std::make_pair(std::distance(lineStartIt, match[0].first),
                                                                       std::distance(lineStartIt, match[0].second)));

                        begin = match[0].second;
                        boost::regex_search(begin, line.cend(), match, m_reWord);
                        if ( false == match[0].matched )
                        {
                            return true;
                        }

                        lineFields.m_operands.push_back(std::make_pair(std::distance(lineStartIt, match[0].first),
                                                                       std::distance(lineStartIt, match[0].second)));

                        translateIdentifiers(lineFields);
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
        messages.push_back ( std::make_pair ( lineNumber,
                                              QObject::tr ( "Warning: line not understood: `%1'." )
                                                          . arg ( line.c_str() )
                                                          . toStdString() ) );
        return false;
    }

    if ( false == secondPass )
    {
        return processDirectives ( messages, lineFields, lineNumber );
    }
    else
    {
        return processInstructions ( messages, lineFields, lineNumber );
    }
}

inline bool AsmTranslatorKcpsmXil::processDirectives ( std::vector<std::pair<unsigned int, std::string> > & messages,
                                                       LineFields & lineFields,
                                                       unsigned int lineNumber )
{
    // Fix strangely formed labels, i.e. `0label   :' -> `_0label:'.
    {
        std::string lbl = lineFields.getLabel(true);
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

            lbl = newIdentifier(lbl.substr ( 0, lbl.size() -1 ));
            lineFields.replaceLabel(changeLetterCase(lbl, m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL]) + ":");
        }
    }

    std::string directive = lineFields.getInstruction();
    if ( true == directive.empty() )
    {
        return true;
    }

    if ( "constant" == directive )
    {
        std::string id = newIdentifier(lineFields.getOperand(0, true));
        fixRadix(lineFields, 1);
        std::string substitute = changeLetterCase ( id, m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] )
                                 + changeLetterCase ( " equ ",m_config->m_letterCase[AsmTranslatorConfig::F_DIRECTIVE] )
                                 + changeLetterCase ( lineFields.getOperand(1, true),
                                                      m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] );

        if ( true == m_instFlag )
        {
            substitute += lineFields.getComment();
            m_prologue.push_back(autoIndent(&substitute, indSz(), true));
            lineFields.replaceAll("; >>>>> (line moved to the beginning) <<<<<");
            messages.push_back ( std::make_pair ( lineNumber,
                                                  QObject::tr ( "Warning: directive `constant' should be used prior to "
                                                                "any instructions." )
                                                              . toStdString() ) );
        }
        else
        {
            lineFields.replaceInstOpr(substitute);
        }
    }
    else if ( "namereg" == directive )
    {
        std::string id = newIdentifier(lineFields.getOperand(1, true));
        m_registers.insert(id);
        lineFields.replaceInstOpr ( changeLetterCase ( id, m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] )
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
    else if ( "default_jump" == directive )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst ( changeLetterCase ( "failjmp",
                                                        m_config->m_letterCase[AsmTranslatorConfig::F_DIRECTIVE] ) );
        }
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

inline bool AsmTranslatorKcpsmXil::processInstructions ( std::vector<std::pair<unsigned int, std::string> > & messages,
                                                         LineFields & lineFields,
                                                         unsigned int lineNumber )
{
    {
        std::string op0 = lineFields.getOperand(0);
        if ( ( "equ" == op0 ) || ( "reg" == op0 ) )
        {
            return true;
        }
    }

    translateIdentifiers(lineFields);

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
         ( "comparecy" == instruction ) )
    {
        fixRadix(lineFields, 1);

        if ( m_registers.end() == m_registers.find(lineFields.getOperand(1)) )
        {
            lineFields.replaceOpr( "#" + lineFields.getOperand(1, true), 1);
        }

        if ( true == m_config->m_shortInstructions )
        {
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
                std::string opr1subst = opr1.substr(first, second - first);

                if ( ( false == opr1subst.empty() ) && ( true == ishex(opr1subst) ) )
                {
                    opr1subst = "0x" + opr1subst;
                }
                lineFields.replaceOpr ( "@" + opr1subst, 1 );
            }
        }
        else
        {
            fixRadix(lineFields, 1);
        }

        if ( true == m_config->m_shortInstructions )
        {
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
    }
    else if ( "outputk" == instruction )
    {
        fixRadix(lineFields, 0);
        fixRadix(lineFields, 1);
        lineFields.replaceOpr("#" + lineFields.getOperand(0, true), 0);
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst("outk");
        }
    }
    else if ( "disable" == instruction )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInstOpr("dis");
        }
    }
    else if ( "enable" == instruction )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInstOpr("ena");
        }
    }
    else if ( "returni" == instruction )
    {
        if ( "enable" == lineFields.getOperand(0) )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInstOpr("retie");
            }
        }
        else if ( "disable" == lineFields.getOperand(0) )
        {
            if ( true == m_config->m_shortInstructions )
            {
                lineFields.replaceInstOpr("retid");
            }
        }
        else
        {
            messages.push_back ( std::make_pair ( lineNumber,
                                                  QObject::tr ( "Error: instruction not understood, `returni enable' or"
                                                                " `returni disable' was expeced." )
                                                              . toStdString() ) );
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
    else if ( true == boost::regex_match(instruction, m_reLdAndRet ) )
    {
        if ( true == m_config->m_shortInstructions )
        {
            lineFields.replaceInst("ldret");
        }
        fixRadix(lineFields, 1);
        lineFields.replaceOpr("#" + lineFields.getOperand(1, true), 1);
    }
    else
    {
        return true;
    }

    lineFields.replaceInst ( changeLetterCase ( lineFields.getInstruction(),
                                                m_config->m_letterCase[AsmTranslatorConfig::F_INSTRUCTION] ) );

    lineFields.replaceAll(autoIndent(lineFields.m_line, indSz()));
    return true;
}

inline bool AsmTranslatorKcpsmXil::ishex ( const std::string & str ) const
{
    for ( const auto c : str )
    {
        if ( 0 == isxdigit(c) )
        {
            return false;
        }
    }

    return true;
}

void AsmTranslatorKcpsmXil::fixRadix ( LineFields & lineFields,
                                       int i )
{
    if ( ( true == lineFields.hasOperand(i) ) && ( true == ishex(lineFields.getOperand(i)) ) )
    {
        lineFields.replaceOpr ( "0x" + lineFields.getOperand(i), i );
    }
}

std::string AsmTranslatorKcpsmXil::newIdentifier ( const std::string & id )
{
    std::string idLowerCase = id;
    std::transform(idLowerCase.begin(), idLowerCase.end(), idLowerCase.begin(), ::tolower);

    if ( ( 0 == isdigit(idLowerCase[0]) ) && ( m_usedIDs.cend() == m_usedIDs.find(idLowerCase) ) )
    {
        m_usedIDs.insert(idLowerCase);
        return idLowerCase;
    }
    else
    {
        std::string newId  = ( "_" + idLowerCase );
        while ( m_usedIDs.cend() != m_usedIDs.find(newId) )
        {
            newId = ( "_" + newId );
        }

        m_usedIDs.insert(newId);
        m_idTranslationMap.insert(std::make_pair(id, newId));

        return newId;
    }
}

inline void AsmTranslatorKcpsmXil::translateIdentifiers ( AsmTranslatorBase::LineFields & lineFields )
{
    std::string id;
    std::map<std::string,std::string>::const_iterator it;
    std::map<std::string,std::string>::const_iterator end = m_idTranslationMap.cend();

    id = lineFields.getLabel(true);
    id = id.substr ( 0, id.size() -1 );
    it = m_idTranslationMap.find(id);
    if ( end != it )
    {
        lineFields.replaceLabel ( it->second + ":" );
    }

    for ( int  i = 0; true == lineFields.hasOperand(i); i++ )
    {
        id = lineFields.getOperand(i, true);
        it = m_defaultSymbols.find(id);
        if ( m_defaultSymbols.cend() != it )
        {
            id = it->second;
        }
        else
        {
            it = m_idTranslationMap.find(id);
            if ( end != it )
            {
                id = it->second;
            }
        }

        lineFields.replaceOpr ( changeLetterCase ( id, m_config->m_letterCase[AsmTranslatorConfig::F_SYMBOL] ), i );
    }
}

inline unsigned int AsmTranslatorKcpsmXil::indSz() const
{
    return ( ( true == m_config->m_shortInstructions ) ? 0x313 : 0x323 );
}

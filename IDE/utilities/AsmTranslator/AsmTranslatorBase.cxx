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
 * @file AsmTranslatorBase.cxx
 */
// =============================================================================

#include "AsmTranslatorBase.h"

// Standard header files.
#include <cctype>
#include <iterator>
#include <algorithm>

AsmTranslatorBase::LineFields::LineFields ( std::string & line )
{
    m_line = &line;
    m_label[0] = -1;
    m_label[1] = -1;
    m_instruction[0] = -1;
    m_instruction[1] = -1;
    m_comment[0] = -1;
    m_comment[1] = -1;
}

std::string AsmTranslatorBase::LineFields::getLabel ( bool keepCase ) const
{
    if ( ( -1 == m_label[0] ) || ( -1 == m_label[1] ) )
    {
        return "";
    }

    std::string result = m_line->substr(m_label[0], m_label[1] - m_label[0]);
    if ( false == keepCase )
    {
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    }
    return result;
}

std::string AsmTranslatorBase::LineFields::getInstruction ( bool keepCase ) const
{
    if ( ( -1 == m_instruction[0] ) || ( -1 == m_instruction[1] ) )
    {
        return "";
    }

    std::string result = m_line->substr(m_instruction[0], m_instruction[1] - m_instruction[0]);
    if ( false == keepCase )
    {
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    }
    return result;
}

std::string AsmTranslatorBase::LineFields::getComment() const
{
    if ( ( -1 == m_comment[0] ) || ( -1 == m_comment[1] ) )
    {
        return "";
    }

    return m_line->substr(m_comment[0], m_comment[1] - m_comment[0]);
}

std::string AsmTranslatorBase::LineFields::getOperand ( unsigned int number,
                                                        bool keepCase ) const
{
    if ( false == hasOperand(number) )
    {
        return "";
    }

    std::string result = m_line->substr ( m_operands[number].first,
                                          m_operands[number].second - m_operands[number].first );

    if ( false == keepCase )
    {
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    }
    return result;
}

bool AsmTranslatorBase::LineFields::hasOperand ( unsigned int number ) const
{
    if ( m_operands.size() <= (size_t) number )
    {
        return false;
    }

    if ( ( -1 == m_operands[number].first ) || ( -1 == m_operands[number].second ) )
    {
        return false;
    }

    return true;
}

void AsmTranslatorBase::LineFields::replaceLabel ( const std::string & substitute )
{
    if ( ( -1 != m_label[0] ) && ( -1 != m_label[1] ) )
    {
        m_line->replace(m_label[0], m_label[1] - m_label[0], substitute);

        int diff = ( int(substitute.size()) - ( m_label[1] - m_label[0] ) );
        m_label[0] += diff;

        if ( -1 != m_instruction[0] )
        {
            m_instruction[0] += diff;
            m_instruction[1] += diff;
        }

        if ( -1 != m_comment[0] )
        {
            m_comment[0] += diff;
            m_comment[1] += diff;
        }

        for ( size_t i = 0; i < m_operands.size(); i++ )
        {
            m_operands[i].first  += diff;
            m_operands[i].second += diff;
        }
    }
}

void AsmTranslatorBase::LineFields::replaceInst ( const std::string & substitute )
{
    if ( ( -1 != m_instruction[0] ) && ( -1 != m_instruction[1] ) )
    {
        m_line->replace(m_instruction[0], m_instruction[1] - m_instruction[0], substitute);

        int diff = ( int(substitute.size()) - ( m_instruction[1] - m_instruction[0] ) );
        m_instruction[1] += diff;

        if ( -1 != m_comment[0] )
        {
            m_comment[0] += diff;
            m_comment[1] += diff;
        }

        for ( size_t i = 0; i < m_operands.size(); i++ )
        {
            m_operands[i].first  += diff;
            m_operands[i].second += diff;
        }
    }
}

void AsmTranslatorBase::LineFields::replaceInstOpr ( const std::string & substitute )
{
    int end;
    if ( 0 != m_operands.size() )
    {
        end = (int) m_operands.back().second;
    }
    else
    {
        end = m_instruction[1];
    }
    if ( ( -1 != m_instruction[0] ) && ( -1 != end ) )
    {
        m_line->replace(m_instruction[0], end - m_instruction[0], substitute);

        m_instruction[1] = ( m_instruction[0] + (int) substitute.size() );
        m_comment[0] = -1;
        m_comment[1] = -1;
        m_operands.clear();
    }
}

void AsmTranslatorBase::LineFields::replaceOpr ( const std::string & substitute,
                                                 unsigned int number )
{
    if ( m_operands.size() > (size_t) number )
    {
        if ( ( -1 != m_operands[number].first ) && ( -1 != m_operands[number].second ) )
        {
            m_line->replace(m_operands[number].first, m_operands[number].second - m_operands[number].first, substitute);

            int diff = ( int(substitute.size()) - ( m_operands[number].second - m_operands[number].first ) );
            m_operands[number].second += diff;
            if ( -1 != m_comment[0] )
            {
                m_comment[0] += diff;
                m_comment[1] += diff;
            }
            for ( size_t i = (size_t) number + 1;
                  i < m_operands.size();
                  i++ )
            {
                m_operands[i].first  += diff;
                m_operands[i].second += diff;
            }
        }
    }
}

void AsmTranslatorBase::LineFields::replaceAll ( const std::string & substitute )
{
    m_line->assign(substitute);

    m_label[0] = -1;
    m_label[1] = -1;
    m_instruction[0] = -1;
    m_instruction[1] = -1;
    m_comment[0] = -1;
    m_comment[1] = -1;
    m_operands.clear();
}

std::string AsmTranslatorBase::changeLetterCase ( const std::string & string,
                                                  AsmTranslatorConfig::LetterCase letterCase ) const
{
    std::string result = string;
    if ( ( false == string.empty() ) && ( 0 == isdigit(string[0]) ) && ( std::string::npos == string.find('\'') ) )
    {
        if ( AsmTranslatorConfig::LC_LOWERCASE == letterCase )
        {
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        }
        else
        {
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        }
    }
    return result;
}

std::string AsmTranslatorBase::autoIndent ( const std::string * line,
                                            unsigned int tabSizes,
                                            bool useLabelCol ) const
{
    if ( AsmTranslatorConfig::IND_KEEP == m_config->m_indentation )
    {
        return (*line);
    }

    int i = 0;
    int size = (int) line->size();
    int fields[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    while ( ( ';' != (*line)[i] ) && ( i < size ) )
    {
        i++;
    }
    fields[6] = i;
    fields[7] = ( size - i );
    size -= fields[7];
    i = 0;

    for ( int idx = 0; ( idx < 5 ) && ( i < size ); idx++ )
    {
        while ( ( 0 != isspace ( (*line)[i] ) ) && ( i < size ) )
        {
            i++;
        }
        fields[idx++] = i;

        if ( 5 == idx )
        {
            fields[idx] = ( size - i );
            if ( 0 != fields[idx] )
            {
                i = size - 1;
                while ( ( 0 != isspace ( (*line)[i] ) ) && ( i >= 0 ) )
                {
                    i--;
                    fields[idx]--;
                }
            }
            break;
        }

        while ( ( 0 == isspace ( (*line)[i] ) ) && ( i < size ) )
        {
            i++;
        }
        fields[idx] = ( i - fields[idx-1] );
    }

    std::string lineField[4];
    for ( int i = 0; i < 4; i++ )
    {
        int pos = fields [ i * 2 ];
        int len = fields [ i * 2 + 1 ];

        if ( ( -1 != pos ) && ( len > 0 ) )
        {
            lineField[i] = line->substr(pos, len);
        }
    }

    bool containsLabel = false;
    int colonIdx = ( fields[0] + fields[1] - 1 );
    if ( ( colonIdx >= 0 ) && ( colonIdx < (int) line->size() ) )
    {
        if ( ':' == (*line)[colonIdx] )
        {
            containsLabel = true;
        }
    }

    if ( ( false == useLabelCol ) && ( false == containsLabel ) )
    {
        if ( true == lineField[2].empty() )
        {
            lineField[2] = lineField[1];
        }
        else if ( false == lineField[1].empty() )
        {
            lineField[2] = lineField[1] + " " + lineField[2];
        }
        lineField[1] = lineField[0];
        lineField[0] = "";
    }

    if ( ( true == lineField[0].empty() ) && ( true == lineField[1].empty() ) && ( true == lineField[2].empty() ) )
    {
        return lineField[3];
    }
    else
    {
        size_t tabMark[3];
        for ( int i = 0; i < 3; i++ )
        {
            tabMark[i] = ( 8 * ( ( tabSizes & ( 0xf << ( 4 * ( 2 - i ) ) ) ) >> ( 4 * ( 2 - i ) ) ) );
        }

        int indent;
        int indentCorrection = 0;
        std::string result;

        for ( int i = 0; i < 4; i++ )
        {
            result += lineField[i];

            bool stop = true;
            for ( int j = ( i + 1 ); j < 4; j++ )
            {
                if ( false == lineField[j].empty() )
                {
                    stop = false;
                }
            }
            if ( ( true == stop ) || ( 3 == i ) )
            {
                break;
            }

            indent = ( ( tabMark[i] - lineField[i].size() ) + indentCorrection );
            indentCorrection = 0;

            if ( indent < 1 )
            {
                indentCorrection = ( indent - 1 );
                indent = 1;
            }

            if ( AsmTranslatorConfig::IND_TABS == m_config->m_indentation )
            {
                indent = ( ( indent / m_config->m_tabSize ) + ( ( 0 == ( indent % m_config->m_tabSize ) ) ? 0 : 1 ) );
                result += std::string(indent, '\t');
            }
            else
            {
                result += std::string(indent, ' ');
            }
        }

        return result;
    }
}

void AsmTranslatorBase::setup ( std::vector<std::pair<unsigned int, std::string> > * messages,
                                std::map<unsigned int, unsigned int> * lineMap,
                                std::vector<std::string> * includedFiles )
{
    m_messages = messages;
    m_lineMap = lineMap;
    m_includedFiles = includedFiles;
}

void AsmTranslatorBase::shiftLineMap ( unsigned int by )
{
    std::map<unsigned int, unsigned int> newLineMap;

    for ( const auto & i : *m_lineMap )
    {
        newLineMap[i.first + by] = i.second;
    }

    m_lineMap->swap(newLineMap);
}

std::ostream & operator << ( std::ostream & out,
                             const AsmTranslatorBase::LineFields & obj )
{
    if ( ( -1 != obj.m_label[0] ) && ( -1 != obj.m_label[1] ) )
    {
        out << "label [" << obj.m_label[0] << "," << obj.m_label[1] << "]:";
        out << " `" << obj.m_line->substr(obj.m_label[0], obj.m_label[1] - obj.m_label[0]) << "'" << std::endl;
    }
    if ( ( -1 != obj.m_instruction[0] ) && ( -1 != obj.m_instruction[1] ) )
    {
        out << "instruction [" << obj.m_instruction[0] << "," << obj.m_instruction[1] << "]:";
        out << " `" << obj.m_line->substr(obj.m_instruction[0], obj.m_instruction[1] - obj.m_instruction[0])
            << "'" << std::endl;
    }
    int oprNum = 0;
    for ( const auto i : obj.m_operands )
    {
        out << "operand #" << oprNum << " [" << i.first << "," << i.second << "]:";
        out << " `" << obj.m_line->substr(i.first, i.second - i.first) << "'" << std::endl;
        oprNum++;
    }
    if ( ( -1 != obj.m_comment[0] ) && ( -1 != obj.m_comment[1] ) )
    {
        out << "comment [" << obj.m_comment[0] << "," << obj.m_comment[1] << "]:";
        out << " `" << obj.m_line->substr(obj.m_comment[0], obj.m_comment[1] - obj.m_comment[0]) << "'" << std::endl;
    }

    return out;
}

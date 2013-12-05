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

std::string AsmTranslatorBase::LineFields::getLabel() const
{
    if ( ( -1 == m_label[0] ) || ( -1 == m_label[1] ) )
    {
        return "";
    }

    std::string result = m_line->substr(m_label[0], m_label[1] - m_label[0]);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string AsmTranslatorBase::LineFields::getInstruction() const
{
    if ( ( -1 == m_instruction[0] ) || ( -1 == m_instruction[1] ) )
    {
        return "";
    }

    std::string result = m_line->substr(m_instruction[0], m_instruction[1] - m_instruction[0]);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string AsmTranslatorBase::LineFields::getOperand ( unsigned int number ) const
{
    if ( false == hasOperand(number) )
    {
        return "";
    }

    std::string result = m_line->substr(m_operands[number].first, m_operands[number].second - m_operands[number].first);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
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

void AsmTranslatorBase::LineFields::replaceInst ( const std::string & substitute )
{
    if ( ( -1 != m_instruction[0] ) && ( -1 != m_instruction[1] ) )
    {
        m_line->replace(m_instruction[0], m_instruction[1] - m_instruction[0], substitute);

        int diff = ( int(substitute.size()) - ( m_instruction[1] - m_instruction[0] ) );
        m_instruction[1] += diff;
        m_comment[0] += diff;
        m_comment[1] += diff;
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

        m_instruction[0] = -1;
        m_instruction[1] = -1;
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
            m_comment[0] += diff;
            m_comment[1] += diff;
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
    for ( auto i = obj.m_operands.cbegin();
          i != obj.m_operands.cend();
          i++ )
    {
        out << "operand #" << std::distance(obj.m_operands.cbegin(), i) << " [" << i->first << "," << i->second << "]:";
        out << " `" << obj.m_line->substr(i->first, i->second - i->first) << "'" << std::endl;
    }
    if ( ( -1 != obj.m_comment[0] ) && ( -1 != obj.m_comment[1] ) )
    {
        out << "comment [" << obj.m_comment[0] << "," << obj.m_comment[1] << "]:";
        out << " `" << obj.m_line->substr(obj.m_comment[0], obj.m_comment[1] - obj.m_comment[0]) << "'" << std::endl;
    }

    return out;
}

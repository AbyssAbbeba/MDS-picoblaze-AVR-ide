// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreProcSupportSupport.cxx
 */
// =============================================================================

#include "CompilerCPreProcSupport.h"

// Standard headers.
#include <cstdlib>
#include <cstring>
#include<iostream>//DEBUG

void CompilerCPreProcSupport::Buffer::append ( const Buffer & sourceBuffer )
{
    static const unsigned int INCR_STEP = 128;

    // Check if there is enough space left in the target buffer; and if not, enlarge the buffer.
    const size_t requiredMinSize = ( sourceBuffer.m_pos + m_pos + 1 );
    if ( requiredMinSize > m_size )
    {
        m_size = requiredMinSize;
        m_size += ( INCR_STEP - ( m_size % INCR_STEP ) );
        m_data = (char*) realloc ( m_data, m_size );
    }

    // Append contents of the source buffer to the target buffer.
    memcpy((m_data + m_pos), sourceBuffer.m_data, sourceBuffer.m_pos);
    m_pos += sourceBuffer.m_pos;
    m_data[m_pos] = '\0';
}

void CompilerCPreProcSupport::Buffer::move ( Buffer & sourceBuffer )
{
    char * auxBuf = m_data;
    m_data = sourceBuffer.m_data;
    sourceBuffer.m_data = auxBuf;

    size_t auxSize = m_size;
    m_size = sourceBuffer.m_size;
    sourceBuffer.m_size = auxSize;

    m_pos = sourceBuffer.m_pos;
    sourceBuffer.m_pos = 0;
}

CompilerCPreProcSupport::Buffer::Buffer()
{
    m_pos = 0;
    m_size = 0;
    m_data = nullptr;
    m_persistent = false;
}

CompilerCPreProcSupport::Buffer::Buffer ( const std::string & data )
{
    m_pos = data.size();
    m_size = m_pos + 1;
    m_data = const_cast<char*>(data.c_str());
    m_persistent = true;
}

CompilerCPreProcSupport::Buffer::Buffer ( const char * data )
{
    m_pos = strlen(data);
    m_size = m_pos + 1;
    m_data = const_cast<char*>(data);
    m_persistent = true;
}

CompilerCPreProcSupport::Buffer::Buffer ( char * data,
                                          unsigned int length )
{
    m_pos = length;
    m_size = length + 1;
    m_data = data;
    m_persistent = true;
}

CompilerCPreProcSupport::Buffer::~Buffer()
{
    if ( ( false == m_persistent ) && ( nullptr != m_data ) )
    {
        free(m_data);
    }
}

CompilerCPreProcSupport::Include::Detection::Detection()
{
    reset();
}

void CompilerCPreProcSupport::Include::Detection::reset()
{
    m_state = STATE_WHITE_SPACE_0;
}

void CompilerCPreProcSupport::Include::Detection::input ( char in )
{
    switch ( m_state )
    {
        case STATE_NEGATIVE:
        case STATE_POSITIVE:
            break;
        case STATE_WHITE_SPACE_0:
            switch ( in )
            {
                case ' ':
                case '\t':
                    break;
                case '#':
                    m_state = STATE_WHITE_SPACE_1;
                    break;
                default:
                    m_state = STATE_NEGATIVE;
            }
            break;
        case STATE_WHITE_SPACE_1:
            switch ( in )
            {
                case ' ':
                case '\t':
                    break;
                case 'i':
                    m_state = STATE_I;
                    break;
                default:
                    m_state = STATE_NEGATIVE;
            }
            break;
        case STATE_I:    m_state = ( ( 'n' == in ) ? STATE_N             : STATE_NEGATIVE ); break;
        case STATE_N:    m_state = ( ( 'c' == in ) ? STATE_C             : STATE_NEGATIVE ); break;
        case STATE_C:    m_state = ( ( 'l' == in ) ? STATE_L             : STATE_NEGATIVE ); break;
        case STATE_L:    m_state = ( ( 'u' == in ) ? STATE_U             : STATE_NEGATIVE ); break;
        case STATE_U:    m_state = ( ( 'd' == in ) ? STATE_D             : STATE_NEGATIVE ); break;
        case STATE_D:    m_state = ( ( 'e' == in ) ? STATE_POSITIVE      : STATE_NEGATIVE ); break;
    }
}

bool CompilerCPreProcSupport::Include::Detection::detected() const
{
    return ( STATE_POSITIVE == m_state );
}

bool CompilerCPreProcSupport::Conditional::get()
{
    if ( true == m_stack.empty() )
    {
        return true;
    }
    return ( STATE_POSITIVE == m_stack.back() );
}

void CompilerCPreProcSupport::Conditional::dirIf ( bool condition )
{
    m_stack.push_back ( condition ? STATE_POSITIVE : STATE_NEGATIVE );
}

void CompilerCPreProcSupport::Conditional::dirElif ( bool condition )
{
    if ( true == m_stack.empty() )
    {
        std::cout << "!!! #elif without an #if...\n";
        throw 0;
    }

    switch ( m_stack.back() )
    {
        case STATE_POSITIVE:
            m_stack.back() = STATE_CLOSED;
            break;
        case STATE_NEGATIVE:
            m_stack.back() = ( condition ? STATE_POSITIVE : STATE_NEGATIVE );
            break;
        case STATE_CLOSED:
            break;
    }
}

void CompilerCPreProcSupport::Conditional::dirEndif()
{
    if ( true == m_stack.empty() )
    {
        std::cout << "!!! #endif without an #if...\n";
        throw 0;
    }
    m_stack.pop_back();
}

void CompilerCPreProcSupport::Conditional::dirElse()
{
    if ( true == m_stack.empty() )
    {
        std::cout << "!!! #else without an #if...\n";
        throw 0;
    }

    switch ( m_stack.back() )
    {
        case STATE_POSITIVE:
            m_stack.back() = STATE_CLOSED;
            break;
        case STATE_NEGATIVE:
            m_stack.back() = STATE_POSITIVE;
            break;
        case STATE_CLOSED:
            break;
    }
}

bool CompilerCPreProcSupport::Conditional::empty()
{
    return m_stack.empty();
}

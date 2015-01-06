// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerLocationMap.cxx
 */
// =============================================================================

#include "CompilerLocationMap.h"

void CompilerLocationMap::addMark ( const CompilerSourceLocation & to,
                                    const CompilerSourceLocation & from )
{
    if ( ( from.m_fileNumber + 1 ) > (int) m_map.size() )
    {
        m_map.resize(from.m_fileNumber + 1);
    }

    auto & lineMap = m_map[from.m_fileNumber];

    LineMark * lineMark = nullptr;
    for ( auto & i : lineMap )
    {
        if ( from.m_lineStart == i.m_org )
        {
            lineMark = &i;
            break;
        }
    }
    if ( nullptr == lineMark )
    {
        lineMap.push_back(LineMark());
        lineMark = &( lineMap.back() );
        lineMark->m_org = from.m_lineStart;
    }

    ColumnMark * columnMark = nullptr;
    for ( auto & i : lineMark->m_columnMap )
    {
        if ( from.m_colStart == i.m_org )
        {
            columnMark = &i;
            break;
        }
    }
    if ( nullptr == columnMark )
    {
        lineMark->m_columnMap.push_back(ColumnMark());
        columnMark = &( lineMark->m_columnMap.back() );
        columnMark->m_org = from.m_colStart;
    }

    auto & diff = columnMark->m_diff;
    diff.m_file = to.m_fileNumber - from.m_fileNumber;
    diff.m_line = to.m_lineStart  - from.m_lineStart;
    diff.m_column = to.m_colStart - from.m_colStart;
    diff.m_origin = -1;
}

void CompilerLocationMap::removeMarks ( const CompilerSourceLocation & line )
{
    if ( line.m_fileNumber >= (int) m_map.size() )
    {
        return;
    }

    std::vector<int> erase;
    for ( int i = ( (int) m_map[line.m_fileNumber].size() - 1 ); i >= 0; i-- )
    {
        if ( line.m_lineStart == m_map[line.m_fileNumber][i].m_org )
        {
            erase.push_back(i);
        }
    }

    auto begin = m_map[line.m_fileNumber].begin();
    for ( auto i : erase )
    {
        m_map[line.m_fileNumber].erase(begin + i);
    }
}

CompilerSourceLocation CompilerLocationMap::translate ( const CompilerSourceLocation & from ) const
{
    if ( from.m_fileNumber >= (int) m_map.size() )
    {
        return from;
    }

    const Difference * diff[2] = { nullptr, nullptr };

    {
        int line = from.m_lineStart;
        int column = from.m_colStart;

        for ( int i = 0; i < 2; i++ )
        {
            for ( const auto & lineMark : m_map[from.m_fileNumber] )
            {
                if ( line > lineMark.m_org )
                {
                    diff[i] = &lineMark.m_columnMap.back().m_diff;
                    break;
                }
                else if ( line == lineMark.m_org )
                {
                    for ( const auto & columnMark : lineMark.m_columnMap )
                    {
                        if ( column >= columnMark.m_org )
                        {
                            diff[i] = &columnMark.m_diff;
                            break;
                        }
                    }

                    if ( nullptr != diff[i] )
                    {
                        break;
                    }
                }
            }

            line = from.m_lineEnd;
            column = from.m_colEnd;
        }
    }

    CompilerSourceLocation result = from;

    if ( nullptr != diff[0] )
    {
        result.m_fileNumber += diff[0]->m_file;
        result.m_lineStart  += diff[0]->m_line;
        result.m_colStart   += diff[0]->m_column;
        result.m_origin      = diff[0]->m_origin;
    }

    if ( nullptr != diff[1] )
    {
        result.m_lineEnd    += diff[1]->m_line;
        result.m_colEnd     += diff[1]->m_column;
    }

    return result;
}

void CompilerLocationMap::sortMap()
{
    for ( LineMap & lineMap : m_map )
    {
        for ( int i = ( (int) lineMap.size() - 1 ); i >= 0; i-- )
        {
            for ( int j = ( i - 1 ); j >= 0; j-- )
            {
                if ( lineMap[i].m_org > lineMap[j].m_org )
                {
                    auto tmp = lineMap[j];
                    lineMap[j] = lineMap[i];
                    lineMap[i] = tmp;
                }
            }

            ColumnMap & columnMap = lineMap[i].m_columnMap;
            for ( int j = ( (int) columnMap.size() - 1 ); j >= 0; j-- )
            {
                for ( int k = ( j - 1 ); k >= 0; k-- )
                {
                    if ( columnMap[j].m_org > columnMap[k].m_org )
                    {
                        auto tmp = columnMap[j];
                        columnMap[j] = columnMap[k];
                        columnMap[k] = tmp;
                    }
                }
            }
        }
    }
}

void CompilerLocationMap::clear()
{
    m_map.clear();
}

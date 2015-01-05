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
#include<iostream>//DEBUG

void CompilerLocationMap::addMark ( const CompilerSourceLocation & to,
                                    const CompilerSourceLocation & from )
{
std::cout << "CompilerLocationMap::addMark("<<to<<" <-- "<<from<<"): ";
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
        }
    }
    if ( nullptr == lineMark )
    {
        lineMap.push_back(LineMark());
        lineMark = &( lineMap.back() );
        lineMark->m_org = from.m_lineStart;
    }

    lineMark->m_columnMap.push_back(ColumnMark());

    auto & columnMark = lineMark->m_columnMap.back();
    columnMark.m_org = from.m_colStart;

    auto & diff = columnMark.m_diff;
    diff.m_file = to.m_fileNumber - from.m_fileNumber;
    diff.m_line = to.m_lineStart  - from.m_lineStart;
    diff.m_column = to.m_colStart - from.m_colStart;
    diff.m_origin = -1;

    std::cout << "diff.m_file=" << diff.m_file << ", diff.m_line=" << diff.m_line << ", diff.m_column=" << diff.m_column << '\n';

    // TODO: handle location origin
}

CompilerSourceLocation CompilerLocationMap::translate ( const CompilerSourceLocation & from ) const
{
    CompilerSourceLocation result = from;

    if ( from.m_fileNumber >= (int) m_map.size() )
    {
        return result;
    }

    const Difference * diff = nullptr;
    for ( const auto & lineMark : m_map[from.m_fileNumber] )
    {
        if ( from.m_lineStart > lineMark.m_org )
        {
            diff = &lineMark.m_columnMap.back().m_diff;
            break;
        }
        else if ( from.m_lineStart == lineMark.m_org )
        {
            for ( const auto & columnMark : lineMark.m_columnMap )
            {
                if ( from.m_colStart >= columnMark.m_org )
                {
                    diff = &columnMark.m_diff;
                    break;
                }
            }
            if ( nullptr != diff )
            {
                break;
            }
        }
    }

    if ( nullptr != diff )
    {
        result.m_fileNumber += diff->m_file;
        result.m_lineStart  += diff->m_line;
        result.m_lineEnd    += diff->m_line;
        result.m_colStart   += diff->m_column;
        result.m_colEnd     += diff->m_column;
        result.m_origin      = diff->m_origin;
    }

    return result;
}

void CompilerLocationMap::sortMap()
{
    for ( LineMap & lineMap : m_map )
    {
        for ( int i = ( lineMap.size() - 1 ); i >= 0; i-- )
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
            for ( int j = ( columnMap.size() - 1 ); j >= 0; j-- )
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

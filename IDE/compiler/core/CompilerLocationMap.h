// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Compiler
 * @file CompilerLocationMap.h
 */
// =============================================================================

#ifndef COMPILERLOCATIONMAP_H
#define COMPILERLOCATIONMAP_H

// Compiler header files.
#include "CompilerSourceLocation.h"

// Standard header files
#include <map>
#include <vector>
#include <utility>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerLocationMap
 */
class CompilerLocationMap
{
    ////    Public Datatypes    ////
    public:
        struct Difference
        {
            int m_file;
            int m_line;
            int m_column;
        };

        struct ColumnMark
        {
            int m_org;

            Difference m_diff;
        };

        struct LineMark
        {
            int m_org;

            std::vector<ColumnMark> m_columnMap;
        };

        typedef std::vector<LineMark> LineMap;
        typedef std::vector<LineMap> FileMap;

    ////    Public Operations    ////
    public:
        void recordMark ( const CompilerSourceLocation & from,
                          const CompilerSourceLocation & to );

        void processRecords();

        CompilerSourceLocation translate ( const CompilerSourceLocation & from );

    ////    Private Attributes    ////
    private:
        std::vector<std::pair<CompilerSourceLocation,CompilerSourceLocation>> m_rawRecords;

        FileMap m_fileMap;
};

#endif // COMPILERLOCATIONMAP_H

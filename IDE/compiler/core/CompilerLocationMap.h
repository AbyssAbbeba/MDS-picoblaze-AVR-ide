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

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerLocationMap
 */
class CompilerLocationMap
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Difference
        {
            int m_file;
            int m_line;
            int m_column;
            int m_origin;
        };

        /**
         * @brief
         */
        struct ColumnMark
        {
            int m_org;
            Difference m_diff;
        };

        /**
         * @brief
         */
        typedef std::vector<ColumnMark> ColumnMap;

        /**
         * @brief
         */
        struct LineMark
        {
            int m_org;
            ColumnMap m_columnMap;
        };

        /**
         * @brief
         */
        typedef std::vector<LineMark> LineMap;

        /**
         * @brief
         */
        typedef std::vector<LineMap> FileMap;

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] to
         * @param[in] from
         */
        void addMark ( const CompilerSourceLocation & to,
                       const CompilerSourceLocation & from );

        /**
         * @brief
         * @param[in] line
         */
        void removeMarks ( const CompilerSourceLocation & line );

        /**
         * @brief
         * @param[in] from
         * @return
         */
        CompilerSourceLocation translate ( const CompilerSourceLocation & from ) const;

        /**
         * @brief The location map cannot be used for translating locations before it is sorted.
         */
        void sortMap();

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        ///
        FileMap m_map;
};

#endif // COMPILERLOCATIONMAP_H

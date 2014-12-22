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

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerLocationMap
 */
class CompilerLocationMap
{
    ////    Public Datatypes    ////
    public:
        struct Location
        {
//             Location ( unsigned int line )
//                      :
//                        m_file(0),
//                        m_line(line),
//                        m_column(0) {}
//
//             Location ( unsigned int file,
//                        unsigned int line )
//                      :
//                        m_file(file),
//                        m_line(line),
//                        m_column(0) {}
//
//             Location ( unsigned int file,
//                        unsigned int line,
//                        unsigned int column )
//                      :
//                        m_file(file),
//                        m_line(line),
//                        m_column(column) {}

            unsigned int m_file;
            unsigned int m_line;
            unsigned int m_column;
        };

    ////    Public Operations    ////
    public:
        void recordMark ( CompilerSourceLocation & from,
                          CompilerSourceLocation & to );

        void processRecords();

    ////    Private Attributes    ////
    private:
//         std::map<int, Marks> m_locationMap;
};

#endif // COMPILERLOCATIONMAP_H

// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Compiler
 * @file CompilerOptions.h
 */
// =============================================================================

#ifndef COMPILEROPTIONS_H
#define COMPILEROPTIONS_H

// Standard header files
#include <string>

class CompilerOptions
{
    ////    Public Attributes    ////
    public:
        ///
        std::string m_hexFile;

        ///
        std::string m_binFile;

        ///
        std::string m_lstFile;

        ///
        std::string m_srecFile;
};

#endif // COMPILEROPTIONS_H

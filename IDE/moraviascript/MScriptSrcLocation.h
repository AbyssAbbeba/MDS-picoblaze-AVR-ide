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
 * @ingroup MoraviaScript
 * @file MScriptSrcLocation.h
 */
// =============================================================================

#ifndef MSCRIPTSRCLOCATION_H
#define MSCRIPTSRCLOCATION_H

// Standard header files
#include <ostream>

// Make sure that the data type of locations is declared (see Bison manual for details)
#if ! defined ( YYLTYPE ) && ! defined ( YYLTYPE_IS_DECLARED )
    typedef struct YYLTYPE
    {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
    } YYLTYPE;
    #define YYLTYPE_IS_TRIVIAL 1
    #define YYLTYPE_IS_DECLARED 1
#endif // ! YYLTYPE && ! YYLTYPE_IS_DECLARED

/**
 * @brief
 * @class MScriptSrcLocation
 * @ingroup MoraviaScript
 */
class MScriptSrcLocation
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptSrcLocation();

        /**
         * @brief
         * @param[in] yylloc
         */
        MScriptSrcLocation ( const YYLTYPE * yylloc );

        /**
         * @brief
         * @param[in] yylloc
         */
        MScriptSrcLocation ( const YYLTYPE & yylloc );

    ////    Public Attributes    ////
    public:
        /// @brief
        unsigned int m_line[2];

        /// @brief
        unsigned int m_column[2];
};

/**
 * @brief Tracing operator
 * @param[in,out] out
 * @param[in] location
 * @return
 */
std::ostream & operator << ( std::ostream & out,
                             const MScriptSrcLocation & location );

#endif // MSCRIPTSRCLOCATION_H

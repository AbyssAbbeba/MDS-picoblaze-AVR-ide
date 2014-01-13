// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptSrcLocation.h
 */
// =============================================================================

#ifndef MSCRIPTSRCLOCATION_H
#define MSCRIPTSRCLOCATION_H

// Forward declarations.
class MScriptBase;

// MScript language interpreter header files.
#include "MScriptSerializable.h"

// Standard header files.
#include <ostream>
#include <string>

// Make sure that the data type of locations is declared (see Bison manual for details).
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
class MScriptSrcLocation : public MScriptSerializable
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

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] core
         * @return
         */
        std::string toString ( const MScriptBase * core ) const;

        /**
         * @brief
         * @param[in] obj
         * @return
         */
        bool operator == ( const MScriptSrcLocation & obj ) const;

        /// @name Serialization methods.
        //@{
            /**
             * @brief
             * @param[in,out]
             */
            virtual void serialize ( MScriptSerializer & output ) const override;

            /**
             * @brief
             * @param[in,out]
             */
            virtual void deserialize ( MScriptSerializer & input ) override;
        //@}

    ////    Public Attributes    ////
    public:
        /// @brief
        int m_line[2];

        /// @brief
        int m_column[2];

        /// @brief
        int m_file;
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

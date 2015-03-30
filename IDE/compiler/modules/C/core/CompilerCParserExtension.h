// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup CompilerC
 * @file CompilerCParserExtension.h
 */
// =============================================================================

#ifndef COMPILERCPARSEREXTENSION_H
#define COMPILERCPARSEREXTENSION_H

#include "CompilerParserExtension.h"

// Standard headers
#include <map>
#include <vector>
#include <string>

class CompilerCParserExtension : public CompilerParserExtension
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Type
        {
            TYPE_UNSPECIFIED,   ///<
            TYPE_TYPEDEF,       ///<
            TYPE_ENUM_CONST,    ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCParserExtension();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void enterScopeBlock();

        /**
         * @brief
         */
        void leaveScopeBlock();

        /**
         * @brief
         * @param[in] id
         * @param[in] type
         */
        void newIdentifier ( const std::string & id,
                             Type type );

        /**
         * @brief
         * @param[in] id
         * @return
         */
        Type findIdentifier ( const std::string & id );

    ////    Public Attributes    ////
    public:
        ///
        bool m_typedef;

    ////    Private Attributes    ////
    private:
        ///
        std::vector<std::map<std::string,Type>> m_scopes;
};

#endif // COMPILERCPARSEREXTENSION_H

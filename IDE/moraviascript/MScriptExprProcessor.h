// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptExprProcessor.h
 */
// =============================================================================

#ifndef MSCRIPTEXPRPROCESSOR_H
#define MSCRIPTEXPRPROCESSOR_H

// Forward declarations.
class MScriptStatement;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptExpr.h"
#include "MScriptValue.h"
#include "MScriptSrcLocation.h"
#include "MScriptExprAlgebra.h"

// Standard header files.
#include <cstddef>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExprProcessor
 */
class MScriptExprProcessor : protected MScriptExprAlgebra
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptExprProcessor ( MScriptInterpretInterface * interpret )
                             :
                               MScriptExprAlgebra ( interpret ),
                               m_interpret ( interpret ) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] node
         * @return
         */
        MScriptStatement * decompose ( MScriptStatement * node );

        /**
         * @brief
         * @param[in] expr
         * @return
         */
        bool isConstantExpr ( const MScriptExpr * expr );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] result
         * @param[in,out] expr
         * @param[in,out] counter
         */
        void breakDown ( MScriptStatement * result,
                         MScriptExpr * expr,
                         unsigned int * counter );

        /**
         * @brief
         * @param[in,out] expr
         */
        void compressExpr ( MScriptExpr * expr );

        /**
         * @brief
         * @param[in,out] expr
         */
        void decomposeAsgn ( MScriptExpr * expr );

        /**
         * @brief
         * @param[in,out] expr
         * @param[in,out] value
         * @return
         */
        void evalConsts ( MScriptExpr * expr,
                          MScriptValue * value = NULL );

        /**
         * @brief
         * @param[in,out] root
         */
        void removeNoOps ( MScriptStatement * root );

        /**
         * @brief
         * @param[in] root
         * @return
         */
        bool detectFunctionCall ( const MScriptExpr * expr ) const;

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;
};

#endif // MSCRIPTEXPRPROCESSOR_H

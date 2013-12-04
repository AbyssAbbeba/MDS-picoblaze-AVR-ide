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
 * @file MScriptExprSolver.h
 */
// =============================================================================

#ifndef MSCRIPTEXPRSOLVER_H
#define MSCRIPTEXPRSOLVER_H

// Forward declarations.
class MScriptStatement;
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptExpr.h"
#include "MScriptValue.h"
#include "MScriptVarTable.h"
#include "MScriptArrayIndex.h"
#include "MScriptSrcLocation.h"
#include "MScriptExprAlgebra.h"

// Standard header files.
#include <cstddef>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExprSolver
 */
class MScriptExprSolver : protected MScriptExprAlgebra
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum DeclarationFlags
        {
            DF_NO_FLAGS = 0x00, ///<
            DF_CONST    = 0x01  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptExprSolver ( MScriptInterpretInterface * interpret,
                            MScriptVarTable * varTable )
                          :
                            MScriptExprAlgebra ( interpret ),
                            m_interpret ( interpret ),
                            m_varTable ( varTable ) {}

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
        MScriptValue eval ( const MScriptExpr * expr );

        /// @overload
        MScriptValue eval ( const MScriptExpr & expr )
        {
            return eval ( &expr );
        }

        /**
         * @brief
         * @param[in] flags
         * @param[in] expr
         * @return
         */
        void declaration ( const MScriptExpr * expr,
                           DeclarationFlags flags = DF_NO_FLAGS );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] index
         * @param[in] input
         * @param[in] location
         */
        void getIndex ( MScriptArrayIndex & index,
                        const MScriptValue & input,
                        const MScriptSrcLocation & location );


    ////    Inline Private Operations    ////
    private:

        /**
         * @brief
         * @param[out] result
         * @param[in] input
         * @param[in] location
         */
        inline void getFinalValue ( MScriptValue & result,
                                    const MScriptValue & input,
                                    const MScriptSrcLocation & location );
        /**
         * @brief
         * @param[out] index
         * @param[in] source
         * @param[in] location
         */
        inline const char * getVariableName ( MScriptArrayIndex * & index,
                                              const MScriptValue & source,
                                              const MScriptSrcLocation & location );
        /**
         * @brief
         * @param[out] index
         * @param[in] left
         * @param[in] right
         * @param[in] location
         * @param[in] oper
         */
        inline void assignment ( MScriptValue & result,
                                 const MScriptValue & left,
                                 const MScriptValue & right,
                                 const MScriptSrcLocation & location,
                                 const MScriptExpr::Operator oper );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        MScriptVarTable * const m_varTable;
};

#endif // MSCRIPTEXPRSOLVER_H

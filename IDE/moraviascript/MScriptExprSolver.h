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
class MScriptExpr;
class MScriptValue;
class MScriptVarTable;
class MScriptStatement;
class MScriptInterpretInterface;

// Standard header files.
#include <cstddef>

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExprSolver
 */
class MScriptExprSolver
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptExprSolver ( MScriptInterpretInterface * interpret,
                            MScriptVarTable * varTable )
                          :
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
         * @param[in,out] expr
         * @return
         */
        bool eval ( const MScriptExpr * expr );

    ////    Private Operations    ////
    private:
        void breakDown ( MScriptStatement * result,
                         MScriptExpr * expr,
                         unsigned int * counter );
        void compressExpr ( MScriptExpr * expr );
        void decomposeAsgn ( MScriptExpr * expr );
        void evalConsts ( MScriptExpr * expr,
                          MScriptValue * value = NULL );
        MScriptStatement * removeNoOps ( MScriptStatement * expr );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        MScriptVarTable * const m_varTable;
};

#endif // MSCRIPTEXPRSOLVER_H

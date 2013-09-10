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
 * @file MScriptInterpret.h
 */
// =============================================================================

#ifndef MSCRIPTINTERPRET_H
#define MSCRIPTINTERPRET_H

// Forward declarations.
class MScriptStatement;

// MScript language interpreter header files.
#include "MScriptBase.h"
#include "MScriptVarTable.h"
#include "MScriptFuncTable.h"
#include "MScriptExecContext.h"
#include "MScriptSrcLocation.h"
#include "MScriptInterpretInterface.h"

// Standard header files.
#include <string>

/**
 * @brief
 * @class MScriptInterpret
 * @ingroup MoraviaScript
 */
class MScriptInterpret : private MScriptExecContext,
                         protected MScriptInterpretInterface
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptInterpret();

        /**
         * @brief
         */
        virtual ~MScriptInterpret();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] rootNode
         */
        void init ( MScriptStatement * rootNode );

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @return
         */
        bool step();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] rootNode
         * @return
         */
        inline bool postprocessCode ( MScriptStatement * rootNode );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalCondition ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalWhile ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalDoWhile ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalFor ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalExpr ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalScope ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalSwitch ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalBreak ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalContinue ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalReturn ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] node
         */
        inline void evalDelete ( const MScriptStatement * node );

        /**
         * @brief
         * @param[in] upTo
         * @param[in] times
         * @param[in] exclusive
         * @return
         */
        inline bool abadonCode ( ExecFlags upTo,
                                 unsigned int times = 1,
                                 bool exclusive = false );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptVarTable * m_varTable;

        /// @brief
        MScriptFuncTable * m_funcTable;
};

#endif // MSCRIPTINTERPRET_H

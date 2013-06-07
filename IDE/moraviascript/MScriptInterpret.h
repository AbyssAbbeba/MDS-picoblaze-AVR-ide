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
#include "MScriptExecContext.h"
#include "MScriptSrcLocation.h"
#include "MScriptBase.h"

// Standard header files.
#include <string>

/**
 * @brief
 * @class MScriptInterpret
 * @ingroup MoraviaScript
 */
class MScriptInterpret : private MScriptExecContext
{
    ////    Constructors and Destructors    ////
    public:
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

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void interpreterMessage ( MScriptSrcLocation location,
                                          MScriptBase::MessageType type,
                                          const std::string & text ) = 0;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] rootNode
         */
        inline void checkCode ( MScriptStatement * rootNode );

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
};

#endif // MSCRIPTINTERPRET_H

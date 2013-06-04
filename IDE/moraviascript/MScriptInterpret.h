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
 * @file MScriptInterpret.h
 */
// =============================================================================

#ifndef MSCRIPTINTERPRET_H
#define MSCRIPTINTERPRET_H

// Forward declarations.
class MScriptStatement;

// MScript language interpreter header files.
#include "MScriptSrcLocation.h"
#include "MScriptBase.h"

// Standard header files.
#include <string>
#include <vector>

/**
 * @brief
 * @class MScriptInterpret
 * @ingroup MoraviaScript
 */
class MScriptInterpret
{
    ////    Constructors and Destructors    ////
    public:
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
        virtual void interpretMessage ( MScriptSrcLocation location,
                                        MScriptBase::MessageType type,
                                        const std::string & text ) = 0;

        /**
         * @brief
         * @param[in,out] rootNode
         */
        inline void checkCode ( MScriptStatement * rootNode );

    ////    Private Attributes    ////
    private:
        /// @name Execution Context
        //@{
            /// @brief
            std::vector<MScriptStatement*> m_programPointer;
        //@}
};

#endif // MSCRIPTINTERPRET_H

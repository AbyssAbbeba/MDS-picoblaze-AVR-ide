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
 * @file MScriptVarTable.h
 */
// =============================================================================

#ifndef MSCRIPTVARTABLE_H
#define MSCRIPTVARTABLE_H

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptVarTable
 */
class MScriptVarTable
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void pushScope();

        /**
         * @brief
         */
        void popScope();

        /**
         * @brief
         * @param[in] variable
         * @return
         */
        bool remove ( const std::string & variable );
};

#endif // MSCRIPTVARTABLE_H

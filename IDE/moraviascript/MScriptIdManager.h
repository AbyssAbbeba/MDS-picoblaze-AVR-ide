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
 * @file MScriptIdManager.h
 */
// =============================================================================

#ifndef MSCRIPTIDMANAGER_H
#define MSCRIPTIDMANAGER_H

/**
 * @brief
 * @class MScriptIdManager
 * @ingroup MoraviaScript
 */
class MScriptIdManager
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptIdManager();

        /**
         * @brief
         */
        virtual ~MScriptIdManager();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        int acquire();

        /**
         * @brief
         * @param[in] id
         */
        void release ( int id );
};

#endif // MSCRIPTIDMANAGER_H

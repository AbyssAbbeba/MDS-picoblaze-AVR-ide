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
 * @file MScriptIdManager.h
 */
// =============================================================================

#ifndef MSCRIPTIDMANAGER_H
#define MSCRIPTIDMANAGER_H

// Standard header files.
#include <vector>
#include <cstddef>

/**
 * @brief
 * @class MScriptIdManager
 * @ingroup MoraviaScript
 */
class MScriptIdManager
{
    ////    Private Static Constants    ////
    private:
        /// @brief
        static constexpr float GROW_FACTOR = 2.0;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptIdManager();

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

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void autoGrow();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<int> m_ids;
};

#endif // MSCRIPTIDMANAGER_H

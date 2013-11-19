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

// Standard header files.
#include <vector>

/**
 * @brief
 * @class MScriptIdManager
 * @ingroup MoraviaScript
 */
class MScriptIdManager
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        struct Id
        {
            /**
             * @brief
             * @param[in] data
             * @param[in] next
             */
            Id ( int data,
                 int next )
               :
                 m_data ( data ),
                 m_next ( next ) {}

            /// @brief
            int m_data;

            /// @brief
            int m_next;
        };

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
        inline void autoReserve();

        /**
         * @brief
         */
        inline void cleanUp();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<Id> m_ids;
};

#endif // MSCRIPTIDMANAGER_H

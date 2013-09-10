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
 * @file MScriptExecContext.h
 */
// =============================================================================

#ifndef MSCRIPTEXECCONTEXT_H
#define MSCRIPTEXECCONTEXT_H

// Forward declarations.
class MScriptStatement;

// Standard header files.
#include <cstddef>
#include <vector>
#include <utility>

/**
 * @brief
 * @class MScriptExecContext
 * @ingroup MoraviaScript
 */
class MScriptExecContext
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ExecFlags
        {
            FLAG_LOOP     = 0x01, ///<
            FLAG_SWITCH   = 0x02, ///<
            FLAG_CALL     = 0x04, ///<
            FLAG_SCOPE    = 0x08, ///<
            FLAG_FUNCTION = 0x10, ///<

            FLAG_NORMAL   = 0x00  ///<
        };

        /**
         * @brief
         */
        typedef std::pair<const MScriptStatement*, ExecFlags> ProgPtr;

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] rootNode
         */
        void init ( const MScriptStatement * rootNode );

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @param[in] node
         * @param[in] flags
         */
        void addNext ( const MScriptStatement * node,
                       ExecFlags flags = FLAG_NORMAL );

        /**
         * @brief
         * @param[in] flags
         */
        void setNextFlags ( ExecFlags flags );

        /**
         * @brief
         * @param[in] node
         * @param[in] flags
         */
        void replaceNext ( const MScriptStatement * node,
                           ExecFlags flags = FLAG_NORMAL );

        /**
         * @brief
         */
        void popNext();

        /**
         * @brief
         * @return
         */
        ExecFlags getNextFlags() const;

        /**
         * @brief
         * @return
         */
        const MScriptStatement * getNextNode() const;

        /**
         * @brief
         * @param[in] level
         */
        void cutOffBranch ( unsigned int level );

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool empty() const
        {
            return m_programPointer.empty();
        }

        /**
         * @brief
         * @return
         */
        std::vector<ProgPtr> & getProgramPointer()
        {
            return m_programPointer;
        }

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<ProgPtr> m_programPointer;
};

#endif // MSCRIPTEXECCONTEXT_H

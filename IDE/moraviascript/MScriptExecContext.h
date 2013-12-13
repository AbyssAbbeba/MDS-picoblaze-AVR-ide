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
            FLAG_LOOP      = 0x01, ///<
            FLAG_SWITCH    = 0x02, ///<
            FLAG_CALL      = 0x04, ///<
            FLAG_SCOPE     = 0x08, ///<
            FLAG_FUNCTION  = 0x10, ///<
            FLAG_NAMESPACE = 0x20, ///<

            FLAG_NORMAL    = 0x00  ///<
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

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] node
         * @param[in] flags
         */
        inline void addNext ( const MScriptStatement * node,
                              ExecFlags flags = FLAG_NORMAL );

        /**
         * @brief
         * @param[in] flags
         */
        inline void setNextFlags ( ExecFlags flags );

        /**
         * @brief
         * @param[in] node
         * @param[in] flags
         */
        inline void replaceNext ( const MScriptStatement * node,
                                  ExecFlags flags = FLAG_NORMAL );

        /**
         * @brief
         */
        inline void popNext();

        /**
         * @brief
         * @return
         */
        inline ExecFlags getNextFlags() const;

        /**
         * @brief
         * @return
         */
        inline const MScriptStatement * getNextNode() const;

        /**
         * @brief
         * @param[in] level
         * @return
         */
        inline const MScriptStatement * cutOffBranch ( unsigned int level );

        /**
         * @brief
         * @return
         */
        inline bool empty() const;

        /**
         * @brief
         * @return
         */
        inline std::vector<ProgPtr> & getProgramPointer();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<ProgPtr> m_programPointer;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void MScriptExecContext::popNext()
{
    if ( false == m_programPointer.empty() )
    {
        m_programPointer.pop_back();
    }
}

inline const MScriptStatement * MScriptExecContext::getNextNode() const
{
    return m_programPointer.back().first;
}

inline MScriptExecContext::ExecFlags MScriptExecContext::getNextFlags() const
{
    return m_programPointer.back().second;
}

inline void MScriptExecContext::addNext ( const MScriptStatement * node,
                                          MScriptExecContext::ExecFlags flags )
{
    if ( NULL != node )
    {
        m_programPointer.push_back(ProgPtr(node, flags));
    }
}

inline void MScriptExecContext::setNextFlags ( MScriptExecContext::ExecFlags flags )
{
    if ( false == m_programPointer.empty() )
    {
        m_programPointer.back().second = flags;
    }
}

inline void MScriptExecContext::replaceNext ( const MScriptStatement * node,
                                              MScriptExecContext::ExecFlags flags )
{
    popNext();
    addNext(node, flags);
}

inline const MScriptStatement * MScriptExecContext::cutOffBranch ( unsigned int level )
{
    const MScriptStatement * last = getNextNode();

    while ( 0 != level )
    {
        last = getNextNode();
        popNext();
        level--;
    }

    return last;
}

inline bool MScriptExecContext::empty() const
{
    return m_programPointer.empty();
}

inline std::vector<MScriptExecContext::ProgPtr> & MScriptExecContext::getProgramPointer()
{
    return m_programPointer;
}

#endif // MSCRIPTEXECCONTEXT_H

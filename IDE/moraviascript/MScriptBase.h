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
 * @file MScriptBase.h
 */
// =============================================================================

#ifndef MSCRIPTBASE_H
#define MSCRIPTBASE_H

// Standard header files.
#include <string>

/**
 * @brief
 * @class MScriptBase
 * @ingroup MoraviaScript
 */
class MScriptBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum MessageType
        {
            MT_ERROR,
            MT_WARNING,
            MT_GENERAL
        };

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] type
         * @return
         */
        std::string msgTypeTostr ( MessageType type ) const;
};

#endif // MSCRIPTBASE_H

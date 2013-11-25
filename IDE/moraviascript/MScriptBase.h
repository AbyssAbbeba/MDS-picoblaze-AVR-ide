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
 * @file MScriptBase.h
 */
// =============================================================================

#ifndef MSCRIPTBASE_H
#define MSCRIPTBASE_H

// Standard header files.
#include <string>
#include <vector>

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
        class MScriptRunTimeError
        {
        };

        /**
         * @brief
         */
        enum MessageType
        {
            MT_ERROR,   ///<
            MT_WARNING, ///<
            MT_REMARK,  ///<
            MT_GENERAL  ///<
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] fileNumber
         * @return
         */
        const std::string & fileNumber2str ( int fileNumber ) const;

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] type
         * @return
         */
        std::string msgTypeToStr ( MessageType type ) const;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<std::string> m_files;
};

#endif // MSCRIPTBASE_H

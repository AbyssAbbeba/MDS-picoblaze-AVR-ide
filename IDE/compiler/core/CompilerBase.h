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
 * @ingroup Compiler
 * @file CompilerBase.h
 */
// =============================================================================

#ifndef COMPILERBASE_H
#define COMPILERBASE_H

// Standard header files.
#include <ostream>
#include <string>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerBase
 */
class CompilerBase
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const char * const PRECOMPILED_CODE_EXTENSION;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum TargetArch
        {
            TA_INVALID,    ///<
            TA_AVR8,       ///<
            TA_PIC8,       ///<
            TA_MCS51,      ///<
            TA_PICOBLAZE   ///<
        };

        /**
         * @brief
         */
        enum LangId
        {
            LI_INVALID, ///<
            LI_ASM      ///<
        };

        /**
         * @brief
         */
        enum MessageType
        {
            MT_INVALID = 0, ///<
            MT_GENERAL,     ///<
            MT_ERROR,       ///<
            MT_WARNING,     ///<
            MT_REMARK       ///<
        };

        /**
         * @brief
         */
        enum DevSpecLoaderFlag
        {
            DSLF_OK,             ///<
            DSLF_UNABLE_TO_READ, ///<
            DSLF_DOES_NOT_EXIST, ///<
            DSLF_ALREADY_LOADED  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~CompilerBase() {};

    protected:
        /**
         * @brief Forbidden constructor.
         */
        CompilerBase() {};

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        LangId m_lang;

        /**
         * @brief
         */
        TargetArch m_arch;
};

/// @name Tracing operators
//@{
    /**
     * @brief Tracing operator for MessageType.
     * @param[in,out] out
     * @param[in] location
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 CompilerBase::MessageType type );
//@}

#endif // COMPILERBASE_H

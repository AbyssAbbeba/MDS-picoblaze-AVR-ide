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
 * @ingroup Compiler
 * @file CompilerBase.h
 */
// =============================================================================

#ifndef COMPILERBASE_H
#define COMPILERBASE_H

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerBase
 */
class CompilerBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum TargetArch
        {
            TA_AVR8,    ///<
            TA_PIC8,    ///<
            TA_MCS51,   ///<
            TA_KCPSM3   ///<
        };

        /**
         * @brief
         */
        enum LangId
        {
            LI_ASM      ///<
        };

        /**
         * @brief
         */
        enum MessageType
        {
            MT_GENERAL, ///<
            MT_ERROR,   ///<
            MT_WARNING, ///<
            MT_REMARK   ///<
        };

        /**
         * @brief
         */
        struct SourceLocation
        {
            /**
             * @brief
             */
            SourceLocation();

            /**
             * @brief
             * @param[in] fileNumber
             * @param[in] lineStart
             * @param[in] lineEnd
             * @param[in] colStart
             * @param[in] colEnd
             */
            SourceLocation ( int fileNumber,
                             int lineStart,
                             int lineEnd,
                             int colStart,
                             int colEnd );

            ///
            int m_fileNumber;

            ///
            int m_lineStart;

            ///
            int m_lineEnd;

            ///
            int m_colStart;

            ///
            int m_colEnd;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~CompilerBase() {};

    protected:
        /**
         * @brief Forbidden constructor
         */
        CompilerBase() {};
};

#endif // COMPILERBASE_H

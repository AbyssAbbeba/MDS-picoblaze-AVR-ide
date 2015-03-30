// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCBackend.h
 */
// =============================================================================

#ifndef COMPILERCBACKEND_H
#define COMPILERCBACKEND_H

// Forward declarations.
class CompilerOptions;
class CompilerSemanticInterface;

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCBackend
 */
class CompilerCBackend
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct DataTypeSizeTable
        {
            unsigned int m_bool;
            unsigned int m_char;
            unsigned int m_shortInt;
            unsigned int m_int;
            unsigned int m_longInt;
            unsigned int m_longLongInt;

            unsigned int m_float;
            unsigned int m_double;
            unsigned int m_longDouble;
            unsigned int m_complexFloat;
            unsigned int m_complexDouble;
            unsigned int m_complexLongDouble;

            unsigned int m_pointer;
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCBackend ( CompilerSemanticInterface * compilerCore,
                           CompilerOptions * opts )
                         :
                           m_compilerCore ( compilerCore ),
                           m_opts ( opts ) {};

    public:
        /**
         * @brief
         */
        virtual ~CompilerCBackend() {}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        const DataTypeSizeTable & dataTypeSize() const
        {
            return m_dataTypeSize;
        }

    ////    Protected Attributes    ////
    protected:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        const CompilerOptions * const m_opts;

        ///
        DataTypeSizeTable m_dataTypeSize;
};

#endif // COMPILERCBACKEND_H

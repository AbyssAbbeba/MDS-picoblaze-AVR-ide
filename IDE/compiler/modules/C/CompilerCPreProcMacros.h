// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCPreProcMacros.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCMACROS_H
#define COMPILERCPREPROCMACROS_H

// Forward declarations.
class CompilerOptions;
class CompilerParserInterface;

// Standard headers.
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>

// C compiler preprocessor header files.
#include "CompilerCPreProcSupport.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreProcMacros
 */
class CompilerCPreProcMacros : private CompilerCPreProcSupport
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum ExpansionMode
        {
            EXP_NORMAL = 0,     ///<
            EXP_RECURSIVE = 1,  ///<
            EXP_EXPRESSION = 2  ///<
        };

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum PredefinedMacro
        {
            PRE_DEF__DATE,                      ///<
            PRE_DEF__TIME,                      ///<
            PRE_DEF__FILE,                      ///<
            PRE_DEF__LINE,                      ///<
            PRE_DEF__STDC,                      ///<
            PRE_DEF__STDC_HOSTED,               ///<
            PRE_DEF__STDC_VERSION,              ///<
            PRE_DEF__STDC_IEC_559,              ///<
            PRE_DEF__STDC_IEC_559_COMPLEX,      ///<
            PRE_DEF__STDC_ISO_10646             ///<
        };

    ////    Private Static Constants    ////
    private:
        ///
        constexpr static const char * STDC_VERSION = "199901L";

        /// Language keywords.
        static const std::set<std::string> KEYWORDS;

        /// C++ named operators
        static const std::map<std::string, std::string> NAMED_OPERATORS;

        /// C++ named operators
        static const std::map<std::string, std::pair<bool, PredefinedMacro>> PREDEFINED_MACROS;

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        struct Macro
        {
            ///
            std::string m_body;

            ///
            std::vector<std::string> m_parameters;
        };

        /**
         * @brief
         */
        struct ExpansionStatus
        {
            ///
            ExpansionStatus() : m_depth(0) {}

            ///
            int m_depth;

            ///
            std::set<std::string> m_macros;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] opts
         */
        CompilerCPreProcMacros ( const CompilerOptions * opts );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] out
         * @param[in] in
         * @param[in] expMode
         */
        void expand ( Buffer & out,
                      const Buffer & in,
                      ExpansionMode expMode = EXP_NORMAL );

        /**
         * @brief
         * @param[in] name
         * @return
         */
        bool isDefined ( const std::string & name );

        /**
         * @brief
         * @param[in] macro
         * @param[in] length
         */
        void define ( char * macro,
                      const int length );

        /**
         * @brief
         * @param[in] macro
         */
        void undef ( char * macro );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] out
         * @param[in] date
         */
        void getTimeOrDate ( Buffer & out,
                             bool date );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] word
         * @return
         */
        inline int isReserved ( const char * word ) const;

        /**
         * @brief
         * @param[in,out] out
         * @param[in] macro
         */
        inline void substitute ( Buffer & out,
                                 const Macro & macro,
                                 const std::vector<std::string> & argVector ) const;

        /**
         * @brief
         * @param[in,out] argVector
         * @param[in] string
         * @param[in] expMode
         * @return
         */
        inline int getArgVector ( std::vector<std::string> & argVector,
                                 const char * string,
                                 ExpansionMode expMode );

        /**
         * @brief
         * @param[in,out] argument
         * @param[in] string
         * @return
         */
        inline unsigned int getArgument ( std::string & argument,
                                          const char * string );

        /**
         * @brief
         * @param[in,out] out
         * @param[in] in
         */
        inline void stringify ( Buffer & out,
                                const Buffer & in ) const;

        /**
         * @brief
         * @param[in,out] out
         * @param[in] in
         * @param[in,out] start
         * @param[in,out] end
         */
        inline void strEscape ( Buffer & out,
                                const Buffer & in,
                                unsigned int & start,
                                unsigned int & end ) const;

        /**
         * @brief
         * @param[in,out] out
         * @param[in] argVector
         * @param[in] index
         * @param[in] varArg
         * @param[in] strf
         * @param[in] concat
         */
        inline void substArg ( Buffer & out,
                               const std::vector<std::string> & argVector,
                               int index,
                               bool varArg,
                               bool strf,
                               bool concat ) const;

        /**
         * @brief
         * @param[in,out] out
         * @param[in] in
         * @param[in] name
         * @param[in] start
         * @param[in] outpos
         */
        inline bool prefinedMacro ( Buffer & out,
                                    const Buffer & in,
                                    const std::string & name,
                                    int start,
                                    int outpos );

    ////    Private Attributes    ////
    private:
        ///
        const CompilerOptions * const m_opts;

        ///
        std::map<std::string,Macro> m_table;

        ///
        ExpansionStatus m_status;
};

#endif // COMPILERCPREPROCMACROS_H

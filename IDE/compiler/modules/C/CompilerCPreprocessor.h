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
 * @file CompilerCPreprocessor.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCESSOR_H
#define COMPILERCPREPROCESSOR_H

// Standard headers.
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdio>
#include <utility>

#include "CompilerCPreprocessorIntr.h"

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreprocessor
 */
class CompilerCPreprocessor : private CompilerCPreprocessorIntr
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum InMode
        {
            MODE_NORMAL, ///<
            MODE_STRING, ///<
            MODE_CHAR,   ///<
            MODE_ANG_BR, ///<
            MODE_COMMENT ///<
        };

        /**
         * @brief
         */
        enum Directive
        {
            DIR_INCLUDE,///<
            DIR_LINE,   ///<
            DIR_PRAGMA, ///<
            DIR_DEFINE, ///<
            DIR_UNDEF,  ///<
            DIR_IF,     ///<
            DIR_ELIF,   ///<
            DIR_IFDEF,  ///<
            DIR_IFNDEF, ///<
            DIR_ELSE,   ///<
            DIR_ENDIF,  ///<
            DIR_WARNING,///<
            DIR_ERROR,  ///<
            DIR_NULL    ///<
        };

        struct Include
        {
            std::string m_file;
            bool m_system;

            struct Detection
            {
                Detection();
                inline void reset();
                inline void input ( char in );
                inline bool detected() const;
                enum State
                {
                    STATE_NEGATIVE,
                    STATE_POSITIVE,
                    STATE_WHITE_SPACE_0,
                    STATE_WHITE_SPACE_1,
                    STATE_I,
                    STATE_N,
                    STATE_C,
                    STATE_L,
                    STATE_U,
                    STATE_D
                } m_state;
            } m_detection;
        };

        class Buffer
        {
            ////    Constructors and Destructors    ////
            public:
                Buffer();
                Buffer ( const std::string & data/*,
                         bool copy = false*/ );
                Buffer ( const char * data/*,
                         bool copy = false */);
                Buffer ( char * data,
                         unsigned int length );
                ~Buffer();

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 * @param[in] sourceBuffer
                 */
                inline void append ( const Buffer & sourceBuffer );

                /**
                 * @brief
                 * @param[in,out] sourceBuffer
                 */
                inline void move ( Buffer & sourceBuffer );

            ////    Public Attributes    ////
            public:
                ssize_t m_pos;     //< Number of characters loaded in the buffer, without NULL(s).
                size_t m_size;     //< Overall size of the buffer.
                char * m_data;     //< Buffer pointer.
                bool m_persistent; //< Do not delete the data when this object is destroyed.
        };

        struct Conditional
        {
            inline bool get();
            inline bool empty();

            inline void dirIf ( bool condition );
            inline void dirElif ( bool condition );
            inline void dirElse();
            inline void dirEndif();

            private:
                enum State
                {
                    STATE_POSITIVE,
                    STATE_NEGATIVE,
                    STATE_CLOSED
                };

                std::vector<State> m_stack;
        };

        class MacroTable
        {
            ////    Private Static Constants    ////
            private:
                /// C++ named operators
                static const std::map<std::string, std::string> s_namedOperators;

                /// Language keywords.
                static const std::set<std::string> s_keywords;

            ////    Public Datatypes    ////
            public:
                enum ExpansionMode
                {
                    EXP_NORMAL = 0,
                    EXP_RECURSIVE = 1,
                    EXP_EXPRESSION = 2
                };

            ////    Private Datatypes    ////
            private:
                struct Macro
                {
                    std::string m_body;
                    std::vector<std::string> m_parameters;
                };

                struct ExpansionStatus
                {
                    ExpansionStatus() : m_depth(0) {}

                    int m_depth;
                    std::set<std::string> m_macros;
                };

            ////    Constructors and Destructors    ////
            public:
                MacroTable ( const CompilerOptions * opts ) : m_opts ( opts ) {}

            ////    Public Operations    ////
            public:
                void expand ( Buffer & out,
                              const Buffer & in,
                              ExpansionMode expMode = EXP_NORMAL,
                              const Buffer * nextBuffer = nullptr );

            ////    Inline Public Operations    ////
            public:
                inline bool isDefined ( const char * name );
                inline void define ( char * macro,
                                     const int length );
                inline void undef ( char * macro );

            ////    Inline Private Operations    ////
            private:
                inline bool isReserved ( const char * word ) const;
                inline void substitute ( Buffer & out,
                                         const Macro & macro,
                                         const std::vector<std::string> & argVector ) const;

            ////    Private Operations    ////
            private:
                int getArgVector ( std::vector<std::string> & argVector,
                                   char * string,
                                   ExpansionMode expMode,
                                   const Buffer * nextBuffer = nullptr );

            ////    Private Attributes    ////
            private:
                const CompilerOptions * const m_opts;
                std::map<std::string,Macro> m_table;
                ExpansionStatus m_status;
        };

    ////    Private Static Constants    ////
    private:
        /// @brief
        static const std::map<std::string, Directive> s_directives;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        CompilerCPreprocessor ( CompilerParserInterface * compilerCore,
                                CompilerOptions * opts );

        /**
         * @brief
         */
        virtual ~CompilerCPreprocessor() {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] inputFiles
         * @return
         */
        char * processFiles ( const std::vector<FILE *> & inputFiles );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] expr
         * @param[in] length
         * @return
         */
        bool evaluateExpr ( char * expr,
                            unsigned int length );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] inBuffer
         * @param[in,out] outBuffer
         * @param[in,out] mergeBuffer
         * @param[in,out] mlineBuffer
         * @return
         */
        inline bool inputProcessing ( Buffer & inBuffer,
                                      Buffer & outBuffer,
                                      Buffer & mergeBuffer,
                                      Buffer & mlineBuffer );

        /**
         * @brief
         * @param[in,out] inBuffer
         * @param[in,out] mergeBuffer
         * @return
         */
        inline bool lineMerging ( Buffer & inBuffer,
                                  Buffer & mergeBuffer );

        /**
         * @brief
         * @param[in,out] buffer
         * @param[out] lineMerge
         * @return
         */
        inline bool initialProcessing ( Buffer & buffer,
                                        bool & lineMerge );

        /**
         * @brief
         * @param[in,out] buffer
         * @return
         */
        inline bool directivesProcessing ( Buffer & buffer );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] directive
         * @return
         */
        inline bool handleDirective ( char * arguments,
                                      Directive directive );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleInclude ( char * arguments,
                                    unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleLine ( char * arguments,
                                 unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handlePragma ( char * arguments,
                                   unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleDefine ( char * arguments,
                                   unsigned int argLength );

        /**
         * @brief
         * @param[in,out] arguments
         * @param[in] argLength
         * @return
         */
        inline void handleUndef ( char * arguments,
                                  unsigned int argLength );

        /**
         * @brief
         * @param[in,out] length
         * @param[in,out] line
         * @param[in] pos
         */
        inline void cutLine ( ssize_t & length,
                              char * line,
                              unsigned int pos );

    ////    Private Attributes    ////
    private:
        ///
        InMode m_inmode;

        ///
        Conditional m_conditional;

        ///
        MacroTable m_macroTable;

        ///
        Include m_include;
};

#endif // COMPILERCPREPROCESSOR_H

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
 * @file CompilerCPreProcSupport.h
 */
// =============================================================================

#ifndef COMPILERCPREPROCSUPPORT_H
#define COMPILERCPREPROCSUPPORT_H

// Compiler header files.
#include "CompilerSourceLocation.h"

// Standard headers.
#include <string>
#include <vector>
#include <utility>

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCPreProcSupport
 */
class CompilerCPreProcSupport
{
    ////    Public Datatypes    ////
    public:
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
        struct Include
        {
            /**
             * @brief
             */
            struct Detection
            {
                /**
                 * @brief
                 */
                enum State
                {
                    STATE_NEGATIVE,     ///<
                    STATE_POSITIVE,     ///<
                    STATE_WHITE_SPACE_0,///<
                    STATE_WHITE_SPACE_1,///<
                    STATE_I,            ///<
                    STATE_N,            ///<
                    STATE_C,            ///<
                    STATE_L,            ///<
                    STATE_U,            ///<
                    STATE_D             ///<
                };

                /**
                 * @brief
                 */
                Detection();

                /**
                 * @brief
                 */
                void reset();

                /**
                 * @brief
                 * @param[in] in
                 */
                void input ( char in );

                /**
                 * @brief
                 * @return
                 */
                bool detected() const;

                ///
                State m_state;
            };

            ///
            std::string m_file;

            ///
            bool m_system;

            ///
            Detection m_detection;
        };

        class Buffer
        {
            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 */
                Buffer();

                /**
                 * @brief
                 * @param[in] data
                 */
                Buffer ( const std::string & data );

                /**
                 * @brief
                 * @param[in] data
                 */
                Buffer ( const char * data );

                /**
                 * @brief
                 * @param[in] data
                 * @param[in] length
                 */
                Buffer ( char * data,
                         unsigned int length );

                /**
                 * @brief
                 */
                ~Buffer();

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 * @param[in] sourceBuffer
                 */
                void append ( const Buffer & sourceBuffer );

                /**
                 * @brief
                 * @param[in,out] sourceBuffer
                 */
                void move ( Buffer & sourceBuffer );

            ////    Public Attributes    ////
            public:
                ssize_t m_pos;     //< Number of characters loaded in the buffer, without NULL(s).
                size_t m_size;     //< Overall size of the buffer.
                char * m_data;     //< Buffer pointer.
                bool m_persistent; //< Do not delete the data when this object is destroyed.
        };

        struct Conditional
        {
            public:
                /**
                 * @brief
                 * @param[out] location
                 * @return
                 */
                bool get ( CompilerSourceLocation * location = nullptr );

                /**
                 * @brief
                 * @return
                 */
                bool empty();

                /**
                 * @brief
                 * @param[in] location
                 * @param[in] condition
                 * @return
                 */
                void dirIf ( const CompilerSourceLocation & location,
                             bool condition );

                /**
                 * @brief
                 * @param[in] location
                 * @param[in] condition
                 * @return
                 */
                bool dirElif ( const CompilerSourceLocation & location,
                               bool condition );

                /**
                 * @brief
                 * @param[in] location
                 * @return
                 */
                bool dirElse ( const CompilerSourceLocation & location );

                /**
                 * @brief
                 */
                bool dirEndif();

            private:
                ///
                enum State
                {
                    STATE_POSITIVE, ///<
                    STATE_NEGATIVE, ///<
                    STATE_CLOSED    ///<
                };

                ///
                std::vector<std::pair<State,CompilerSourceLocation>> m_stack;
        };

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] location
         * @param[in] lineCorrection
         * @return
         */
        static CompilerSourceLocation locationCorrection ( const CompilerSourceLocation & location,
                                                           unsigned int lineCorrection = 0 );
};

#endif // COMPILERCPREPROCSUPPORT_H

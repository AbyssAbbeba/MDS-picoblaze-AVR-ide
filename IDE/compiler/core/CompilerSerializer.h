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
 * @file CompilerSerializer.h
 */
// =============================================================================

#ifndef COMPILERSERIALIZER_H
#define COMPILERSERIALIZER_H

// Forward declarations.
class CompilerLocationTracker;

// Compiler compiler header files.
#include "CompilerBase.h"

// Standard header files.
#include <cstdio>
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <cstdint>
#include <cstddef>

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerSerializer
 */
class CompilerSerializer
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const int INTERFACE_VERSION = 1;

        /// @brief
        static const char * const COMMON_FILE_HEADER;

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class Exception
        {
            ////    Public Datatypes    ////
            public:
                /**
                 * @brief
                 */
                enum Type
                {
                    ILLEGAL_ACCESS,     ///<
                    BUFFER_OVERFLOW,    ///<
                    BUFFER_UNDERFLOW,   ///<
                    DATA_STREAM_FAILURE ///<
                };

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 * @param[in] type
                 */
                Exception ( Type type ) : m_type ( type ) {};

            ////    Public Attributes    ////
            public:
                /// @brief
                const Type m_type;
        };

        /**
         * @brief
         */
        union SFloat2UInt32
        {
            /**
             * @brief
             */
            SFloat2UInt32 ( float f )
            {
                m_sfloat = f;
            }

            /**
             * @brief
             */
            SFloat2UInt32 ( uint32_t i )
            {
                m_uint32 = i;
            }

            /// @brief
            float m_sfloat;

            /// @brief
            uint32_t m_uint32;
        };

        /**
         * @brief
         */
        union DFloat2UInt64
        {
            /**
             * @brief
             */
            DFloat2UInt64 ( double f )
            {
                m_dfloat = f;
            }

            /**
             * @brief
             */
            DFloat2UInt64 ( uint64_t i )
            {
                m_uint64 = i;
            }

            /// @brief
            double m_dfloat;

            /// @brief
            uint64_t m_uint64;
        };

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum Role
        {
            SERIALIZER,  ///<
            DESERIALIZER ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] input
         * @param[in,out] files
         * @param[in,out] locationTracker
         * @param[in] lang
         * @param[in] arch
         * @param[in] hide
         */
        CompilerSerializer ( std::istream & input,
                             std::vector<std::pair<std::string,FILE*>> & files,
                             CompilerLocationTracker * locationTracker,
                             CompilerBase::LangId lang,
                             CompilerBase::TargetArch arch,
                             bool hide = false );

        /**
         * @brief
         * @param[in,out] output
         * @param[in] files
         * @param[in] locationTracker
         * @param[in] lang
         * @param[in] arch
         */
        CompilerSerializer ( std::ostream & output,
                             const std::vector<std::pair<std::string,FILE*>> & files,
                             const CompilerLocationTracker * locationTracker,
                             CompilerBase::LangId lang,
                             CompilerBase::TargetArch arch );

    ////    Public Operations    ////
    public:
        /// @name Writing methods, i.e. for object serialization.
        //@{
            /**
             * @brief Write one 8-bit unsigned integer.
             * @param[in] val (Value to serialize.)
             */
            void write ( uint8_t val );

            /**
             * @brief Write one 16-bit unsigned integer.
             * @param[in] val (Value to serialize.)
             */
            void write ( uint16_t val );

            /**
             * @brief Write one 32-bit unsigned integer.
             * @param[in] val (Value to serialize.)
             */
            void write ( uint32_t val );

            /**
             * @brief Write one 64-bit unsigned integer.
             * @param[in] val (Value to serialize.)
             */
            void write ( uint64_t val );

            /**
             * @brief Write one single precision floating-point number.
             * @param[in] val (Value to serialize.)
             */
            void write ( float val );

            /**
             * @brief Write one double precision floating-point number.
             * @param[in] val (Value to serialize.)
             */
            void write ( double val );

            /**
             * @brief Write a NULL terminated (C language style) string.
             * @param[in] val (String to serialize.)
             */
            void write ( const char * val );

            /**
             * @brief Write a C++ STL string.
             * @param[in] val (String to serialize.)
             */
            void write ( const std::string & val );
        //@}

        /// @name Reading methods, i.e. for object deserialization.
        //@{
            /**
             * @brief Read one 8-bit unsigned integer.
             * @return (Value read from the serialized object.)
             */
            uint8_t read_ui8();

            /**
             * @brief Read one 16-bit unsigned integer.
             * @return (Value read from the serialized object.)
             */
            uint16_t read_ui16();

            /**
             * @brief Read one 32-bit unsigned integer.
             * @return (Value read from the serialized object.)
             */
            uint32_t read_ui32();

            /**
             * @brief Read one 64-bit unsigned integer.
             * @return (Value read from the serialized object.)
             */
            uint64_t read_ui64();

            /**
             * @brief Read one single precision floating-point number.
             * @return (Value read from the serialized object.)
             */
            float read_float();

            /**
             * @brief Read one double precision floating-point number.
             * @return (Value read from the serialized object.)
             */
            double read_double();

            /**
             * @brief Read a string of arbitrary length.
             * @note This method automatically allocates storage for the string. 
             * @warning The return value has to be deallocated elsewhere (using delete[] operator);
             * @return NULL terminated (C language style) string read from the serialized object.
             */
            char * read_c_str_copy();

            /**
             * @brief Read a string of length up to bufferSize.
             * @note This method does not allocate any storage for the string.
             * @warning The buffer has to be preallocated to be able to hold at least n bytes.
             * @param[out] buffer Target storage.
             * @param[in] n Target storage capacity in bytes.
             * @return buffer.
             */
            char * read_c_str ( char * buffer,
                                size_t n );

            /**
             * @brief Read a string of arbitrary length.
             * @param[out] buffer Target storage.
             * @return C++ STL string read from the serialized object.
             */
            void read_std_str ( std::string & buffer );
        //@}

        /**
         * @brief
         * @param[in] number
         * @return
         */
        int translateFileNumber ( int number ) const;

        /**
         * @brief
         * @param[in] origin
         * @return
         */
        int translateLOrigin ( int origin );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void serial();

        /**
         * @brief
         */
        inline void deserial();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::istream * const m_input;

        /// @brief
        std::ostream * const m_output;

        /// @brief
        const Role m_role;

        /// @brief
        int m_locInitShift;

        /// @brief
        std::vector<int> m_fileNumberMap;
};

#endif // COMPILERSERIALIZER_H

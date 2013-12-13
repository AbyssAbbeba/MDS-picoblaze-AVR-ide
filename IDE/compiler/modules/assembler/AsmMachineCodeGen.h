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
 * @ingroup Assembler
 * @file AsmMachineCodeGen.h
 */
// =============================================================================

#ifndef ASMMACHINECODEGEN_H
#define ASMMACHINECODEGEN_H

// Forward declarations.
class DataFile;
class CompilerSemanticInterface;
class CompilerOptions;

// Standard headers.
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmMachineCodeGen
 */
class AsmMachineCodeGen
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int INITIAL_MAX_SIZE = 128; // Should be natural power of 2.

        /// @brief
        static const unsigned int SIZE_HINT = ( 1024 * 1024 ); // Limit maximum size to 1MB.

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Endianness
        {
            E_BIG_ENDIAN,
            E_LITTLE_ENDIAN
        };

        /**
         * @brief
         */
        enum WordSize
        {
            WORD_1B = 1,
            WORD_2B = 2,
            WORD_3B = 3,
            WORD_4B = 4
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmMachineCodeGen();

        /**
         * @brief
         */
        ~AsmMachineCodeGen();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] address
         */
        void setOrigin ( unsigned int address );

        /**
         * @brief
         * @param[in] code
         */
        unsigned int setCode ( uint32_t code );

        /**
         * @brief
         * @param[in] address
         * @param[in] code
         */
        void setCode ( unsigned int address,
                       uint32_t code );

        /**
         * @brief
         * @param[in] increment
         */
        void incrementAddr ( unsigned int increment = 1 );

        /**
         * @brief
         * @param[in] wordSize
         * @param[in] byteOrder
         * @param[in,out] target
         * @throw DataFileException
         */
        void output ( WordSize wordSize,
                      Endianness byteOrder,
                      DataFile * target ) const;

        /**
         * @brief
         * @param[in] wordSize
         * @param[in] byteOrder
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        void output ( WordSize wordSize,
                      Endianness byteOrder,
                      CompilerSemanticInterface * compilerCore,
                      const CompilerOptions * opts );

        /**
         * @brief
         */
        void clear();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        unsigned int size() const
        {
            return m_size;
        }

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] maxAddr
         * @return
         */
        inline bool reserve ( unsigned int maxAddr );

        /**
         * @brief
         * @param[in] wordSize
         * @param[in] byteOrder
         * @param[in,out] dataFile
         * @param[in] fileName
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        inline void saveMachineCode ( WordSize wordSize,
                                      Endianness byteOrder,
                                      DataFile * dataFile,
                                      const std::string & fileName,
                                      CompilerSemanticInterface * compilerCore,
                                      const CompilerOptions * opts );

        /**
         * @brief
         * @param[in] wordSize
         * @return
         */
        inline unsigned int sizeB ( WordSize wordSize ) const;

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_address;

        ///
        unsigned int m_maxSize;

        ///
        unsigned int m_size;

        ///
        std::vector<uint32_t> m_code;

        ///
        std::vector<char> m_used;
};

#endif // ASMMACHINECODEGEN_H

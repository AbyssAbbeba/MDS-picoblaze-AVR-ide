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
 * @ingroup Assembler
 * @file AsmMachineCodeGen.h
 */
// =============================================================================

#ifndef ASMMACHINECODEGEN_H
#define ASMMACHINECODEGEN_H

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
        static const unsigned int INITIAL_MAX_SIZE = 4;

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
        void setOrigin ( uint32_t address );

        /**
         * @brief
         * @param[in] code
         */
        uint32_t setCode ( uint32_t code );

        /**
         * @brief
         * @param[in] address
         * @param[in] code
         */
        void setCode ( uint32_t address,
                       uint32_t code );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] maxAddr
         */
        inline void reserve ( uint32_t maxAddr );

    ////    Private Attributes    ////
    private:
        ///
        uint32_t m_maxSize;

        ///
        uint32_t m_address;

        ///
        std::vector<uint32_t> m_code;

        ///
        std::vector<char> m_used;
};

#endif // ASMMACHINECODEGEN_H

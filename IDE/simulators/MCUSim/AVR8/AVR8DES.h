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
 * @ingroup AVR8
 * @file AVR8DES.h
 */
// =============================================================================

#ifndef AVR8DES_H
#define AVR8DES_H

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DES
 */
class AVR8DES
{
    ////    Static Private Constants    ////
    private:
        /// @brief
        static const uint64_t IP_TABLE [ 64 ];

        /// @brief
        static const uint64_t IPI_TABLE [ 64 ];

        /// @brief
        static const uint32_t E_TABLE [ 48 ];

        /// @brief
        static const uint32_t P_TABLE [ 32 ];

        /// @brief
        static const uint32_t S_TABLE [ 8 ] [ 4 ] [ 16 ];

        /// @brief
        static const uint64_t PC1_TABLE [ 56 ];

        /// @brief
        static const uint64_t PC2_TABLE [ 48 ];

        /// @brief
        static const int R_TABLE [ 16 ];

    ////    Static Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] dataBlock
         * @param[in,out] keyBlock
         * @param[in] round
         * @param[in] decipher
         * @return
         */
        static void cipher ( uint64_t & dataBlock,
                             uint64_t & keyBlock,
                             unsigned int round,
                             bool decipher );

    ////    Static Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] input
         * @param[in] permutationTable
         * @return
         */
        template<typename InputType, typename OutputType, int outputSize>
            inline static OutputType permute ( InputType input,
                                               const InputType * permutationTable );

        /**
         * @brief
         * @param[in] input
         * @return
         */
        inline static uint64_t initialPermutation ( uint64_t input );

        /**
         * @brief
         * @param[in] input
         * @return
         */
        inline static uint64_t initialPermutationInv ( uint64_t input );

        /**
         * @brief
         * @param[in] input
         * @return
         */
        inline static uint32_t permutationFunction ( uint32_t input );

        /**
         * @brief
         * @param[in] input
         * @return
         */
        inline static uint32_t substitutionFunction ( uint64_t input );

        /**
         * @brief
         * @param[in] halfBlock
         * @param[in] subkey
         * @return
         */
        inline static uint32_t feistelFunction ( uint32_t halfBlock,
                                                 uint64_t subkey );

        /**
         * @brief
         * @param[in] halfBlock
         * @return
         */
        inline static uint64_t expansionFunction ( uint32_t halfBlock );

        /**
         * @brief
         * @param[in,out] keyBlock
         * @param[in] round
         * @param[in] decipher
         * @return
         */
        inline static uint64_t keySchedule ( uint64_t & keyBlock,
                                             unsigned int round,
                                             bool decipher );

        /**
         * @brief
         * @param[in] keyBlock
         * @return
         */
        inline static uint64_t permutedChoice1 ( uint64_t keyBlock );

        /**
         * @brief
         * @param[in] keyBlock
         * @return
         */
        inline static uint64_t permutedChoice2 ( uint64_t keyBlock );

        /**
         * @brief
         * @param[in] input
         * @param[in] by
         * @return
         */
        inline static uint32_t rotateLeft32 ( uint32_t input,
                                              unsigned int by );

        /**
         * @brief
         * @param[in] input
         * @param[in] by
         * @return
         */
        inline static uint32_t rotateRight32 ( uint32_t input,
                                               unsigned int by );
};

#endif // AVR8DES_H

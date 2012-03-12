/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup AVR8
 * @file AVR8DES.h
 */

#ifndef AVR8DES_H
#define AVR8DES_H

#include <cstddef>
#include <cstdint>

/**
 * @brief
 * @ingroup AVR8
 * @class AVR8DES
 */
class AVR8DES {
public:
	static void cipher(uint64_t & dataBlock, uint64_t & keyBlock, unsigned int round, bool decipher);

private:
	static const uint64_t IP_TABLE[64];
	static const uint64_t IPI_TABLE[64];
	static const uint32_t E_TABLE[48];
	static const uint32_t P_TABLE[32];
	static const uint32_t S_TABLE[8][4][16];
	static const uint64_t PC1_TABLE[56];
	static const uint64_t PC2_TABLE[48];
	static const int R_TABLE[16];

	template<typename InputType, typename OutputType, int outputSize>
	inline static OutputType permute(InputType input, const InputType * permutationTable);

	inline static uint64_t initialPermutation(uint64_t input);
	inline static uint64_t initialPermutationInv(uint64_t input);

	inline static uint32_t permutationFunction(uint32_t input);
	inline static uint32_t substitutionFunction(uint64_t input);
	inline static uint32_t feistelFunction(uint32_t halfBlock, uint64_t subkey);
	inline static uint64_t expansionFunction(uint32_t halfBlock);

	inline static uint64_t keySchedule(uint64_t & keyBlock, unsigned int round, bool decipher);
	inline static uint64_t permutedChoice1(uint64_t keyBlock);
	inline static uint64_t permutedChoice2(uint64_t keyBlock);
	inline static uint32_t rotateLeft32(uint32_t input, unsigned int by);
	inline static uint32_t rotateRight32(uint32_t input, unsigned int by);
};

#endif // AVR8DES_H

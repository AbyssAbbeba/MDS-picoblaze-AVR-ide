// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AVR8
 * @file AVR8DES.cxx
 */
// =============================================================================

#include "AVR8DES.h"

#define M32(b)  ( uint32_t(1) << (32 - b) )
#define M48(b)  ( uint64_t(1) << (48 - b) )
#define M56(b)  ( uint64_t(1) << (56 - b) )
#define M64(b)  ( uint64_t(1) << (64 - b) )

const uint64_t AVR8DES::IP_TABLE[64] =
{
    M64(58), M64(50), M64(42), M64(34), M64(26), M64(18), M64(10), M64( 2),
    M64(60), M64(52), M64(44), M64(36), M64(28), M64(20), M64(12), M64( 4),
    M64(62), M64(54), M64(46), M64(38), M64(30), M64(22), M64(14), M64( 6),
    M64(64), M64(56), M64(48), M64(40), M64(32), M64(24), M64(16), M64( 8),
    M64(57), M64(49), M64(41), M64(33), M64(25), M64(17), M64( 9), M64( 1),
    M64(59), M64(51), M64(43), M64(35), M64(27), M64(19), M64(11), M64( 3),
    M64(61), M64(53), M64(45), M64(37), M64(29), M64(21), M64(13), M64( 5),
    M64(63), M64(55), M64(47), M64(39), M64(31), M64(23), M64(15), M64( 7)
};

const uint64_t AVR8DES::IPI_TABLE[64] =
{
    M64(40), M64( 8), M64(48), M64(16), M64(56), M64(24), M64(64), M64(32),
    M64(39), M64( 7), M64(47), M64(15), M64(55), M64(23), M64(63), M64(31),
    M64(38), M64( 6), M64(46), M64(14), M64(54), M64(22), M64(62), M64(30),
    M64(37), M64( 5), M64(45), M64(13), M64(53), M64(21), M64(61), M64(29),
    M64(36), M64( 4), M64(44), M64(12), M64(52), M64(20), M64(60), M64(28),
    M64(35), M64( 3), M64(43), M64(11), M64(51), M64(19), M64(59), M64(27),
    M64(34), M64( 2), M64(42), M64(10), M64(50), M64(18), M64(58), M64(26),
    M64(33), M64( 1), M64(41), M64( 9), M64(49), M64(17), M64(57), M64(25)
};

const uint64_t AVR8DES::PC1_TABLE[56] =
{
    M64(57), M64(49), M64(41), M64(33), M64(25), M64(17), M64( 9),
    M64( 1), M64(58), M64(50), M64(42), M64(34), M64(26), M64(18),
    M64(10), M64( 2), M64(59), M64(51), M64(43), M64(35), M64(27),
    M64(19), M64(11), M64( 3), M64(60), M64(52), M64(44), M64(36),
    M64(63), M64(55), M64(47), M64(39), M64(31), M64(23), M64(15),
    M64( 7), M64(62), M64(54), M64(46), M64(38), M64(30), M64(22),
    M64(14), M64( 6), M64(61), M64(53), M64(45), M64(37), M64(29),
    M64(21), M64(13), M64( 5), M64(28), M64(20), M64(12), M64( 4)
};

const uint64_t AVR8DES::PC2_TABLE[48] =
{
    M56(14), M56(17), M56(11), M56(24), M56( 1), M56( 5),
    M56( 3), M56(28), M56(15), M56( 6), M56(21), M56(10),
    M56(23), M56(19), M56(12), M56( 4), M56(26), M56( 8),
    M56(16), M56( 7), M56(27), M56(20), M56(13), M56( 2),
    M56(41), M56(52), M56(31), M56(37), M56(47), M56(55),
    M56(30), M56(40), M56(51), M56(45), M56(33), M56(48),
    M56(44), M56(49), M56(39), M56(56), M56(34), M56(53),
    M56(46), M56(42), M56(50), M56(36), M56(29), M56(32)
};

const uint32_t AVR8DES::E_TABLE[48] =
{
    M32(32), M32( 1), M32( 2), M32( 3), M32( 4), M32( 5),
    M32( 4), M32( 5), M32( 6), M32( 7), M32( 8), M32( 9),
    M32( 8), M32( 9), M32(10), M32(11), M32(12), M32(13),
    M32(12), M32(13), M32(14), M32(15), M32(16), M32(17),
    M32(16), M32(17), M32(18), M32(19), M32(20), M32(21),
    M32(20), M32(21), M32(22), M32(23), M32(24), M32(25),
    M32(24), M32(25), M32(26), M32(27), M32(28), M32(29),
    M32(28), M32(29), M32(30), M32(31), M32(32), M32( 1)
};

const uint32_t AVR8DES::P_TABLE[32] =
{
    M32(16), M32( 7), M32(20), M32(21),
    M32(29), M32(12), M32(28), M32(17),
    M32( 1), M32(15), M32(23), M32(26),
    M32( 5), M32(18), M32(31), M32(10),
    M32( 2), M32( 8), M32(24), M32(14),
    M32(32), M32(27), M32( 3), M32( 9),
    M32(19), M32(13), M32(30), M32( 6),
    M32(22), M32(11), M32( 4), M32(25)
};

const int AVR8DES::R_TABLE[16] =
{
    // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 // - Iteration number
       1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 // - Number of rotations
};

const uint32_t AVR8DES::S_TABLE[8][4][16] =
{
    {
        /*   0        1        2        3        4        5        6        7  ←Col      : S₁ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₁ */
        {0xE<<28, 0x4<<28, 0xD<<28, 0x1<<28, 0x2<<28, 0xF<<28, 0xB<<28, 0x8<<28, /*  0   : S₁ */
         0x3<<28, 0xA<<28, 0x6<<28, 0xC<<28, 0x5<<28, 0x9<<28, 0x0<<28, 0x7<<28},
        {0x0<<28, 0xF<<28, 0x7<<28, 0x4<<28, 0xE<<28, 0x2<<28, 0xD<<28, 0x1<<28, /*  1   : S₁ */
         0xA<<28, 0x6<<28, 0xC<<28, 0xB<<28, 0x9<<28, 0x5<<28, 0x3<<28, 0x8<<28},
        {0x4<<28, 0x1<<28, 0xE<<28, 0x8<<28, 0xD<<28, 0x6<<28, 0x2<<28, 0xB<<28, /*  2   : S₁ */
         0xF<<28, 0xC<<28, 0x9<<28, 0x7<<28, 0x3<<28, 0xA<<28, 0x5<<28, 0x0<<28},
        {0xF<<28, 0xC<<28, 0x8<<28, 0x2<<28, 0x4<<28, 0x9<<28, 0x1<<28, 0x7<<28, /*  3   : S₁ */
         0x5<<28, 0xB<<28, 0x3<<28, 0xE<<28, 0xA<<28, 0x0<<28, 0x6<<28, 0xD<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₂ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₂ */
        {0x9<<28, 0x7<<28, 0x2<<28, 0xD<<28, 0xC<<28, 0x0<<28, 0x5<<28, 0xA<<28, /*  0   : S₂ */
         0xF<<28, 0x1<<28, 0x8<<28, 0xE<<28, 0x6<<28, 0xB<<28, 0x3<<28, 0x4<<28},
        {0xC<<28, 0x0<<28, 0x1<<28, 0xA<<28, 0x6<<28, 0x9<<28, 0xB<<28, 0x5<<28, /*  1   : S₂ */
         0x3<<28, 0xD<<28, 0x4<<28, 0x7<<28, 0xF<<28, 0x2<<28, 0x8<<28, 0xE<<28},
        {0x5<<28, 0x8<<28, 0xC<<28, 0x6<<28, 0x9<<28, 0x3<<28, 0x2<<28, 0xF<<28, /*  2   : S₂ */
         0x0<<28, 0xE<<28, 0x7<<28, 0xB<<28, 0xA<<28, 0x4<<28, 0xD<<28, 0x1<<28},
        {0xB<<28, 0x6<<28, 0x7<<28, 0xC<<28, 0x0<<28, 0x5<<28, 0xE<<28, 0x9<<28, /*  3   : S₂ */
         0xD<<28, 0x8<<28, 0xA<<28, 0x1<<28, 0x3<<28, 0xF<<28, 0x4<<28, 0x2<<28}

    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₃ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₃ */
        {0x1<<28, 0xD<<28, 0xC<<28, 0x7<<28, 0xB<<28, 0x4<<28, 0x2<<28, 0x8<<28, /*  0   : S₃ */
         0xA<<28, 0x0<<28, 0x9<<28, 0xE<<28, 0x6<<28, 0x3<<28, 0xF<<28, 0x5<<28},
        {0x2<<28, 0x8<<28, 0x5<<28, 0xE<<28, 0xC<<28, 0xB<<28, 0xF<<28, 0x1<<28, /*  1   : S₃ */
         0xD<<28, 0x7<<28, 0x0<<28, 0x9<<28, 0x3<<28, 0x4<<28, 0x6<<28, 0xA<<28},
        {0xB<<28, 0x1<<28, 0x2<<28, 0xC<<28, 0x5<<28, 0xA<<28, 0xE<<28, 0x7<<28, /*  2   : S₃ */
         0xD<<28, 0x6<<28, 0x4<<28, 0x9<<28, 0x8<<28, 0xF<<28, 0x3<<28, 0x0<<28},
        {0x4<<28, 0xF<<28, 0xE<<28, 0x3<<28, 0xB<<28, 0x5<<28, 0x2<<28, 0xC<<28, /*  3   : S₃ */
         0x1<<28, 0xA<<28, 0xD<<28, 0x0<<28, 0x6<<28, 0x9<<28, 0x8<<28, 0x7<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₄ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₄ */
        {0x1<<28, 0x2<<28, 0x8<<28, 0x5<<28, 0xB<<28, 0xC<<28, 0x4<<28, 0xF<<28, /*  0   : S₄ */
         0x7<<28, 0xD<<28, 0xE<<28, 0x3<<28, 0x0<<28, 0x6<<28, 0x9<<28, 0xA<<28},
        {0x4<<28, 0x7<<28, 0x2<<28, 0xC<<28, 0x1<<28, 0xA<<28, 0xE<<28, 0x9<<28, /*  1   : S₄ */
         0xD<<28, 0x8<<28, 0xB<<28, 0x5<<28, 0x6<<28, 0xF<<28, 0x0<<28, 0x3<<28},
        {0xF<<28, 0x1<<28, 0x3<<28, 0xE<<28, 0x5<<28, 0x2<<28, 0x8<<28, 0x4<<28, /*  2   : S₄ */
         0xA<<28, 0x6<<28, 0x9<<28, 0x0<<28, 0xC<<28, 0xB<<28, 0x7<<28, 0xD<<28},
        {0x9<<28, 0x4<<28, 0x5<<28, 0xB<<28, 0xC<<28, 0x7<<28, 0x2<<28, 0xE<<28, /*  3   : S₄ */
         0x3<<28, 0xF<<28, 0x0<<28, 0x6<<28, 0xA<<28, 0x1<<28, 0xD<<28, 0x8<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₅ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₅ */
        {0x2<<28, 0xC<<28, 0x4<<28, 0x1<<28, 0x7<<28, 0xA<<28, 0xB<<28, 0x6<<28, /*  0   : S₅ */
         0x8<<28, 0x5<<28, 0x3<<28, 0xF<<28, 0xD<<28, 0x0<<28, 0xE<<28, 0x9<<28},
        {0xE<<28, 0xB<<28, 0x2<<28, 0xC<<28, 0x4<<28, 0x7<<28, 0xD<<28, 0x1<<28, /*  1   : S₅ */
         0x5<<28, 0x0<<28, 0xF<<28, 0xA<<28, 0x3<<28, 0x9<<28, 0x8<<28, 0x6<<28},
        {0x4<<28, 0x2<<28, 0x1<<28, 0xB<<28, 0xA<<28, 0xD<<28, 0x7<<28, 0x8<<28, /*  2   : S₅ */
         0xF<<28, 0x9<<28, 0xC<<28, 0x5<<28, 0x6<<28, 0x3<<28, 0x0<<28, 0xE<<28},
        {0xB<<28, 0x8<<28, 0xC<<28, 0x7<<28, 0x1<<28, 0xE<<28, 0x2<<28, 0xD<<28, /*  3   : S₅ */
         0x6<<28, 0xF<<28, 0x0<<28, 0x9<<28, 0xA<<28, 0x4<<28, 0x5<<28, 0x3<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₆ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₆ */
        {0xC<<28, 0x1<<28, 0xA<<28, 0xF<<28, 0x9<<28, 0x2<<28, 0x6<<28, 0x8<<28, /*  0   : S₆ */
         0x0<<28, 0xD<<28, 0x3<<28, 0x4<<28, 0xE<<28, 0x7<<28, 0x5<<28, 0xB<<28},
        {0xA<<28, 0xF<<28, 0x4<<28, 0x2<<28, 0x7<<28, 0xC<<28, 0x9<<28, 0x5<<28, /*  1   : S₆ */
         0x6<<28, 0x1<<28, 0xD<<28, 0xE<<28, 0x0<<28, 0xB<<28, 0x3<<28, 0x8<<28},
        {0x9<<28, 0xE<<28, 0xF<<28, 0x5<<28, 0x2<<28, 0x8<<28, 0xC<<28, 0x3<<28, /*  2   : S₆ */
         0x7<<28, 0x0<<28, 0x4<<28, 0xA<<28, 0x1<<28, 0xD<<28, 0xB<<28, 0x6<<28},
        {0x4<<28, 0x3<<28, 0x2<<28, 0xC<<28, 0x9<<28, 0x5<<28, 0xF<<28, 0xA<<28, /*  3   : S₆ */
         0xB<<28, 0xE<<28, 0x1<<28, 0x7<<28, 0x6<<28, 0x0<<28, 0x8<<28, 0xD<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₇ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₇ */
        {0x4<<28, 0xB<<28, 0x2<<28, 0xE<<28, 0xF<<28, 0x0<<28, 0x8<<28, 0xD<<28, /*  0   : S₇ */
         0x3<<28, 0xC<<28, 0x9<<28, 0x7<<28, 0x5<<28, 0xA<<28, 0x6<<28, 0x1<<28},
        {0xD<<28, 0x0<<28, 0xB<<28, 0x7<<28, 0x4<<28, 0x9<<28, 0x1<<28, 0xA<<28, /*  1   : S₇ */
         0xE<<28, 0x3<<28, 0x5<<28, 0xC<<28, 0x2<<28, 0xF<<28, 0x8<<28, 0x6<<28},
        {0x1<<28, 0x4<<28, 0xB<<28, 0xD<<28, 0xC<<28, 0x3<<28, 0x7<<28, 0xE<<28, /*  2   : S₇ */
         0xA<<28, 0xF<<28, 0x6<<28, 0x8<<28, 0x0<<28, 0x5<<28, 0x9<<28, 0x2<<28},
        {0x6<<28, 0xB<<28, 0xD<<28, 0x8<<28, 0x1<<28, 0x4<<28, 0xA<<28, 0x7<<28, /*  3   : S₇ */
         0x9<<28, 0x5<<28, 0x0<<28, 0xF<<28, 0xE<<28, 0x2<<28, 0x3<<28, 0xC<<28}
    },{
        /*   0        1        2        3        4        5        6        7  ←Col      : S₈ */
        /*   8        9       10       11       12       13       14       15  ←Col Row↓ : S₈ */
        {0xD<<28, 0x2<<28, 0x8<<28, 0x4<<28, 0x6<<28, 0xF<<28, 0xB<<28, 0x1<<28, /*  0   : S₈ */
         0xA<<28, 0x9<<28, 0x3<<28, 0xE<<28, 0x5<<28, 0x0<<28, 0xC<<28, 0x7<<28},
        {0x1<<28, 0xF<<28, 0xD<<28, 0x8<<28, 0xA<<28, 0x3<<28, 0x7<<28, 0x4<<28, /*  1   : S₈ */
         0xC<<28, 0x5<<28, 0x6<<28, 0xB<<28, 0x0<<28, 0xE<<28, 0x9<<28, 0x2<<28},
        {0x7<<28, 0xB<<28, 0x4<<28, 0x1<<28, 0x9<<28, 0xC<<28, 0xE<<28, 0x2<<28, /*  2   : S₈ */
         0x0<<28, 0x6<<28, 0xA<<28, 0xD<<28, 0xF<<28, 0x3<<28, 0x5<<28, 0x8<<28},
        {0x2<<28, 0x1<<28, 0xE<<28, 0x7<<28, 0x4<<28, 0xA<<28, 0x8<<28, 0xD<<28, /*  3   : S₈ */
         0xF<<28, 0xC<<28, 0x9<<28, 0x0<<28, 0x3<<28, 0x5<<28, 0x6<<28, 0xB<<28}
    }
};

//           ╭─────────────╮
//           │ Input (64b) │
//           ╰──────┬──────╯
//       ╔══════════╧══════════╗
//       ║ Initial permutation ║
//       ╚══════════╤══════════╝
//          32b     │      32b
//    ╭─────╱──────←┴→─────╱──────╮
// ╭──┴──╮  ╭───────────────╮  ╭──┴──╮
// │  L₀ │  │Subkey K₀ (48b)│  │  R₀ │
// ╰──┬──╯  ╰───────┬───────╯  ╰──┬──╯
// ╔══╧══╗ ╔════════╧═════════╗   │
// ║ XOR ╟←╢ Feistel function ╟←──┤
// ╚══╤══╝ ╚══════════════════╝   │
//    ┊                           ┊
//    ┊                           ┊     ─╮
//    │             ╭─────────────╯      │
//    ╰─────────────┼─────────────╮      │
//    ╭─────────────╯             │      │
// ╭──┴──╮  ╭───────────────╮  ╭──┴──╮   │
// │  Lₙ │  │Subkey Kₙ (48b)│  │  Rₙ │   ├ Repeat 15×
// ╰──┬──╯  ╰───────┬───────╯  ╰──┬──╯   │
// ╔══╧══╗ ╔════════╧═════════╗   │      │
// ║ XOR ╟←╢ Feistel function ╟←──┤      │
// ╚══╤══╝ ╚══════════════════╝   │      │
//    │     32b            32b    │     ─╯
//    ╰─────╱─────→─┬─←────╱──────╯
//          ╭───────┴───────╮
//          │ Output (64b)  │
//          ╰───────────────╯
void AVR8DES::cipher ( uint64_t & dataBlock,
                       uint64_t & keyBlock,
                       unsigned int round,
                       bool decipher )
{
    uint64_t data;

    if ( true == decipher )
    {
        data = initialPermutationInv(dataBlock);
    }
    else
    {
        data = initialPermutation(dataBlock);
    }

    // Divide 'data' into two, left and right, 32b halves.
    uint32_t leftHalfBlock  = ( data >> 32 );                      // upper 32b
    uint32_t rightHalfBlock = ( data & ( ( 0x1ULL << 32 ) - 1 ) ); // lower 32b

    // Generate the subkey.
    uint64_t subkey = keySchedule(keyBlock, round, decipher);

    // Perform encryption round.
    leftHalfBlock ^= feistelFunction(rightHalfBlock, subkey);

    // Reassemble 'data' from the halves.
    if ( 15 != round )
    {
        // Normal round, swap right and left block.
        data = (uint64_t(rightHalfBlock) << 32) | leftHalfBlock;
    }
    else
    {
        // Final round, DO NOT swap the blocks.
        data = (uint64_t(leftHalfBlock) << 32) | rightHalfBlock;
    }

    if ( true == decipher )
    {
        dataBlock = initialPermutation(data);
    }
    else
    {
        dataBlock = initialPermutationInv(data);
    }
}

template<typename InputType, typename OutputType, int outputSize>
    inline OutputType AVR8DES::permute ( InputType input,
                                         const InputType * permutationTable )
{
    OutputType output = 0;
    for ( int i = (outputSize - 1); i >= 0 ; i-- ) {
        if ( input & permutationTable[i] ) {
            output |= 0x1;
        }
        output <<= 1;
    }
    return output;
}

inline uint64_t AVR8DES::initialPermutation ( uint64_t input )
{
    return permute<uint64_t,uint64_t,64>(input, IP_TABLE);
}

inline uint64_t AVR8DES::initialPermutationInv ( uint64_t input )
{
    return permute<uint64_t,uint64_t,64>(input, IPI_TABLE);
}

//         ╭──────────────────╮          ╭───────────────╮      E - Expansion function
//         │ Half block (32b) │          │  Subkey (48b) │      S - Substitution function
//         ╰────────┬─────────╯          ╰───────┬───────╯      P - Permutation function
//               ╔══╧══╗                         │
//               ║  E  ║                         │
//               ╚══╤══╝   48b  ╔═════╗     48b  │
//                  ╰──────╱───→╢ XOR ╟←────╱────╯
//                              ╚══╤══╝
//  ╭┬┬┬┬┬──┬┬┬┬┬┬──┬┬┬┬┬┬──┬┬┬┬┬┬─┴─┬┬┬┬┬┬──┬┬┬┬┬┬──┬┬┬┬┬┬──┬┬┬┬┬╮
// ╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗ ╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗╔╧╧╧╧╧╧╗
// ║  S₁  ║║  S₂  ║║  S₃  ║║  S₄  ║ ║  S₅  ║║  S₆  ║║  S₇  ║║  S₈  ║
// ╚═╤╤╤╤═╝╚═╤╤╤╤═╝╚═╤╤╤╤═╝╚═╤╤╤╤═╝ ╚═╤╤╤╤═╝╚═╤╤╤╤═╝╚═╤╤╤╤═╝╚═╤╤╤╤═╝
//   ╰┴┴┴────┴┴┴┴────┴┴┴┴────┴┴┴┴──┬──┴┴┴┴────┴┴┴┴────┴┴┴┴────┴┴┴╯
//                              ╔══╧══╗
//                              ║  P  ║
//                              ╚══╤══╝
//                          ╭──────┴──────╮
//                          │ 32b result  │
//                          ╰─────────────╯
inline uint32_t AVR8DES::feistelFunction ( uint32_t halfBlock,
                                           uint64_t subkey )
{
    return permutationFunction(substitutionFunction( subkey ^ expansionFunction(halfBlock )));
}

inline uint32_t AVR8DES::permutationFunction ( uint32_t input )
{
    return permute<uint32_t,uint32_t,32>(input, P_TABLE);
}

inline uint32_t AVR8DES::substitutionFunction ( uint64_t input )
{
    uint32_t output = 0;
    for ( int i = 0; i < 8; i-- )
    {
        // 6b block:
        // ╭────┬────┬────┬────┬────┬────╮
        // │ R₁ │ C₃ │ C₂ │ C₁ │ C₀ │ R₀ │
        // ╰────┴────┴────┴────┴────┴────╯
        unsigned int block = (input & 0x3f);
        unsigned int col = (block & 0x1e) >> 1; // R₁R₀
        unsigned int row = ((block & 0x20) >> 4) | (block & 0x01); // C₃C₂C₁C₀

        input >>= 6;
        output >>= 4;

        output |= S_TABLE[i][row][col];
    }
    return output;
}

inline uint64_t AVR8DES::expansionFunction ( uint32_t halfBlock )
{
    return permute<uint32_t,uint64_t,48>(halfBlock, E_TABLE);
}

inline uint64_t AVR8DES::permutedChoice1 ( uint64_t keyBlock )
{
    return permute<uint64_t,uint64_t,56>(keyBlock, PC1_TABLE);
}

inline uint64_t AVR8DES::permutedChoice2 ( uint64_t keyBlock )
{
    return permute<uint64_t,uint64_t,48>(keyBlock, PC2_TABLE);
}

inline uint32_t AVR8DES::rotateLeft32 ( uint32_t input,
                                        unsigned int by )
{
    by %= 32;
    return (input << by) | (input >> (32 - by));
}

inline uint32_t AVR8DES::rotateRight32 ( uint32_t input,
                                         unsigned int by )
{
    by %= 32;
    return (input >> by) | (input << (32 - by));
}

//      ╭───────────╮
//      │ Key (64b) │                   Note: the key has 64b, however, only 56
//      ╰─────┬─────╯                   actually affects the cipher, the other
//  ╔═════════╧═════════╗               8b are redundant,they might be used e.g.
//  ║ Permuted choice 1 ║               for parity checking.
//  ╚═════════╤═════════╝
//       28b  │   28b
//    ╭──╱────┴───╱───╮                 C   - Left half block of the key (28b)
// ╭──┴──╮         ╭──┴──╮              D   - Right half block of the key (28b)
// │ C₀  │         │ D₀  │              < < - Left rotation by R_TABLE[n]
// ╰──┬──╯         ╰──┬──╯
//╔═══╧═══╗       ╔═══╧═══╗
//║  < <  ║       ║  < <  ║
//╚═══╤═══╝       ╚═══╤═══╝
// ╭──┴──╮         ╭──┴──╮
// │ C₁  ├→╮       │ D₁  ├→╮
// ╰──┬──╯ │       ╰──┬──╯ │  ╔═══════════════════╗
//    │    ╰──────────┼────┴─→╢ Permuted choice 2 ╟──→ Subkey K₁ (48b)
//    ┊               ┊       ╚═══════════════════╝
//    ┊               ┊
//╔═══╧═══╗       ╔═══╧═══╗
//║  < <  ║       ║  < <  ║
//╚═══╤═══╝       ╚═══╤═══╝
// ╭──┴──╮         ╭──┴──╮
// │ Cₙ  ├→╮       │ Dₙ  ├→╮
// ╰──┬──╯ │       ╰──┬──╯ │  ╔═══════════════════╗
//    │    ╰──────────┼────┴─→╢ Permuted choice 2 ╟──→ Subkey Kₙ (48b)
//    ┊               ┊       ╚═══════════════════╝
//    ┊               ┊
//╔═══╧═══╗       ╔═══╧═══╗
//║  < <  ║       ║  < <  ║
//╚═══╤═══╝       ╚═══╤═══╝
// ╭──┴──╮         ╭──┴──╮
// │ C₁₆ ├→╮       │ D₁₆ ├→╮
// ╰─────╯ │       ╰─────╯ │  ╔═══════════════════╗
//         ╰───────────────┴─→╢ Permuted choice 2 ╟──→ Subkey K₁₆ (48b)
//                            ╚═══════════════════╝
inline uint64_t AVR8DES::keySchedule ( uint64_t & keyBlock,
                                       unsigned int round,
                                       bool decipher )
{
    uint64_t subkey;

    if ( 0 == round )
    {
        // The first round.
        keyBlock = permutedChoice1(keyBlock);
    }

    int numberOfRotations;
    if ( true == decipher )
    {
        numberOfRotations = R_TABLE[15 - round];
    }
    else
    {
        numberOfRotations = R_TABLE[round];
    }

    uint32_t lowerHalf = keyBlock & ((0x1 << 28) - 1);
    keyBlock >>= 28;
    uint32_t upperHalf = keyBlock & ((0x1 << 28) - 1);

    if ( true == decipher )
    {
        subkey = permutedChoice2(keyBlock);

        keyBlock = rotateRight32(upperHalf, numberOfRotations);
        keyBlock <<= 28;
        keyBlock |= rotateRight32(lowerHalf, numberOfRotations);
    }
    else
    {
        keyBlock = rotateLeft32(upperHalf, numberOfRotations);
        keyBlock <<= 28;
        keyBlock |= rotateLeft32(lowerHalf, numberOfRotations);

        subkey = permutedChoice2(keyBlock);
    }

    return subkey;
}

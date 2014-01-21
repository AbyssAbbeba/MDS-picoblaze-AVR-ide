// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AVR8
 * @file AVR8DES.cxx
 */
// =============================================================================

#include "AVR8DES.h"

constexpr uint32_t m32 ( int b )
{
    return ( uint32_t(1) << (32 - b) );
}

constexpr uint64_t m56 ( int b )
{
    return ( uint64_t(1) << (56 - b) );
}

constexpr uint64_t m64 ( int b )
{
    return ( uint64_t(1) << (64 - b) );
}

constexpr uint32_t sb ( int v )
{
    return ( uint32_t(v) << 28 );
}

const uint32_t AVR8DES::P_TABLE[32] =
{
    m32(16), m32( 7), m32(20), m32(21),
    m32(29), m32(12), m32(28), m32(17),
    m32( 1), m32(15), m32(23), m32(26),
    m32( 5), m32(18), m32(31), m32(10),
    m32( 2), m32( 8), m32(24), m32(14),
    m32(32), m32(27), m32( 3), m32( 9),
    m32(19), m32(13), m32(30), m32( 6),
    m32(22), m32(11), m32( 4), m32(25)
};

const uint32_t AVR8DES::E_TABLE[48] =
{
    m32(32), m32( 1), m32( 2), m32( 3), m32( 4), m32( 5),
    m32( 4), m32( 5), m32( 6), m32( 7), m32( 8), m32( 9),
    m32( 8), m32( 9), m32(10), m32(11), m32(12), m32(13),
    m32(12), m32(13), m32(14), m32(15), m32(16), m32(17),
    m32(16), m32(17), m32(18), m32(19), m32(20), m32(21),
    m32(20), m32(21), m32(22), m32(23), m32(24), m32(25),
    m32(24), m32(25), m32(26), m32(27), m32(28), m32(29),
    m32(28), m32(29), m32(30), m32(31), m32(32), m32( 1)
};

const uint64_t AVR8DES::PC2_TABLE[48] =
{
    m56(14), m56(17), m56(11), m56(24), m56( 1), m56( 5),
    m56( 3), m56(28), m56(15), m56( 6), m56(21), m56(10),
    m56(23), m56(19), m56(12), m56( 4), m56(26), m56( 8),
    m56(16), m56( 7), m56(27), m56(20), m56(13), m56( 2),
    m56(41), m56(52), m56(31), m56(37), m56(47), m56(55),
    m56(30), m56(40), m56(51), m56(45), m56(33), m56(48),
    m56(44), m56(49), m56(39), m56(56), m56(34), m56(53),
    m56(46), m56(42), m56(50), m56(36), m56(29), m56(32)
};

const uint64_t AVR8DES::PC1_TABLE[56] =
{
    m64(57), m64(49), m64(41), m64(33), m64(25), m64(17), m64( 9),
    m64( 1), m64(58), m64(50), m64(42), m64(34), m64(26), m64(18),
    m64(10), m64( 2), m64(59), m64(51), m64(43), m64(35), m64(27),
    m64(19), m64(11), m64( 3), m64(60), m64(52), m64(44), m64(36),
    m64(63), m64(55), m64(47), m64(39), m64(31), m64(23), m64(15),
    m64( 7), m64(62), m64(54), m64(46), m64(38), m64(30), m64(22),
    m64(14), m64( 6), m64(61), m64(53), m64(45), m64(37), m64(29),
    m64(21), m64(13), m64( 5), m64(28), m64(20), m64(12), m64( 4)
};

const uint64_t AVR8DES::IPI_TABLE[64] =
{
    m64(40), m64( 8), m64(48), m64(16), m64(56), m64(24), m64(64), m64(32),
    m64(39), m64( 7), m64(47), m64(15), m64(55), m64(23), m64(63), m64(31),
    m64(38), m64( 6), m64(46), m64(14), m64(54), m64(22), m64(62), m64(30),
    m64(37), m64( 5), m64(45), m64(13), m64(53), m64(21), m64(61), m64(29),
    m64(36), m64( 4), m64(44), m64(12), m64(52), m64(20), m64(60), m64(28),
    m64(35), m64( 3), m64(43), m64(11), m64(51), m64(19), m64(59), m64(27),
    m64(34), m64( 2), m64(42), m64(10), m64(50), m64(18), m64(58), m64(26),
    m64(33), m64( 1), m64(41), m64( 9), m64(49), m64(17), m64(57), m64(25)
};

const uint64_t AVR8DES::IP_TABLE[64] =
{
    m64(58), m64(50), m64(42), m64(34), m64(26), m64(18), m64(10), m64( 2),
    m64(60), m64(52), m64(44), m64(36), m64(28), m64(20), m64(12), m64( 4),
    m64(62), m64(54), m64(46), m64(38), m64(30), m64(22), m64(14), m64( 6),
    m64(64), m64(56), m64(48), m64(40), m64(32), m64(24), m64(16), m64( 8),
    m64(57), m64(49), m64(41), m64(33), m64(25), m64(17), m64( 9), m64( 1),
    m64(59), m64(51), m64(43), m64(35), m64(27), m64(19), m64(11), m64( 3),
    m64(61), m64(53), m64(45), m64(37), m64(29), m64(21), m64(13), m64( 5),
    m64(63), m64(55), m64(47), m64(39), m64(31), m64(23), m64(15), m64( 7)
};

const int AVR8DES::R_TABLE[16] =
{
    // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 // - Iteration number
       1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 // - Number of rotations
};

const uint32_t AVR8DES::S_TABLE[8][4][16] =
{
    {
        /*   0        1        2        3                 5        6        7  ←Col      : S₁ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₁ */
        {sb(0xE), sb(0x4), sb(0xD), sb(0x1), sb(0x2), sb(0xF), sb(0xB), sb(0x8), /*  0   : S₁ */
         sb(0x3), sb(0xA), sb(0x6), sb(0xC), sb(0x5), sb(0x9), sb(0x0), sb(0x7)},
        {sb(0x0), sb(0xF), sb(0x7), sb(0x4), sb(0xE), sb(0x2), sb(0xD), sb(0x1), /*  1   : S₁ */
         sb(0xA), sb(0x6), sb(0xC), sb(0xB), sb(0x9), sb(0x5), sb(0x3), sb(0x8)},
        {sb(0x4), sb(0x1), sb(0xE), sb(0x8), sb(0xD), sb(0x6), sb(0x2), sb(0xB), /*  2   : S₁ */
         sb(0xF), sb(0xC), sb(0x9), sb(0x7), sb(0x3), sb(0xA), sb(0x5), sb(0x0)},
        {sb(0xF), sb(0xC), sb(0x8), sb(0x2), sb(0x4), sb(0x9), sb(0x1), sb(0x7), /*  3   : S₁ */
         sb(0x5), sb(0xB), sb(0x3), sb(0xE), sb(0xA), sb(0x0), sb(0x6), sb(0xD)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₂ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₂ */
        {sb(0x9), sb(0x7), sb(0x2), sb(0xD), sb(0xC), sb(0x0), sb(0x5), sb(0xA), /*  0   : S₂ */
         sb(0xF), sb(0x1), sb(0x8), sb(0xE), sb(0x6), sb(0xB), sb(0x3), sb(0x4)},
        {sb(0xC), sb(0x0), sb(0x1), sb(0xA), sb(0x6), sb(0x9), sb(0xB), sb(0x5), /*  1   : S₂ */
         sb(0x3), sb(0xD), sb(0x4), sb(0x7), sb(0xF), sb(0x2), sb(0x8), sb(0xE)},
        {sb(0x5), sb(0x8), sb(0xC), sb(0x6), sb(0x9), sb(0x3), sb(0x2), sb(0xF), /*  2   : S₂ */
         sb(0x0), sb(0xE), sb(0x7), sb(0xB), sb(0xA), sb(0x4), sb(0xD), sb(0x1)},
        {sb(0xB), sb(0x6), sb(0x7), sb(0xC), sb(0x0), sb(0x5), sb(0xE), sb(0x9), /*  3   : S₂ */
         sb(0xD), sb(0x8), sb(0xA), sb(0x1), sb(0x3), sb(0xF), sb(0x4), sb(0x2)}

    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₃ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₃ */
        {sb(0x1), sb(0xD), sb(0xC), sb(0x7), sb(0xB), sb(0x4), sb(0x2), sb(0x8), /*  0   : S₃ */
         sb(0xA), sb(0x0), sb(0x9), sb(0xE), sb(0x6), sb(0x3), sb(0xF), sb(0x5)},
        {sb(0x2), sb(0x8), sb(0x5), sb(0xE), sb(0xC), sb(0xB), sb(0xF), sb(0x1), /*  1   : S₃ */
         sb(0xD), sb(0x7), sb(0x0), sb(0x9), sb(0x3), sb(0x4), sb(0x6), sb(0xA)},
        {sb(0xB), sb(0x1), sb(0x2), sb(0xC), sb(0x5), sb(0xA), sb(0xE), sb(0x7), /*  2   : S₃ */
         sb(0xD), sb(0x6), sb(0x4), sb(0x9), sb(0x8), sb(0xF), sb(0x3), sb(0x0)},
        {sb(0x4), sb(0xF), sb(0xE), sb(0x3), sb(0xB), sb(0x5), sb(0x2), sb(0xC), /*  3   : S₃ */
         sb(0x1), sb(0xA), sb(0xD), sb(0x0), sb(0x6), sb(0x9), sb(0x8), sb(0x7)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₄ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₄ */
        {sb(0x1), sb(0x2), sb(0x8), sb(0x5), sb(0xB), sb(0xC), sb(0x4), sb(0xF), /*  0   : S₄ */
         sb(0x7), sb(0xD), sb(0xE), sb(0x3), sb(0x0), sb(0x6), sb(0x9), sb(0xA)},
        {sb(0x4), sb(0x7), sb(0x2), sb(0xC), sb(0x1), sb(0xA), sb(0xE), sb(0x9), /*  1   : S₄ */
         sb(0xD), sb(0x8), sb(0xB), sb(0x5), sb(0x6), sb(0xF), sb(0x0), sb(0x3)},
        {sb(0xF), sb(0x1), sb(0x3), sb(0xE), sb(0x5), sb(0x2), sb(0x8), sb(0x4), /*  2   : S₄ */
         sb(0xA), sb(0x6), sb(0x9), sb(0x0), sb(0xC), sb(0xB), sb(0x7), sb(0xD)},
        {sb(0x9), sb(0x4), sb(0x5), sb(0xB), sb(0xC), sb(0x7), sb(0x2), sb(0xE), /*  3   : S₄ */
         sb(0x3), sb(0xF), sb(0x0), sb(0x6), sb(0xA), sb(0x1), sb(0xD), sb(0x8)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₅ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₅ */
        {sb(0x2), sb(0xC), sb(0x4), sb(0x1), sb(0x7), sb(0xA), sb(0xB), sb(0x6), /*  0   : S₅ */
         sb(0x8), sb(0x5), sb(0x3), sb(0xF), sb(0xD), sb(0x0), sb(0xE), sb(0x9)},
        {sb(0xE), sb(0xB), sb(0x2), sb(0xC), sb(0x4), sb(0x7), sb(0xD), sb(0x1), /*  1   : S₅ */
         sb(0x5), sb(0x0), sb(0xF), sb(0xA), sb(0x3), sb(0x9), sb(0x8), sb(0x6)},
        {sb(0x4), sb(0x2), sb(0x1), sb(0xB), sb(0xA), sb(0xD), sb(0x7), sb(0x8), /*  2   : S₅ */
         sb(0xF), sb(0x9), sb(0xC), sb(0x5), sb(0x6), sb(0x3), sb(0x0), sb(0xE)},
        {sb(0xB), sb(0x8), sb(0xC), sb(0x7), sb(0x1), sb(0xE), sb(0x2), sb(0xD), /*  3   : S₅ */
         sb(0x6), sb(0xF), sb(0x0), sb(0x9), sb(0xA), sb(0x4), sb(0x5), sb(0x3)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₆ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₆ */
        {sb(0xC), sb(0x1), sb(0xA), sb(0xF), sb(0x9), sb(0x2), sb(0x6), sb(0x8), /*  0   : S₆ */
         sb(0x0), sb(0xD), sb(0x3), sb(0x4), sb(0xE), sb(0x7), sb(0x5), sb(0xB)},
        {sb(0xA), sb(0xF), sb(0x4), sb(0x2), sb(0x7), sb(0xC), sb(0x9), sb(0x5), /*  1   : S₆ */
         sb(0x6), sb(0x1), sb(0xD), sb(0xE), sb(0x0), sb(0xB), sb(0x3), sb(0x8)},
        {sb(0x9), sb(0xE), sb(0xF), sb(0x5), sb(0x2), sb(0x8), sb(0xC), sb(0x3), /*  2   : S₆ */
         sb(0x7), sb(0x0), sb(0x4), sb(0xA), sb(0x1), sb(0xD), sb(0xB), sb(0x6)},
        {sb(0x4), sb(0x3), sb(0x2), sb(0xC), sb(0x9), sb(0x5), sb(0xF), sb(0xA), /*  3   : S₆ */
         sb(0xB), sb(0xE), sb(0x1), sb(0x7), sb(0x6), sb(0x0), sb(0x8), sb(0xD)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₇ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₇ */
        {sb(0x4), sb(0xB), sb(0x2), sb(0xE), sb(0xF), sb(0x0), sb(0x8), sb(0xD), /*  0   : S₇ */
         sb(0x3), sb(0xC), sb(0x9), sb(0x7), sb(0x5), sb(0xA), sb(0x6), sb(0x1)},
        {sb(0xD), sb(0x0), sb(0xB), sb(0x7), sb(0x4), sb(0x9), sb(0x1), sb(0xA), /*  1   : S₇ */
         sb(0xE), sb(0x3), sb(0x5), sb(0xC), sb(0x2), sb(0xF), sb(0x8), sb(0x6)},
        {sb(0x1), sb(0x4), sb(0xB), sb(0xD), sb(0xC), sb(0x3), sb(0x7), sb(0xE), /*  2   : S₇ */
         sb(0xA), sb(0xF), sb(0x6), sb(0x8), sb(0x0), sb(0x5), sb(0x9), sb(0x2)},
        {sb(0x6), sb(0xB), sb(0xD), sb(0x8), sb(0x1), sb(0x4), sb(0xA), sb(0x7), /*  3   : S₇ */
         sb(0x9), sb(0x5), sb(0x0), sb(0xF), sb(0xE), sb(0x2), sb(0x3), sb(0xC)}
    },{
        /*   0        1        2        3                 5        6        7  ←Col      : S₈ */
        /*   8        9       10       11       1        13       14       15  ←Col Row↓ : S₈ */
        {sb(0xD), sb(0x2), sb(0x8), sb(0x4), sb(0x6), sb(0xF), sb(0xB), sb(0x1), /*  0   : S₈ */
         sb(0xA), sb(0x9), sb(0x3), sb(0xE), sb(0x5), sb(0x0), sb(0xC), sb(0x7)},
        {sb(0x1), sb(0xF), sb(0xD), sb(0x8), sb(0xA), sb(0x3), sb(0x7), sb(0x4), /*  1   : S₈ */
         sb(0xC), sb(0x5), sb(0x6), sb(0xB), sb(0x0), sb(0xE), sb(0x9), sb(0x2)},
        {sb(0x7), sb(0xB), sb(0x4), sb(0x1), sb(0x9), sb(0xC), sb(0xE), sb(0x2), /*  2   : S₈ */
         sb(0x0), sb(0x6), sb(0xA), sb(0xD), sb(0xF), sb(0x3), sb(0x5), sb(0x8)},
        {sb(0x2), sb(0x1), sb(0xE), sb(0x7), sb(0x4), sb(0xA), sb(0x8), sb(0xD), /*  3   : S₈ */
         sb(0xF), sb(0xC), sb(0x9), sb(0x0), sb(0x3), sb(0x5), sb(0x6), sb(0xB)}
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

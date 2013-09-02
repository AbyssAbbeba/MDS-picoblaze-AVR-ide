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
 * @ingroup DisasmPicoBlaze
 * @file DAsmPicoBlazeOPCodes.h
 */
// =============================================================================

#ifndef DASMPICOBLAZEOPCODES_H
#define DASMPICOBLAZEOPCODES_H

/**
 * @brief
 * @ingroup DisasmPicoBlaze
 * @namespace DAsmPicoBlazeOPCodes
 */
namespace DAsmPicoBlazeOPCodes
{
    /**
     * @brief
     */
    enum Kcpsm1CPLD
    {
        KCPSM1CPLD_LOAD_SX_KK   = 0x00,
        KCPSM1CPLD_AND_SX_KK    = 0x01,
        KCPSM1CPLD_OR_SX_KK     = 0x02,
        KCPSM1CPLD_XOR_SX_KK    = 0x03,
        KCPSM1CPLD_ADD_SX_KK    = 0x04,
        KCPSM1CPLD_ADDCY_SX_KK  = 0x05,
        KCPSM1CPLD_SUB_SX_KK    = 0x06,
        KCPSM1CPLD_SUBCY_SX_KK  = 0x07,
        KCPSM1CPLD_LOAD_SX_SY   = 0x08,
        KCPSM1CPLD_AND_SX_SY    = 0x09,
        KCPSM1CPLD_OR_SX_SY     = 0x0A,
        KCPSM1CPLD_XOR_SX_SY    = 0x0B,
        KCPSM1CPLD_ADD_SX_SY    = 0x0C,
        KCPSM1CPLD_ADDCY_SX_SY  = 0x0D,
        KCPSM1CPLD_SUB_SX_SY    = 0x0E,
        KCPSM1CPLD_SUBCY_SX_SY  = 0x0F,
        KCPSM1CPLD_INPUT_SX_PP  = 0x10,
        KCPSM1CPLD_OUTPUT_SX_PP = 0x11,
        KCPSM1CPLD_RETURN       = 0x12,
        KCPSM1CPLD_SHIFT_ROTATE = 0x14,
        KCPSM1CPLD_RETURNI      = 0x16,
        KCPSM1CPLD_INPUT_SX_SY  = 0x18,
        KCPSM1CPLD_OUTPUT_SX_SY = 0x19,
        KCPSM1CPLD_JUMP         = 0x1A,
        KCPSM1CPLD_CALL         = 0x1B,
        KCPSM1CPLD_INTERRUPT    = 0x1E
    };

    /**
     * @brief
     */
    enum Kcpsm1
    {
        KCPSM1_LOAD_SX_KK       = 0x0,
        KCPSM1_AND_SX_KK        = 0x1,
        KCPSM1_OR_SX_KK         = 0x2,
        KCPSM1_XOR_SX_KK        = 0x3,
        KCPSM1_ADD_SX_KK        = 0x4,
        KCPSM1_ADDCY_SX_KK      = 0x5,
        KCPSM1_SUB_SX_KK        = 0x6,
        KCPSM1_SUBCY_SX_KK      = 0x7,
        KCPSM1_1000             = 0x8,
        KCPSM1_1001             = 0x9,
        KCPSM1_INPUT_SX_PP      = 0xA,
        KCPSM1_INPUT_SX_SY      = 0xB,
        KCPSM1_1100             = 0xC,
        KCPSM1_1101             = 0xD,
        KCPSM1_OUTPUT_SX_PP     = 0xE,
        KCPSM1_OUTPUT_SX_SY     = 0xF
    };

    /**
     * @brief
     */
    enum Kcpsm2
    {
        KCPSM2_LOAD_SX_KK       = 0x00,
        KCPSM2_AND_SX_KK        = 0x01,
        KCPSM2_OR_SX_KK         = 0x02,
        KCPSM2_XOR_SX_KK        = 0x03,
        KCPSM2_ADD_SX_KK        = 0x04,
        KCPSM2_ADDCY_SX_KK      = 0x05,
        KCPSM2_SUB_SX_KK        = 0x06,
        KCPSM2_SUBCY_SX_KK      = 0x07,
        KCPSM2_LOAD_SX_SY       = 0x08,
        KCPSM2_AND_SX_SY        = 0x09,
        KCPSM2_OR_SX_SY         = 0x0A,
        KCPSM2_XOR_SX_SY        = 0x0B,
        KCPSM2_ADD_SX_SY        = 0x0C,
        KCPSM2_ADDCY_SX_SY      = 0x0D,
        KCPSM2_SUB_SX_SY        = 0x0E,
        KCPSM2_SUBCY_SX_SY      = 0x0F,
        KCPSM2_INPUT_SX_PP      = 0x10,
        KCPSM2_OUTPUT_SX_PP     = 0x11,
        KCPSM2_RETURN           = 0x12,
        KCPSM2_SHIFT_ROTATE     = 0x14,
        KCPSM2_RETURNI          = 0x16,
        KCPSM2_INPUT_SX_SY      = 0x18,
        KCPSM2_OUTPUT_SX_SY     = 0x19,
        KCPSM2_JUMP             = 0x1A,
        KCPSM2_CALL             = 0x1B,
        KCPSM2_DIS_ENA_INT      = 0x1E
    };

    /**
     * @brief
     */
    enum Kcpsm3
    {
        KCPSM3_LOAD             = 0x00,
        KCPSM3_INPUT            = 0x02,
        KCPSM3_FETCH            = 0x03,
        KCPSM3_AND              = 0x05,
        KCPSM3_OR               = 0x06,
        KCPSM3_XOR              = 0x07,
        KCPSM3_TEST             = 0x09,
        KCPSM3_COMPARE          = 0x0A,
        KCPSM3_ADD              = 0x0C,
        KCPSM3_ADDCY            = 0x0D,
        KCPSM3_SUB              = 0x0E,
        KCPSM3_SUBCY            = 0x0F,
        KCPSM3_SHIFT_ROTATE     = 0x10,
        KCPSM3_RETURN           = 0x15,
        KCPSM3_OUTPUT           = 0x16,
        KCPSM3_STORE            = 0x17,
        KCPSM3_CALL             = 0x18,
        KCPSM3_JUMP             = 0x1A,
        KCPSM3_RETURNI          = 0x1C,
        KCPSM3_DIS_ENA_INT      = 0x1E
    };

    /**
     * @brief
     */
    enum Kcpsm6
    {
        KCPSM6_LOAD_SX_SY       = 0x00,
        KCPSM6_LOAD_SX_KK       = 0x01,
        KCPSM6_STAR_SX_SY       = 0x16,
        KCPSM6_AND_SX_SY        = 0x02,
        KCPSM6_AND_SX_KK        = 0x03,
        KCPSM6_OR_SX_SY         = 0x04,
        KCPSM6_OR_SX_KK         = 0x05,
        KCPSM6_XOR_SX_SY        = 0x06,
        KCPSM6_XOR_SX_KK        = 0x07,
        KCPSM6_ADD_SX_SY        = 0x10,
        KCPSM6_ADD_SX_KK        = 0x11,
        KCPSM6_ADDCY_SX_SY      = 0x12,
        KCPSM6_ADDCY_SX_KK      = 0x13,
        KCPSM6_SUB_SX_SY        = 0x18,
        KCPSM6_SUB_SX_KK        = 0x19,
        KCPSM6_SUBCY_SX_SY      = 0x1A,
        KCPSM6_SUBCY_SX_KK      = 0x1B,
        KCPSM6_TEST_SX_SY       = 0x0C,
        KCPSM6_TEST_SX_KK       = 0x0D,
        KCPSM6_TESTCY_SX_SY     = 0x0E,
        KCPSM6_TESTCY_SX_KK     = 0x0F,
        KCPSM6_COMPARE_SX_SY    = 0x1C,
        KCPSM6_COMPARE_SX_KK    = 0x1D,
        KCPSM6_COMPARECY_SX_SY  = 0x1E,
        KCPSM6_COMPARECY_SX_KK  = 0x1F,
        KCPSM6_SHIFT_ROTATE     = 0x14,
        KCPSM6_REGBANK          = 0x37,
        KCPSM6_INPUT_SX_SY      = 0x08,
        KCPSM6_INPUT_SX_PP      = 0x09,
        KCPSM6_OUTPUT_SX_SY     = 0x2C,
        KCPSM6_OUTPUT_SX_PP     = 0x2D,
        KCPSM6_OUTPUTK_KK_P     = 0x2B,
        KCPSM6_STORE_SX_SY      = 0x2E,
        KCPSM6_STORE_SX_SS      = 0x2F,
        KCPSM6_FETCH_SX_SY      = 0x0A,
        KCPSM6_FETCH_SX_SS      = 0x0B,
        KCPSM6_DIS_ENA_INT      = 0x28,
        KCPSM6_RETURNI          = 0x29,
        KCPSM6_JUMP_AAA         = 0x22,
        KCPSM6_JUMP_Z_AAA       = 0x32,
        KCPSM6_JUMP_NZ_AAA      = 0x36,
        KCPSM6_JUMP_C_AAA       = 0x3A,
        KCPSM6_JUMP_NC_AAA      = 0x3E,
        KCPSM6_JUMP_SX_SY       = 0x26,
        KCPSM6_CALL_AAA         = 0x20,
        KCPSM6_CALL_Z_AAA       = 0x30,
        KCPSM6_CALL_NZ_AAA      = 0x34,
        KCPSM6_CALL_C_AAA       = 0x38,
        KCPSM6_CALL_NC_AAA      = 0x3C,
        KCPSM6_CALL_SX_SY       = 0x24,
        KCPSM6_RETURN           = 0x25,
        KCPSM6_RETURN_Z         = 0x31,
        KCPSM6_RETURN_NZ        = 0x35,
        KCPSM6_RETURN_C         = 0x39,
        KCPSM6_RETURN_NC        = 0x3D,
        KCPSM6_LD_RET_SX_KK     = 0x21
    };
};

#endif // DASMPICOBLAZEOPCODES_H

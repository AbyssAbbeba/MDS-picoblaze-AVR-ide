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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeInstructionSet6.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEINSTRUCTIONSET6_H
#define ASMPICOBLAZEINSTRUCTIONSET6_H

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeInstructionSetN.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeInstructionSet6
 */
class AsmPicoBlazeInstructionSet6 : public AsmPicoBlazeInstructionSetN
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum OPER_BIT_SIZE
        {
            OBS_AAA = 12, ///< Address
            OBS_SXY =  4, ///< Register
            OBS_KK  =  8, ///< Constant
            OBS_PP  =  8, ///< 8-bit Port ID
            OBS_P   =  4, ///< 4-bit Port ID
            OBS_SS  =  8  ///< Scratch Pad RAM
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] stmt
         * @return
         */
        int resolveOPcode ( const CompilerStatement * stmt ) const;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getAAA ( const CompilerStatement * stmt,
                              int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getSXY ( const CompilerStatement * stmt,
                              int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getKK ( const CompilerStatement * stmt,
                             int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getPP ( const CompilerStatement * stmt,
                             int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getP ( const CompilerStatement * stmt,
                            int index ) const;

        /**
         * @brief
         * @param[in] stmt
         * @param[in] index
         * @return
         */
        unsigned int getSS ( const CompilerStatement * stmt,
                             int index ) const;
};

#endif // ASMPICOBLAZEINSTRUCTIONSET6_H

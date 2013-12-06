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
 * @file AsmPicoBlazeInstructionSet1.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEINSTRUCTIONSET1_H
#define ASMPICOBLAZEINSTRUCTIONSET1_H

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeInstructionSetN.h"

/**
 * @brief
 * @ingroup PicoBlazeAsm;
 * @class AsmPicoBlazeInstructionSet1
 */
class AsmPicoBlazeInstructionSet1 : public AsmPicoBlazeInstructionSetN
{
    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum OPER_BIT_SIZE
        {
            OBS_AAA = 8, ///< Address
            OBS_SXY = 4, ///< Register
            OBS_KK  = 8, ///< Constant
            OBS_PP  = 8  ///< Port ID
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
};

#endif // ASMPICOBLAZEINSTRUCTIONSET1_H

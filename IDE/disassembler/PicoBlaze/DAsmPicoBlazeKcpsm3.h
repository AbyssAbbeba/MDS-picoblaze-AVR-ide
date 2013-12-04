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
 * @file DAsmPicoBlazeKcpsm3.h
 */
// =============================================================================

#ifndef DASMPICOBLAZEKCPSM3_H
#define DASMPICOBLAZEKCPSM3_H

#include "DAsmPicoBlazeCore.h"

/**
 * @brief
 * @ingroup DisasmPicoBlaze
 * @class DAsmPicoBlazeKcpsm3
 */
class DAsmPicoBlazeKcpsm3 : public DAsmPicoBlazeCore
{
    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        virtual bool phase1 ( unsigned int code,
                              unsigned int addr );

        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        virtual void phase2 ( unsigned int code,
                              unsigned int addr );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_sy_kk ( std::string & line,
                        unsigned int code,
                        const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_sy_ss ( std::string & line,
                        unsigned int code,
                        const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_sy_pp ( std::string & line,
                        unsigned int code,
                        const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void shift_rotate ( std::string & line,
                            unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void inst_JUMP ( std::string & line,
                         unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void inst_CALL ( std::string & line,
                         unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void otherOpCodes ( std::string & line,
                            unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         * @param[in] cnd
         */
        void aaa ( std::string & line,
                   unsigned int code,
                   const char * inst,
                   const char * cnd = NULL );
};

#endif // DASMPICOBLAZEKCPSM3_H

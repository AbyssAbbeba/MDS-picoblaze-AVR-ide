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
 * @ingroup DisasmPicoBlaze
 * @file DAsmPicoBlazeKcpsm1CPLD.h
 */
// =============================================================================

#ifndef DASMPICOBLAZEKCPSM1CPLD_H
#define DASMPICOBLAZEKCPSM1CPLD_H

#include "DAsmPicoBlazeCore.h"

/**
 * @brief
 * @ingroup DisasmPicoBlaze
 * @class DAsmPicoBlazeKcpsm1CPLD
 */
class DAsmPicoBlazeKcpsm1CPLD : public DAsmPicoBlazeCore
{
    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        virtual bool phase1 ( unsigned int code,
                              unsigned int addr ) override;

        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        virtual void phase2 ( unsigned int code,
                              unsigned int addr ) override;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_sy ( std::string & line,
                     unsigned int code,
                     const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_at_sy ( std::string & line,
                        unsigned int code,
                        const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_pp ( std::string & line,
                     unsigned int code,
                     const char * inst );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         * @param[in] inst
         */
        void sx_kk ( std::string & line,
                     unsigned int code,
                     const char * inst );

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
                   const char * cnd = nullptr );

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
        void inst_RETURN ( std::string & line,
                           unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void inst_RETURNI ( std::string & line,
                            unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void inst_INTERRUPT ( std::string & line,
                              unsigned int code );
};

#endif // DASMPICOBLAZEKCPSM1CPLD_H

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
 * @file DAsmPicoBlazeKcpsm6.h
 */
// =============================================================================

#ifndef DASMPICOBLAZEKCPSM6_H
#define DASMPICOBLAZEKCPSM6_H

#include "DAsmPicoBlazeCore.h"

/**
 * @brief
 * @ingroup DisasmPicoBlaze
 * @class DAsmPicoBlazeKcpsm6
 */
class DAsmPicoBlazeKcpsm6 : public DAsmPicoBlazeCore
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DAsmPicoBlazeKcpsm6() : DAsmPicoBlazeCore ( IS_18b ) {}

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
        void sx_ss ( std::string & line,
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
         */
        void at_sx_sy ( std::string & line,
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
         * @param[in] inst
         */
        void kk_p ( std::string & line,
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
        void otherOpCodes ( std::string & line,
                            unsigned int code );
};

#endif // DASMPICOBLAZEKCPSM6_H

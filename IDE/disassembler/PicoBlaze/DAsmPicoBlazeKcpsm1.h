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
 * @file DAsmPicoBlazeKcpsm1.h
 */
// =============================================================================

#ifndef DASMPICOBLAZEKCPSM1_H
#define DASMPICOBLAZEKCPSM1_H

#include "DAsmPicoBlazeCore.h"

/**
 * @brief
 * @ingroup DisasmPicoBlaze
 * @class DAsmPicoBlazeKcpsm1
 */
class DAsmPicoBlazeKcpsm1 : public DAsmPicoBlazeCore
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DAsmPicoBlazeKcpsm1() : DAsmPicoBlazeCore ( IS_16b ) {}

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
        void inst_1000_and_1001 ( std::string & line,
                                  unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        void otherOpCodes ( std::string & line,
                            unsigned int code );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        inline void inst_JUMP ( std::string & line,
                                unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        inline void inst_CALL ( std::string & line,
                                unsigned int code );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] code
         */
        inline void inst_RETURN ( std::string & line,
                                  unsigned int code );
};

#endif // DASMPICOBLAZEKCPSM1_H

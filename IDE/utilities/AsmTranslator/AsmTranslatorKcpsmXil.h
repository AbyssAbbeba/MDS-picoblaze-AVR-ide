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
 * @ingroup AsmTranslator
 * @file AsmTranslatorKcpsmXil.h
 */
// =============================================================================

#ifndef ASMTRANSLATORKCPSMXIL_H
#define ASMTRANSLATORKCPSMXIL_H

// AsmTranslator header files.
#include "AsmTranslatorBase.h"

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorKcpsmXil
 */
class AsmTranslatorKcpsmXil : public AsmTranslatorBase
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] output
         * @param[in] line
         * @return
         */
        virtual bool process ( std::vector<std::string> & messages,
                               std::ostream & output,
                               const std::string & line );
};

#endif // ASMTRANSLATORKCPSMXIL_H
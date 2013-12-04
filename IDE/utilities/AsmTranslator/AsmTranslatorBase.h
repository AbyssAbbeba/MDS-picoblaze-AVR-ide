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
 * @file AsmTranslatorBase.h
 */
// =============================================================================

#ifndef ASMTRANSLATORBASE_H
#define ASMTRANSLATORBASE_H

// Standard header files.
#include <string>
#include <vector>

// Used for i18n only.
#include <QObject>

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorBase
 */
class AsmTranslatorBase
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] line
         * @return
         */
        virtual bool process ( std::vector<std::string> & messages,
                               std::string & line ) = 0;
};

#endif // ASMTRANSLATORBASE_H

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
 * @file AsmTranslator.h
 */
// =============================================================================

#ifndef ASMTRANSLATOR_H
#define ASMTRANSLATOR_H

// Standard header files.
#include <string>
#include <vector>

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslator
 */
class AsmTranslator
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Variant
        {
            V_KCPSM_XILINX
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~AsmTranslator();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] variant
         * @param[in,out] output
         * @param[in,out] input
         */
        bool translate ( Variant variant,
                         std::ostream & output,
                         std::istream & input );

        /**
         * @brief
         * @param[in] variant
         * @param[in,out] output
         * @param[in,out] input
         * @param[in] makeBackup
         * @return
         */
        bool translate ( Variant variant,
                         const std::string & output,
                         const std::string & input,
                         bool makeBackup = false );

        /**
         * @brief
         * @return
         */
        const std::vector<std::string> & getMessages() const;

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<std::string> m_messages;
};

#endif // ASMTRANSLATOR_H

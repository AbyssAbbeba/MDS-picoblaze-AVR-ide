// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup LicenseCertificate
 * @file LicenseCertificate.h
 */
// =============================================================================

#ifndef LICENSECERTIFICATE_H
#define LICENSECERTIFICATE_H

// Standard header files.
#include <string>
#include <fstream>

/**
 * @brief
 * @ingroup LicenseCertificate
 * @class LicenseCertificate
 */
class LicenseCertificate
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] certificate
         */
        LicenseCertificate ( std::ifstream & certificate );

        /**
         * @brief
         * @param[in] certificate
         */
        LicenseCertificate ( const std::string & certificate );

        /**
         * @brief
         */
        virtual ~LicenseCertificate() {}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        bool isValid() const
        {
            return m_isValid;
        }

    ////    Private Operations    ////
    public:
        /**
         * @brief
         * @param[in] certificate
         */
        void readAndVerifyCertificate ( const std::string & certificate );

    ////    Private Attributes    ////
    private:
        /// @brief
        bool m_isValid;
};

#endif // LICENSECERTIFICATE_H

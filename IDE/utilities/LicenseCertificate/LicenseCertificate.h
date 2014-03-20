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
    private:
        /**
         * @brief
         * @param[in] certificate
         */
        void loadCertificate ( const std::string & certificate );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[out] data
         * @param[out] size
         * @param[in] certificate
         * @return
         */
        inline bool inflate ( char * & data,
                              size_t & size,
                              const std::string & certificate );

        /**
         * @brief
         * @param[in] data
         * @param[in] size
         * @return
         */
        inline bool verify ( const char * data,
                             size_t size );

        /**
         * @brief
         * @param[in] data
         * @return
         */
        inline bool parseXML ( const char * data );

    ////    Private Attributes    ////
    private:
        /// @brief
        bool m_isValid;
};

#endif // LICENSECERTIFICATE_H

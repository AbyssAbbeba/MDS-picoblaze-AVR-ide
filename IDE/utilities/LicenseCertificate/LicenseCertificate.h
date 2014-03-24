// =============================================================================
/**
 * @brief
 * C++ Interface: Library for validating and reading license certificates.
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
#include <vector>
#include <fstream>

/**
 * @brief License certificate validation and parsing.
 * @ingroup LicenseCertificate
 * @class LicenseCertificate
 */
class LicenseCertificate
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief Load license certificate from file.
         * @param[in,out] certificate Binary file containing the certificate.
         */
        LicenseCertificate ( std::ifstream & certificate );

        /**
         * @brief Load license certificate from string.
         * @param[in] certificate Binary data containing the certificate.
         */
        LicenseCertificate ( const std::string & certificate );

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
         * @brief Decompress the certificate.
         * @param[out] data Inflated data will be stored here, the space will be automatically allocated.
         * @param[out] size Size of data stored in "data".
         * @param[in] certificate Binary data containing deflated certificate.
         * @return True on success, false otherwise (in that case parameters data and size will not be used).
         */
        inline bool inflate ( char * & data,
                              size_t & size,
                              const std::string & certificate );

        /**
         * @brief Verify digital signature stored along with the certificate.
         * @param[in] data Uncompressed content of the certificate.
         * @param[in] size Size of the uncompressed data.
         * @return True on success, false otherwise.
         */
        inline bool verify ( const char * data,
                             size_t size );

        /**
         * @brief Read and parse XML part of the certificate.
         * @param[in] data Uncompressed content of the certificate.
         * @return True on success, false otherwise.
         */
        inline bool parseXML ( const char * data );

    ////    Public Attributes    ////
    public:
        /// @brief
        bool m_isValid;

        /// @brief Version of license certificate data format.
        std::string m_version;

        /// @brief Unique user identifier of the licensee.
        unsigned int m_userid;

        /// @brief Unique license identifier.
        unsigned int m_licenseid;

        /// @brief List of products licensed to the licensee.
        std::vector<std::string> m_products;

        /// @brief Date when this certificated was legally issued in form of Unix style time stamp.
        unsigned int m_date;

        /// @brief Number of seconds for which the certificate remains valid since "m_date", 0 means indefinitely.
        unsigned int m_expiry;

        /// @brief Street line #1.
        std::string m_addrStreet1;

        /// @brief Street line #2.
        std::string m_addrstreet2;

        /// @brief Municipality (not necessarily a city).
        std::string m_addrcity;

        /// @brief Postal code.
        std::string m_addrpost;

        /// @brief The lowest level of state, i.e. a province of a republic or realm.
        std::string m_addrstate1;

        /// @brief Any middle level of state, i.e. a republic or realm as a part of some bigger country or union.
        std::string m_addrstate2;

        /// @brief The highest level of state, i.e. a country or a union of countries.
        std::string m_addrstate3;

        /// @brief Name of the licensee.
        std::string m_name;

        /// @brief Type of the licensee, e.g. "legal entity".
        std::string m_detailsType;

        /// @brief Contact email.
        std::string m_detailsContactEmail;

        /// @brief Contact address.
        std::string m_detailsContactAddress;

        /// @brief Contact phone number(s).
        std::string m_detailsContactPhone;

        /// @brief Department of the licensee to which the license was granted.
        std::string m_detailsDepartment;
};

#endif // LICENSECERTIFICATE_H

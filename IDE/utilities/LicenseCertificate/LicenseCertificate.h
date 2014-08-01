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
    ////    Public Datatypes & Attributes   ////
    public:
        /// @brief
        bool m_isValid;

        /// @brief Version of license certificate data format.
        std::string m_version;

        /// @brief Information of the licensed product and type of license.
        struct Product
        {
            /// Name of product covered by the license.
            std::string m_designation;

            /// Number of licenses issued.
            std::string m_licences;

            /// Product variant, one of {Commercial,Noncommercial}.
            std::string m_variant;

            /// Product grade, one of {Basic,Premium,Professional,Ultimate}.
            std::string m_grade;

            /// Processor architecture or similar.
            std::string m_target;
        } m_product;

        /// @brief Information about to whom the license was or has been granted.
        struct Licensee
        {
            /// Subject name
            std::string m_name;

            /// Type of subject, one of: {company,school,individual,other}.
            std::string m_type;

            /// Email address associated with the registered customer.
            std::string m_email;

            /// Contact phone (may be empty).
            std::string m_phone;

            /// @brief Address of the licensee.
            struct Address
            {
                /// Sovereign state (e.g. United States of America, Federal Republic of Germany, Russian Federation).
                std::string m_country;

                /// Dependent state (optional) (e.g. Republic of Texas, Freistaat Bayern, Novosibirsk Oblast)
                std::string m_state;

                /// City/town or other type of municipality (e.g. Dallas, München, Novosibirsk).
                std::string m_municipality;

                /// Street name.
                std::string m_street;

                /// House number or similar designation.
                std::string m_number;

                /// Postal code.
                std::string m_post;
            } m_address;
        } m_licensee;

        /// @brief Software details related to certificate file generation.
        struct System
        {
            /// Unix time stamp (seconds since 1970-01-01 00:00:00 UTC) - time of generation.
            std::string m_timestamp;

            /// Some Unique User ID from web page DB table(s) hashed with MD5 as hexadecimal string.
            std::string m_userid;

            /// Some Unique Order ID from web page DB table(s) hashed with MD5 as hexadecimal string.
            std::string m_orderid;
        } m_system;

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
};

#endif // LICENSECERTIFICATE_H

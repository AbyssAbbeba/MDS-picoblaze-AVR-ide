// =============================================================================
/**
 * @brief
 * C++ Implementation: Library for validating and reading license certificates.
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup LicenseCertificate
 * @file LicenseCertificate.cxx
 */
// =============================================================================

#include "LicenseCertificate.h"
#include "LicenseCertificateKey.h"

// Standard header files.
#ifndef NDEBUG
#  include <iostream>
#endif // NDEBUG

// Crypto++
#include <CryptoPP/rsa.h>
#include <CryptoPP/sha.h>
#include <CryptoPP/pssr.h>
#include <CryptoPP/gzip.h>
#include <CryptoPP/filters.h>

// Qt (mainly its XML parser).
#include <QStringList>
#include <QDomDocument>

LicenseCertificate::LicenseCertificate ( std::ifstream & certificate )
{
    int byte;
    std::string data;

    while ( EOF != ( byte = certificate.get() ) )
    {
        data.push_back ( (char)(byte) );
    }

    loadCertificate(data);
}

LicenseCertificate::LicenseCertificate ( const std::string & certificate )
{
    loadCertificate(certificate);
}

void LicenseCertificate::loadCertificate ( const std::string & certificate )
{
    char * data = nullptr;
    size_t size;

    m_isValid = ( inflate(data, size, certificate) && verify(data, size) && parseXML(data) );

    if ( nullptr != data )
    {
        delete [] data;
    }
}

inline bool LicenseCertificate::inflate ( char * & data,
                                          size_t & size,
                                          const std::string & certificate )
{
    using namespace CryptoPP;

    try
    {
        Gunzip unzipper;
        unzipper.Put ( (const byte*) certificate.data(), certificate.size() );
        unzipper.Flush(true);
        size = unzipper.MaxRetrievable();
        data = new char [ size ];
        unzipper.Get ( (byte*) data, size );
    }
    catch ( const Inflator::Err & )
    {
        #ifndef NDEBUG
            std::cerr << "Decompression error." << std::endl;
        #endif // NDEBUG

        return false;
    }

    return true;
}

inline bool LicenseCertificate::verify ( const char * data,
                                         size_t size )
{
    return true;
    using namespace CryptoPP;

    bool successul;
    RSA::PublicKey publicKey;

    publicKey.Load ( StringSource ( (const byte*) LicenseCertificateKey,
                                    sizeof(LicenseCertificateKey),
                                    true ) . Ref() );

    StringSource (
        (const byte*) data,
        size,
        true,
        new SignatureVerificationFilter (
            RSASS<PKCS1v15, SHA256>::Verifier ( publicKey ),
            new ArraySink (
                ( byte* ) ( &successul ),
                sizeof ( m_isValid )
            ),
            ( SignatureVerificationFilter::PUT_RESULT | SignatureVerificationFilter::SIGNATURE_AT_END )
        )
    );

    if ( false == successul )
    {
        #ifndef NDEBUG
            std::cerr << "Signature verification error." << std::endl;
        #endif // NDEBUG
        return false;
    }

    return true;
}

inline bool LicenseCertificate::parseXML ( const char * data )
{
    QDomDocument certificate;

    if ( false == certificate.setContent(QString(data)) )
    {
        return false;
    }

    // Parse tag <certificate>
    QDomElement tagCertificate = certificate.documentElement();
    if ( ( true == tagCertificate.isNull() ) || ( "certificate" != tagCertificate.tagName() ) )
    {
        return false;
    }
    m_version   = tagCertificate.attribute("version"    ).toStdString();
    m_date      = tagCertificate.attribute("date"       ).toUInt(nullptr, 16);
    m_userid    = tagCertificate.attribute("userid"     ).toUInt(nullptr, 16);
    m_expiry    = tagCertificate.attribute("expiry", "0").toUInt(nullptr, 16);
    m_licenseid = tagCertificate.attribute("licenseid"  ).toUInt(nullptr, 16);
    for ( const auto & product : tagCertificate.attribute("products").split(',') )
    {
        m_products.push_back(product.toStdString());
    }

    for ( QDomElement n = tagCertificate.firstChildElement();
          false == n.isNull();
          n = n.nextSiblingElement() )
    {
        if ( "address" == n.tagName() )
        {
            m_addrcity    = n.attribute("city"   ).toStdString();
            m_addrpost    = n.attribute("post"   ).toStdString();
            m_addrstate1  = n.attribute("state1" ).toStdString();
            m_addrstate2  = n.attribute("state2" ).toStdString();
            m_addrstate3  = n.attribute("state3" ).toStdString();
            m_addrStreet1 = n.attribute("street1").toStdString();
            m_addrstreet2 = n.attribute("street2").toStdString();
        }
        else if ( "name" == n.tagName() )
        {
            m_name = n.text().toStdString();
        }
        else if ( "details" == n.tagName() )
        {
            for ( QDomElement m = n.firstChildElement();
                  false == m.isNull();
                  m = m.nextSiblingElement() )
            {
                if ( "type" == m.tagName() )
                {
                    m_detailsType = m.text().toStdString();
                }
                else if ( "contact" == m.tagName() )
                {
                    m_detailsContactEmail   = m.attribute("email"  ).toStdString();
                    m_detailsContactPhone   = m.attribute("phone"  ).toStdString();
                    m_detailsContactAddress = m.attribute("address").toStdString();
                }
                else if ( "department" == m.tagName() )
                {
                    m_detailsDepartment = m.text().toStdString();
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

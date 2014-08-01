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
    m_version = tagCertificate.attribute("version").toStdString();

    for ( QDomElement n = tagCertificate.firstChildElement();
          false == n.isNull();
          n = n.nextSiblingElement() )
    {
        if ( "product" == n.tagName() )
        {
            for ( QDomElement m = n.firstChildElement();
                  false == m.isNull();
                  m = m.nextSiblingElement() )
            {
                if ( "designation" == m.tagName() )
                {
                    m_product.m_designation = m.text().toStdString();
                }
                else if ( "licences" == m.tagName() )
                {
                    m_product.m_licences = m.text().toStdString();
                }
                else if ( "variant" == m.tagName() )
                {
                    m_product.m_variant = m.text().toStdString();
                }
                else if ( "grade" == m.tagName() )
                {
                    m_product.m_grade = m.text().toStdString();
                }
                else if ( "target" == m.tagName() )
                {
                    m_product.m_target = m.text().toStdString();
                }
                else
                {
                    return false;
                }
            }
        }
        else if ( "licensee" == n.tagName() )
        {
            for ( QDomElement m = n.firstChildElement();
                  false == m.isNull();
                  m = m.nextSiblingElement() )
            {
                if ( "name" == m.tagName() )
                {
                    m_licensee.m_name = m.text().toStdString();
                }
                else if ( "type" == m.tagName() )
                {
                    m_licensee.m_type = m.text().toStdString();
                }
                else if ( "email" == m.tagName() )
                {
                    m_licensee.m_email = m.text().toStdString();
                }
                else if ( "phone" == m.tagName() )
                {
                    m_licensee.m_phone = m.text().toStdString();
                }
                else if ( "address" == m.tagName() )
                {
                    for ( QDomElement o = m.firstChildElement();
                          false == o.isNull();
                           o = o.nextSiblingElement() )
                    {
                        if ( "country" == o.tagName() )
                        {
                            m_licensee.m_address.m_country = o.text().toStdString();
                        }
                        else if ( "state" == o.tagName() )
                        {
                            m_licensee.m_address.m_state = o.text().toStdString();
                        }
                        else if ( "municipality" == o.tagName() )
                        {
                            m_licensee.m_address.m_municipality = o.text().toStdString();
                        }
                        else if ( "street" == o.tagName() )
                        {
                            m_licensee.m_address.m_street = o.text().toStdString();
                        }
                        else if ( "number" == o.tagName() )
                        {
                            m_licensee.m_address.m_number = o.text().toStdString();
                        }
                        else if ( "post" == o.tagName() )
                        {
                            m_licensee.m_address.m_post = o.text().toStdString();
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
        }
        else if ( "system" == n.tagName() )
        {
            for ( QDomElement m = n.firstChildElement();
                  false == m.isNull();
                  m = m.nextSiblingElement() )
            {
                if ( "timestamp" == m.tagName() )
                {
                    m_system.m_timestamp = m.text().toStdString();
                }
                else if ( "userid" == m.tagName() )
                {
                    m_system.m_userid = m.text().toStdString();
                }
                else if ( "orderid" == m.tagName() )
                {
                    m_system.m_orderid = m.text().toStdString();
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

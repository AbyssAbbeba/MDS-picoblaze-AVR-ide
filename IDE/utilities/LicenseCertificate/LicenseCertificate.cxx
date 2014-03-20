// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
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
#include <iostream>

// Crypto++
#include <CryptoPP/rsa.h>
#include <CryptoPP/sha.h>
#include <CryptoPP/pssr.h>
#include <CryptoPP/gzip.h>
#include <CryptoPP/filters.h>

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
    char * data;
    size_t size;

    m_isValid = ( inflate(data, size, certificate) && verify(data, size) && parseXML(data) );
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
    catch ( const Inflator::Err & e )
    {
        std::cerr << "Decompression error: " << e.what() << std::endl;
        delete [] data;
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
        delete [] data;
    }

    return successul;
}

inline bool LicenseCertificate::parseXML ( const char * data )
{
    return true;
}

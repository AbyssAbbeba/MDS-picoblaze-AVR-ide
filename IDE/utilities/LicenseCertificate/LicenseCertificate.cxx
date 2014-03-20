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

    readAndVerifyCertificate(data);
}

LicenseCertificate::LicenseCertificate ( const std::string & certificate )
{
    readAndVerifyCertificate(certificate);
}

void LicenseCertificate::readAndVerifyCertificate ( const std::string & certificate )
{
    using namespace CryptoPP;

    size_t size;
    byte * data;

    m_isValid = false;

    try
    {
        Gunzip unzipper;
        unzipper.Put ( (const byte*) certificate.data(), certificate.size() );
        unzipper.Flush(true);
        size = unzipper.MaxRetrievable();
        data = new byte [ size ];
        unzipper.Get ( data, size );
    }
    catch ( const Inflator::Err & e )
    {
        std::cerr << "Decompression error: " << e.what() << std::endl;
        return;
    }

    RSA::PublicKey publicKey;
    publicKey.Load ( StringSource ( (const byte*) LicenseCertificateKey,
                                    sizeof(LicenseCertificateKey),
                                    true ) . Ref() );

    StringSource (
        data,
        size,
        true,
        new SignatureVerificationFilter (
            RSASS<PKCS1v15, SHA256>::Verifier ( publicKey ),
            new ArraySink (
                ( byte* ) ( &m_isValid ),
                sizeof ( m_isValid )
            ),
            ( SignatureVerificationFilter::PUT_RESULT | SignatureVerificationFilter::SIGNATURE_AT_END )
        )
    );
}


#include "LicenseCertificate.h"
#if 0
#include "rsa.h"
using CryptoPP::RSA;
using CryptoPP::RSASS;
using CryptoPP::InvertibleRSAFunction;
#include "base64.h"
#include "pssr.h"
using CryptoPP::PSS;

#include "sha.h"
using CryptoPP::SHA1;

#include "files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "filters.h"
using CryptoPP::ArraySink;
using CryptoPP::SignerFilter;
using CryptoPP::SignatureVerificationFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "secblock.h"
using CryptoPP::SecByteBlock;

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;
#endif
int main ( int argc, char ** argv )
{    /*try
    {*/
#if 0
        AutoSeededRandomPool rng;

// RSA::PrivateKey privateKey;
// RSA::PublicKey publicKey;

// publicKey.Load(StringSource(pub_key,true,new CryptoPP::Base64Decoder()).Ref());
// privateKey.Load(StringSource(priv_key,true,new CryptoPP::Base64Decoder()).Ref());

//         CryptoPP::HexDecoder decoder;
//         decoder.Put( (byte*)pub_key.c_str(), pub_key.size() );
//         decoder.MessageEnd();
//         publicKey.Load( decoder );
//
//         CryptoPP::HexDecoder decoder2;
//         decoder2.Put( (byte*)priv_key.c_str(), priv_key.size() );
//         decoder2.MessageEnd();
//         privateKey.Load( decoder2 );

/*
  openssl genrsa -out LicenseKey.pem 2048
  openssl rsa -in LicenseKey.pem -pubout -outform DER -out LicenseKeyPublic.der
  openssl pkcs8 -nocrypt -in LicenseKey.pem -inform PEM -topk8 -outform DER -out LicenseKey.der

  openssl dgst -sha256 -sign private.pem < data.txt

  base64 rsa-openssl.pub.der  | gawk '{print "\"" $0 "\""}'
  base64 rsa-openssl.priv.der | gawk '{print "\"" $0 "\""}'
 */

std::string priv_key =
"MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAOKeg45nIqTds7hqVaonY8kZ5gX3"
"LSfXsANReIGyVfUMJxplJL2Z5PwiqnqReEk5kaIohgElfoKOw/I4CqO8T0U7VvOFo+vzOhtbxrbl"
"I4LZ21LUvEwlitdN1guQjIcFr7qnoWoRyVDaOTLoHt8K8aeXGlZiDO+k+e5VESKe8jBbAgMBAAEC"
"gYB0Ty7uE3jwbOkkqU62Oek4+ZXZUfdXD5hC27mht0nurwISxf6JYUbpMVGV/rthua5JL8s3I2s+"
"zs86rhCn4Ko9wuefEZ52K7sUNHw4sCk9r0PhdV5+bNZNSYAVCj+Pc+35EL1ls5PF/0AqpV4qesax"
"Xqfm8psMrmuAHmvVUHpoIQJBAPP9YJxT8Ze26Okupt1u8O9MsPI/k6382Qp9G6CanbUFrzPQGpmR"
"rn1ZJuP+2FsrfnhQW6HKdetLEEdDnybtuo0CQQDtxj4OQl8MUboL712ohijxZSHcJ8EOo/zdOq/8"
"8Jbgtl3nRXbqw+f+osewZitVYNAXzscCH9YMq74qNP5GnBCHAkBCc1qxmOcuk8FhaLRHkKSL+A4l"
"HA2jkD6obaRtnE6hc7fg26ey9YtZVkOSSuEwC2t5ZyFdsh9lgt8fGZqp5ostAkB4U2Mh+ip5ZH5e"
"/dIKr2HWzxP0glWrYAdduQ5IVUQ1EXSxzbBhG6SycpPVS87QFP8ZnTWOeKfsn5jLaSfoeZ/PAkA7"
"W1ZO3fw70aGj3ETXsTwKIMA9o7DyIJIU3ovl+rqddk0a+mUl+9iBoyzfuNmIOmgBBnpaL5ccy6ph"
"AlwPdhGo";


// std::string pub_key =
// "MIIBojANBgkqhkiG9w0BAQEFAAOCAY8AMIIBigKCAYEAxe/ZG+h4zvEyywcrZm8buhk01GXr9ZOM"
// "L68TkSpjoHzdLbfk4rCv9+uWpGjz5aaowJ7O8KeeHAPO3Sf/3rGEOSRfobCuniq3wn7ED/wtyo/H"
// "fiROpZiWjCe4ZG06LD/IeF75WOs0dCOvtfozDLRBpV/OutBnOzeTkQaAd93RnCTeyUMlquO7Ts9M"
// "kBbJ/WmkzhoDGjaeqoXEMzy+CKzEqTfiyTolB5BrbzJGrz+MFdAH+jkutJ3C0ALM6uZAYLHWX4jI"
// "d80IDKV3/EvE9miibnIThvwFPXJvK9zB8c1Zh47VZYnD83FY2ieJp6pDDu4DInI6Znse9auQnQtX"
// "/hDRL9yKkm1KsSBFzruKbHVQwbEhmxWZlR0e+nFa/N9490vxUSMKFnFhzX1ODfvQt/ylevcvcHEy"
// "mWk+nO4R5fwKK79PDSP1t5/Ry6QR0ve6pVDmTmxJMBESqvp1biUtvyvlXTMDh93fRUYEIyTGiXgY"
// "9HYSlkGJYitjy66Kz496K55dAgMBAAE=";

// cat rsa-openssl.pub.der | hexdump -C | colrm 1 10 | colrm 49 68 | colrm 24 24 | gawk '{for(i=1;i<=NF;i++){printf("0x" $i ", ")}} END {print("")}'

constexpr size_t length = 162;
byte data [] = {
    0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01,
    0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xe2, 0x9e, 0x83,
    0x8e, 0x67, 0x22, 0xa4, 0xdd, 0xb3, 0xb8, 0x6a, 0x55, 0xaa, 0x27, 0x63, 0xc9, 0x19, 0xe6, 0x05,
    0xf7, 0x2d, 0x27, 0xd7, 0xb0, 0x03, 0x51, 0x78, 0x81, 0xb2, 0x55, 0xf5, 0x0c, 0x27, 0x1a, 0x65,
    0x24, 0xbd, 0x99, 0xe4, 0xfc, 0x22, 0xaa, 0x7a, 0x91, 0x78, 0x49, 0x39, 0x91, 0xa2, 0x28, 0x86,
    0x01, 0x25, 0x7e, 0x82, 0x8e, 0xc3, 0xf2, 0x38, 0x0a, 0xa3, 0xbc, 0x4f, 0x45, 0x3b, 0x56, 0xf3,
    0x85, 0xa3, 0xeb, 0xf3, 0x3a, 0x1b, 0x5b, 0xc6, 0xb6, 0xe5, 0x23, 0x82, 0xd9, 0xdb, 0x52, 0xd4,
    0xbc, 0x4c, 0x25, 0x8a, 0xd7, 0x4d, 0xd6, 0x0b, 0x90, 0x8c, 0x87, 0x05, 0xaf, 0xba, 0xa7, 0xa1,
    0x6a, 0x11, 0xc9, 0x50, 0xda, 0x39, 0x32, 0xe8, 0x1e, 0xdf, 0x0a, 0xf1, 0xa7, 0x97, 0x1a, 0x56,
    0x62, 0x0c, 0xef, 0xa4, 0xf9, 0xee, 0x55, 0x11, 0x22, 0x9e, 0xf2, 0x30, 0x5b, 0x02, 0x03, 0x01,
    0x00, 0x01
};

        CryptoPP::Base64Decoder decoder;
        decoder.Put( (byte*)priv_key.c_str(), priv_key.size() );
        decoder.MessageEnd();
        RSA::PrivateKey privateKey;
        privateKey.Load(decoder);

//         CryptoPP::Base64Decoder _decoder;
//         _decoder.Put( (byte*)pub_key.c_str(), pub_key.size() );
//         _decoder.MessageEnd();

        RSA::PublicKey publicKey;
        publicKey.Load(StringSource( data, length, true ).Ref());

        // Message
        string message = "Yoda said, Do or Do Not. There is not try.\n";
        string signature;

        ////////////////////////////////////////////////
        // Sign and Encode
        RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer( privateKey );
        StringSource( message, true,
            new SignerFilter( rng, signer,
                new StringSink( signature )
            ) // SignerFilter
        ); // StringSource

        ////////////////////////////////////////////////
        // Verify and Recover
bool result = false;

// std::cout << signature;
        StringSource( message+signature, true,
            new SignatureVerificationFilter(
                RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA256>::Verifier( publicKey ),
        new ArraySink((byte*)(&result), sizeof(result)),
                SignatureVerificationFilter::PUT_RESULT | SignatureVerificationFilter::SIGNATURE_AT_END
            ) // SignatureVerificationFilter
        ); // StringSource

        if ( true == result )
        {
//             cout << "Verified signature on message" << endl;
//             cout << signature;
        }
#if 1
        std::ifstream f(argv[1], std::ios_base::binary);
        LicenseCertificate crt(f);
        cout<<std::boolalpha<<"verified="<<crt.isValid()<<"\n";
#endif
//     }
//     catch( CryptoPP::Exception& e )
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
#endif
    return 0;
}
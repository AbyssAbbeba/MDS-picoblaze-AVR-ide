
== Licence certifate specification ==

=== Foreword ===
This simple specification assumes basic user-level knowledge of contemporary asymmetric cryptography, and general
knowledge of computer usage above common user level; and it's intended mainly for software developers.

=== File format ===
MDS licence certifate is a gzip compressed digitally signed XML file containing information about licensee.

+-gzip----------+  <-- Entire certificate is deflated using GZIP Compression (RFC 1952).
| +-----------+ |
| |    XML    | |  <-- XML data, see LicenseCertificate.dtd and CertificateExample.xml
| +-----------+ |
| |     0     | |  <-- Binary 0 (NULL), this is mandatory.
| +-----------+ |
| | signature | |  <-- RSA digital signature (everything is subject to the crypto-hash generation including the NULL).
| +-----------+ |
+---------------+

==== Recommendations ====
File base name: complete product name
File name extension: .cert


=== Cryptographic security ===

==== Specification of the applied means of cryptography ====
Cryptosystem: RSA
Cryptography standard: PKCS #1
Signature scheme: RSASSA-PKCS1-v1_5
Cryptographic hash function: SHA-256
Recommended length of key modulus: 2048b
Used key formats: PEM formatted and raw binary ASN.1 DER

==== Some OpenSSL CLI utility commands useful in accordance with the aforementioned specifications ====
# Generate PEM encoded RSA private key with 2048 bit long modulus.
openssl genrsa -out LicenseKey.pem 2048 -outform PEM

# Generate raw binary ANS.1 BER (DER) encoded public key from the PEM encoded private key.
openssl rsa -in LicenseKey.pem -pubout -inform PEM -outform DER -out LicenseKeyPublic.der

# Convert the private key from PEM to raw binary ANS.1 BER (DER) encoding.
openssl pkcs8 -nocrypt -in LicenseKey.pem -inform PEM -topk8 -outform DER -out LicenseKey.der

# Compute digital signature.
openssl dgst -sha256 -binary -keyform PEM -sign LicenseKey.pem < data.file > signature.file

# Verify digital signature.
openssl dgst -sha256 -verify -keyform PEM LicenseKeyPublic.pem -signature signature.file data.file

==== Links ====
RSA Cryptography Standard: http://www.emc.com/collateral/white-papers/h11300-pkcs-1v2-2-rsa-cryptography-standard-wp.pdf


=== A simple recipe for certificate generation using command line tools ===
This example should work basically on arbitrary POSIX operating system (e.g. Linux, Unix, BSD, etc.).

1) Take file CertificateExample.xml, copy it as certificate.xml, and modify its contents to fit your needs
2) append binary NULL to the certificate, i.e. run: printf "\x0" >> certificate.xml
2) run: openssl dgst -sha256 -binary -keyform PEM -sign LicenseKey.pem < certificate.xml > certificate.sgn
3) concatenate and gzip compress files certificate.xml and certificate.sgn (in this order), i.e.
   run: cat certificate.xml certificate.sgn | gzip --stdout --fast > certificate.cert

=== Random remarks ===
 - This thing has in essence nothing to do with public key certificates and cryptographic certificates in general.
 - Private key is expected to be PEM encoded RSA key in file named `LicenseKey.pem' (in this directory).


== Licence certifate specification ==

=== File format ===
MDS licence certifate is a gzip compressed digitally signed XML file containing information about licensee.

+-gzip----------+
| +-----------+ |
| |    XML    | |
| +-----------+ |
| | signature | |
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
Used key formats: PEM, ASN.1 DER

==== Some OpenSSL CLI utility commands useful in accordance with the aforementioned specifications ====
# Generate PEM encoded RSA private key with 2048 bit long modulus.
openssl genrsa -out LicenseKey.pem 2048 -outform PEM

# Generate ANS.1 BER (DER) encoded public key from the PEM encoded private key.
openssl rsa -in LicenseKey.pem -pubout -inform PEM -outform DER -out LicenseKeyPublic.der

# Convert the private key from PEM to ANS.1 BER (DER) encoding.
openssl pkcs8 -nocrypt -in LicenseKey.pem -inform PEM -topk8 -outform DER -out LicenseKey.der

# Compute digital signature.
openssl dgst -sha256 -binary -keyform PEM -sign LicenseKey.pem < data.file > signature.file

# Verify digital signature.
openssl dgst -sha256 -verify -keyform PEM LicenseKeyPublic.pem -signature signature.file data.file

==== Links ====
RSA Cryptography Standard: http://www.emc.com/collateral/white-papers/h11300-pkcs-1v2-2-rsa-cryptography-standard-wp.pdf


=== Simple recipe for certificate generation using command line tools ===
This example should work basically on arbitrary POSIX complaint operating system (e.g. any Linux, Unix, BSD, etc.).

1) Take file CertificateExample.xml, copy it as certificate.xml, and modify its contents to fit your needs
2) run: openssl dgst -sha256 -binary -keyform PEM -sign LicenseKey.pem < certificate.xml > certificate.sgn
3) concatenate and gzip compress files certificate.xml and certificate.sgn (in this order), i.e.
   run: cat certificate.xml certificate.sgn | gzip --stdout --fast > certificate.cert

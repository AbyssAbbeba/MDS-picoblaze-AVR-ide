# ==============================================================================
#
# Coplete Crypto++ version 5.6.2 modified by Moravia Microsystems, s.r.o.
# C++ library Crypto++ was written and placed in the public domain by Wei Dai.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( CryptoPP )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# Add a library to the project using the specified source files.
add_library ( ${PROJECT_NAME}
              3way.cpp      adler32.cpp       algebra.cpp   algparam.cpp  arc4.cpp       asn.cpp       authenc.cpp
              base32.cpp    base64.cpp        basecode.cpp  bfinit.cpp    blowfish.cpp   blumshub.cpp  camellia.cpp
              cast.cpp      casts.cpp         cbcmac.cpp    ccm.cpp       channels.cpp   cmac.cpp      cpu.cpp
              crc.cpp       cryptlib_bds.cpp  cryptlib.cpp  default.cpp   des.cpp        dessp.cpp     dh2.cpp
              dh.cpp        dll.cpp           dsa.cpp       eax.cpp       ec2n.cpp       eccrypto.cpp  ecp.cpp
              elgamal.cpp   emsa2.cpp         eprecomp.cpp  esign.cpp     files.cpp      filters.cpp   fips140.cpp
              fipstest.cpp  gcm.cpp           gf2_32.cpp    gf256.cpp     gf2n.cpp       gfpcrypt.cpp  gost.cpp
              gzip.cpp      hex.cpp           hmac.cpp      hrtimer.cpp   ida.cpp        idea.cpp      integer.cpp
              iterhash.cpp  luc.cpp           mars.cpp      marss.cpp     md2.cpp        md4.cpp       md5.cpp
              misc.cpp      modes.cpp         mqueue.cpp    mqv.cpp       nbtheory.cpp   network.cpp   oaep.cpp
              osrng.cpp     panama.cpp        pch.cpp       pkcspad.cpp   polynomi.cpp   pssr.cpp      pubkey.cpp
              queue.cpp     rabin.cpp         randpool.cpp  rc2.cpp       rc5.cpp        rc6.cpp       rdtables.cpp
              rijndael.cpp  ripemd.cpp        rng.cpp       rsa.cpp       rw.cpp         safer.cpp     salsa.cpp
              seal.cpp      seed.cpp          serpent.cpp   sha3.cpp      shacal2.cpp    sha.cpp       sharkbox.cpp
              shark.cpp     simple.cpp        skipjack.cpp  socketft.cpp  sosemanuk.cpp  square.cpp    squaretb.cpp
              strciphr.cpp  tea.cpp           tftables.cpp  tiger.cpp     tigertab.cpp   trdlocal.cpp  ttmac.cpp
              twofish.cpp   vmac.cpp          wait.cpp      wake.cpp      whrlpool.cpp   winpipes.cpp  xtr.cpp
              xtrcrypt.cpp  zdeflate.cpp      zinflate.cpp  zlib.cpp )

# Disable compiler warnings for this library.
set_target_properties ( ${PROJECT_NAME} PROPERTIES
                                        COMPILE_FLAGS "-w" )

# On POSIX link to the pthread library.
if ( TARGET_OS STREQUAL "Windows" )
    target_link_libraries ( ${PROJECT_NAME} ws2_32 )
else()
    target_link_libraries ( ${PROJECT_NAME} pthread )
endif()

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()

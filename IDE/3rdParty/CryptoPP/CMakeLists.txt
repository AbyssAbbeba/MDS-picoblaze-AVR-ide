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
              3way.cpp      ccm.cpp           elgamal.cpp   idea.cpp      polynomi.cpp  serpent.cpp    twofish.cpp
              adhoc.cpp     channels.cpp      emsa2.cpp     integer.cpp   pssr.cpp      sha3.cpp       validat1.cpp
              adler32.cpp   cmac.cpp          eprecomp.cpp  iterhash.cpp  pubkey.cpp    shacal2.cpp    validat2.cpp
              algebra.cpp   cpu.cpp           esign.cpp     luc.cpp       queue.cpp     sha.cpp        validat3.cpp
              algparam.cpp  crc.cpp           files.cpp     mars.cpp      rabin.cpp     sharkbox.cpp   vmac.cpp
              arc4.cpp      cryptlib_bds.cpp  filters.cpp   marss.cpp     randpool.cpp  shark.cpp      wait.cpp
              asn.cpp       cryptlib.cpp      fips140.cpp   md2.cpp       rc2.cpp       simple.cpp     wake.cpp
              authenc.cpp   datatest.cpp      fipsalgt.cpp  md4.cpp       rc5.cpp       skipjack.cpp   whrlpool.cpp
              base32.cpp    default.cpp       fipstest.cpp  md5.cpp       rc6.cpp       socketft.cpp   winpipes.cpp
              base64.cpp    des.cpp           gcm.cpp       misc.cpp      rdtables.cpp  sosemanuk.cpp  xtr.cpp
              basecode.cpp  dessp.cpp         gf2_32.cpp    modes.cpp     regtest.cpp   square.cpp     xtrcrypt.cpp
              bench2.cpp    dh2.cpp           gf256.cpp     mqueue.cpp    rijndael.cpp  squaretb.cpp   zdeflate.cpp
              bench.cpp     dh.cpp            gf2n.cpp      mqv.cpp       ripemd.cpp    strciphr.cpp   zinflate.cpp
              bfinit.cpp    dll.cpp           gfpcrypt.cpp  nbtheory.cpp  rng.cpp       tea.cpp        zlib.cpp
              blowfish.cpp  cbcmac.cpp        gost.cpp      network.cpp   rsa.cpp       test.cpp       ttmac.cpp
              blumshub.cpp  dsa.cpp           gzip.cpp      oaep.cpp      rw.cpp        tftables.cpp   seed.cpp
              camellia.cpp  eax.cpp           hex.cpp       osrng.cpp     safer.cpp     tiger.cpp      pkcspad.cpp
              cast.cpp      ec2n.cpp          hmac.cpp      panama.cpp    salsa.cpp     tigertab.cpp   ida.cpp
              casts.cpp     eccrypto.cpp      hrtimer.cpp   pch.cpp       seal.cpp      trdlocal.cpp   ecp.cpp
              dlltest.cpp )

# Disable compiler warnings for this library.
set_target_properties ( ${PROJECT_NAME} PROPERTIES
                                        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -w" )

# On POSIX link to the pthread library.
if ( NOT TARGET_OS STREQUAL "Windows" )
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

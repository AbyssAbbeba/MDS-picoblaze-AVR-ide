# ==============================================================================
#
# Crypto++ version 5.6.2 modified by Moravia Microsystems, s.r.o.
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

## Add a library to the project using the specified source files,
 # in this case it will produce dynamic C++ library.
add_library ( ${PROJECT_NAME}
              asn.cpp
              cpu.cpp
              crc.cpp
              rng.cpp
              rsa.cpp
              sha.cpp
              misc.cpp
              gzip.cpp
              oaep.cpp
              pssr.cpp
              queue.cpp
              osrng.cpp
              emsa2.cpp
              mqueue.cpp
              pubkey.cpp
              simple.cpp
              algebra.cpp
              filters.cpp
              fips140.cpp
              hrtimer.cpp
              integer.cpp
              pkcspad.cpp
              iterhash.cpp
              nbtheory.cpp
              algparam.cpp
              cryptlib.cpp
              eprecomp.cpp
              randpool.cpp
              rijndael.cpp
              trdlocal.cpp
              zdeflate.cpp
              zinflate.cpp )

## Disable compiler warnings for this library.
set_target_properties ( ${PROJECT_NAME} PROPERTIES
                                        COMPILE_FLAGS "-w" )

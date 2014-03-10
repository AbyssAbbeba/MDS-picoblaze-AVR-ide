# ======================================================================================================================
#
#    M   M  OO  RRR    A   V   V III   A      M   M III  OOO  RRR   OO   SSSS Y   Y  SSSS TTTTT EEEEE M   M  SSSS
#    MM MM O  O R  R  A A  V   V  I   A A     MM MM  I  O   O R  R O  O S      Y Y  S       T   E     MM MM S
#    M M M O  O RRR  A   A V   V  I  A   A    M M M  I  O   O RRR  O  O  SS     Y    SS     T   EEEEE M M M  SS
#    M   M O  O RR   AAAAA  V V   I  AAAAA    M   M  I  O   O RR   O  O    SS   Y      SS   T   E     M   M    SS
#    M   M O  O R R  A   A  V V   I  A   A    M   M  I  O   O R R  O  O      S  Y        S  T   E     M   M      S
#    M   M  OO  R  R A   A   V   III A   A    M   M III  OOO  R  R  OO   SSSS   Y    SSSS   T   EEEEE M   M  SSSS
#
# ======================================================================================================================
#
# The entire Multitarget Development System.
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ======================================================================================================================

# ----------------------------------------------------------------------------------------------------------------------
# GENERAL SETUP.
# ----------------------------------------------------------------------------------------------------------------------

# Set project name.
project ( IDE )

# Set the minimum required version of CMake.
cmake_minimum_required ( VERSION 2.8 FATAL_ERROR )

# Create verbose makefiles if on.
set ( CMAKE_VERBOSE_MAKEFILE OFF )

set ( INSTALL_DIR_BIN "bin" )
set ( INSTALL_DIR_LIB "lib/mds" )
set ( INSTALL_DIR_SHARE "share/mds" )
set ( INSTALL_DIR_DOC "doc/mds" )
set ( INSTALL_DIR_INCLUDE "include/mds" )
set ( INSTALL_DIR_DEMOPROJECT "share/mds/demoproject" )

set ( PRODUCT_VERSION "1.0" )
set ( COPYRIGHT_YEARS "2013, 2014" )
set ( PRODUCT_NAME    "Multitarget Development System" )

## Set global compilation flags:
 # - for all build types:
set ( CMAKE_C_FLAGS            "-Wall -Wextra -Wno-unused-local-typedefs -pipe" )
 # - for debug build only:
set ( CMAKE_C_FLAGS_DEBUG      "-O0 -g" )
 # - for release build only:
set ( CMAKE_C_FLAGS_RELEASE    "-O2 -flto -DNDEBUG" )
 # - for minimum size release build only:
set ( CMAKE_C_FLAGS_MINSIZEREL "-Os -flto -DNDEBUG" )

# Set the same compilation flags for C++ as they are set for C.
set ( CMAKE_CXX_FLAGS              "${CMAKE_C_FLAGS} -std=c++0x" )
set ( CMAKE_CXX_FLAGS_DEBUG        "${CMAKE_C_FLAGS_DEBUG}" )
set ( CMAKE_CXX_FLAGS_RELEASE      "${CMAKE_C_FLAGS_RELEASE}" )
set ( CMAKE_CXX_FLAGS_MINSIZEREL   "${CMAKE_C_FLAGS_MINSIZEREL}" )

# ----------------------------------------------------------------------------------------------------------------------
# USER OPTIONS
# ----------------------------------------------------------------------------------------------------------------------

# Optionally start test subjects in Valgrind:Memcheck.
option ( TEST_MEMCHECK "Start test subjects in Valgrind:Memcheck." ON )

# Optionally colorize compiler output messages.
if ( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCPP )
    option ( COLOR_GCC "Filter compiler output messages to add VT100 terminal compatible color codes to them." ON )
    if ( COLOR_GCC )
        set ( COLOR_GCC ON )
        set_property ( GLOBAL PROPERTY RULE_LAUNCH_LINK    "bash ${CMAKE_SOURCE_DIR}/ldColor.sh"  )
        set_property ( GLOBAL PROPERTY RULE_LAUNCH_COMPILE "bash ${CMAKE_SOURCE_DIR}/gccColor.sh" )
    else()
        set ( COLOR_GCC OFF )
    endif()
    message ( STATUS "Color output is: ${COLOR_GCC}." )
endif()

# Set default build type to Debug.
if ( NOT CMAKE_BUILD_TYPE )
  set ( CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: Debug, Release, and MinSizeRel."
        FORCE )
endif()
message ( STATUS "Configuring environment for build in mode: ${CMAKE_BUILD_TYPE}." )

# When in Debug build mode, build all libraries as shared by default.
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set ( BUILD_SHARED_LIBS ON )
else()
    set ( BUILD_SHARED_LIBS OFF )
    set ( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    -Wl,-O1 -Wl,--strip-all" )
    set ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-O1 -Wl,--strip-all" )
endif()

# Optionally build with support for coverage analysis support.
option ( TEST_COVERAGE "Configure build for coverage analysis." OFF )
if ( TEST_COVERAGE )
    set ( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS}             --coverage" )
    set ( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS}           --coverage" )
    set ( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    --coverage -lgcov" )
    set ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage -lgcov" )
    message ( STATUS "This build is configured for coverage analysis!" )
endif()

# Target architecture.
set ( HOST_ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}" )
if ( HOST_ARCH STREQUAL "AMD64" )
    set ( HOST_ARCH "x86_64")
endif()
if ( NOT TARGET_ARCH )
    message ( STATUS "Target architecture not specified, using native: ${HOST_ARCH}." )
    set ( TARGET_ARCH "${HOST_ARCH}")
endif()
if ( TARGET_ARCH STREQUAL "x86_64" )
    set ( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64 -march=x86-64 -mfpmath=sse" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -march=x86-64 -mfpmath=sse" )
elseif ( TARGET_ARCH STREQUAL "x86" )
    set ( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32 -march=i686 -msse -mfpmath=sse" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -march=i686 -msse -mfpmath=sse" )
else()
    message ( FATAL_ERROR "Target architecture \"${TARGET_ARCH}\" not recognized!" )
endif()
message ( STATUS "Target architecture for this build: ${TARGET_ARCH}." )

# Target operating system.
set ( HOST_OS "${CMAKE_HOST_SYSTEM_NAME}" )
if ( NOT TARGET_OS )
    message ( STATUS "Target operating system not specified, using native: ${HOST_OS}." )
    set ( TARGET_OS "${HOST_OS}" )
endif()
if ( TARGET_OS STREQUAL "Linux" )
    # Pick up one dynamic linker (program interpreter) for all 32b/64b Linux distributions.
    if ( TARGET_ARCH STREQUAL "x86_64" )
        set ( DYNAMIC_LINKER "/lib64/ld-linux-x86-64.so.2" )
    else()
        set ( DYNAMIC_LINKER "/lib/ld-linux.so.2" )
    endif()
    set ( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    -Wl,--dynamic-linker=${DYNAMIC_LINKER}" )
    set ( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--dynamic-linker=${DYNAMIC_LINKER}" )

    # Make all code position independent.
    set ( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
elseif ( TARGET_OS STREQUAL "Windows" )
    # Output all executable binaries and dynamic link libraries to the same directory.
    set ( LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR} )
    set ( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR} )
elseif ( TARGET_OS STREQUAL "Darwin" )
#     # Link statically with std. lib.
#     set ( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -static-libstdc++ -static-libgcc" )
#     set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc" )
else()
    message ( FATAL_ERROR "Target operating system \"${TARGET_OS}\" not recognized!" )
endif()
message ( STATUS "Target operating system for this build: ${TARGET_OS}." )

# ----------------------------------------------------------------------------------------------------------------------
# CROSS COMPILATION
# ----------------------------------------------------------------------------------------------------------------------

message ( STATUS "Host architecture: ${HOST_ARCH}." )
message ( STATUS "Host operating system: ${HOST_OS}." )
if ( ( NOT TARGET_OS STREQUAL HOST_OS ) OR ( NOT TARGET_ARCH STREQUAL HOST_ARCH ) )
    include ( "${CMAKE_SOURCE_DIR}/cross-compilation.cmake" )
endif()

# ----------------------------------------------------------------------------------------------------------------------
# REQUIRED COMPILER AND ITS MINIMUM REQUIRED VERSION.
# ----------------------------------------------------------------------------------------------------------------------

set ( GCC_MIN_VER "4.8.1" )

if ( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCPP )
    exec_program ( ${CMAKE_CXX_COMPILER}
                   ARGS --version
                   OUTPUT_VARIABLE gccOutput )
    string ( REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" gccVersion "${gccOutput}" )
    message ( STATUS "Found GCC compiler: ${CMAKE_CXX_COMPILER} (version: \"${gccVersion}\", minimum required is: \"${GCC_MIN_VER}\")" )

    if ( "${gccVersion}" VERSION_LESS "${GCC_MIN_VER}" )
        message ( SEND_ERROR "Minimum required version of GCC compiler is \"${GCC_MIN_VER}\"." )
    endif()
else()
    message ( SEND_ERROR "Required compiler is GCC" )
endif()

# ----------------------------------------------------------------------------------------------------------------------
# LIBRARIES & TOOLS
# ----------------------------------------------------------------------------------------------------------------------

# Find Qt4 directories and libraries.
set ( QT_MIN_VER "4.8.1" )
set ( QT_IS_STATIC OFF )
find_package ( Qt4 ${QT_MIN_VER} REQUIRED
               COMPONENTS QtCore QtGui QtXml QtHelp )
include ( ${QT_USE_FILE} )

# Find Boost include directories and libraries.
set ( BOOST_MIN_VER "1.48" )
set ( Boost_USE_STATIC_LIBS OFF )
find_package ( Boost ${BOOST_MIN_VER} REQUIRED
               COMPONENTS regex filesystem system )
include_directories ( ${Boost_INCLUDE_DIRS} )

# Find Flex (lexer generator) and Bison (parser generator) executables.
find_package ( FLEX  2.5.35 REQUIRED )
find_package ( BISON 2.5    REQUIRED )

# Find Doxygen (not required).
find_package ( Doxygen 1.7.6 )

# Find QtHelp tools.
include ( "${CMAKE_SOURCE_DIR}/QtHelpTools.cmake" )

# Find LaTeX tools (native CMake's FindLatex is crummy).
include ( "${CMAKE_SOURCE_DIR}/MDSFindLatex.cmake" )

# Include our own macros for using Flex and Bison.
include ( "${CMAKE_SOURCE_DIR}/FlexBisonPair.cmake" )

# Include our own macros for using windres - compiler for Windows RC files.
include ( "${CMAKE_SOURCE_DIR}/windres.cmake" )

# ----------------------------------------------------------------------------------------------------------------------
# SUPPORT FOR AUTOMATED TESTS
# ----------------------------------------------------------------------------------------------------------------------

# After this command, tests may be added in the project using ADD_TEST command.
enable_testing()

# CTest is a testing tool distributed as a part of CMake.
include ( "CTest" )

# Include our own test macros.
include ( "${CMAKE_SOURCE_DIR}/tests.cmake" )

# ----------------------------------------------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ----------------------------------------------------------------------------------------------------------------------

# Build API and user documentation.
add_subdirectory ( docs )

# Build compiler collection.
add_subdirectory ( compiler )

# Build GUI.
add_subdirectory ( GUI )

# Build Moravia Script language support.
# add_subdirectory ( moraviascript )

# Build dev. hardware support software.
add_subdirectory ( HW )

# Build simulators.
add_subdirectory ( simulators )

# Build disassemblers.
add_subdirectory ( disassembler )

# Build utility libraries.
add_subdirectory ( utilities )

# Build 3rd party software.
add_subdirectory ( 3rdParty )

# Build automated tests.
if ( BUILD_TESTING )
    add_subdirectory ( tests )
endif()

# Build installation packages.
add_subdirectory ( pkgs )

# ==============================================================================
#
# (C) copyright 2014 Moravia Microsystems, s.r.o.
#
# author: Martin Ošmera <martin.osmera@moravia-microsystems.com>
#
# ==============================================================================

message ( STATUS "CROSS COMPILATION requested (host: ${HOST_OS}-${HOST_ARCH}, target: ${TARGET_OS}-${TARGET_ARCH})." )

macro ( FROM__LINUX_X86_64__TO__LINUX_X86 )
    # Configure paths and searching.
    set ( libPath "/usr/lib/i386-linux-gnu" )
    set ( CMAKE_FIND_ROOT_PATH "/lib/i386-linux-gnu" "${libPath}" )
    set ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    set ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    set ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
    foreach ( path ${CMAKE_FIND_ROOT_PATH} )
        if ( NOT IS_DIRECTORY "${path}" )
            message ( SEND_ERROR "\"${path}\" not found." )
        endif()
    endforeach()

    # Configure Boost.
    find_path ( Boost_INCLUDE_DIR NAMES boost/config.hpp )
    set ( BOOST_LIBRARYDIR "${libPath}" )
    foreach ( component regex system filesystem )
        string( TOUPPER ${component} COMPONENT )
        set ( Boost_${COMPONENT}_FOUND TRUE )
        set ( Boost_${COMPONENT}_LIBRARY_RELEASE "${libPath}/libboost_${component}.so")
        if ( NOT EXISTS "${Boost_${COMPONENT}_LIBRARY_RELEASE}" )
            message ( SEND_ERROR "\"${Boost_${COMPONENT}_LIBRARY_RELEASE}'\" not found." )
        endif()
    endforeach()
    if ( NOT IS_DIRECTORY "${Boost_INCLUDE_DIR}" )
        message ( SEND_ERROR "\"${Boost_INCLUDE_DIR}\" not found." )
    endif()

    # Configure Qt.
    set ( QT_QTCORE_LIBRARY_RELEASE "${libPath}/libQtCore.so" )
    if ( NOT EXISTS "${QT_QTCORE_LIBRARY_RELEASE}" )
        message ( SEND_ERROR "\"${QT_QTCORE_LIBRARY_RELEASE}'\" not found." )
    endif()
endmacro()

macro ( windows_cross )
    # The name of the target operating system.
    set ( CMAKE_SYSTEM_NAME "Windows" )

    if ( TARGET_ARCH STREQUAL "x86" )
        set ( prefix "i686" )
    else()
        set ( prefix "x86_64" )
    endif()

    # Which compilers to use for C and C++.
    set ( CMAKE_C_COMPILER   ${prefix}-w64-mingw32-gcc )
    set ( CMAKE_CXX_COMPILER ${prefix}-w64-mingw32-g++ )
    set ( CMAKE_RC_COMPILER  ${prefix}-w64-mingw32-windres )

    # Here is the target environment located.
    set ( CMAKE_FIND_ROOT_PATH /usr/${prefix}-w64-mingw32 )
    set ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    set ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY  )
    set ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY  )
    foreach ( path ${CMAKE_FIND_ROOT_PATH} )
        if ( NOT IS_DIRECTORY "${path}" )
            message ( SEND_ERROR "\"${path}\" not found." )
        endif()
    endforeach()

    # Configure Boost.
    set ( BOOST_LIBRARYDIR "${CMAKE_FIND_ROOT_PATH}/Boost/lib" )
    file ( GLOB Boost_INCLUDE_DIR FOLLOW_SYMLINKS "${BOOST_LIBRARYDIR}/../include/boost-*" )
    list ( GET Boost_INCLUDE_DIR 0 Boost_INCLUDE_DIR )

    foreach ( component regex system filesystem )
        string( TOUPPER ${component} COMPONENT )
        set ( Boost_${COMPONENT}_FOUND TRUE )
        file ( GLOB_RECURSE Boost_${COMPONENT}_LIBRARY_RELEASE FOLLOW_SYMLINKS
               "${BOOST_LIBRARYDIR}/libboost_${component}*.dll" )
        list ( GET Boost_${COMPONENT}_LIBRARY_RELEASE 0 Boost_${COMPONENT}_LIBRARY_RELEASE )

        if ( NOT EXISTS "${Boost_${COMPONENT}_LIBRARY_RELEASE}" )
            message ( SEND_ERROR "\"${Boost_${COMPONENT}_LIBRARY_RELEASE}'\" not found." )
        endif()
    endforeach()
    if ( NOT IS_DIRECTORY "${Boost_INCLUDE_DIR}" )
        message ( SEND_ERROR "\"${Boost_INCLUDE_DIR}\" not found." )
    endif()
endmacro()

if ( HOST_OS STREQUAL "Linux" )
    if ( TARGET_OS STREQUAL "Linux" )
        if ( HOST_ARCH STREQUAL "x86" )
            message ( FATAL_ERROR "You have to be running 64b Linux in order to compile for it." )
        elseif ( HOST_ARCH STREQUAL "x86_64" )
            if ( TARGET_ARCH STREQUAL "x86" )
                from__linux_x86_64__to__linux_x86()
            else()
                message ( FATAL_ERROR "This cross compilation is not supported for target architecture: ${TARGET_ARCH}." )
            endif()
        else()
            message ( FATAL_ERROR "This cross compilation is not supported for host architecture: ${HOST_ARCH}." )
        endif()
    elseif ( TARGET_OS STREQUAL "Windows" )
        # Windows cross compilation...
        windows_cross()
    else()
        message ( FATAL_ERROR "This cross compilation is not supported for operating system: ${TARGET_OS}." )
    endif()
else()
    message ( FATAL_ERROR "This cross compilation is not supported on operating system: ${HOST_OS}." )
endif()

# cross-compilation.cmake ends here

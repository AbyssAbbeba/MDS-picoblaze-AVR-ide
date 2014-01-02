# ==============================================================================
#
# Environment for building installation packages.
#
# (C) copyright 2013 Moravia Microsystems, s.r.o.
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( pkgs )

# Set the minimum required version of CMake
cmake_minimum_required ( VERSION 2.8 )

# Create verbose makefiles if on
set ( CMAKE_VERBOSE_MAKEFILE OFF )


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

if ( CMAKE_HOST_APPLE )
    #
    ## Apple MAC OS X
    #

elseif ( CMAKE_HOST_WIN32 )
    #
    ## Microsoft Windows
    #

    FILE ( GLOB libgcc "C:/MinGW/bin/libgcc*.dll" )
    FILE ( GLOB libstdc "C:/MinGW/bin/libstdc*.dll" )
    FILE ( GLOB libmingwm "C:/MinGW/bin/mingwm*.dll" )
    FILE ( GLOB libqtcore "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtCored*.dll" )
    FILE ( GLOB libqtgui "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtGuid*.dll" )
    FILE ( GLOB libqtxml "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin/QtXmld*.dll" )
    INSTALL ( FILES ${libgcc} ${libstdc} ${libmingwm} ${libqtcore} ${libqtgui} ${libqtxml}
              DESTINATION bin )

else()
    #
    ## GNU/Linux and possibly other POSIX systems
    #

    # Install .desktop spec and application icon
    install ( FILES mmIDEicon.png DESTINATION share/pixmaps/ )
    install ( FILES mmIDE.desktop DESTINATION share/applications/ )
    install ( FILES application-x-mmIDE.xml DESTINATION share/mime/packages/ )

endif()

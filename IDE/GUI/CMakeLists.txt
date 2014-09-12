# set project's name
  PROJECT( GUI )

windres_rc ( OUTPUT_VAR         RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS IDE"
             ICON               "windres/mds-ide.ico"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             "mds-ide"
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )


    file ( READ ${CMAKE_CURRENT_SOURCE_DIR}/resources/html/license-${MDS_VARIANT}.html licenseTerms )
    file ( WRITE ${CMAKE_CURRENT_SOURCE_DIR}/resources/html/license.html "${licenseTerms}" )
    configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/resources/html/about-general.html.in
                     ${CMAKE_CURRENT_SOURCE_DIR}/resources/html/about-general.html @ONLY )
    set_directory_properties ( PROPERTIES
                               ADDITIONAL_MAKE_CLEAN_FILES "resources/html/about-general.html;resources/html/license.html" )


  # with SET() command you can change variables or define new ones
  # here we define SAMPLE_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( SAMPLE_SRCS
       ./main.cpp
  )

  # another list, this time it includes all header files that should be treated with moc
  SET( SAMPLE_MOC_HDRS
  )

  # some .ui files
   SET( SAMPLE_UIS
   )

  # and finally an resource file
   SET( SAMPLE_RCS
        ./icons.qrc
   )

  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )

  # this will run uic on .ui files:
  # QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

  # and finally this will run moc:
  QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )


  set ( HOST_OS "${CMAKE_HOST_SYSTEM_NAME}" )
  if ( NOT TARGET_OS )
        message ( STATUS "Target operating system not specified, using native: ${HOST_OS}." )
        set ( TARGET_OS "${HOST_OS}" )
  endif()

  if ( TARGET_OS STREQUAL "Windows" )
        set ( win32 "WIN32" )
  endif()
  ADD_EXECUTABLE( mds-ide ${win32} ${RC_FILE} ${SAMPLE_SRCS} ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS} )


# List of additional libraries which can be enabled or disabled for this particular build.
set ( MDS_ADDITONAL_LIBS "" )

if ( MDS_FEATURE_TRIAL )
    list ( APPEND MDS_ADDITONAL_LIBS "TrialExpired" )
endif()
if ( MDS_FEATURE_LICENCE_CERTIFICATE )
    list ( APPEND MDS_ADDITONAL_LIBS "LicenseInitWidget" "LicenseCertificate" )
endif()

  # here we instruct CMake to build "mmIDE" executable from all of the source files

  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( mds-ide ${QT_LIBRARIES} ${MDS_ADDITONAL_LIBS} Mainform GuiCfg ErrorDialog  )


  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  include_directories ( ${CMAKE_BINARY_DIR}
                        "${CMAKE_CURRENT_BINARY_DIR}/mainform/" )
# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the widgets
add_subdirectory(widgets)
add_subdirectory(dialogs)
add_subdirectory(mainform)
add_subdirectory(project)
add_subdirectory(errordialog)
add_subdirectory(guicfg)
add_subdirectory(resources)

# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install( TARGETS mds-ide
         RUNTIME DESTINATION ${INSTALL_DIR_BIN}
         LIBRARY DESTINATION ${INSTALL_DIR_LIB} )

# set project's name
  PROJECT( GUI )


# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)



windres_rc ( OUTPUT_VAR         EXEC_RC_FILE
             FILETYPE           "APP"
             FILE_VERSION       "1.0"
             FILEOS             "NT_WINDOWS32"
             FILE_DESCRIPTION   "MDS IDE"
             ICON               "windres/mds-ide.ico"
             COMPANY_NAME       "Moravia Microsystems, s.r.o."
             TARGET             ${COMPILER_EXEC_TARGET}
             PRODUCT_NAME       ${PRODUCT_NAME}
             PRODUCT_VERSION    ${PRODUCT_VERSION}
             YEARS              ${COPYRIGHT_YEARS} )

  
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

  SET( CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -s")

  # enable warnings
  ADD_DEFINITIONS( -Wall -std=c++0x )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  # SET( QT_USE_QT3SUPPORT TRUE )   
   SET( QT_USE_QTXML TRUE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )

  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )
  
  # this will run uic on .ui files:
  # QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

link_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/mainform/"
	"${CMAKE_CURRENT_BINARY_DIR}/guicfg/"
)

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/mainform/"
)
  
  # here we instruct CMake to build "mmIDE" executable from all of the source files
  ADD_EXECUTABLE( mds-ide ${EXEC_RC_FILE} ${SAMPLE_SRCS} ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( mds-ide ${QT_LIBRARIES} Mainform GuiCfg)




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



# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

install( TARGETS mds-ide
         RUNTIME DESTINATION ${INSTALL_DIR_BIN}
         LIBRARY DESTINATION ${INSTALL_DIR_LIB} )

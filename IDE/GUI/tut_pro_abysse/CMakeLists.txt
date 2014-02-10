# set project's name
  PROJECT( GUI )


# Set the minimum required version of CMake
cmake_minimum_required(VERSION 2.8)

  
  # with SET() command you can change variables or define new ones
  # here we define SAMPLE_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( SAMPLE_SRCS
       ./main.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( SAMPLE_MOC_HDRS
        ./main.h
  )
  
  # some .ui files
   SET( SAMPLE_UIS
        untitled.ui
   )
  
  # and finally an resource file
   SET( SAMPLE_RCS
   )

  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )
  
  # this will run uic on .ui files:
  QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )
  
  # here we instruct CMake to build "mmIDE" executable from all of the source files
  ADD_EXECUTABLE( testwidget ${SAMPLE_SRCS} ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( testwidget ${QT_LIBRARIES})




# ------------------------------------------------------------------------------
# BUILD COMPONENTS IN THE SUBDIRECTORIES
# ------------------------------------------------------------------------------

# Build the widgets


# ------------------------------------------------------------------------------
# INSTALLATION OPTIONS
# ------------------------------------------------------------------------------

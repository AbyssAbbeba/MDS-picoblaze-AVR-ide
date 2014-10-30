# ==============================================================================
#
# GUI Configuration
#
# (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
#
# ==============================================================================

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

project(GuiCfg)

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

QT4_WRAP_CPP( MOC_SRCS guicfg.h )

add_library ( ${PROJECT_NAME}
              ${MOC_SRCS}
              guicfg.cpp )

target_link_libraries ( ${PROJECT_NAME}
                        ${QT_LIBRARIES} ErrorDialog )

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()

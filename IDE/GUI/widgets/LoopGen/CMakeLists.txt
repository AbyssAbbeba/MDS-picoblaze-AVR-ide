##

# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( LoopGen )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

QT4_WRAP_CPP( MOC_SRCS loop_gen.h )
QT4_WRAP_UI( UI_HDRS loop_gen.ui )

add_library ( ${PROJECT_NAME}
              ${MOC_SRCS}
              ${UI_HDRS}
              loop_gen.cpp )

target_link_libraries ( ${PROJECT_NAME}
                        ${QT_LIBRARIES} Highlighter GuiCfg )

if ( BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME}
              RUNTIME DESTINATION ${INSTALL_DIR_BIN}
              LIBRARY DESTINATION ${INSTALL_DIR_LIB} )
endif()

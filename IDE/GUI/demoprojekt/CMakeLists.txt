# set project's name
  PROJECT ( DEMOPROJECT )


foreach ( extension "asm" "prc" "v" "vhd" "mds-project" )
    file ( GLOB files2install *.${extension} )
    install ( FILES ${files2install} DESTINATION "${INSTALL_DIR_DEMOPROJECT}" )
endforeach ( extension )
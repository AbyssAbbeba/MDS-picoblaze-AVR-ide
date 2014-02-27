# set project's name
project ( DEMOPROJECT )

foreach ( extension "mds-project" "asm" "ihex" "dbg" "v" "vhd" )
    file ( GLOB files2install Example/*.${extension} )
    install ( FILES ${files2install} DESTINATION ${INSTALL_DIR_DEMOPROJECT} )
endforeach ( extension )

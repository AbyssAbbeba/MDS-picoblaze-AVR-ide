# set project's name
project ( MDSExample )

foreach ( extension "mds-project" "asm" )
    file ( GLOB files2install MDSExample/*.${extension} )
    install ( FILES ${files2install} DESTINATION ${INSTALL_DIR_DEMOPROJECT} )
endforeach ( extension )

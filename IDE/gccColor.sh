#! /bin/bash

exec "$@" |& gawk '
    {
        sub ( /: error: /,   ": \033[41;30merror:\033[m ",   $0 )
        sub ( /: warning: /, ": \033[43;30mwarning:\033[m ", $0 )
        sub ( /: note: /,    ": \033[33;1mnote:\033[m ",     $0 )

        sub ( /[^:]+(:[[:digit:]]+:[[:digit:]]+)?:/, "\033[30;1m&\033[m", $0 )

        print ( $0 )
    }
'
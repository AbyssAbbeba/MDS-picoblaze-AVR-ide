#! /usr/bin/gawk -f

BEGIN {
    ORS = ""
}

{
    sub ( /^[[:space:]]+/, " ", $0 )
    print ( $0 )
}

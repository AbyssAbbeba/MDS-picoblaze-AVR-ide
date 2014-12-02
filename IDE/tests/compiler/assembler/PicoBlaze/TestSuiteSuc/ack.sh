#! /bin/bash

if (( ! $# )); then
    echo "Test case name(s) required." > /dev/stderr
    exit 1
fi

while (( $# )); do
    name="${1%.*}"
    shift

    if [ ! -e "testcases/${name}.asm" ]; then
        echo "Error: no test case named \"${name}\" found."
        exit 1
    fi

    for e in bin dbg hex mac rawhex str v crt err lst mem srec sym vhd; do
        cp -ufv results/${name}.${e} expected/${name}.${e}.exp
    done
done

#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="Adaptable"

source "../../../libCompile.sh"

if [ ! -d "../results/" ]; then
    mkdir "../results/"
fi

for i in *; do
    if [ -d "${i}" ]; then
        cd "${i}"
        runBuild "../../results/${i}"
        cd ..
    fi
done

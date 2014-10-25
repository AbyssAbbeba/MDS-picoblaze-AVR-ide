#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="Adaptable"

source "../../../libCompile.sh"

runBuild "../results"

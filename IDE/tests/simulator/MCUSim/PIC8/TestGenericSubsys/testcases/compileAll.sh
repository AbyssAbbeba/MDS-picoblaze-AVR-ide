#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="pic8"
DEV=""

source "../../../libCompile.sh"

runBuild "../results"

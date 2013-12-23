#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="PicoBlaze"
DEV="kcpsm1"

source "../../../libCompile.sh"

runBuild "../results"

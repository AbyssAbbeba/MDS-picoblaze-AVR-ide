#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="PicoBlaze"
DEV="kcpsm6"

source "../../../libCompile.sh"

runBuild "../results"

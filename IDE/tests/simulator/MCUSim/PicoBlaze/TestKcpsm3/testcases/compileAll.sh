#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="PicoBlaze"
DEV="kcpsm3"

source "../../../libCompile.sh"

runBuild "../results"

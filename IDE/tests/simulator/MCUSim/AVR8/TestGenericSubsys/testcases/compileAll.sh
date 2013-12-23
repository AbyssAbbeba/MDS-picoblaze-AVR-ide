#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

LANG="asm"
ARCH="avr8"
DEV=""

source "../../../libCompile.sh"

runBuild "../results"

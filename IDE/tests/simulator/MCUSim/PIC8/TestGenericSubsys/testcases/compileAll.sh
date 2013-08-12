#! /bin/bash

cd $(dirname "$0")

declare -r COMPILER_EXEC="../../../../../../compiler/compiler"

if [ "$(uname -o)" == "Msys" ]; then
    COMPILER_EXEC+=".exe"
fi

if [ ! -e "${COMPILER_EXEC}" ]; then
    echo "Unable to locate compiler binary."
    exit 1
fi

for i in *.asm; do
    echo "Building $i ..."

    ${COMPILER_EXEC}                            \
        --plang=asm                             \
        --arch=pic8                             \
        --file="$i"                             \
        --hex="../results/${i%%.asm}.hex"       \
        --hex="../results/${i%%.asm}.lst"       \
        &> /dev/null
done

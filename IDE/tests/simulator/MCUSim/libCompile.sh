#! /bin/bash

declare COMPILER_EXEC="${PWD}"
while [[ "${COMPILER_EXEC}" != "/" ]]; do
    if [[ -f "${COMPILER_EXEC}/compiler/compiler" ]]; then
        COMPILER_EXEC+="/compiler/compiler"
        break
    fi
    COMPILER_EXEC="$(readlink -n -f "${COMPILER_EXEC}/..")"
done

if [[ "$(uname -o)" == "Msys" ]]; then
    COMPILER_EXEC+=".exe"
fi

if [[ ! -x "${COMPILER_EXEC}" ]]; then
    echo "Unable to locate compiler executable binary." > /dev/stderr
    exit 1
fi

function runBuild()
{
    if ! ls *.asm &> /dev/null; then
        return
    fi

    for i in *.asm; do
        if [[ -f "${1}/${i%%.asm}.hex" && "${1}/${i%%.asm}.hex" -nt "$i" ]]; then
            continue
        fi

        echo -n "Building $i ... "
        if ! output=$( ${COMPILER_EXEC}                 \
                            --plang="${LANG}"           \
                            --arch="${ARCH}"            \
                            --dev="${DEV}"              \
                            --file="${i}"               \
                            --hex="${1}/${i%%.asm}.hex" \
                            --lst="${1}/${i%%.asm}.lst" 2>&1 )
        then
            echo "[ERROR]"
            echo "${output}" > /dev/stderr
        else
            echo "[OK]"
        fi

    done
}

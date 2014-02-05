#! /bin/bash

declare BASE_INC_DIR=''
declare COMPILER_EXEC="${PWD}"
declare -r OS="$(uname -o)"

if [[ "${OS}" == 'Cygwin' || "${OS}" == 'Msys' ]]; then
    while [[ "${COMPILER_EXEC}" != "/" ]]; do
        if [[ -f "${COMPILER_EXEC}/mds-compiler.exe" ]]; then
            COMPILER_EXEC+="/mds-compiler.exe"
            break
        fi
        COMPILER_EXEC="$( readlink -n -f "${COMPILER_EXEC}/.." )"
    done
    BASE_INC_DIR="$( cygpath -m "$( dirname "${COMPILER_EXEC}" )/compiler/include" )"
else
    while [[ "${COMPILER_EXEC}" != "/" ]]; do
        if [[ -f "${COMPILER_EXEC}/compiler/mds-compiler" ]]; then
            COMPILER_EXEC+="/compiler/mds-compiler"
            break
        fi
        COMPILER_EXEC="$( readlink -n -f "${COMPILER_EXEC}/.." )"
    done
    BASE_INC_DIR="$( dirname "${COMPILER_EXEC}" )/include"
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

    if [[ ! -d "${1}" ]]; then
        mkdir "${1}"
    fi

    for i in *.asm; do
        if [[ -f "${1}/${i%%.asm}.hex" && "${1}/${i%%.asm}.hex" -nt "$i" ]]; then
            continue
        fi

        echo -n "Building $i ... "
        if ! output="$( LC_ALL="C" ${COMPILER_EXEC}                 \
                                   --dev="${DEV}"                   \
                                   --arch="${ARCH}"                 \
                                   --plang="${LANG}"                \
                                   --hex="${1}/${i%%.asm}.hex"      \
                                   --lst="${1}/${i%%.asm}.lst"      \
                                   --base-inc-dir="${BASE_INC_DIR}" \
                                   "${i}" 2>&1 )"
        then
            echo "[ERROR]"
            rm "${1}/${i%%.asm}.hex"
            echo "${output}" > /dev/stderr
        else
            echo "[OK]"
        fi

    done
}

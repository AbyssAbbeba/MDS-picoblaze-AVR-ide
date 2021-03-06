#! /bin/bash

declare BASE_INC_DIR=''
declare COMPILER_EXEC="${PWD}"
declare -r OS="$(uname -o)"

if [ ! -z "${DEV}" ]; then
    declare -r DEVICE_OPTION="--device=${DEV}"
else
    declare -r DEVICE_OPTION=""
fi

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
    local hexFile

    if ! ls *.asm &> /dev/null; then
        return
    fi

    if [[ ! -d "${1}" ]]; then
        mkdir "${1}"
    fi

    for i in *.asm; do
        hexFile="${1}/${i%%.asm}.hex"

        if [[ -s "${hexFile}" && ! "${i}" -nt "${hexFile}" ]]; then
            continue
        fi

        if ! output="$( LC_ALL="C" ${COMPILER_EXEC}                 \
                                   ${DEVICE_OPTION}                 \
                                   --architecture="${ARCH}"         \
                                   --language="${LANG}"             \
                                   --hex="${1}/${i%%.asm}.hex"      \
                                   --lst="${1}/${i%%.asm}.lst"      \
                                   --base-inc-dir="${BASE_INC_DIR}" \
                                   "${i}" 2>&1 )"
        then
            echo "Building $i ... [ERROR]"
            rm "${1}/${i%%.asm}.hex"
            echo "${output}" > /dev/stderr
        fi

    done
}

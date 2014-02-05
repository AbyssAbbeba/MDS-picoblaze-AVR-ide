#! /bin/bash

declare -r OS="$(uname -o)"
if [[ "${OS}" == 'Cygwin' || "${OS}" == 'Msys' ]]; then
    if ! which 'cygpath' &> /dev/null; then
        echo "Error: cygpath is missging."
        exit 1
    fi
    declare -ir inWindows=1
else
    declare -ir inWindows=0
fi

if [[ "$#" -lt 3 ]]; then
    exit 1
fi

if (( inWindows )); then
    cd "$(dirname "$(readlink -n -f "$( cygpath "${0}" )")" )/results" || exit 1
else
    cd "$(dirname "$(readlink -n -f "${0}")" )/results" || exit 1
fi
readonly TEST_NAME="${1}"
readonly GCOV_LOG="${TEST_NAME}.gcov.log"
readonly MAIN_DIR="$(readlink -f ../..)"

shift

: > "${TEST_NAME}-Results.xml"

if [[ ! -d "${TEST_NAME}" ]]; then
    mkdir "${TEST_NAME}" || exit 1
fi
if ! which gcov &> /dev/null; then
    echo "Error: unable to find gcov executable, please install gcov." > /dev/stderr
    exit 1
fi
if ! which bc &> /dev/null; then
    echo "Error: unable to find bc executable, please install bc." > /dev/stderr
    exit 1
fi

echo "Note: console output from gcov will be captured in file: \"${GCOV_LOG}\"."

if (( inWindows )); then
        eval "$@"
        status=$?

        declare -ra GCDA_FILES=( $(find -L "${MAIN_DIR}" -type f -name '*.gcda') )
        echo -n "Coverage analysis: processing ${#GCDA_FILES[@]} .gcda and .gcno files ... "

        for gcda in "${GCDA_FILES[@]}"; do
            src="$( readlink -f "$( dirname "${gcda}" )/../../$( basename "${gcda%%.gcda}" )" )"

            mkdir -p "${TEST_NAME}/${dirCounter:=0}"

            cd "${TEST_NAME}/${dirCounter}"
            gcov --long-file-names --display-progress --object-file "$(cygpath -m "${gcda}")" "$(cygpath -m "${src}")"
            cd ../..
            rm "${gcda}"

            let dirCounter++
        done >> "${GCOV_LOG}" 2>&1
else
    prefixStrip=0
    base="${MAIN_DIR}"
    while [[ "${base}" =~ /[^/]+ ]]; do
        base="${base:${#BASH_REMATCH[0]}}"
        let prefixStrip++
    done
    readonly TEST_DIR="${PWD}/${TEST_NAME}"

    GCOV_PREFIX_STRIP=${prefixStrip} GCOV_PREFIX="${TEST_DIR}" eval "$@"
    status=$?

    declare -ra GCDA_FILES=( $(find -L "${TEST_DIR}" -type f -name '*.gcda') )
    echo -n "Coverage analysis: processing ${#GCDA_FILES[@]} .gcda and .gcno files ... "

    for gcda in "${GCDA_FILES[@]}"; do
        gcno="${MAIN_DIR}${gcda:${#TEST_DIR}}"
        gcno="${gcno%%.gcda}.gcno"
        src="$(readlink -f "$( dirname "${gcno}" )/../../$(basename "${gcno%%.gcno}")")"

        ln -s "${gcno}" "${gcda%%.gcda}.gcno"
        mkdir -p "${TEST_NAME}/${dirCounter:=0}"

        cd "${TEST_NAME}/${dirCounter}"
        gcov --long-file-names --display-progress --object-file "${gcda}" "${src}"
        cd ../..

        let dirCounter++
    done >> "${GCOV_LOG}" 2>&1
fi

echo "[DONE]"
echo ""

exit ${status}

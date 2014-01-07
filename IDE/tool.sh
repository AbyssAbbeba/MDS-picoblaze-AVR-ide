#! /bin/bash

export LC_ALL="${LANG}"

readonly VERSION="0.2"
declare -ir CPU_CORES=$( which lscpu &> /dev/null && lscpu 2> /dev/null | \
                         gawk 'BEGIN { n = 1 } END { print(n) } /^CPU\(s\)/ { n = $2; exit }' || echo 1 )
declare -ir PP=$(( CPU_CORES + 1 ))

function build() {
    local generator

    if [ "$(uname -o)" == "Msys" ]; then
        # Build on Windows.
        QT_PATH="$(for i in /c/QtSDK/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
        export PATH="${QT_PATH}:${PATH}"
        generator='MSYS Makefiles'
    else
        # Build on a POSIX system.
        generator='Unix Makefiles'
    fi

    cmake -DCOLOR_GCC=ON                  \
          -DTEST_COVERAGE=OFF             \
          -DCMAKE_COLOR_MAKEFILE=ON       \
          -DCMAKE_BUILD_TYPE=${bt:-Debug} \
          -G "${generator}"               \
          . || exit 1

    if ! make -j${PP} --keep-going; then
        echo -e "\nBuild FAILED!\n" > /dev/stderr
        exit 1
    fi
}

function tests() {
    requirePrograms 'xsltproc' 'valgrind' 'gcov'

    local -r BUILD_LOG="tests/results/BuildLog.log"
    local -r TIME_FILE="tests/results/startTime.log"

    if [[ -e tests/results ]]; then
        rm -rf tests/results || exit 1
    fi

    mkdir tests/results || exit 1
    date +'%s' > "${TIME_FILE}"

    cmake -DCOLOR_GCC=OFF                   \
          -DCMAKE_BUILD_TYPE=${bt:-Debug}   \
          -DTEST_COVERAGE=${cov:-ON}        \
          -DTEST_MEMCHECK=${val:-ON}        \
          -DCMAKE_COLOR_MAKEFILE=OFF . 2>&1 \
        | tee "${BUILD_LOG}"
    make -j${PP} --keep-going 2>&1 | tee -a "${BUILD_LOG}"

    if [[ -z "${1}" ]]; then
        ctest -j${CPU_CORES}
    else
        ctest -j${CPU_CORES} --tests-regex "${1}"
    fi
    make test_analysis
}

function clean() {
    cmake .
    make clean

    for dirGlob in 'CMakeFiles' 'Testing' '_CPack_Packages'; do
        rm -rfv $(find -type d -name "${dirGlob}")
    done
    for fileGlob in 'Makefile' 'Doxyfile' 'DartConfiguration.tcl' 'CPackSourceConfig.cmake' 'CPackConfig.cmake' \
                    '*-Linux.*' '*~' '*.a' '*.so' 'moc_*.cxx' '.directory' 'CMakeCache.txt' 'cmake_install.cmake' \
                    'CTestTestfile.cmake' 'install_manifest.txt'
    do
        rm -fv $(find -type f -name "${fileGlob}")
    done
}

function repoSync() {
    local -r REPO=$( git remote show | head -n 1 )

    git commit -a -m "(no comment)"
    git pull ${REPO} master
    git push ${REPO} master
}

function requirePrograms() {
    for program in "${@}"; do
        if ! which xsltproc &> /dev/null; then
            echo "${program} is missing, please install ${program} and run again." > /dev/stderr
            exit 1
        fi
    done
}

function countLines() {
    requirePrograms 'cloc'
    cloc --exclude-dir=3rdParty .
}

function printVersion() {
    printf "%s\n" "$VERSION"
}

function unknownOption() {
    printf "Unknow option. Try to run './${1} -h' to get help.\n\n"
    printHelp
}

function printHelp() {
    printf "This is a tool for doing various things with the code here.\n"
    printf "Version: %s\n" "$VERSION"
    printf "\n"
    printf "Options:\n"
    printf "    -b     Build.\n"
    printf "    -t     Build and run all tests.\n"
    printf "    -T <r> Build and run only the tests matching regular expression <r>.\n"
    printf "    -c     Clean up the directories by removing all automatically generated files.\n"
    printf "    -s     Synchronize with the central repository.\n"
    printf "    -l     Count number of lines source code files.\n"
    printf "    -q     Print some statistics regarding generated binary files.\n"
    printf "    -h     Print this message.\n"
    printf "    -V     Print version of this script.\n"
    printf "\n"
    printf "The script might be run with these variables: (value in parentheses is default value)\n"
    printf "    bt=<b>   Set build type to <b>, options are: 'Debug', 'Release', and 'MinSizeRel' (all: 'Debug').\n"
    printf "    cov=<on|off>   Configure build for coverage analysis (normal: 'OFF', tests: 'ON').\n"
    printf "    val=<on|off>   Turn on/off Valgrind:Memcheck during automated test run. (normal: n/a, tests: 'ON').\n"
    printf "\n"
    printf "Example:\n"
    printf "    bt=Release cov=off ./tool.sh -t     # Run tests without coverage and with binaries built for release.\n"
    printf "    bt=MinSizeRel ./tool.sh -csb        # Clear, synchronize GIT, and build for minimum size binaries.\n"
    printf "\n"
    printf "Order of options does not matter because order of operations is fixed (see the script code for details).\n"
    printf "\n"
}

function main() {
    set +o pipefail
    cd "$(dirname "$(readlink -n -f "${0}")" )"
    echo "Using up to ${CPU_CORES} CPU cores."

    local -A opts

    # Parse CLI options using `getopts' utility.
    while getopts ":hVcltbsmqT:" opt; do
        case $opt in
            b) opts['b']=1;;
            t) opts['t']=1;;
            T) opts['T']="${OPTARG}";;
            c) opts['c']=1;;
            l) opts['l']=1;;
            s) opts['s']=1;;
            q) opts['q']=1;;
            h) printHelp; exit;;
            V) printVersion; exit;;
            ?) unknownOption "$(basename "${0}")"; exit 1;;
        esac
    done

    if (( ! ${#opts[@]} )); then
        printHelp
        exit
    fi

    if [[ ! -z "${opts['c']}" ]]; then
        clean
    fi

    if [[ ! -z "${opts['s']}" ]]; then
        repoSync
    fi

    if [[ ! -z "${opts['l']}" ]]; then
        countLines
    fi

    if [[ ! -z "${opts['T']}" ]]; then
        tests "${opts['T']}"
    elif [[ ! -z "${opts['t']}" ]]; then
        tests
    elif [[ ! -z "${opts['b']}" ]]; then
        build
    fi

    if [[ ! -z "${opts['q']}" ]]; then
        for i in $(find . -executable); do
            if [[ -f "$i" ]]; then
                if file "$i" | grep 'ELF' &>/dev/null; then
                    ls -l $i
                fi
            fi
        done | tee /dev/stderr | gawk 'BEGIN {b=0} END {printf("%.2f MB\n", b/(1024*1024))} {b+=$5}'
    fi
}

main "$@"

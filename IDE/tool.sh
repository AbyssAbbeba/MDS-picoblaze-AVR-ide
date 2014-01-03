#! /bin/bash

readonly VERSION="0.2"
declare -ir CPU_CORES=$( which lscpu > /dev/null && lscpu | \
                         gawk 'BEGIN { n = 1 } END { print(n) } /^CPU\(s\)/ { n = $2; exit }' || echo 1 )
declare -ir PP=$(( CPU_CORES + 1 ))

echo "Using up to ${CPU_CORES} CPU cores."

function build() {
    if [ "$(uname -o)" == "Msys" ]; then
        # Build on Windows.
        QT_PATH="$(for i in /c/QtSDK/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
        export PATH="${QT_PATH}:${PATH}"
        cmake -DCOLOR_GCC=ON                    \
              -DTEST_COVERAGE=OFF               \
              -DCMAKE_COLOR_MAKEFILE=ON         \
              -DCMAKE_BUILD_TYPE="${1:-Debug}"  \
              -G "MSYS Makefiles"               \
              . || exit 1
    else
        # Build on a POSIX system.
        cmake -DCOLOR_GCC=ON                    \
              -DTEST_COVERAGE=OFF               \
              -DCMAKE_COLOR_MAKEFILE=ON         \
              -DCMAKE_BUILD_TYPE="${1:-Debug}"  \
              . || exit 1
    fi

    make -j${PP}
}

function tests() {
    if [[ -e tests/results ]]; then
        rm -rf tests/results || exit 1
    fi

    mkdir tests/results || exit 1
    date +'%s' > tests/results/startTime.log

    local -r BUILD_LOG="tests/results/BuildLog.txt"
    local -r STATUS_FILE="tests/results/status.txt"
    local -i status=1

    cmake -DCMAKE_BUILD_TYPE=Debug          \
          -DTEST_COVERAGE=${cov:-ON}        \
          -DTEST_MEMCHECK=${val:-ON}        \
          -DCOLOR_GCC=OFF                   \
          -DCMAKE_COLOR_MAKEFILE=OFF . 2>&1 \
        | tee "${BUILD_LOG}" || status=0
    make -j${PP} 2>&1 | tee -a "${BUILD_LOG}" || status=0
    echo ${status} > "${STATUS_FILE}"

    if [[ -z "${1}" ]]; then
        ctest -j${PP}
    else
        ctest -j${PP} --tests-regex "${1}"
    fi
    make test_analysis
}

function clean() {
    cmake .
    make clean

    for dirGlob in 'CMakeFiles' 'Testing' '_CPack_Packages'; do
        rm -rfv $(find -type d -name "${fileGlob}")
    done
    for fileGlob in 'Makefile''Doxyfile' 'DartConfiguration.tcl' 'CPackSourceConfig.cmake' 'CPackConfig.cmake' \
                    '*-Linux.*' '*~' '*.a' '*.so' 'moc_*.cxx' '.directory' 'CMakeCache.txt' 'cmake_install.cmake' \
                    'CTestTestfile.cmake' 'install_manifest.txt'
    do
        rm -fv $(find -type f -name "${fileGlob}")
    done
}

function repoSync() {
    local -r REPO=$( git remote show | head -n 1 )

    git commit -a -m "."
    git pull ${REPO} master
    git push ${REPO} master
}

function countLines() {
    cloc .
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
    printf "    -B <t> Build with <t> build type specification.\n"
    printf "    -t     Build and run all tests.\n"
    printf "    -T <r> Build and run only the tests matching regular expression <r>.\n"
    printf "    -c     Clean up the directories by removing all automatically generated files.\n"
    printf "    -s     Synchronize with the central repository.\n"
    printf "    -l     Count number of lines source code files.\n"
    printf "    -q     Print some statistics regarding generated binary files.\n"
    printf "    -h     Print this message.\n"
    printf "    -V     Print version of this script.\n"
    printf "\n"
    printf "Order of options does not matter.\n"
}

function main() {
    cd "$(dirname "$(readlink -n -f "${0}")" )"

    local -A opts

    # Parse CLI options using `getopts' utility.
    while getopts ":hVcltbsmqT:B:" opt; do
        case $opt in
            b) opts['b']=1;;
            t) opts['t']=1;;
            T) opts['T']="${OPTARG}";;
            B) opts['B']="${OPTARG}";;
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
    elif [[ ! -z "${opts['B']}" ]]; then
        build "${opts['B']}"
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

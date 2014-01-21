#! /bin/bash

readonly VERSION="0.2"
declare -ir CPU_CORES=$( which lscpu &> /dev/null && lscpu 2> /dev/null | \
                         gawk 'BEGIN { n = 1 } END { print(n) } /^CPU\(s\)/ { n = $2; exit }' || echo 1 )
declare -ir PP=$(( CPU_CORES + 1 ))

function build() {
    local generator

    requirePrograms 'cmake' 'make'

    if [[ "$(uname -o)" == "Msys" ]]; then
        # Build on Windows.
        findQtSDKAndBoost
        generator='MSYS Makefiles'
        color='off'
    else
        # Build on a POSIX system.
        generator='Unix Makefiles'
    fi

    cmake -G "${generator}"                     \
          -DTEST_COVERAGE=OFF                   \
          -DTARGET_OS="${os}"                   \
          -DTARGET_ARCH="${arch}"               \
          -DCOLOR_GCC=${color:-on}              \
          -DCMAKE_BUILD_TYPE=${bt:-Debug}       \
          -DCMAKE_COLOR_MAKEFILE=${color:-on}   \
          . || exit 1

    if ! make -j${PP} --keep-going; then
        echo -e "\nBuild FAILED!\n" > /dev/stderr
        exit 1
    fi
}

function tests() {
    local -r BUILD_LOG="tests/results/BuildLog.log"
    local -r TIME_FILE="tests/results/startTime.log"
    local generator

    requirePrograms 'xsltproc' 'cmake' 'make'
    if [[ "$(uname -o)" == "Msys" ]]; then
        # Test on Windows.
        findQtSDKAndBoost
        cov='off'
        val='off'
        color='off'
        generator='MSYS Makefiles'
    else
        # Test on a POSIX system.
        if [[ "${val}" =~ [oO][nN] ]]; then
            requirePrograms 'valgrind'
        fi
        if [[ "${cov}" =~ [oO][nN] ]]; then
            requirePrograms 'gcov'
        fi
        generator='Unix Makefiles'
    fi

    if [[ -e tests/results ]]; then
        rm -rf tests/results || exit 1
    fi

    mkdir tests/results || exit 1
    date +'%s' > "${TIME_FILE}"

    cmake -G "${generator}"                     \
          -DCOLOR_GCC=${color:-off}             \
          -DTARGET_OS="${os}"                   \
          -DTARGET_ARCH="${arch}"               \
          -DTEST_COVERAGE=${cov:-on}            \
          -DTEST_MEMCHECK=${val:-on}            \
          -DCMAKE_BUILD_TYPE=${bt:-Debug}       \
          -DCMAKE_COLOR_MAKEFILE=${color:-off}  \
          . 2>&1                                \
        | tee "${BUILD_LOG}"

    make -j${PP} --keep-going 2>&1 | tee -a "${BUILD_LOG}"

    if [[ -z "${1}" ]]; then
        ctest -j${CPU_CORES}
    else
        ctest -j${CPU_CORES} --tests-regex "${1}"
    fi
    make test_analysis
}

function findQtSDKAndBoost() {
    echo "Attempting to locate QtSDK ..."
    QT_PATH="$(for i in /c/QtSDK/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
    if [[ ! -e "${QT_PATH}" ]]; then
        echo "Warning: QtSDK was not found in expected location (in C:\\), searching the entire filesystem..."
        for base in $(find '/c' -type d -name 'QtSDK'); do
            QT_PATH="$(for i in "${base}"/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
            break
        done
        if [[ ! -e "${QT_PATH}" ]]; then
            echo "Error: unable to locate QtSDK."
            exit 1
        fi
    fi
    echo "Qt libraries found in: ${QT_PATH}"
    export PATH="${QT_PATH}:${PATH}"

    if [[ -z "${BOOST_DIR}" ]]; then
        BOOST="$(for i in "${PROGRAMFILES}"/boost/boost_*; do echo $i; break; done)"
        if [[ ! -e "${BOOST}" ]]; then
            echo "Error: unable to locate boost."
            exit 1
        else
            export BOOST_DIR="${BOOST}"
        fi
    fi
}

function clean() {
    requirePrograms 'cmake' 'make'

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
    requirePrograms 'git'

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
    echo "This is a tool for doing various things with the code here."
    echo "Version: ${VERSION}"
    echo ""
    echo "Options:"
    echo "    -b     Build."
    echo "    -t     Build and run all tests."
    echo "    -T <r> Build and run only the tests matching regular expression <r>."
    echo "    -c     Clean up the directories by removing all automatically generated files."
    echo "    -s     Synchronize with the central repository."
    echo "    -l     Count number of lines source code files."
    echo "    -q     Print some statistics regarding generated binary files."
    echo "    -h     Print this message."
    echo "    -V     Print version of this script."
    echo ""
    echo "The script might be run with these variables: (value in parentheses is the default value)"
    echo "    bt=<b>         Set build type to <b>, options are: 'Debug', 'Release', and 'MinSizeRel' (all: 'Debug')."
    echo "    cov=<on|off>   Configure build for coverage analysis (normal: 'off', tests: 'on')."
    echo "    val=<on|off>   Turn on/off Valgrind:Memcheck during automated test run. (normal: n/a, tests: 'on')."
    echo "    color=<on|off> Turn on/off color output. (normal: 'on', tests: 'off')."
    echo "    arch=<arch>    Set target architecture to build for (options: 'x86', and 'x86_64'). (all: <native>)."
    echo "    os=<os>        Set target operating system to build for (options: 'Linux', 'Windows', and 'Darwin'). (all: <native>)."
    echo ""
    echo "Example:"
    echo "    bt=Release cov=off ./tool.sh -t     # Run tests without coverage and with binaries built for release."
    echo "    bt=MinSizeRel ./tool.sh -csb        # Clear, synchronize GIT, and build for minimum size binaries."
    echo ""
    echo "Order of options does not matter because order of operations is fixed (see the script code for details)."
    echo ""
}

function main() {
    set +o pipefail
    cd "$(dirname "$(readlink -n -f "${0}")" )"
    echo "Using up to ${CPU_CORES} CPU cores."

    local -a opts

    # Parse CLI options using `getopts' utility.
    while getopts ":hVcltbsmqT:" opt; do
        case $opt in
            b) opts[0]=1;;
            t) opts[1]=1;;
            T) opts[2]="${OPTARG}";;
            c) opts[3]=1;;
            l) opts[4]=1;;
            s) opts[5]=1;;
            q) opts[6]=1;;
            h) printHelp; exit;;
            V) printVersion; exit;;
            ?) unknownOption "$(basename "${0}")"; exit 1;;
        esac
    done

    if (( 0 == ${#opts[@]} )); then
        printHelp
        exit 1
    fi

    if [[ ! -z "${opts[3]}" ]]; then
        clean
    fi

    if [[ ! -z "${opts[5]}" ]]; then
        repoSync
    fi

    if [[ ! -z "${opts[4]}" ]]; then
        countLines
    fi

    if [[ ! -z "${opts[2]}" ]]; then
        tests "${opts[2]}"
    elif [[ ! -z "${opts[1]}" ]]; then
        tests
    elif [[ ! -z "${opts[0]}" ]]; then
        build
    fi

    if [[ ! -z "${opts[6]}" ]]; then
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

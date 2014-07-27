#! /bin/bash

readonly VERSION="0.3.1"
declare -ir DETECTED_CPU_CORES=$( which lscpu &> /dev/null && lscpu 2> /dev/null | \
                                  gawk 'BEGIN { n = 1 } END { print(n) } /^CPU\(s\)/ { n = $2; exit }' || echo 1 )

declare -i CPU_CORES=${DETECTED_CPU_CORES}
declare -i PP=${CPU_CORES}

declare allowParallelCovTest=1
declare cmakeGenerator='<invalid>'
declare makeProgram='echo "Error: No make selected"; '

function initialize() {
    cd "$(dirname "$(readlink -n -f "${0}")" )"
    echo "Using up to ${CPU_CORES} CPU core(s) (${DETECTED_CPU_CORES} auto-detected)."
    set +o pipefail

    PP=$(( ( 1 == CPU_CORES ) ? 1 : ( CPU_CORES + 1 ) ))

    case "$(uname -o)" in
        'GNU/Linux')
            # Build on GNU/Linux.
            cmakeGenerator='Unix Makefiles'
            makeProgram='make'
            ;;

        'Cygwin'|'Msys')
            # Build on Windows.
            cmakeGenerator='MinGW Makefiles'
            makeProgram='mingw32-make'
            allowParallelCovTest=0
            val='off' # Disable Valgrind, on Windows there is no Valgrind.
            ;;

        *)
            # Unsupported OS.
            echo "Operating system \"$( uname -o )\" is not supported." > /dev/stderr
            exit 1
            ;;
    esac
}

function build() {
    requirePrograms 'cmake' ${makeProgram} 'gawk' 'sed'

    cmake -G "${cmakeGenerator}"                \
          -DTEST_COVERAGE=OFF                   \
          -DTARGET_OS="${os}"                   \
          -DTARGET_ARCH="${arch}"               \
          -DBUILD_TESTING=${btest:-on}          \
          -DCOLOR_GCC=${color:-on}              \
          -DCMAKE_BUILD_TYPE=${bt:-Debug}       \
          -DCMAKE_COLOR_MAKEFILE=${color:-on}   \
          -DMDS_VARIANT=${variant:=PROFI}       \
          . || exit 1

    if ! ${makeProgram} -j ${PP} --keep-going; then
        echo -e "\nBuild FAILED!\n" > /dev/stderr
        exit 1
    fi
}

function tests() {
    local -r BUILD_LOG="tests/results/BuildLog.log"
    local -r TIME_FILE="tests/results/startTime.log"
    local -i TEST_PP=${CPU_CORES}

    requirePrograms 'cmake' ${makeProgram} 'xsltproc' 'gawk' 'sed'

    if [[ ! "${val}" =~ [oO][fF][fF] ]]; then
        requirePrograms 'valgrind'
    fi
    if [[ ! "${cov}" =~ [oO][fF][fF] ]]; then
        requirePrograms 'gcov'
        if (( ! allowParallelCovTest )); then
            TEST_PP=1
        fi
        cov='on'
    else
        cov='off'
    fi

    if [[ -e tests/results ]]; then
        rm -rf tests/results || exit 1
    fi

    mkdir tests/results || exit 1
    date +'%s' > "${TIME_FILE}"

    cmake -G "${cmakeGenerator}"                \
          -DCOLOR_GCC=${color:-off}             \
          -DTARGET_OS="${os}"                   \
          -DTARGET_ARCH="${arch}"               \
          -DTEST_COVERAGE=${cov}                \
          -DTEST_MEMCHECK=${val:-on}            \
          -DCMAKE_BUILD_TYPE=${bt:-Debug}       \
          -DCMAKE_COLOR_MAKEFILE=${color:-off}  \
          . 2>&1                                \
        | tee "${BUILD_LOG}"

    ${makeProgram} -j ${PP} --keep-going 2>&1 | tee -a "${BUILD_LOG}"

    if [[ -z "${1}" ]]; then
        ctest -j ${TEST_PP}
    else
        ctest -j ${TEST_PP} --tests-regex "${1}"
    fi

    bash tests/results2html.sh ${cov} ${CPU_CORES}
}

function clean() {
    requirePrograms 'cmake' ${makeProgram}

    cmake .
    ${makeProgram} clean

    for dirGlob in 'CMakeFiles' 'Testing' '_CPack_Packages'; do
        rm -rfv $(find -type d -name "${dirGlob}")
    done

    for fileGlob in 'Makefile' 'Doxyfile' 'DartConfiguration.tcl' 'CPackSourceConfig.cmake' 'CPackConfig.cmake' \
                    '*-Linux.*' '*~' '*.a' '*.so' 'moc_*.cxx' '.directory' '*.dll' '*.exe' 'CMakeCache.txt'     \
                    'cmake_install.cmake' 'CTestTestfile.cmake' 'install_manifest.txt'
    do
        rm -fv $(find -type f -name "${fileGlob}")
    done
}

function repoSync() {
    requirePrograms 'git'

    local -r REMOTE_REPOSITORY=$( git remote show | head -n 1 )

    git commit -a -m "(no comment)"
    git pull ${REMOTE_REPOSITORY} master
    git push ${REMOTE_REPOSITORY} master
}

function verify() {
    requirePrograms 'cmake' 'git' 'gawk' 'sed' ${makeProgram}

    tmp="$( mktemp --directory )"
    git clone --local --no-hardlinks -- "$( readlink -n -f .. )" "${tmp}" || exit 1
    bt=Release bash "${tmp}/IDE/tool.sh" -b && bt=Debug bash "${tmp}/IDE/tool.sh" -b
    rm -rf "${tmp}"
}

function requirePrograms() {
    for program in "${@}"; do
        if ! which "${program}" &> /dev/null; then
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
    echo "    -j <n> Set <n> CPU cores, this option overrides auto-detection."
    echo "    -l     Count number of lines source code files."
    echo "    -q     Print some statistics regarding generated binary files."
    echo "    -h     Print this message."
    echo "    -V     Print version of this script."
    echo "    -v     Verify that the project can be compiled using the files currently available in local repository."
    echo ""
    echo "The script might be run with these variables: (value in parentheses is the default value)"
    echo "    bt=<b>          Set build type to <b>, options are: 'Debug', 'Release', and 'MinSizeRel' (all: 'Debug')."
    echo "    cov=<on|off>    Configure build for coverage analysis (normal: 'off', tests: 'on')."
    echo "    val=<on|off>    Turn on/off Valgrind:Memcheck during automated test run. (normal: n/a, tests: 'on')."
    echo "    color=<on|off>  Turn on/off color output. (normal: 'on', tests: 'off')."
    echo "    arch=<arch>     Set target architecture to build for (options: 'x86', and 'x86_64'). (all: <native>)."
    echo "    os=<os>         Set target operating system to build for (options: 'Linux', 'Windows', and 'Darwin') (all: <native>)."
    echo "    btest=<on|off>  Build test binaries  (normal: 'on', tests: [always on])."
    echo "    variant=<PROFI|TRIAL|NONCOMMERCIAL> Set MDS_VARIANT."
    echo ""
    echo "Example:"
    echo "    bt=Release cov=off ./tool.sh -t     # Run tests without coverage and with binaries built for release."
    echo "    bt=MinSizeRel ./tool.sh -csb        # Clear, synchronize GIT, and build for minimum size binaries."
    echo ""
    echo "Order of options does not matter because order of operations is fixed (see the script code for details)."
    echo ""
}

function main() {
    local -A opts

    # Parse CLI options using `getopts' utility.
    while getopts ":hVvcltbsmqT:j:" opt; do
        case $opt in
            b) opts['b']=1;;
            t) opts['t']=1;;
            T) opts['T']="${OPTARG}";;
            c) opts['c']=1;;
            l) opts['l']=1;;
            s) opts['s']=1;;
            q) opts['q']=1;;
            v) opts['v']=1;;
            j)
                if [[ "${OPTARG}" =~ ^[0-9]+$ ]]; then
                    CPU_CORES=${OPTARG}
                else
                    echo "Error: \`${OPTARG}' is not a valid number of processor cores."
                    exit 1
                fi
                ;;
            h) printHelp; exit;;
            V) printVersion; exit;;
            ?) unknownOption "$(basename "${0}")"; exit 1;;
        esac
    done

    initialize

    if (( 0 == ${#opts[@]} )); then
        printHelp
        exit 1
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

    if [[ ! -z "${opts['v']}" ]]; then
        verify
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
            if [[ "$i" =~ \./CMakeFiles/ ]]; then
                continue
            elif [[ -f "$i" ]]; then
                if file "$i" | grep 'ELF' &>/dev/null; then
                    ls -l $i
                fi
            fi
        done | tee /dev/stderr | gawk 'BEGIN {b=0;n=0} END {printf("%d: %.2f MB\n", n, b/(1024*1024))} {b+=$5;n++}'
    fi
}

main "$@"

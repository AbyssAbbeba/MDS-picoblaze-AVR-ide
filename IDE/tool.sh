#! /bin/bash

readonly VERSION=0.2

declare -i options_b=0
declare -i options_c=0
declare -i options_T=0
declare -i options_l=0
declare -i options_q=0
declare -i options_s=0
declare -i options_t=0
declare -ir CPU_CORES=$( which lscpu > /dev/null && lscpu | \
                         gawk 'BEGIN { n = 1 } END { print(n) } /^CPU\(s\)/ { n = $2; exit }' || echo 1 )
declare -ir PP=$(( CPU_CORES + 1 ))

echo "Using up to ${CPU_CORES} CPU cores."

function build() {
    if [ "$(uname -o)" == "Msys" ]; then
        # Build on Windows.
        QT_PATH="$(for i in /c/QtSDK/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
        export PATH="${QT_PATH}:${PATH}"
        cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_COVERAGE=OFF -DCOLOR_GCC=ON -DCMAKE_COLOR_MAKEFILE=OFF \
              -G "MSYS Makefiles" . || exit 1
    else
        # Build on a POSIX system.
        cmake -DCMAKE_BUILD_TYPE=Debug -DTEST_COVERAGE=OFF -DCOLOR_GCC=ON -DCMAKE_COLOR_MAKEFILE=OFF . || exit 1
    fi

    make -j${PP}
}

function repoSync() {
    local -r REPO=$( git remote show | head -n 1 )

    git commit -a -m "."
    git pull ${REPO} master
    git push ${REPO} master
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
    if [[ "${1}" != "0" ]]; then
        make -j${PP} 2>&1 | tee -a "${BUILD_LOG}" || status=0
    fi
    echo ${status} > "${STATUS_FILE}"

    ctest -j${PP}
    make test_analysis
}

function printVersion() {
    printf "%s\n" "$VERSION"
    exit 1
}

function printHelp() {
    printf "This is a tool for doing various things with the code here.\n"
    printf "Version: %s\n" "$VERSION"
    printf "\n"
    printf "Options:\n"
    printf "    -b    Build.\n"
    printf "    -h    Print this message.\n"
    printf "    -t    Build and run all tests.\n"
    printf "    -V    Print version of this script.\n"
    printf "    -T    Run all tests (without build).\n"
    printf "    -s    Synchronize with the central repository.\n"
    printf "    -l    Count number of lines in .cxx, .cpp, .c, and .h files.\n"
    printf "    -q    Print some statistics regarding generated binary files.\n"
    printf "    -c    Clean up the directories by removing all automatically generated files.\n"
    printf "\n"
    printf "Order of options does not matter.\n"
    exit 1
}

function unknownOption() {
    printf "Unknow option. Try to run './${1} -h' to get help.\n\n"
    printHelp
}

function clean() {
    cmake .
    make clean

    rm -fv $(find -type f -name '*~')
    rm -fv $(find -type f -name '*.a')
    rm -fv $(find -type f -name '*.so')
    rm -fv $(find -type f -name 'moc_*.cxx')
    rm -fv $(find -type f -name '.directory')
    rm -fv $(find -type f -name 'CMakeCache.txt')
    rm -fv $(find -type f -name 'cmake_install.cmake')
    rm -fv $(find -type f -name 'CTestTestfile.cmake')
    rm -fv $(find -type f -name 'install_manifest.txt')
    rm -rfv $(find -type d -name 'CMakeFiles')
    rm -rfv $(find -type d -name 'Testing')
    rm -rfv '_CPack_Packages'

    rm -fv 'Doxyfile'
    rm -fv 'DartConfiguration.tcl'
    rm -fv 'CPackSourceConfig.cmake'
    rm -fv 'CPackConfig.cmake'
    rm -fv *-Linux.*

    rm -fv $(find -type f -name 'Makefile')

#     while true; do
#         if [[ "${options_y}" != "1" ]]; then
#             printf "Remove also Makefiles? [yes]: "
#             read response
#         fi
#
#         if [[ "${options_y}" == "1" || "$response" == "yes" || "$response" == "y" || "$response" == "" ]]; then
#             rm -fv $(find -type f -name 'Makefile')
#             break
#         elif [[ "$response" == "no" || "$response" == "n" ]]; then
#             break
#         else
#             printf "Please respond 'yes' or 'no'.\n\n"
#         fi
#     done
}

function countLines() {
    local -r tempFile=$(mktemp)

    find -type f -name '*.asm' > "$tempFile"
    find -type f -name '*.psm' >> "$tempFile"
    sort "$tempFile" | while read f; do
        wc -lc "$f"
    done | gawk '
        BEGIN {
            printf("\n")
            printf(" LINES  BYTES  FILE\n")
            printf(" -----  -----  -------------------------------------------------------------\n")

            t=0
            l=0
            c=0
        }
        END {
            printf(" -----  -----  -------------------------------------------------------------\n")
            printf(" LINES  BYTES  FILE\n")
            printf("\n")
            printf(" +----- In total ------+    +-------- In average --------+\n")
            printf(" | %8d lines      |    | %8.2f bytes / line      |\n", l, c/l)
            printf(" | %8.2f megabytes  |    | %8.2f kilobytes / file  |\n", c/(1024*1024), c/(t*1024))
            printf(" | %8d files      |    | %8.2f lines / file      |\n", t, l/t)
            printf(" +---------------------+    +----------------------------+\n")
            printf("\n")
        }
        {
            t++
            l+=$1
            c+=$2
            printf("%6d %6d  %s\n", $1, $2, $3)
        }
    '

    find -type f -name '*.cxx' > "$tempFile"
    find -type f -name '*.awk' >> "$tempFile"
    find -type f -name '*.sh' >> "$tempFile"
    find -type f -name '*.cpp' >> "$tempFile"
    find -type f -name '*.c' >> "$tempFile"
    find -type f -name '*.h' >> "$tempFile"
    find -type f -name '*.l' >> "$tempFile"
    find -type f -name '*.y' >> "$tempFile"
    find -type f -name 'CMakeLists.txt' >> "$tempFile"

    sort "$tempFile" | while read f; do
        wc -lc "$f"
    done | gawk '
        BEGIN {
            printf("\n")
            printf(" LINES  BYTES  FILE\n")
            printf(" -----  -----  -------------------------------------------------------------\n")

            t=0
            l=0
            c=0
        }
        END {
            printf(" -----  -----  -------------------------------------------------------------\n")
            printf(" LINES  BYTES  FILE\n")
            printf("\n")
            printf(" +----- In total ------+    +-------- In average --------+\n")
            printf(" | %8d lines      |    | %8.2f bytes / line      |\n", l, c/l)
            printf(" | %8.2f megabytes  |    | %8.2f kilobytes / file  |\n", c/(1024*1024), c/(t*1024))
            printf(" | %8d files      |    | %8.2f lines / file      |\n", t, l/t)
            printf(" +---------------------+    +----------------------------+\n")
            printf("\n")
        }
        {
            t++
            l+=$1
            c+=$2
            printf("%6d %6d  %s\n", $1, $2, $3)
        }
    '

    rm "$tempFile"
}

function main() {
    local -i optTaken=0

    cd "$(dirname "$(readlink -n -f "${0}")" )"

    # Parse CLI options using `getopts' utility
    while getopts ":hVcltTbsmq" opt; do
        optTaken=1

        case $opt in
            h) printHelp;;
            V) printVersion;;
            b) options_b=1;;
            t) options_t=1;;
            T) options_T=1;;
            c) options_c=1;;
            l) options_l=1;;
            s) options_s=1;;
            q) options_q=1;;
            ?) unknownOption "$(basename "${0}")";;
        esac
    done

    if (( ! $optTaken )); then
        printHelp
    fi

    if (( ${options_c} )); then
        clean
    fi
    if (( ${options_s} )); then
        repoSync
    fi
    if (( ${options_l} )); then
        countLines
    fi
    if (( ${options_T} )); then
        tests 0
    elif (( ${options_t} )); then
        tests 1
    elif (( ${options_b} )); then
        build
    fi
    if (( ${options_q} )); then
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

#! /bin/bash

readonly VERSION=0.1

declare -i options_a=0
declare -i options_b=0
declare -i options_c=0
declare -i options_y=0
declare -i options_l=0
declare -i options_s=0
declare -i options_m=0

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

    while true; do
        if [ "${options_y}" != "1" ]; then
            printf "Remove also Makefiles? [yes]: "
            read response
        fi

        if [[ "${options_y}" == "1" || "$response" == "yes" || "$response" == "y" || "$response" == "" ]]; then
            rm -fv $(find -type f -name 'Makefile')
            break
        elif [[ "$response" == "no" || "$response" == "n" ]]; then
            break
        else
            printf "Please respond 'yes' or 'no'.\n\n"
        fi
    done
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

function buildAll() {
    build
    make manual
    make doxygen
    make test
    make package
}

function execCMake() {
    if [ "$(uname -o)" == "Msys" ]; then
        # Build on Windows.
        QT_PATH="$(for i in /c/QtSDK/Desktop/Qt/*/mingw/bin; do echo $i; break; done)"
        export PATH="${QT_PATH}:${PATH}"
        cmake -DCMAKE_BUILD_TYPE=Debug -G "MSYS Makefiles" . || exit 1
    else
        # Build on a POSIX system.
        cmake -DCMAKE_BUILD_TYPE=Debug . || exit 1
    fi
}

function build() {
    # Build in n separate processes where n is the number of CPU cores plus one.
    execCMake
    which lscpu > /dev/null && make -j$(lscpu | gawk '/^CPU\(s\)/ {printf("%d", ($2+1))}') || make
}

function repoSync() {
    git commit -a -m "."
    git pull mms master
    git push mms master
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
    printf "    -a    Build EVERYTHING, and run the tests.\n"
    printf "    -c    Clean up the directories by removing all temporary files.\n"
    printf "    -l    Count number of lines in .cxx, .cpp, .c, and .h files.\n"
    printf "    -y    Automatically assume a positive response to any prompt.\n"
    printf "    -V    Print version of this script.\n"
    printf "    -s    Synchronize with the central repository.\n"
    printf "    -h    Print this message.\n"
    printf "\n"
    printf "Order of options doesn't matter.\n"
    exit 1
}

function unknownOption() {
    printf "Unknow option. Try to run './${1} -h' to get help.\n\n"
    printHelp
}

function main() {
    local -i optTaken=0

    cd "$(dirname "${0}")"

    # Parse CLI options using `getopts' utility
    while getopts ":hVcylabsm" opt; do
        optTaken=1

        case $opt in
            h) printHelp;;
            V) printVersion;;
            b) options_b=1;;
            a) options_a=1;;
            c) options_c=1;;
            y) options_y=1;;
            l) options_l=1;;
            s) options_s=1;;
            m) options_m=1;;
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
    if (( ${options_m} && !${options_b} && !${options_a} )); then
        execCMake
    fi
    if (( ${options_a} )); then
        buildAll
    fi
    if (( ${options_b} && !${options_a} )); then
        build
    fi
}

main "$@"

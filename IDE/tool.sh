#! /bin/bash

readonly VERSION=0.1

declare -i options_a=0
declare -i options_c=0
declare -i options_y=0
declare -i options_l=0

function clean() {
	cmake .
	make clean

	rm -rfv "docs/doxygen/html"
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
	
	find -type f -name '*.cxx' >> "$tempFile"
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
			l=0
			c=0
		}
		END {
			printf("\nIn total:\n%8d lines,\n%8.2f megabytes.\n", l, c/(1024*1024))
		}
		{
			l+=$1
			c+=$2
			printf("%5d %6d  %s\n", $1, $2, $3)
		}
	'

	rm "$tempFile"
}

function buildAll() {
	if [ "$(uname -o)" == "Msys" ]; then
		# Build on Windows
		export PATH="/c/QtSDK/Desktop/Qt/4.8.0/mingw/bin:/c/MinGW/swigwin-2.0.4:${PATH}"
		cmake -G "MSYS Makefiles" . || exit 1
	else
		# Build on a POSIX system
		cmake . || exit 1
	fi

	make doc
	make all
	make test
	make package
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
	printf "    -a    Build EVERYTHING, and run the tests.\n"
	printf "    -c    Clean up the directories by removing all temporary files.\n"
	printf "    -l    Count number of lines in .cxx, .cpp, .c, and .h files.\n"
	printf "    -y    Automatically assume a positive response to any prompt.\n"
	printf "    -V    Print version of this script.\n"
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
	while getopts ":hVcyla" opt; do
		optTaken=1

		case $opt in
			h) printHelp;;
			V) printVersion;;
			a) options_a=1;;
			c) options_c=1;;
			y) options_y=1;;
			l) options_l=1;;
			?) unknownOption "$(basename "${0}")";;
		esac
	done

	if (( ! $optTaken )); then
		printHelp
	fi

	if (( ${options_c} )); then
		clean
	fi
	if (( ${options_l} )); then
		countLines
	fi
	if (( ${options_a} )); then
		buildAll
	fi
}

main "$@"

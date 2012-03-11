#! /bin/bash

readonly VERSION=0.1

declare -A options=( [c]=0 [y]=0 [w]=0 [l]=0 )

function clean() {
	make clean 2> /dev/null

	rm -rfv "docs/doxygen/html"
	rm -fv $(find -type f -name '*~')
	rm -fv $(find -type f -name 'CMakeCache.txt')
	rm -fv $(find -type f -name 'cmake_install.cmake')
	rm -rfv $(find -type d -name 'CMakeFiles')

	while true; do
		if [ "${options[y]}" != "1" ]; then
			printf "Remove also Makefiles? [yes]: "
			read response
		fi

		if [[ "${options[y]}" == "1" || "$response" == "yes" || "$response" == "y" || "$response" == "" ]]; then
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

	sort "$tempFile" | while read f; do
		wc -lc "$f"
	done | gawk '
		BEGIN {
			l=0
			c=0
		}
		END {
			printf("\nIn total:\n%8d lines,\n%8d bytes.\n", l, c)
		}
		{
			l+=$1
			c+=$2
			printf("%5d %6d  %s\n", $1, $2, $3)
		}
	'

	rm "$tempFile"
}

function buildOnWindows() {
	cmake -G "MSYS Makefiles" . && make
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
	printf "    -c    Clean up the directories by removing all temporary files.\n"
	printf "    -w    Build on Microsoft® Windows® OS (in MSYS environment).\n"
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
	while getopts ":hVcywl" opt; do
		optTaken=1

		case $opt in
			h) printHelp;;
			V) printVersion;;
			c) options[c]=1;;
			y) options[y]=1;;
			w) options[w]=1;;
			l) options[l]=1;;
			?) unknownOption "$(basename "${0}")";;
		esac
	done
	
	if (( ! $optTaken )); then
		printHelp
	fi

	if (( ${options[c]} )); then
		clean
	fi
	if (( ${options[l]} )); then
		countLines
	fi
	if (( ${options[w]} )); then
		buildOnWindows
	fi
}

main "$@"

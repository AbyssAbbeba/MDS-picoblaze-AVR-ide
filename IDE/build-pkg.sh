#! /bin/bash

# Parse CLI options using `getopts' utility.
declare CPU_CORES=""
while getopts ":j:" opt; do
    case $opt in
        j)
            if [[ "${OPTARG}" =~ ^[0-9]+$ ]]; then
                CPU_CORES="-j ${OPTARG}"
            else
                echo "Error: \`${OPTARG}' is not a valid number of processor cores."
                exit 1
            fi
            ;;
        ?) unknownOption "$(basename "${0}")"; exit 1;;
    esac
done

cd "$(dirname "$(readlink -n -f "${0}")" )"

./tool.sh -c || exit $?

for grade in Premium; do # <-- {Basic, Premium, Professional, Ultimate}
    for variant in  Commercial; do # <-- {Commercial, Noncommercial, Trial}
        for target in PicoBlaze; do # <-- {PicoBlaze, AVR8, All}
            btest=off bt=Debug grade=${grade} target=${target} variant=${variant} ./tool.sh -b ${CPU_CORES} && \
            cpack . || exit $?
        done
    done
done

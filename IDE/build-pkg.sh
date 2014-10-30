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

btest=off bt=Release grade=Basic target=PicoBlaze variant=Noncommercial ./tool.sh -b ${CPU_CORES} && cpack . || exit $?

for variant in Commercial; do # <-- {Commercial, Noncommercial, Trial}
    for grade in Basic Premium; do # <-- {Basic, Premium, Professional, Ultimate}
        for target in PicoBlaze; do # <-- {PicoBlaze, AVR8, All}
            btest=off bt=Release grade=${grade} target=${target} variant=${variant} ./tool.sh -b ${CPU_CORES} && \
            cpack . || exit $?
        done
    done
done

btest=off bt=Release grade=Premium target=PicoBlaze variant=Trial ./tool.sh -b ${CPU_CORES} && cpack . || exit $?

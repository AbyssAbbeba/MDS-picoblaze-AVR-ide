#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

./tool.sh -c || exit $?

for variant in Commercial Noncommercial Trial; do # <-- {Commercial, Noncommercial, Trial}
    for grade in Professional; do # <-- {Basic, Premium, Professional, Ultimate}
        for target in PicoBlaze; do # <-- {PicoBlaze, AVR8, All}
            btest=off bt=Release grade=${grade} target=${target} variant=${variant} ./tool.sh -b && cpack . || exit $?
        done
    done
done

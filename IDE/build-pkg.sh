#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

./tool.sh -c || exit $?

for variant in Commercial; do # <-- {Commercial, Noncommercial, Trial}
    for grade in Basic Premium; do # <-- {Basic, Premium, Professional, Ultimate}
        for target in PicoBlaze; do # <-- {PicoBlaze, AVR8, All}
            btest=off bt=Release grade=${grade} target=${target} variant=${variant} ./tool.sh -b && cpack . || exit $?
        done
    done
done

btest=off bt=Release grade=Basic target=PicoBlaze variant=Noncommercial ./tool.sh -b && cpack . || exit $?
btest=off bt=Release grade=Premium target=PicoBlaze variant=Trial ./tool.sh -b && cpack . || exit $?

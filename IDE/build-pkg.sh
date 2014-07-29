#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

./tool.sh -c || exit $?

for variant in Profi Trial Noncommercial; do
    btest=off bt=Release variant=${variant} ./tool.sh -b || exit $?
    cpack . || exit $?
done

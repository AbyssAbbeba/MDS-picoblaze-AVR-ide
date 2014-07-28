#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )"

./tool.sh -c || exit $?

for variant in Profi Trial Noncommercial; do
    bt=Release variant=${variant} ./tool.sh -b || exit $?
done

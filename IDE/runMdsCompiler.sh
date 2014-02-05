#! /bin/bash

# for i in {30..37}; do echo -e "\E[${i}m${i}\E[m "; done

set +o pipefail

declare -i color="${1}"
shift

declare -a args=( "${@}" )
declare -r os="$(uname -o)"

if [[ "${os}" == 'Cygwin' || "${os}" == 'Msys' ]]; then
    for (( i=0; i < ${#}; i++ )); do
        args[${i}]="$( sed 's"\\"/"g' <<< "${args[${i}]}" )"
    done
fi

if [[ "0" == ${color} || ! -t 1 || -z "${TERM}" || "${TERM}" == 'dumb' ]]; then
    exec "${args[@]}" 1> /dev/null
    exit $?
fi

exec "${args[@]}" | gawk '
    {
        sub ( /: Error: /,   ": \033[41;30merror:\033[m ",   $0 )
        sub ( /: Warning: /, ": \033[43;30mwarning:\033[m ", $0 )
        sub ( /: Remark: /,    ": \033[33;1mnote:\033[m ",     $0 )

        sub ( /^([[:alpha:]]:)?[^ :]+(:[\.[:digit:]]+((:|-)[\.[:digit:]]+)?)?:/, "\033[30;1m&\033[m", $0 )

        print ( $0 )
    }
'

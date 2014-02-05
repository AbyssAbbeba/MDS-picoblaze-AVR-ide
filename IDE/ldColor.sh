#! /bin/bash

# for i in {30..37}; do echo -e "\E[${i}m${i}\E[m "; done

declare -a args=( "${@}" )
declare -r os="$(uname -o)"

if [[ "${os}" == 'Cygwin' || "${os}" == 'Msys' ]]; then
    for (( i=0; i < ${#}; i++ )); do
        args[${i}]="$( sed 's"\\"/"g' <<< "${args[${i}]}" )"
    done
fi

if [[ ! -t 1 || -z "${TERM}" || "${TERM}" == 'dumb' ]]; then
    exec "${args[@]}"
    exit $?
fi

exec "${args[@]}" 2>&1 | gawk '
    {
        sub ( /: error: /,               ": \033[41;30merror:\033[m ",              $0 )
        sub ( /: undefined reference/,   ": \033[41;30mundefined reference\033[m ", $0 )
        sub ( /^([[:alpha:]]:)?[^ :]+:/, "\033[30;1m&\033[m",                       $0 )

        print ( $0 )
    }
'

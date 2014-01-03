#! /bin/bash

cd "$(dirname "$(readlink -n -f "${0}")" )/results" || exit 1

declare -r TEST_SUB_DIR="$(readlink -f "${1}")"
declare -r HTML_FILE="${2}"
declare -r FILE_PREFIX="${3}"
declare -r MAIN_DIR="$(readlink -f ../..)"
declare -a SRC_FILES=( $(find -L "${MAIN_DIR}" -type f -name '*.cxx') \
                       $(find -L "${MAIN_DIR}" -type f -name '*.cpp') \
                       $(find -L "${MAIN_DIR}" -type f -name '*.c'  ) \
                       $(find -L "${MAIN_DIR}" -type f -name '*.h'  ) )

declare -a GCOV_FILES=( $(find -L "${TEST_SUB_DIR}" -type f -name '*.gcov' 2> /dev/null ) )
declare -A RESULTS
declare -A FILENAMES
declare -i dotCounter=0

function isInSources() {
    for (( i=0; i < ${#SRC_FILES[@]}; i++ )); do
        if [[ "${1}" == "${SRC_FILES[$i]}" ]]; then
            return 0
        fi
    done
    return 1
}

dotCounter=0
echo -n "Processing ${#GCOV_FILES[@]} gcov input files ..."
for gcovFile in "${GCOV_FILES[@]}"; do
    filename=""
    lineno=0
    inResults=0

    let dotCounter++
    if (( 10 == ${dotCounter} )); then
        dotCounter=0
        echo -n "."
    fi

    unset infile
    IFS=$'\n' read -r -d '' -a infile < "${gcovFile}"
    fsize=${#infile[@]}

    for (( idx=0; idx < fsize; idx++ )); do
        line="${infile[${idx}]}"

        if [[ "${line}" =~ ^' '*-:' '*0:Source: ]]; then
            filename="$(readlink -f "${line:${#BASH_REMATCH[0]}}")"
            line="${line:0:${#BASH_REMATCH[0]}}${filename}"
            if ! isInSources "${filename}"; then
                filename=""
                break
            fi
            for fn in ${!FILENAMES[*]}; do
                if [[ "${filename}" == "${fn}" ]]; then
                    inResults=1
                    break
                fi
            done
        elif [[ "${line}" =~ ^' '*-:' '*0: ]]; then
            continue
        fi

        if (( 1 == ${inResults} )); then
            orgLine="${RESULTS[${filename}:${lineno}]}"
            if [[ "${line}" =~ ^' '*#####: || "${line}" =~ ^' '*-: ]]; then
                line="${orgLine}"
            elif [[ "${line}" =~ ^' '*[0-9]+: && "${orgLine}" =~ ^' '*[0-9]+: ]]; then
                [[ "${line}" =~ ^' '*[0-9]+: ]]
                n=$(( ${#BASH_REMATCH[0]} - 1))
                a="${line:0:${n}}"

                [[ "${orgLine}" =~ ^' '*[0-9]+: ]]
                n=$(( ${#BASH_REMATCH[0]} - 1))
                b="${orgLine:0:${n}}"

                line="$(( a + b ))${orgLine:n}"
            fi
        fi

        RESULTS[${filename}:${lineno}]="${line}"
        let lineno++
    done

    if [[ "${filename}" != "" && "${inResults}" -eq 0 ]]; then
        FILENAMES[${filename}]="${lineno}"
    fi
done
echo " [DONE]"

if (( 0 == ${#FILENAMES[*]} )); then
    echo "No coverage analysis data available, exiting."
    exit
fi

echo -n "Generating ${#FILENAMES[*]} HTML output files ..."

echo "<?xml version='1.0' encoding='utf-8'?>" > "${HTML_FILE}"
echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"" >> "${HTML_FILE}"
echo "        \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" >> "${HTML_FILE}"
echo "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">" >> "${HTML_FILE}"
echo "<head>" >> "${HTML_FILE}"
echo "    <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\" />" >> "${HTML_FILE}"
echo "    <meta name=\"content-language\" content=\"en\"/>" >> "${HTML_FILE}"
echo "    <link rel=\"shortcut icon\" href=\"favicon.png\" type=\"image/png\"/>" >> "${HTML_FILE}"
echo "    <title> ${TEST_SUB_DIR}: Test Coverage Analysis </title>" >> "${HTML_FILE}"
echo "    <style type=\"text/css\">" >> "${HTML_FILE}"
echo "        a {" >> "${HTML_FILE}"
echo "                text-decoration: underline;" >> "${HTML_FILE}"
echo "                color: #0000FF;" >> "${HTML_FILE}"
echo "        }" >> "${HTML_FILE}"
echo "        a:hover {" >> "${HTML_FILE}"
echo "                text-decoration: none;" >> "${HTML_FILE}"
echo "                color: #0088FF;" >> "${HTML_FILE}"
echo "        }" >> "${HTML_FILE}"
echo "    </style>" >> "${HTML_FILE}"
echo "</head>" >> "${HTML_FILE}"
echo "<body style=\"text-align: center\">" >> "${HTML_FILE}"
echo "    <div>" >> "${HTML_FILE}"
echo "        <a href=\"index.html\"><img src=\"logo.png\" width=\"309\" height=\"83\" alt=\"\"/></a>">> "${HTML_FILE}"
echo "        <br/><i>Moravia Microsystems, s.r.o.</i><br/><br/>" >> "${HTML_FILE}"
echo "    </div>" >> "${HTML_FILE}"
echo "    <table style=\"width: 90%; margin: 0 auto\">" >> "${HTML_FILE}"
echo "        <col/>" >> "${HTML_FILE}"
echo "        <col/>" >> "${HTML_FILE}"
echo "        <col/>" >> "${HTML_FILE}"
echo "        <col/>" >> "${HTML_FILE}"
echo "        <col/>" >> "${HTML_FILE}"
echo "        <tr style=\"background-color: skyblue\">" >> "${HTML_FILE}"
echo "                <th colspan=\"5\">Test Coverage Analysis for $(basename ${TEST_SUB_DIR})</th>" >> "${HTML_FILE}"
echo "        </tr>" >> "${HTML_FILE}"
echo "        <tr style=\"background-color: #ffffc0\">" >> "${HTML_FILE}"
echo "            <th style=\"width: 10%\"> Executed blocks </th>" >> "${HTML_FILE}"
echo "            <th style=\"width: 10%\"> Missed blocks </th>" >> "${HTML_FILE}"
echo "            <th style=\"width: 10%\"> Blocks total </th>" >> "${HTML_FILE}"
echo "            <th style=\"width: 10%\"> Coverage </th>" >> "${HTML_FILE}"
echo "            <th> File </th>" >> "${HTML_FILE}"
echo "        </tr>" >> "${HTML_FILE}"

declare -ra SORTED_FILENAMES=( $( for filename in ${!FILENAMES[*]}; do
                                      echo "${filename}"
                                  done | sort ) )

dotCounter=0
declare -i fileno=0
declare -i executedTotal=0
declare -i missedTotal=0
for filename in "${SORTED_FILENAMES[@]}"; do
    targetFile="${FILE_PREFIX}${fileno}.gcov.log"
    targetFileHtml="${FILE_PREFIX}${fileno}.gcov.html"
    executed=0
    missed=0

    let dotCounter++
    if (( 10 == ${dotCounter} )); then
        dotCounter=0
        echo -n "."
    fi

    for (( lineno=0; lineno < ${FILENAMES[${filename}]}; lineno++ )); do
        padding=""
        line="${RESULTS[${filename}:${lineno}]}"
        if [[ "${line}" =~ ^' '*[^:]+: ]]; then
            for (( i=0; i < ( 10 - ${#BASH_REMATCH[0]} ); i++ )); do
                padding+=" "
            done
        fi

        if [[ "${line}" =~ ^' '*[0-9]+: ]]; then
            let executed++
        elif [[ "${line}" =~ ^' '*#####: ]]; then
            let missed++
        fi

        echo "${padding}${line}"
    done > "${targetFile}"

    wait
    gawk -f "../gcov2html.awk" -v INDEX="${HTML_FILE}" \
                               -v TITLE="${filename}" "${targetFile}" > "${targetFileHtml}" &

    if (( ${executed} + ${missed} )); then
        percentage="$(bc -q <<<"scale=2; ${executed} * 100.0 / ( ${executed} + ${missed})") %"
    else
        percentage="n/a"
    fi

    if (( ( 1 + fileno ) % 4 )); then
        color="#90EE90"
    else
        color="#AAFFAA"
    fi

    echo "        <tr style=\"background-color: ${color}\">" >> "${HTML_FILE}"
    echo "            <td> ${executed} </td>" >> "${HTML_FILE}"
    echo "            <td> ${missed} </td>" >> "${HTML_FILE}"
    echo "            <td> $(( executed + missed )) </td>" >> "${HTML_FILE}"
    echo "            <td> ${percentage} </td>" >> "${HTML_FILE}"
    echo -n "            <td style=\"text-align: left; padding-left: 5px\"> " >> "${HTML_FILE}"
    echo    "<a href=\"${targetFileHtml}\">.${filename##${MAIN_DIR}}</a> </td>" >> "${HTML_FILE}"
    echo "        </tr>" >> "${HTML_FILE}"

    let executedTotal+=${executed}
    let missedTotal+=${missed}
    let fileno++
done
if (( ${executedTotal} + ${missedTotal} )); then
    readonly FINAL_PERCENTAGE=$( bc -q <<<"scale=2; ${executedTotal} * 100.0 / ( ${executedTotal} + ${missedTotal} )" )
else
    readonly FINAL_PERCENTAGE=""
fi
echo "        <tr style=\"background-color: #ffffc0\">" >> "${HTML_FILE}"
echo "            <td><b> ${executedTotal} </b></td>" >> "${HTML_FILE}"
echo "            <td><b> ${missedTotal} </b></td>" >> "${HTML_FILE}"
echo "            <td><b> $(( executedTotal + missedTotal )) </b></td>" >> "${HTML_FILE}"
if [[ "" != ${FINAL_PERCENTAGE} ]]; then
    echo "            <td style=\"background-color: #ffffc0\"><b> ${FINAL_PERCENTAGE} % </b></td>" >> "${HTML_FILE}"
else
    echo "            <td style=\"background-color: #ffffc0\"><b> n/a </b></td>" >> "${HTML_FILE}"
fi
echo "            <td style=\"text-align: left; padding-left: 5px\"><b> Total </b></td>" >> "${HTML_FILE}"
echo "        </tr>" >> "${HTML_FILE}"
echo "    </table>" >> "${HTML_FILE}"
echo "    <div>" >> "${HTML_FILE}"
echo "        <br/><br/>" >> "${HTML_FILE}"
echo "        <hr style=\"width: 90%; margin: 0 auto; color: maroon\"/><br/>" >> "${HTML_FILE}"
echo "        Generated by $(basename ${0}) on $(date +'%c')." >> "${HTML_FILE}"
echo "    </div>" >> "${HTML_FILE}"
echo "</body>" >> "${HTML_FILE}"
echo "</html>" >> "${HTML_FILE}"

if [[ "" != "${FINAL_PERCENTAGE}" ]]; then
    echo "${FINAL_PERCENTAGE}" > "${HTML_FILE%%.html}.sum"
else
    rm "${HTML_FILE%%.html}.sum" &> /dev/null
fi

echo " [DONE]"

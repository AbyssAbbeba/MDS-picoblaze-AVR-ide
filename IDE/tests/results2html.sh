#! /bin/bash

echo "Test results converter, converts XML files generated by CUnit as test run reports to HTML files, etc."
echo "(C) 2013, 2014 Moravia Microsystems"
echo ""

if ! which xsltproc &> /dev/null; then
    echo "Error: unable to find xsltproc executable, please install xsltproc." > /dev/stderr
    exit 1
fi

declare -ri CLEAN_UP=${clean_up:-1}
declare -ri MERGE_LOGS=${merge_logs:-0}
declare -r  XSLT_PROC_LOG_SUFFIX="xsltproc.log"
declare -ri CPU_CORES=$( which lscpu &> /dev/null && lscpu |
                         gawk 'BEGIN {n=1} END {print(n)} /^CPU\(s\)/ {n=$2;exit}' || echo 1 )

# ======================================================================================================================
# SUPPORT FOR PARALLEL RUN OF MULTIPLE JOBS
# ======================================================================================================================

## Run all jobs in array ${JOBS[@]} (bash scripts) in parallel in accordance with CPU_CORES variable
 # ( #jobs <= CPU_CORES+1 ), if CPU_CORES == 1 then jobs are run sequentially (i.e. not in parallel).
function runParallelJobs() {
    local -a runningJobs
    local -i next
    local    job

    if (( 1 == ${CPU_CORES} )); then
        for job in "${JOBS[@]}"; do
            wait
            bash -c "${job}" &
        done
    else
        next=0
        while (( ${next} < ${#JOBS[@]} )); do
            runningJobs=( $( jobs -pr ) )
            for (( i=0; i < ( 1 + ${CPU_CORES} - ${#runningJobs[@]} ); i++ )); do
                if (( ${next} >= ${#JOBS[@]} )); then
                    # No more jobs left to do.
                    break
                fi

                # Run additional job.
                bash -c "${JOBS[${next}]}" &
                let next++
            done
            sleep 0.5
        done
    fi

    wait
}

# Ensure termination of all jobs running in background, when the script is terminated.
for signal in SIGINT SIGTERM; do
    trap 'for pid in $( jobs -pr ); do
                echo "Terminating child process ${pid}."
                kill ${pid} &> /dev/null &
            done
            wait
            echo ""
            exit 1' ${signal}
done

# ======================================================================================================================

cd "$(dirname "$(readlink -n -f "${0}")" )/results" || exit 1

cp ../*.png .
gawk -f '../TestLog2html.awk' \
     -v TITLE="Test log from $(date +'%c')" '../../Testing/Temporary/LastTest.log' > 'TestLog.html' &

if [[ "1" == "${CPU_CORES}" ]]; then
    echo "Warning: this machine appears to have only one CPU core, parallel processing disabled!"
else
    echo "Using up to ${CPU_CORES} CPU cores."
fi

unset JOBS
idx=0
for i in *-Results.xml; do
    if [[ ! -f "$i" ]]; then
        echo "Warning: nothing to do here." > /dev/stderr
        exit 0;
    fi

    JOBS[${idx}]="
        echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ... [STARTED]'
        if xsltproc --path .. CUnit-Run.xsl '${i}' > '${i%%.xml}.html' 2> '${i}.${XSLT_PROC_LOG_SUFFIX}'; then
            echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ... [DONE]'
        else
            echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ...  [FAILED]'
            rm '${i%%.xml}.html'
        fi"

    let idx++
done
for i in *-Listing.xml; do
    if [[ ! -f "$i" ]]; then
        break
    fi

    JOBS[${idx}]="
        echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ... [STARTED]'
        if xsltproc --path .. CUnit-List.xsl '${i}' > '${i%%.xml}.html' 2> "${i}.${XSLT_PROC_LOG_SUFFIX}"; then
            echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ... [DONE]'
        else
            echo 'XSTL processor \"${i}\" to \"${i%%.xml}.html\" ... [FAILED]'
            rm '${i%%.xml}.html'
        fi"

    let idx++
done
runParallelJobs

echo ""
echo "Initiating final stage of coverage analysis:"
unset JOBS
idx=1
prefix=1
JOBS[0]="../gcovanalysis.sh . 'Total-Coverage.html' 0"
for i in *-Results.xml; do
    test="${i%%-Results.xml}"
    JOBS[${idx}]="../gcovanalysis.sh '${test}' '${test}-Coverage.html' '${prefix}'"
    let prefix++
    let idx++
done
runParallelJobs
echo "Coverage analysis complete."

echo -n "Generating final results ... "

if (( MERGE_LOGS )); then
    readonly sepLine="================================================================================"

    echo -e "\n\n" >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log
    echo "gcov.log" >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log
    cat gcov.log >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log

    echo -e "\n\n" >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log
    echo "${XSLT_PROC_LOG}" >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log
    cat ${XSLT_PROC_LOG} >> BuildLog.log
    echo "${sepLine}" >> BuildLog.log
fi

readonly RESULTS_PKG="TestResults-$(date +'%Y-%m-%d-%H-%M').tar.bz2"
echo "<?xml version='1.0' encoding='utf-8'?>" > index.html
echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"" >> index.html
echo "        \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" >> index.html
echo "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">" >> index.html
echo "<head>" >> index.html
echo "    <meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\" />" >> index.html
echo "    <meta name=\"content-language\" content=\"en\"/>" >> index.html
echo "    <link rel=\"shortcut icon\" href=\"favicon.png\" type=\"image/png\"/>" >> index.html
echo "    <title> Automated Test Run Summary Reports </title>" >> index.html
echo "    <style type=\"text/css\">" >> index.html
echo "        a {" >> index.html
echo "            text-decoration: underline;" >> index.html
echo "            color: #0000FF;" >> index.html
echo "        }" >> index.html
echo "        a:hover {" >> index.html
echo "            text-decoration: none;" >> index.html
echo "            color: #0088FF;" >> index.html
echo "        }" >> index.html
echo "        td.filename {" >> index.html
echo "            text-align: left;" >> index.html
echo "            padding-left: 5px" >> index.html
echo "        }" >> index.html
echo "        td.percentage {" >> index.html
echo "            background-color: #50ff50;" >> index.html
echo "            background-repeat: no-repeat;" >> index.html
echo "            background-position: right center;" >> index.html
echo "        }" >> index.html
echo "    </style>" >> index.html
echo "    </style>" >> index.html
echo "</head>" >> index.html
echo "<body style=\"text-align: center\">" >> index.html
echo "    <div>" >> index.html
echo "        <a href=\"index.html\"><img src=\"logo.png\" width=\"309\" height=\"83\" alt=\"\"/></a>" >> index.html
echo "        <br/><i>Moravia Microsystems, s.r.o.</i><br/><br/>" >> index.html
echo "    </div>" >> index.html
echo "    <div style=\"width: 90%; margin: 0 auto; text-align: left\">" >> index.html

sed $'s/\033[^m]*m//g' BuildLog.log > BuildLog.log.0
gawk -f '../BuildLog2html.awk' -v TITLE="Build log from $(date +'%c')" 'BuildLog.log.0' > 'BuildLog.html' &
warnings=$( grep ': warning:' BuildLog.log.0 | wc -l )
errors=$( grep ': [Ee]rror:' BuildLog.log.0 | wc -l )
if (( 0 == errors )); then
    status="<b style=\"color: green\">OK</b>"
    if (( 0 == warnings )); then
        status+=" <span style=\"color: green\">(0 warnings)</span>"
    else
        status+=" <span style=\"color: #DD8800\">(<b>${warnings}</b> warnings)</span>"
    fi
else
    status="<span style=\"color: red\">FAILED (<b>${errors}</b> errors)</span>"
fi

echo "        <a href=\"BuildLog.html\">[ Build: ${status} ]</a>" >> index.html
echo "        &nbsp;|&nbsp; <a href=\"TestLog.html\">[ Show Test Run Log ]</a>" >> index.html
echo "        &nbsp;|&nbsp; <a href=\"${RESULTS_PKG}\">[ Download Results ]</a>" >> index.html
echo "        <br/><br/>" >> index.html
echo "    </div>" >> index.html
echo "    <table style=\"width: 90%; margin: 0 auto\">" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <col/>" >> index.html
echo "        <tr style=\"background-color: skyblue\">" >> index.html
echo "                <th colspan=\"7\"> Automated Test Run Summary Reports </th>" >> index.html
echo "        </tr>" >> index.html
echo "        <tr style=\"background-color: #ffffc0\">" >> index.html
echo "            <th style=\"width: 5%\"> Suites </th>" >> index.html
echo "            <th style=\"width: 5%\"> Cases </th>" >> index.html
echo "            <th style=\"width: 10%\"> Coverage </th>" >> index.html
echo "            <th style=\"width: 20%\" colspan=\"2\"> Memcheck </th>" >> index.html
echo "            <th style=\"width: 15%\"> Status </th>" >> index.html
echo "            <th> Test subject </th>" >> index.html
echo "        </tr>" >> index.html
declare -i suitesTotal=0
declare -i casesTotal=0
declare -i memErrorsTotal=0
declare -i failedTotal=0
declare    memLeaksTotal=0
for i in *-Results.xml; do
    TEST_NAME="${i%%-Results.xml}"
    LIST_FILE_XML="${TEST_NAME}-Listing.xml"
    LIST_FILE_HTML="${TEST_NAME}-Listing.html"
    VALGRIND_LOG="${TEST_NAME}-Valgring.log"
    VALGRIND_HTML="${VALGRIND_LOG%%.log}.html"
    COVERAGE_HTML="${TEST_NAME}-Coverage.html"

    if [[ "1" == "${CLEAN_UP}" && -d "${TEST_NAME}" ]]; then
        rm -rf "${TEST_NAME}"
    fi

    SUCCESSFUL_CASES=$(gawk 'BEGIN {n=0} END {print(n)} /<CUNIT_RUN_TEST_SUCCESS>/ {n+=1}' "${i}")

    echo "        <tr style=\"background-color: #90EE90\">" >> index.html

    if [[ -f "${LIST_FILE_XML}" ]]; then
        CASES=$(gawk 'BEGIN {n=0} END {print(n)} /<TEST_CASE_DEFINITION>/ {n+=1}' "${LIST_FILE_XML}")
        SUITES=$(gawk 'BEGIN {n=0} END {print(n)} /<CUNIT_ALL_TEST_LISTING_SUITE>/ {n+=1}' "${LIST_FILE_XML}")
        let suitesTotal+=${SUITES}
        let casesTotal+=${CASES}
    else
        CASES=0
        SUITES=0
    fi

    if [[ -f "${LIST_FILE_HTML}" ]]; then
        anchorStart="<a href=\"${LIST_FILE_HTML}\">"
        anchorEnd="</a>"
    else
        anchorStart=""
        anchorEnd=""
    fi
    echo "            <td>${anchorStart} ${SUITES} ${anchorEnd}</td>" >> index.html
    echo "            <td>${anchorStart} ${CASES} ${anchorEnd}</td>" >> index.html

    if [[ -f "${COVERAGE_HTML}" ]]; then
        if [[ -f "${COVERAGE_HTML%%.html}.sum" ]]; then
            coverage="$( cat "${COVERAGE_HTML%%.html}.sum" ) %"
        else
            coverage="n/a"
        fi
        echo "            <td><a href=\"${COVERAGE_HTML}\"> ${coverage} </a></td>" >> index.html
    else
        echo "            <td> -- </td>" >> index.html
    fi

    if [[ -f "${VALGRIND_LOG}" ]]; then
        cp "${VALGRIND_LOG}" "${VALGRIND_LOG}.0"
        gawk -f '../FilterMemcheckResults.awk' "${VALGRIND_LOG}.0" > "${VALGRIND_LOG}"
        gawk -f '../valgrind2html.awk' -v TITLE="Memcheck log ${TEST_NAME}" "${VALGRIND_LOG}" > "${VALGRIND_HTML}" &

        read leaked errors <<< "$( gawk '
            BEGIN {
                leaks  = 0
                errors = 0
                accept = 0
            }
            END {
                printf ( "%.2f %d\n", ( leaks / 1024.0 ), errors )
            }
            /^==[0-9]+== *LEAK SUMMARY:/ {
                accept = 1
            }
            /^==[0-9]+== *ERROR SUMMARY:/ {
                errors = $4
            }
            ( !accept ) {
                next
            }
            /definitely lost:/ {
                sub ( /,/, "", $4 )
                leaks += $4
            }
            /indirectly lost:/ {
                sub ( /,/, "", $4 )
                leaks += $4
            }
            /possibly lost:/ {
                sub ( /,/, "", $4 )
                leaks += $4
            }' "${VALGRIND_LOG}" )"

        if [[ "${leaked}" == "0.00" ]]; then
            style="background-color: #50ff50"
        else
            style="background-color: #ffbb00"
        fi
        echo "            <td style=\"${style}\"><a href=\"${VALGRIND_HTML}\"> ${leaked} kB </a></td>" >> index.html
        memLeaksTotal="$( bc -q <<< "scale = 2; ${memLeaksTotal} + ${leaked} " )"

        if [[ "${errors}" == "0" ]]; then
            style="background-color: #50ff50"
        else
            style="background-color: #ffbb00"
        fi
        echo "            <td style=\"${style}\"><a href=\"${VALGRIND_HTML}\">${errors} error(s)</a></td>" >> index.html
        let memErrorsTotal+=${errors}
    else
        echo "            <td> -- </td>" >> index.html
        echo "            <td> -- </td>" >> index.html
    fi

    if [[ -f "${i%%.xml}.html" ]]; then
        if (( ! CASES )); then
            echo "            <td><a href=\"${i%%.xml}.html\"> Passed </a></td>" >> index.html
        elif (( CASES != SUCCESSFUL_CASES )); then
            style="background-image:url('bgr.png'); background-size: $(( 100 - SUCCESSFUL_CASES * 100 / CASES ))% 100%;"
            echo -n "            <td class=\"percentage\" style=\"${style}\"> " >> index.html
            echo    "<a href=\"${i%%.xml}.html\">$(( CASES - SUCCESSFUL_CASES )) case(s) failed </a></td>" >> index.html
            let failedTotal+=$(( CASES - SUCCESSFUL_CASES ))
        else
            echo "            <td><a href=\"${i%%.xml}.html\"> Passed </a></td>" >> index.html
        fi
    else
        echo "            <td style=\"background-color: #555555; color: #FFFFFF\"> CRASHED </td>" >> index.html
    fi

    echo "            <td style=\"text-align: left; padding-left: 5px\">${TEST_NAME}</td>" >> index.html
    echo "        </tr>" >> index.html
done
echo "        <tr style=\"background-color: #ffffc0\">" >> index.html
echo "            <td><b>${suitesTotal}</b></th>" >> index.html
echo "            <td><b>${casesTotal}</b></th>" >> index.html
if [[ -f "Total-Coverage.html" ]]; then
    if [[ -f Total-Coverage.sum ]]; then
        coverage="$( cat Total-Coverage.sum ) %"
    else
        coverage="??"
    fi
    echo "            <td><a href=\"Total-Coverage.html\"><b>${coverage}</b></a></th>" >> index.html
else
    echo "            <td><b>--</b></th>" >> index.html
fi
echo "            <td style=\"width: 10%\"><b>${memLeaksTotal} kB</b></th>" >> index.html
echo "            <td><b>${memErrorsTotal}</b></th>" >> index.html
echo "            <td><b>${failedTotal}</b></th>" >> index.html
echo "            <td style=\"text-align: left; padding-left: 5px\"><b>Total</b></th>" >> index.html
echo "        </tr>" >> index.html
echo "    </table>" >> index.html
echo "    <div>" >> index.html
echo "        <br/><br/>" >> index.html
echo "        <hr style=\"width: 90%; margin: 0 auto; color: maroon\" /><br/>" >> index.html
echo "        Generated by $(basename ${0}) on $(date +'%c').<br/>" >> index.html
if [[ -f "startTime.log" ]]; then
    seconds=$(( $(date +'%s') - $( cat "startTime.log" ) ))
    printf -v buildTime "%dh %dm %ds" $(( seconds / 3600 )) $(( ( seconds % 3600 ) / 60 )) $(( seconds % 60 ))
    echo "        Real time spent on processing: ${buildTime}." >> index.html
fi
echo "    </div>" >> index.html
echo "</body>" >> index.html
echo "</html>" >> index.html

if (( ${CLEAN_UP} )); then
    rm *.sum *.log *.xml *.0 2> /dev/null
fi

cp index.html index.html.0
gawk '/\[ Download Results \]/ {next} {print($0)}' index.html.0 > index.html
wait
tar cjf "${RESULTS_PKG}" *.html *.png
mv index.html.0 index.html
echo "[DONE]"

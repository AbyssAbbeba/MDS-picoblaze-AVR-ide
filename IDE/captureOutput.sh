#! /bin/bash

readonly LOG_FILE="${1}"
shift
"${@}" >> "${LOG_FILE}" 2>&1

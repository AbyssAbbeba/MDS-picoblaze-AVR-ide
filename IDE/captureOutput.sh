#! /bin/bash

readonly LOG_FILE="${1}"
shift
exec "${@}" >> "${LOG_FILE}" 2>&1

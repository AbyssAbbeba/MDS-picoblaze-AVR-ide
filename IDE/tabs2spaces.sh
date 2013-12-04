#! /bin/bash

declare -r tmpFile="$(mktemp)"

while (( 0 != $# )); do
    cp "$1" "$tmpFile"
    :>"$1"

    gawk -v TABWIDTH_NEW=4 -v TABWIDTH_OLD=8 -v TARGET_FILE="$1" '
        BEGIN {
            noOfTabs = 0;
            noOfSpaces = 0;
        }

        END {
            printf("%s: %d tabs converted to %d spaces\n", TARGET_FILE, noOfTabs, noOfSpaces)
        }

        {
            tbIdx = 0;  # Character index "in next tabulator"
            bol = 1;    # Beginning Of Line

            for ( i = 1; i <= length($0); i++ ) {
                if ( 1 == bol ) {
                    tabWidth = TABWIDTH_NEW;
                } else {
                    tabWidth = TABWIDTH_OLD;
                }

                ch = substr($0, i, 1);
                if ( "\t" == ch ) {
                    for ( j = 0; j < ( tabWidth - tbIdx ); j++ ) {
                        printf " " > TARGET_FILE;
                        noOfSpaces++;
                    }
                    noOfTabs++;
                    tbIdx = 0;
                } else {
                    printf ch > TARGET_FILE;
                    tbIdx++;
                    tbIdx %= tabWidth;
                    bol = 0;
                }
            }
            printf "\n" > TARGET_FILE;
        }' "$tmpFile"

        shift
done

rm "$tmpFile"

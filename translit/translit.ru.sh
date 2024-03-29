#!/bin/bash

# 2022-12-20

# https://stackoverflow.com/q/19619490
# https://stackoverflow.com/q/6980090
#
# The substitution "${1:-/dev/stdin}" takes $1 if defined. Otherwise, the file
# name of the standard input of the own process is used.
#
# [ "$1" ] && FILE=$1 || FILE="-" equivalent FILE=${1:--}

while read -r line
do
    echo "$line" |
        sed 'y/ЁёЩщ/ЕеШш/; y/ХхЦцЫыЭэ/HhCcYyEe/; s/[ЪЬъь]//g' |
        LC_CTYPE=C iconv -f UTF-8 -t ASCII//TRANSLIT |
        tr -s '[:blank:]' '.'
done < "${1:-/dev/stdin}"

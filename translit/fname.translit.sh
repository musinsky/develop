#!/usr/bin/bash

# 2023-03-08

if [ ! -e "$1" ]; then
    printf "no such file or directory '%s'\n" "$1"
    exit 1
fi

printf "original name: "
if ! echo "$1" | grep --color=always -P '[^A-Za-z0-9/._-]'; then
    # portable
    printf "%s   # portable name\n" "$1"
fi

NAME2="$1"
if echo "$NAME2" | grep -P '[А-Яа-я]' > /dev/null; then
    # cyrillic
    NAME2="$(echo "$NAME2" | sed 'y/ЁёЩщ/ЕеШш/; y/ХхЦцЫыЭэ/HhCcYyEe/; s/[ЪЬъь]//g')"
    NAME2="$(echo "$NAME2" | LC_CTYPE=C iconv -f UTF-8 -t ASCII//TRANSLIT)"
else
    # non-cyrillic (latin)
    NAME2="$(echo "$NAME2" | iconv -f UTF-8 -t ASCII//TRANSLIT)"
fi

printf "ascii    name: "
if ! echo "$NAME2" | grep --color=always -P '[^A-Za-z0-9/._-]'; then
    printf "%s\n" "$NAME2"
fi

R_CHAR='.'     # file
if [ -d "$1" ]; then
    R_CHAR='_' # dir
fi

NAME2="$(echo "$NAME2" | sed "s/[^A-Za-z0-9/._-]/$R_CHAR/g")"
NAME2="$(echo "$NAME2" | tr -s '._' "$R_CHAR")"
NAME2="$(echo "$NAME2" | sed 's/\.\-\././g')"   # '.-.' => '.'

printf "portable name: %s" "$NAME2"
if [ "$1" = "$NAME2" ]; then
    printf "   # name unchanged\n\n"
else
    printf "\n\n"
fi

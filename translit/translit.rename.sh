#!/usr/bin/bash

# 2023-03-10

if [ ! -e "$1" ]; then
    printf "no such file or directory '%s'\n" "$1"
    exit 1
fi

printf "original name: "
if ! echo "$1" | grep --color -P '[^A-Za-z0-9/._-]'; then
    printf "%s   # portable name\n" "$1"
fi

NAME2="$1"
if echo "$NAME2" | grep --silent -P '[А-Яа-я]'; then
    # cyrillic
    NAME2="$(echo "$NAME2" | sed 'y/ЁёЩщ/ЕеШш/; y/ХхЦцЫыЭэ/HhCcYyEe/; s/[ЪЬъь]//g')"
    NAME2="$(echo "$NAME2" | LC_CTYPE=C iconv -f UTF-8 -t ASCII//TRANSLIT)"
else
    # non-cyrillic (latin)
    NAME2="$(echo "$NAME2" | iconv -f UTF-8 -t ASCII//TRANSLIT)"
fi

printf "ascii    name: "
if ! echo "$NAME2" | grep --color -P '[^A-Za-z0-9/._-]'; then
    printf "%s\n" "$NAME2"
    #printf "%s   # portable name\n" "$1"
fi

CHAR2='.'     # file.name
if [ -d "$1" ]; then
    CHAR2='_' # dir_name
fi

NAME2="$(echo "$NAME2" | sed "s/[^A-Za-z0-9/._-]/$CHAR2/g")"
NAME2="$(echo "$NAME2" | tr -s '._' "$CHAR2")"
# NAME2="$(echo "$NAME2" | sed 's/\.\-\./\./g')"   # '.-.' => '.'
# https://www.shellcheck.net/wiki/SC2001
NAME2="${NAME2//.-./.}"

printf "portable name: %s" "$NAME2"
if [ "$1" = "$NAME2" ]; then
    printf "   # name unchanged\n\n"
    read -n 1 -r -s -p "press any key to exit"
    #printf "\n"
    exit 0
else
    printf "\n\n"
fi

read -r -p "rename original to portable name? [y]:"
if [[ $REPLY =~ ^[Yy]$ ]]; then
    mv -v -i "$1" "$NAME2" | grep --silent . && \
        printf "'%s'\nhas been renamed to\n'%s'\n" "$1" "$NAME2"
else
    printf "'%s' not renamed\n" "$1"
fi

read -n 1 -r -s -p "press any key to exit"

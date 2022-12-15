#!/bin/bash

# 2022-12-15

# https://en.wikipedia.org/wiki/Cyrillic_script_in_Unicode
# https://en.wikipedia.org/wiki/Russian_alphabet
# subset of Cyrillic (U+0400..U+04FF)
# subset of Russian alphabet U+0401,(U+0410..U+042F),(U+0430..U+044F),U+0451
#                            CCL IO,(CCL A ..CCL YA),(CSL A ..CSL YA),CSL IO
# modern Russian alphabet consists of 33 letters

wget -nc https://www.unicode.org/Public/UNIDATA/UnicodeData.txt

UDRA='UnicodeData.ru.alphabet.txt'
sed    -e '/^0000/,/^0400/d' -e '/^0402/,/^040F/d' UnicodeData.txt > "$UDRA"
sed -i -e '/^0450/d'         -e '/^0452/,$d'       "$UDRA"
sed -i -e 's/;[^;]*//2g' "$UDRA"

# sort check (sorted by first word 'NNNN;CYRILLIC' as one word)
LC_ALL=C sort "$UDRA" > "$UDRA.sort"
diff "$UDRA" "$UDRA.sort" || { printf "not sorted\n"; exit 1; }
rm "$UDRA.sort"
# count check
(( "$(wc --lines < $UDRA)" != 66 )) && { printf "not 2*33 letters\n"; exit 1; }
printf "'%s' created\n" "$UDRA"


# printf '\u044F'; printf '%b' '\u044F'
# UNICODE_HEX="U+044F"; printf ${UNICODE_HEX/U+/"\U"}
function print_letter {
    printf '%b\n' "$(printf '\\u%04X' "$1")"
    (( "$1" == 0x042F )) && printf '\n'
}
function print_letter_row {
    printf '%b' "$(printf '\\u%04X' "$1")"
    need_space=1
    (( "$1" == 0x042F )) && { printf '\n'; need_space=0; }
    (( "$1" == 0x044F )) && { printf '\n'; need_space=0; }
    (( "$need_space" == 1 )) && printf ' '
}
function print_info {
    printf '%04d | U+%04X | %b\n' "$1" "$1" "$(printf '\\u%04X' "$1")"
    (( "$1" == 0x042F )) && printf '\n'
}
function gen_ru_unicode_capital {
    for ((num = 0x0410; num <= 0x0415; num++)); do   # [А-Е]
        $1 "$num"
    done
    $1     "0x0401"                                  # Ё
    for ((num = 0x0416; num <= 0x042F; num++)); do   # [Ж-Я]
        $1 "$num"
    done
}
function gen_ru_unicode_small {
    for ((num = 0x0430; num <= 0x0435; num++)); do   # [а-е]
        $1 "$num"
    done
    $1     "0x0451"                                  # ё
    for ((num = 0x0436; num <= 0x044F; num++)); do   # [ж-я]
        $1 "$num"
    done
}

gen_ru_unicode_capital "print_letter" >  russian.alphabet.01.column.utf8
gen_ru_unicode_small   "print_letter" >> russian.alphabet.01.column.utf8
gen_ru_unicode_capital "print_letter_row" >  russian.alphabet.01.row.utf8
gen_ru_unicode_small   "print_letter_row" >> russian.alphabet.01.row.utf8
gen_ru_unicode_capital "print_info" >  russian.alphabet.02.column.utf8
gen_ru_unicode_small   "print_info" >> russian.alphabet.02.column.utf8


## sort notes

# man sort
#
# The locale specified by the environment affects sort order. Set LC_ALL=C to
# get the traditional sort order that uses native byte values.

# https://www.gnu.org/software/coreutils/manual/html_node/sort-invocation.html
#
# Unless otherwise specified, all comparisons use the character collating
# sequence specified by the LC_COLLATE locale.
#
# Note that setting only LC_COLLATE has two problems. First, it is ineffective
# if LC_ALL is also set. Second, it has undefined behavior if LC_CTYPE (or LANG,
# if LC_CTYPE is unset) is set to an incompatible value. For example, you get
# undefined behavior if LC_CTYPE is ja_JP.PCK but LC_COLLATE is en_US.UTF-8.
#
# ?! LC_ALL=C vs LC_ALL=C.UTF-8 vs CaseSensitive ?!

# https://apple.stackexchange.com/q/335741
#
# When LC_COLLATE is defined as LC_COLLATE="en_US.UTF-8" on macOS, LC_COLLATE
# uses the C or POSIX definiton. The default behavior of sort is to sort in
# ascii order where uppercase has precedence over lowercase.
#
# On Linux, when LC_COLLATE="en_US.UTF-8" the LC_COLLATE definiton is
# non-standard. The default behavior of sort is to ignore-case and lowercase has
# precedence over uppercase.


# https://stackoverflow.com/q/51202710
#
# You're looking for ordinal sorting, where characters are sorted numerically by
# their Unicode code point ("ASCII value") and therefore all uppercase letters,
# as a group, sort before all lowercase letters.
#
# As of Windows PowerShell Sort-Object uses lexical sorting, where
# case-sensitive sorting simply means that the lowercase form of a given letter
# comes directly before its uppercase form, not all letters collectively; e.g.,
# b sorts before B, but they both come after both a and A (also, the logic is
# reversed from the ordinal case, where it is uppercase letters that come
# first).

#!/bin/bash

# 2022-12-12

# https://en.wikipedia.org/wiki/Cyrillic_script_in_Unicode
# https://en.wikipedia.org/wiki/Russian_alphabet
# subset of Cyrillic (U+0400..U+04FF)
# subset of Russian alphabet U+0401,(U+0410..U+042F),(U+0430..U+044F),U+0451
#                            CCL IO, CCL A.. CCL YA , CSL A.. CSL YA, CSL IO

wget https://www.unicode.org/Public/UNIDATA/UnicodeData.txt

URA='UnicodeData.ru.alphabet.txt'
sed    -e '/^0000/,/^0400/d' -e '/^0402/,/^040F/d' UnicodeData.txt > "$URA"
sed -i -e '/^0450/d'         -e '/^0452/,$d'       "$URA"

sed -i -e 's/;[^;]*//2g' "$URA"

LC_ALL=C sort "$URA" > "$URA.sort"
diff "$URA" "$URA.sort" || { printf "not sorted \n"; exit 1; }
rm "$URA.sort"

if [ "$(wc --lines < $URA)" -ne "66" ]; then
    printf "not 2*33 letters\n";
    exit 1;
fi

printf "'%s' created\n" "$URA"

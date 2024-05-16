#!/usr/bin/env bash

# 2024-05-16

printf "== EXAM1 ==\n"
sqlite3 -init dst.files.sql <<EOF
.echo on
#  CLI comment
-- SQL comment
.show
SELECT * FROM DST_files WHERE id=12;   -- don't forget for ';' at EOL (only SQL comment)
EOF

printf "\n== EXAM2 ==\n"
OUTPUT_F="$(mktemp)" || { echo 'mktemp error'; exit 1; } # on Fedora in RAM
SEPARATOR=","

sqlite3 -init dst.files.sql <<EOF
.mode ascii
# mode by default: 'list', mode 'ascii' only as example
# mode via sqlite3 option: '-ascii' or '-csv' or '-list' or etc.
.separator "$SEPARATOR" "\n"
# separator by default for mode 'list':     "|" (col) and   "\n" (row)
# separator by default for mode 'ascii': "\037" (col) and "\036" (row)
# separator by default for mode 'csv':      "," (col) and "\r\n" (row)
# separator via sqlite3 option: '-separator SEP' or '-newline SEP'
.output "$OUTPUT_F"
# output by default: stdout
SELECT prefix, name FROM DST_files WHERE format='full' AND real_as_int=0;
# second SELECT only for example
#SELECT prefix, name FROM DST_files WHERE format='full' AND real_as_int=1;
EOF

printf "=== parse option 1 ===\n"
while IFS="$SEPARATOR" read -r c_prefix c_name
do
    printf "/custom_path1/%s/%s.dvo\n" "$c_prefix" "$c_name"
done < "$OUTPUT_F"

printf "=== parse option 2 ===\n"
# for row in $(cat "$OUTPUT_F")   # not recommended, see: SC2013, SC2066
while IFS= read -r row   # whole line/row (empty or IFS=$'\n')
do
    IFS="$SEPARATOR" read -r -a column <<< "$row"   #  here strings <<<
    printf "/custom_path2/%s/%s.dvo\n" "${column[0]}" "${column[1]}"
done < "$OUTPUT_F"

printf "=== parse option 3 ===\n"
while IFS="$SEPARATOR" read -r -a column3
do
    printf "/custom_path3/%s/%s.dvo\n" "${column3[0]}" "${column3[1]}"
done < "$OUTPUT_F"

rm "$OUTPUT_F"

printf "\n== EXAM3 ==\n"
# similar parse option 1
sqlite3 -init dst.files.sql <<EOF |   # with pipe | symbol
SELECT id, name, prefix FROM DST_files WHERE id<5 ORDER BY id DESC;
EOF
while IFS='|' read -r c_id c_name c_prefix   # 'list' mode with '|' colseparator
do
    printf "id[%02d]: file '%s/%s.dvo'\n" "$c_id" "$c_prefix" "$c_name"
done

printf "\n== EXAM4 ==\n"
# similar parse option 3
sqlite3 -init dst.files.sql <<EOF |   # with pipe | symbol
SELECT * FROM DST_files WHERE id<5 ORDER BY id DESC;
EOF
while IFS='|' read -r -a line                # 'list' mode with '|' colseparator
do
    printf "id[%02d] %s format: file '%s/%s.dvo'\n" \
           "${line[0]}" "${line[3]}" "${line[2]}" "${line[1]}"
done

# https://www.baeldung.com/linux/csv-parsing
# https://www.cyberciti.biz/faq/unix-howto-read-line-by-line-from-file/
# https://www.cyberciti.biz/faq/unix-linux-bash-read-comma-separated-cvsfile/
# https://unix.stackexchange.com/q/88490   # pipe with EOF

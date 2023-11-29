#!/usr/bin/env bash

# 2023-11-29

# https://github.com/Institut-Zdravotnych-Analyz/covid19-data
CSV_FNAME='OpenData_Slovakia_Covid_PCRTests_AgeGroup_District.csv'
#CSV_FNAME='OpenData_Slovakia_Covid_PositiveTests_District.csv' # smaller file
CSV_GH='https://raw.githubusercontent.com/Institut-Zdravotnych-Analyz/covid19-data/main/PCR_Tests'
FNAME="${0%.sh}.csv"
MNAME="${0%.sh}.more.csv"
SNAME="${0%.sh}.sample.csv"

TMP_F="$(mktemp)" || { echo 'mktemp error'; exit 1; }
wget "$CSV_GH/$CSV_FNAME" -O "$TMP_F" || { echo "wget error"; rm "$TMP_F"; exit 1; }
dos2unix "$TMP_F"
tr --delete '"' < "$TMP_F" > "$FNAME"
rm "$TMP_F"

# cat "$FNAME" "$FNAME" > "$MNAME"   # duplicate the content of file
# # cat "$MNAME" >> "$MNAME"         # !!! infinite loop !!!

head "$FNAME" > "$SNAME"
grep --max-count=1 --after-context=15 'Bardejov' "$FNAME" >> "$SNAME"

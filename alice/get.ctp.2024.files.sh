#!/usr/bin/env bash

# 2025-11-07

# server: lxplus.cern.ch
# /eos/project/a/alicetrg/backup/2025_rhel9/pit/ctpctr/home/trigger/WORK/ctpc/
# dir contains *.cc files
# 20240115.cc -> 20250209.cc => 371 files
# 2024-01-15  -> 2025-02-09  => 392 days
#
# /eos/project/a/alicetrg/backup/2025_rhel9/pit/ctpctr/home/trigger/WORK/runs/
# dir contains config files for runs (i.a. *.rcfg2 or *.rcfg)
# 546110.rcfg2 -> 560895.rcfg2 => 3536 files
# 546110.rcfg  -> 560895.rcfg  => 3508 files
# 2024-01-15   -> 2025-02-07

# NOTE older files: /afs/cern.ch/work/a/alicetrg/public/run3/
# 20220713.tgz -> 20231202.tgz => 487 files
# 533551.rcfg2 (2023-03-27) -> 546015.rcfg2 (2023-12-15)
# 530351.rcfg  (2023-01-12) -> 546015.rcfg  (2023-12-15)

print_usage() {
    local bname=${0##*/}
    printf 'Usage: %s DATE\n' "$bname"
    printf 'Example:\n'
    printf '   %s 20240115.cc # one cc file\n' "$bname"
    printf '   %s 2024-01-15  # config files for runs (all in this day)\n' "$bname"
    exit 1
}
check_result() {
    local ftar="$1"
    if tar -tzf "$ftar" | grep --quiet --max-count=1 '.'; then
        printf "found and downloaded as '%s'\n" "$ftar"
    else
        printf "not found\n"
        rm -f "$ftar"
    fi
}

[ "$#" -ne 1 ] && print_usage   # just one argument
CERN_SRV="lxplus.cern.ch"
CERN_USR="$USER"
CTP_DIR="/eos/project/a/alicetrg/backup/2025_rhel9/pit/ctpctr/home/trigger/WORK"
OUT_FILE="$PWD"
DAY="$1"

if [ "${DAY##*.}" = "cc" ]; then
    printf "find '%s' file\n" "$DAY"
    OUT_FILE="$OUT_FILE/$DAY.tar.gz"
    ssh "$CERN_USR@$CERN_SRV" \
        "cd $CTP_DIR/ctpc && find . -maxdepth 1 -type f -name $DAY " \
        "-print0 | tar --null --files-from=- -czvf -" > "$OUT_FILE"
    check_result "$OUT_FILE"
    exit
fi

if [ "$(date --date="$DAY" +%F 2>/dev/null)" = "$DAY" ]; then
    printf "find '%s' config files for runs\n" "$DAY"
    OUT_FILE="$OUT_FILE/$DAY.run.config.tar.gz"
    ssh "$CERN_USR@$CERN_SRV" \
        "cd $CTP_DIR/runs && find . -maxdepth 1 -type f " \
        "\( -name '*.rcfg2' -o -name '*.rcfg' \) -newermt $DAY ! -newermt $DAY+1day " \
        "-print0 | tar --null --files-from=- -czvf -" > "$OUT_FILE"
    check_result "$OUT_FILE"
    exit
fi

printf "unrecognized request\n"

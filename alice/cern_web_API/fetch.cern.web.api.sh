#!/usr/bin/env bash

# 2026-05-26

print_token_info() {
  printf "\n\$TOKEN header (1st) and payload (2nd) parts:\n"
  echo "${TOKEN}" | cut -d'.' -f1 | basenc --base64url --decode | jq
  echo "${TOKEN}" | cut -d'.' -f2 | basenc --base64url --decode | jq
  printf "\$TOKEN validity interval:\n"   # user token from Firefox is valid 7 days
  echo "${TOKEN}" | cut -d'.' -f2 | basenc --base64url --decode |
    jq --raw-output '.iat | strflocaltime("%F %T %z")'
  echo "${TOKEN}" | cut -d'.' -f2 | basenc --base64url --decode |
    jq --raw-output '.exp | strflocaltime("%F %T %z")'
  # echo "${TOKEN}" | cut -d'.' -f2 | basenc --base64url --decode |
  #   jq '.iat' | LC_TIME=C xargs -I{} date -d "@{}"
  # echo "${TOKEN}" | cut -d'.' -f2 | basenc --base64url --decode |
  #   jq '.exp' | LC_TIME=C xargs -I{} date -d "@{}"
  printf "\n"
}
check_host_certificate() {
  # https://curl.se/docs/manpage.html
  # Exit code 60: Peer certificate cannot be authenticated with known CA certificates.
  curl --silent "${HOST}" > /dev/null
  local rc=$? # arithmetic
  if [[ $rc == 60 ]]; then
    curl "${HOST}"
    printf "\n#################################\n\n"
    printf "# You need install CERN-CA-certs to your system\n"
    printf "# $ sudo dnf install CERN-CA-certs   # CERN AlmaLinux\n"
    printf "#\n# or manually\n"
    printf "# 1st) download CERN CA Certificate\n"   # Not After: Mar 29 08:34:17 2032 GMT
    printf "#      '%s'\n" 'https://ca.cern.ch/cafiles/certificates/CERN%20Certification%20Authority(2).crt'
    printf "# 2nd) curl --cacert '/path/to/CERN Certification Authority(2).crt' %s\n\n" "${HOST}"
    exit
  fi
}
safe_token_out() {
  sed -i "s/${TOKEN}/xxx/" "${TMPF}"
  cat "${TMPF}"
}

HOST='https://ali-bookkeeping.cern.ch'
check_host_certificate
TMPF="$(mktemp)" || { printf 'mktemp error\n'; exit 1; }

# $ export TOKEN='xxx'
[[ -z "${TOKEN}" ]] && { printf "\$TOKEN variable is not set\n"; exit 1; }
print_token_info

# https://ali-bookkeeping.cern.ch/?page=lhc-fill-details&fillNumber=11745
# Firefox Web Developer Tools (debug mode, F12) and 'Network' tab
# see in 'Headers' tab sections: 'GET', 'Response Headers' and 'Request Headers' (try switch to Raw mode)
# compare with
# curl "${HOST}/api/lhcFills/11745?token=${TOKEN}" --output /dev/null --silent --verbose
# curl "${HOST}/api/lhcFills/11745?token=${TOKEN}" --output /dev/null --silent --trace -
# curl "${HOST}/api/lhcFills/11745?token=${TOKEN}" --output /dev/null --silent --dump-header -

curl "${HOST}/api/lhcFills/11745?token=${TOKEN}" --output out.01.json --silent \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "# get/fetch one specific fill number,   JSON: %s\n" "$(head -c40 out.01.json)"
printf "=> number of runs: %s" "$(jq '.data.runs[].runNumber' out.01.json | wc -l)"
printf " (only PHYSICS: %s)\n" \
       "$(jq '.data.runs[] | select (.definition == "PHYSICS") | .runNumber' out.01.json | wc -l)"

# https://ali-bookkeeping.cern.ch/?page=lhc-fill-details&fillNumber=11745
# Stable beams start: 16/05/2026, 19:17:36   1778951856000
# Stable beams end:   17/05/2026, 00:27:49   1778970469000
# all:     53 runs, Per quality (good: 16, none: 29, bad: 5, test: 3)
# PHYSICS:  3 runs, Per quality (good: 3)

curl --get "${HOST}/api/lhcFills" --output out.02.json --silent \
     --data-urlencode "filter[fillNumbers]=11745" \
     --data-urlencode "token=${TOKEN}" \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "# query of any filter(s) [fillNumbers], JSON: %s\n" "$(head -c40 out.02.json)"
printf "=> number of runs: %s\n" "$(jq '.data[].runs[].runNumber' out.02.json | wc -l)"

curl --get "${HOST}/api/lhcFills" --output out.02a.json --silent \
     --data-urlencode "filter[fillNumbers]=11745" \
     --data-urlencode "filter[hasStableBeams]=true" \
     --data-urlencode "token=${TOKEN}" \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "=> number of runs: %s\n" "$(jq '.data[].runs[].runNumber' out.02a.json | wc -l)"

# ALICE use milliseconds precision
SB_START="$(date -d "2026-05-01 00:00:00" "+%s000")" # 1777586400000
SB_END="$(date -d   "2026-05-17 23:59:59" "+%s000")" # 1779055199000
# https://ali-bookkeeping.cern.ch/?page=lhc-fill-overview
# time interval <SB_START, SB_END): 25 stable fills, last fill 11745

curl --get "${HOST}/api/lhcFills" --output out.03.json --silent \
     --data-urlencode "filter[hasStableBeams]=true" \
     --data-urlencode "filter[stableBeamsStart][from]=${SB_START}" \
     --data-urlencode "filter[stableBeamsEnd][to]=${SB_END}" \
     --data-urlencode "token=$TOKEN" \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "=> number of fills: %s\n" "$(jq '.data[].fillNumber' out.03.json | wc -l)"

printf "\n### api/statistics/lhcFills ###\n"
# https://ali-bookkeeping.cern.ch/?page=statistics
curl "${HOST}/api/statistics/lhcFills?token=${TOKEN}" --output out.10.json --silent \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "# example with API error\n"

curl "${HOST}/api/statistics/lhcFills?from=${SB_START}&to=${SB_END}&token=${TOKEN}" \
     --output out.11.json --silent \
     --write-out "\n%{url_effective}\n=> %{filename_effective}: %{size_download} bytes\n" > "${TMPF}"
safe_token_out
printf "=> number of fills: %s\n\n" "$(jq '.data[].fillNumber' out.11.json | wc -l)"

rm "${TMPF}"

# curl https://ali-bookkeeping.cern.ch/api/lhcFills/11745?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/lhcFills/INVALID?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/lhcFills/11745/logs?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/lhcFills/11745/NOTFOUND?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/lhcFills?token="${TOKEN}" --output lhcFills.full.json
# Be careful, bash does not perform as "curl --data-urlencode", you need to add the quotes manually
# curl "https://ali-bookkeeping.cern.ch/api/lhcFills?from=0&to=1&token=${TOKEN}" | jq # API error

# curl https://ali-bookkeeping.cern.ch/api/statistics/lhcFills?token="${TOKEN}" | jq   # API errors
# curl https://ali-bookkeeping.cern.ch/api/statistics/lhcFills/NOTFOUND?token="${TOKEN}" | jq
# Be careful, bash does not perform as "curl --data-urlencode", you need to add the quotes manually
# curl "https://ali-bookkeeping.cern.ch/api/statistics/lhcFills?from=1777586400000&to=1779055199000&token=${TOKEN}" | jq

# curl https://ali-bookkeeping.cern.ch/api/runs/571831?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/status/database?token="${TOKEN}" | jq
# curl https://ali-bookkeeping.cern.ch/api/logs?token="${TOKEN}" --output logs.full.json

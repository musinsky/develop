#!/usr/bin/env bash

# 2026-07-30

curl_get() {
  local json="$1.json"
  local ts="$2"
  local path="$3"

  local ts_lower_millis
  local ts_upper_millis
  ts_lower_millis="$(date --date="${ts}" +%s%3N)"
  ts_upper_millis="$(date --date="2026-06-30" +%s%3N)" # end of LHC Run3
  local ccdb_url="http://alice-ccdb.cern.ch/browse/${path}"
  local curl_headers=(
    --header "Accept: application/json"          # default 'text/plain'
    --header "If-Not-Before: ${ts_lower_millis}" # >= creation timestamp
    --header "If-Not-After: ${ts_upper_millis}"  # <= creation timestamp
  )

  curl "${curl_headers[@]}" "${ccdb_url}" --output "${json}" \
    --write-out "Downloaded: '%{filename_effective}' (%{size_download} bytes)\n\n"
}

curl_get "ccdb.sample.big" "2000-01-01" "RCT/Info/RunInformation" # 228 MB, ~ 245.000 objects
curl_get "ccdb.sample.normal" "2026-03-01" "GLO/Config/GRPLHCIF"  #   2 MB, ~   2.000 objects
curl_get "ccdb.sample.small" "2026-06-26" "GLO/Config/GRPLHCIF"   #  35 KB, ~      30 objects

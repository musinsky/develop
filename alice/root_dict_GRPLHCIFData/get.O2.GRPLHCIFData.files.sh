#!/usr/bin/env bash

# 2026-05-25

get_file() {
  local file="${1}"
  local file_bn="${file##*/}" # basename in bash
  local date
  curl --silent --write-out "%{url}\n" \
       --location "${ALICEO2_GH_RAW}/${file}" --remote-name \
       --output-dir "${OUT_DIR}" --create-dirs

  # prepend info
  date=$(curl --silent "${ALICEO2_GH_API}?path=${file}&per_page=1" |
           jq --raw-output '.[0].commit.committer.date')
  date=$(date --date "${date}" +%F) # formatted date
  {
    printf "// %s (%s)\n\n" "${ALICEO2_GH_BLOB}/${file}" "${date}"
    cat "${OUT_DIR}/${file_bn}"
  } > tmp.txt && mv tmp.txt "${OUT_DIR}/${file_bn}"
}

ALICEO2_GH='AliceO2Group/AliceO2'
ALICEO2_GH_BLOB="https://github.com/${ALICEO2_GH}/blob/dev"
ALICEO2_GH_RAW="https://raw.githubusercontent.com/${ALICEO2_GH}/dev"
ALICEO2_GH_API="https://api.github.com/repos/${ALICEO2_GH}/commits"
OUT_DIR='AliceO2_GRPLHCIFData'

get_file "DataFormats/Parameters/include/DataFormatsParameters/GRPLHCIFData.h"
sed -i 's/CommonTypes\/Units.h/Units.h/'                    "${OUT_DIR}/GRPLHCIFData.h"
sed -i 's/CommonConstants\/LHCConstants.h/LHCConstants.h/'  "${OUT_DIR}/GRPLHCIFData.h"
sed -i 's/CommonDataFormat\/BunchFilling.h/BunchFilling.h/' "${OUT_DIR}/GRPLHCIFData.h"

get_file "Common/Types/include/CommonTypes/Units.h"

get_file "Common/Constants/include/CommonConstants/LHCConstants.h"

get_file "DataFormats/common/include/CommonDataFormat/BunchFilling.h"
sed -i 's/CommonConstants\/LHCConstants.h/LHCConstants.h/' "${OUT_DIR}/BunchFilling.h"

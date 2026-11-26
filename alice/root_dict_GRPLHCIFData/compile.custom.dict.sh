#!/usr/bin/env bash

# 2026-11-26
# shellcheck disable=SC2046

set -e -x
SOURCE_DIR="$(realpath AliceO2_GRPLHCIFData)"
OUT_DIR="."
# OUT_DIR="${SOURCE_DIR}"
OUT_NAME="GRPLHCIFData.only.dict"

rootcling -f "${OUT_DIR}/dict_${OUT_NAME}.cxx" \
  -s "lib${OUT_NAME}.so" -rml "lib${OUT_NAME}.so" -rmf "lib${OUT_NAME}.rootmap" \
  -I "${SOURCE_DIR}" \
  "${SOURCE_DIR}/GRPLHCIFData.h" "${SOURCE_DIR}/BunchFilling.h" "LinkDef.h"

# g++ -fPIC -shared "${OUT_DIR}/dict_${OUT_NAME}.cxx" "${SOURCE_DIR}/BunchFilling.cxx" -I "${SOURCE_DIR}" \
g++ -fPIC -shared "${OUT_DIR}/dict_${OUT_NAME}.cxx" -I "${SOURCE_DIR}" \
  -o "${OUT_DIR}/lib${OUT_NAME}.so" $(root-config --cflags --libs)

# -O0 is default gcc optimization option
# rm "${OUT_DIR}/dict_${OUT_NAME}.cxx" "${OUT_DIR}/lib${OUT_NAME}.rootmap"
# rm "${OUT_DIR}/lib${OUT_NAME}.so" "${OUT_DIR}/lib${OUT_NAME}_rdict.pcm"

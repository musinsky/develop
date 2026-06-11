#!/usr/bin/env bash

# 2026-06-11
# shellcheck disable=SC2046

SOURCE_DIR="$(realpath AliceO2_GRPLHCIFData)"
OUT_DIR="."
# OUT_DIR="${SOURCE_DIR}"
OUT_NAME="GRPLHCIFData"

rootcling -f "${OUT_DIR}/dict_${OUT_NAME}.cxx" \
          -I "${SOURCE_DIR}" GRPLHCIFData.h LinkDef.h
g++ -fPIC -shared -O0 "${OUT_DIR}/dict_${OUT_NAME}.cxx" -I "${SOURCE_DIR}" \
    -o "${OUT_DIR}/lib${OUT_NAME}.only.dict.so" $(root-config --cflags --libs)

# -O0 is default gcc optimization option
# rm "${OUT_DIR}/dict_${OUT_NAME}.cxx" "${OUT_DIR}/dict_${OUT_NAME}_rdict.pcm"
# rm "${OUT_DIR}/lib${OUT_NAME}.only.dict.so"

#!/usr/bin/env bash

# 2026-05-25
# shellcheck disable=SC2046

NAME='GRPLHCIFData'
HDR_DIR='AliceO2_GRPLHCIFData'

rootcling -f "${NAME}_dict.cxx" "$HDR_DIR/GRPLHCIFData.h" LinkDef.h
g++ -fPIC -shared -O0 "${NAME}_dict.cxx" -o "lib${NAME}.only.dict.so" $(root-config --cflags --libs)

# rm "${NAME}_dict.cxx" "${NAME}_dict_rdict.pcm"

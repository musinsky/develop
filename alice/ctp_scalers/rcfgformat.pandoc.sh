#!/usr/bin/env bash

# 2025-11-09

# https://gitlab.cern.ch/aliceCTP3/ctp3-ipbus/-/blob/master/doc/rcfgformat.md
MDF="rcfgformat.md"   # site required authentication

MDF_TMP="$(mktemp)" || exit 1
awk '
     BEGIN { print "**[aliceCTP3/ctp3-ipbus-master/doc/rcfgformat.md](https://gitlab.cern.ch/aliceCTP3/ctp3-ipbus/-/blob/master/doc/rcfgformat.md) (2023-03-26)**\n" }
     /^```$/ && !block_start { print "```default"; block_start=1; next }
     /^```$/ &&  block_start { block_start=0 }
     { print }
    ' "$MDF" > "$MDF_TMP"

THEME_TMP="$(mktemp --suffix='.theme')" || exit 1
pandoc --print-highlight-style=tango > "$THEME_TMP"
sed -i "s/#f8f8f8/#dfdfdf/" "$THEME_TMP"   # darker gray (for printing)
#                 #e6e6e6                  # darker gray (as Emacs print header)

MDF_PDF="$MDF.pdf"
pandoc "$MDF_TMP" --from markdown --output "$MDF_PDF" --to pdf \
       --template eisvogel \
       --highlight-style "$THEME_TMP" \
       -V disable-header-and-footer \
       -V geometry:a4paper \
       -V geometry:margin=20mm \
       -V fontsize:10pt \
       -V linkcolor:blue

rm -f "$MDF_TMP"
rm -f "$THEME_TMP"

# pandoc --list-highlight-languages (using Haskell skylighting)
#        => https://hackage.haskell.org/package/skylighting
#        => https://github.com/jgm/skylighting
# pandoc --listing (using TeX)
#        => https://ctan.org/pkg/listings
# pandoc --list-highlight-styles

# Eisvogel => https://github.com/Wandmalfarbe/pandoc-latex-template

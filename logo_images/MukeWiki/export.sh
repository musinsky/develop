#!/usr/bin/bash

# 2022-06-13

# Exo2 fonts check
FONTS="Exo2"
fc-list -q "$FONTS"
if [ $? -eq 1 ]; then
    echo "$FONTS fonts not found"
    exit 1
fi

# all outputs in one separate dir
EDIR="exports"
mkdir -p $EDIR

MUKEOBJECT=MukeWiki.Inkscape.object.svg
echo "inkscape exporting ..."

## mediawiki favicon
# v tomto pripade (obsah bez textu) je uplne jedno ci pouzijem *.object.svg alebo *.path.svg
# existuje aj moznost s --export-id, ale vtedy je favicon uplne "bez okrajov",
# co nie je prilis esteticke, vhodne
inkscape --export-filename=$EDIR/MukeWiki.favicon.png \
         --export-width=32 --export-area=170:20:530:380 $MUKEOBJECT

## mediawiki logo (vector 2010)
inkscape --export-filename=$EDIR/MukeWiki.logo.svg \
         --export-plain-svg --export-text-to-path $MUKEOBJECT

## mediawiki logos, icon (vector 2022)
inkscape --export-filename=$EDIR/MukeWiki.logo.icon.svg \
         --export-plain-svg --export-id=g29479 --export-id-only $MUKEOBJECT
# change width and height to 50 and 50 (mediawiki recommendation)
sed -i 's/width.*/width=\"50\"/'   $EDIR/MukeWiki.logo.icon.svg # one separate line
sed -i 's/height.*/height=\"50\"/' $EDIR/MukeWiki.logo.icon.svg # one separate line

## mediawiki logos, wordmark (vector 2022)
# Exo2 font-weight (Ultra-Bold=800) je pre wordmark az "prilis tlsta"
# (pre logo ako celok OK), vhodnejsia Exo2 font-weight pre wordmark Semi-Bold=600

inkscape --export-filename=- --export-id=text31681 --export-id-only $MUKEOBJECT \
    | sed 's/font-weight:800/font-weight:600/' \
    | inkscape --pipe --export-filename=$EDIR/MukeWiki.logo.wordmark.svg \
               --export-plain-svg --export-text-to-path

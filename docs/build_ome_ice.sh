#!/bin/sh

# Build using something along the lines of
# MCPP_HOME=/usr/local ICE_HOME=/usr/local/Cellar/zeroc-ice33/3.3 make


OME_RESOURCES_DIR=~/work/openmicroscopy/components/blitz/resources
N_STRIP_COMPONENTS=9
OUT_DIR=./ome
ICE_HOME=/usr/local/Cellar/zeroc-ice33/3.3
SLICE2RST=../src/slice2rst

function slice2rst {
    in=$1
    out=`echo "$in" | cut -d/ -f"$N_STRIP_COMPONENTS"- | tr / _`
    out=${out%.ice}.rst
    "$SLICE2RST" -I"$ICE_HOME/share/slice" -I"$OME_RESOURCES_DIR" "$in" \
	> "$OUT_DIR/$out"
}

mkdir -p "$OUT_DIR"
icefiles=`find "${OME_RESOURCES_DIR}" -name \*ice`
for ice in $icefiles; do
    echo Processing "$ice"
    slice2rst "$ice"
done




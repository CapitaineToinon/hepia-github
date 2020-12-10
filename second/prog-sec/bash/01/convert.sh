#!/bin/bash

# Antoine Sutter <antoinesutter@gmail.com>
# Created on 29/10/2020

shopt -s -o nounset

# remove spaces and quotes from string
function renameFile {
    tmp=${1// }
    tmp=${tmp//\"}
    tmp=${tmp//\'}
    echo "$tmp"
}

# return the filetype
function filetype {
    file -bi "$1" | cut -d'/' -f1
}

# Check if dependencies are installed
function checkDependencies {
    for dep in "$@"; do
        if ! command -v "$dep" &> /dev/null; then
            echo "The command $dep could not be found and is needed for this script to run correctly"
            exit 1
        fi
    done
}

# -- MAIN -- #
declare SOURCE
declare DEST
declare RESOLUTION
declare HELP="Wrong usage of the command. Usage : $0 source destination [resolution]"

checkDependencies convert mkdir cp

# argument 1 and 2 are required
if [ $# -lt 2 ]; then
    echo "$HELP"
    exit 1
fi

SOURCE=$1
DEST=$2
RESOLUTION=${3-}

# exit if source dir doesn't exist
if [ ! -d "$SOURCE" ]; then
    echo "$SOURCE is not a directory"
    exit 1
fi

# create destination dir if doesn't exist
mkdir -p "$DEST"

# copy the files
for f in "$SOURCE"/*; do
    if [ "$(filetype "$f")" == "image" ]; then
        filenameOnly=$(basename "$f")
        destFilename=$(renameFile "$filenameOnly")

        if [ -z "$RESOLUTION" ]; then
            # just copy the file
            cp "$SOURCE/$filenameOnly" "$DEST/$destFilename"
        else
            # copy and convert the file at the same time
            # TODO: format for file with .txt extensions
            convert "$SOURCE/$filenameOnly" -resample "$RESOLUTION" "$DEST/$destFilename"
        fi
    fi
done

# -- END MAIN -- #
unset SOURCE
unset DEST
unset RESOLUTION
unset HELP

exit 0

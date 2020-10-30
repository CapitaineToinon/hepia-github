#!/bin/bash
#
# Antoine Sutter
# 22 jan 2018

shopt -s -o nounset pipefail errexit

# **************************************** VARIABLES **************************************** #

declare CONVERTTYPE
declare MAX_COLOR
declare RED
declare GREEN
declare BLUE
declare HELP="Wrong usage of the command. Usage : $0 {-p | -a} red green blue"

# **************************************** FUNCTIONS **************************************** #

function GetCONVERTTYPE
{
    INPUT="dummy"

    while [[ ! $INPUT =~ ^[Pp]$ ]] && [[ ! $INPUT =~ ^[Aa]$ ]] && [[ -n "$INPUT" ]]; do
        read -rp "percent/absolute [P/a] : " INPUT
    done;
    
    INPUT=${INPUT:-p}
    echo "$INPUT"
    unset INPUT
}

function GetCOLOR
{
    local COLOR=-1
    local VALID=1

    if test "$CONVERTTYPE" == "a"; then 
        MAX_COLOR=255
    else
        MAX_COLOR=100
    fi

    while [[ $VALID -ne 0 ]] || [[ -z "$COLOR" ]]; do
        read -rp "How much $1 (0-$MAX_COLOR): " COLOR
        VALID=$(IfColorInRage "$COLOR")
    done;

    echo "$COLOR" # return
}

function IfColorInRage
{
    if [ -z "${1+x}" ]; then 
        echo "1" # return false
    else
        if test "$CONVERTTYPE" == "a"; then 
            MAX_COLOR=255
        else
            MAX_COLOR=100
        fi

        if [[ "$1" -le "$MAX_COLOR" && "$1" -ge "0" ]] && [[ $1 =~ ^-?[0-9]+$ ]]; then 
            echo "0" # return true
        else
            echo "1" # return false
        fi
    fi
}


# **************************************** BODY **************************************** #

if test $# -eq 0; then
    # Prompt mode
    CONVERTTYPE=$(GetCONVERTTYPE)
    RED=$(GetCOLOR "red")
    GREEN=$(GetCOLOR "green")
    BLUE=$(GetCOLOR "blue")
else 
    # Command line mode
    if test $# -eq 4; then
        case "$1" in
            "-p") CONVERTTYPE="p";;
            "-a") CONVERTTYPE="a";;
            *) echo "$HELP"; exit 1;;
        esac

        if test $CONVERTTYPE == "a"; then 
            MAX_COLOR=255
        else
            MAX_COLOR=100
        fi

        if [[ $(IfColorInRage "$2") -eq 0 ]] && [[ $(IfColorInRage "$3") -eq 0 ]] && [[ $(IfColorInRage "$4") -eq 0 ]]; then
            RED=$2
            GREEN=$3
            BLUE=$4
        else
            echo "Please enter a color value between 0 and $MAX_COLOR"
            exit
        fi

    else
        echo "$HELP"
        exit
    fi
fi

if test "$CONVERTTYPE" == "p"; then 
    ("RED=(255 * $RED) / 100")
    ("GREEN=(255 * $GREEN) / 100")
    ("BLUE=(255 * $BLUE) / 100")
fi

printf 'Hexa value : #%02X%02X%02X\n' "$RED" "$GREEN" "$BLUE"

# **************************************** UNSET **************************************** #
unset CONVERTTYPE
unset MAX_COLOR
unset RED
unset GREEN
unset BLUE
unset HELP
#!/bin/bash
# Antoine Sutter

shopt -s -o nounset

# List all pid of a current user
function getProcessesForUser {
    ps -u "$1" -o pid=
}

# test if a signal exists
function findSignal {
    kill -l | grep -c "$1"
}

# -- MAIN -- #
declare USERNAME
declare SIGNAL
declare COUNT
declare HELP="Wrong usage of the command. Usage : $0 <username or userid> <signal>"

# argument 1 and 2 are required
if [ $# -lt 2 ]; then
    echo "$HELP"
    exit 1
fi

USERNAME="$1"
SIGNAL="$2"

# add "SIG" at the start of the signal if needed
if [[ ! $SIGNAL == "SIG"* ]]; then
    SIGNAL="SIG$SIGNAL"
    SIGNAL=${SIGNAL^^}
fi

# test if a signal exists
COUNT=$(findSignal "$SIGNAL")
if [ "$COUNT" -eq 0 ]; then
    echo "The signal $SIGNAL does not exist. Please use kill -l to see which signals you can use."
    exit 1
fi

# kill
for pid in $(getProcessesForUser "$USERNAME"); do
    kill -"$SIGNAL" "$pid"
done

unset USERNAME
unset SIGNAL
unset COUNT
unset HELP

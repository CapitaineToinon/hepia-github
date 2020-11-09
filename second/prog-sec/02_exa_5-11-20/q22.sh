#!/bin/bash

pattern="$1"

for i in *; do 
if [ -f "$i" ]; then
    nb_inst=$(grep "$pattern" "$i" | wc -l ); 
    if [ $nb_inst -eq 1 ]; then
        echo "$i": contient "$nb_inst" fois "$pattern"; 
    fi
    if [ $nb_inst -gt 1 ]; then
        echo "$i": contient "$nb_inst" fois "$pattern"; 
    fi
fi
done
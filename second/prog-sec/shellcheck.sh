#!/bin/bash

if [ "$(shellcheck ./**/*.sh | wc -l)" -eq 0 ]; then
    echo "OK"
fi
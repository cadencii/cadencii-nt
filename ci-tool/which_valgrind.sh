#!/bin/bash

function which_valgrind { (
    local valgrind=$(which valgrind64 2>/dev/null)

    if [ -z "$valgrind" ]; then
        valgrind=$(which valgrind 2>/dev/null)
    fi

    if [ -z "$valgrind" ]; then
        echo "valgrind not found"
        exit 1
    fi

    echo $valgrind
) }


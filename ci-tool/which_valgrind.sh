#!/bin/bash

function which_valgrind { (
    local valgrind=$(which valgrind64)

    if [ -z "$valgrind" ]; then
        valgrind=$(which valgrind)
    fi

    if [ -z "$valgrind" ]; then
        echo "valgrind not found"
        exit 1
    fi

    echo $valgrind
) }


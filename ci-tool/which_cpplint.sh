#!/bin/bash

function which_cpplint {
    local cpplint=$(which cpplint 2>/dev/null)
    if [ -z "$cpplint" ]; then
        svn export http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py /tmp/cpplint.py >/dev/null 2>&1
        cpplint="python /tmp/cpplint.py"
    fi
    if [ -z "$cpplint" ]; then
        echo "cpplint command not found"
        exit 1
    fi
    echo $cpplint
}


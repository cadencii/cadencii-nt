#!/bin/bash

function which_qmake {
    local qmake=$(which qmake 2>/dev/null)
    if [ -z "$qmake" ]; then
        qmake=$(which qmake-qt4 2>/dev/null)
    fi
    if [ -z "$qmake" ]; then
        for file in $(find ~/QtSDK/Desktop/Qt/*/gcc/bin -name qmake); do
            qmake=$file
            break
        done
    fi
    if [ -z "$qmake" ]; then
        echo "qmake command not found"
        exit 1
    fi
    echo $qmake
}


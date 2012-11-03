#!/bin/bash

function run_ui_test { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    . ${workspace}/ci-tool/which_qmake.sh
    local qmake=$(which_qmake)

    cd ${workspace}/ui-test
    rm -f bin/*
    rm -f Makefile
    ${qmake} ui-test.pro -o Makefile
    make
    xterm -display localhost:1 -e ${workspace}/ui-test/bin/ui-test -xunitxml -o ${workspace}/ui-test/result.xml
    cd ${workspace}/ui-test
    make clean
) }

run_ui_test


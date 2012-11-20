#!/bin/bash

function run_ui_test { (
    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)
    . ${workspace}/ci-tool/which_qmake.sh
    local qmake=$(which_qmake)

    cd ${workspace}/ui-test
    rm -f bin/*
    rm -f Makefile
    ${qmake} CONFIG+=declarative_debug -o Makefile ui-test.pro
    make
    xterm -display localhost:1 -e ${workspace}/ui-test/bin/ui-test -xunitxml -o ${workspace}/ui-test/result.xml
    cd ${workspace}/ui-test
    make clean
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

run_ui_test


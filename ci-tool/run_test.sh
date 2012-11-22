#!/bin/bash

function run_test { (
    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)
    rm -f ${workspace}/test/cppunit.xml
    rm -f ${workspace}/coverage.xml
    g++ ${workspace}/test/*.cpp ${workspace}/test/**/*.cpp -w -coverage -ldl -lcppunit -o ${workspace}/test.exe
    ${workspace}/test.exe -o ${workspace}/test/cppunit.xml -f ${workspace}/test
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

run_test


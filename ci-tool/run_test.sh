#!/bin/bash

function run_test { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    rm -f ${workspace}/test/cppunit.xml
    rm -f ${workspace}/coverage.xml
    g++ ${workspace}/test/*.cpp ${workspace}/test/**/*.cpp -w -coverage -ldl -lcppunit -o ${workspace}/test.exe
    ${workspace}/test.exe -o ${workspace}/test/cppunit.xml -f ${workspace}/test
) }

run_test


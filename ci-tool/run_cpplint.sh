#!/bin/bash

function run_cpplint { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    local result="${workspace}/cpplint_result.txt"
    rm -f ${result}
    for file in $(find ${workspace} -name '*.cpp' -or -name '*.hpp' 2>&1 | grep -v "^${workspace}/vsq/" | grep -v "^${workspace}/qt-solutions/" | grep -v "^${workspace}/test/" | grep -v "^${workspace}/ui-test/" | grep -v "^${workspace}/bin/"); do
        python ${workspace}/ci-tool/cpplint.py ${file} 2>&1 \
            | grep -v '^Done processing ' \
            |  grep -v '^Total errors found: ' \
            | grep -v ' \[1\]$' \
            | grep -v 'Lines should be <= 80 characters long ' \
            | grep -v 'Include the directory when naming .h files' \
            | tee -a ${result}
    done
) }

run_cpplint


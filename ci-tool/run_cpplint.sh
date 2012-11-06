#!/bin/bash

function run_cpplint { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    local result="${workspace}/cpplint_result.txt"
    rm -f ${result}
    for file in $(find ${workspace} -name '*.cpp' -or -name '*.hpp' 2>&1 | grep -v "^${workspace}/vsq/" | grep -v "^${workspace}/qt-solutions/" | grep -v "^${workspace}/test/" | grep -v "^${workspace}/ui-test/" | grep -v "^${workspace}/bin/"); do
        python ${workspace}/ci-tool/cpplint.py ${file} >> ${result} 2>&1
    done
) }

run_cpplint


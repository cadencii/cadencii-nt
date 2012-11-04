#!/bin/bash

function run_cppcheck { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    . ${workspace}/ci-tool/which_qmake.sh
    local qmake=$(which_qmake)

    cd ${workspace}
    rm -f Makefile
    $qmake -o Makefile Cadencii.pro
    make
    ignoreMocFileOption=""
    for mocFile in $(find ${workspace} 2>&1 | grep moc_); do
        ignoreMocFileOption="${ignoreMocFileOption} -i ${mocFile}"
    done
    cppcheck ${workspace} -i ${workspace}/qt-solutions -i ${workspace}/vsq ${ignoreMocFileOption} --enable=all -q --xml 2>&1 \
        | sed "s:${workspace}/::g" \
        | tee ${workspace}/cppcheck-result.xml
    cd ${workspace}
    make clean
    rm -f Makefile
) }

run_cppcheck


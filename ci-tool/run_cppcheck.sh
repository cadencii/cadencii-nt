#!/bin/bash

function run_cppcheck { (
    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)

    cppcheck ${workspace} -i ${workspace}/qt-solutions -i ${workspace}/vsq -i ${workspace}/bin --enable=performance,portability,information -q --xml 2>&1 \
        | sed "s:${workspace}/::g" \
        | tee ${workspace}/cppcheck-result.xml
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

run_cppcheck


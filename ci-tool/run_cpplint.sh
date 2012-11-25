#!/bin/bash

function run_cpplint { (
    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)
    local result="${workspace}/cpplint_result.txt"
    rm -f ${result}
    touch ${result}

    for \
        file \
    in \
        $(
            find ${workspace} -name '*.cpp' -or -name '*.hpp' 2>&1 \
            | grep -v "^${workspace}/vsq/" \
            | grep -v "^${workspace}/qt-solutions/" \
            | grep -v "^${workspace}/qt-property-browser/" \
            | grep -v "^${workspace}/test/" \
            | grep -v "^${workspace}/ui-test/" \
            | grep -v "^${workspace}/bin/" \
        )
    do

        local tempFile=$(mktemp /tmp/cpplint_XXXXXXXXXXXXX)
        cat ${file} | cpplint - 2>&1 \
            | sed "s:^-\:\(.*\)\$:${file}\:\1:g" \
            | grep -v '^Done processing ' \
            | grep -v '^Total errors found: ' \
            | grep -v 'Lines should be <= 80 characters long ' \
            | grep -v 'Include the directory when naming .h files' \
            | grep -v 'Using sizeof(type).  Use sizeof(varname) instead if possible' \
            > ${tempFile}

        while read line; do
            local grepResult=$(echo ${line} | grep 'All parameters should be named in a function')
            if [ -n "${grepResult}" ]; then
                local warnedFile=$(echo ${line} | cut -f1 -d':')
                local lineNumber=$(echo ${line} | cut -f2 -d':')
                local warnedLine=$(
                    cat ${warnedFile} \
                    | head -${lineNumber} \
                    | tail -1 \
                    | grep -v 'SIGNAL' \
                    | grep -v 'SLOT'
                )
                if [ -n "${warnedLine}" ]; then
                    echo ${line} | tee -a ${result}
                fi
            else
                echo ${line} | tee -a ${result}
            fi
        done < ${tempFile}

        rm -f ${tempFile}
    done
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

run_cpplint


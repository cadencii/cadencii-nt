#!/bin/bash

function run_cppcheck { (
    local workspace=$(cd $(dirname $0)/../; pwd)

    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)

    find ${workspace} 2>/dev/null \
        | sed "s:^${workspace}/\\(.*\\)\$:\\1:g" \
        | grep -v "^vsq/" \
        | egrep '.hpp$|.cpp$' \
        | grep -v '^qt-solutions/' \
        | grep -v '^qt-property-browser/' \
        | grep -v '^test/' \
        | grep -v '^ui-test/' \
        | grep -v '^bin/' \
        > ${workspace}/Makefile.cppcheck.conf

    local source_files=""
    for source in $(cat ${workspace}/Makefile.cppcheck.conf); do
        if [ -f "${workspace}/${source}" ]; then
            source_files="${source_files} ${workspace}/${source}"
        fi
    done

    local make_file="${workspace}/Makefile.cppcheck"
    local result_files=""
    echo "XML_FILES=" | tr -d '\n' > ${make_file}
    for source in ${source_files}; do
        local result_path="$(get_cppcheck_result_path ${source} ${workspace})"
        result_files="${result_files} ${result_path}"
        echo "${result_path} " | tr -d '\n' >> ${make_file}
    done
    echo "" >> ${make_file}
    echo "" >> ${make_file}

    local suppress_list='missingInclude unusedFunction'
    for source in ${source_files}; do
        echo "$(get_cppcheck_result_path ${source} ${workspace}): ${source}" >> ${make_file}
        echo "	cppcheck \"\$<\" --enable=all -q --xml 2>&1 \\" >> ${make_file}
        for suppress in ${suppress_list}; do
            echo "	    | grep -v '\"${suppress}\"' \\" >> ${make_file}
        done
        echo "	    > \"\$@\"" >> ${make_file}
        echo "" >> ${make_file}
    done
    echo "all: \$(XML_FILES)" >> ${make_file}

    for file in $(ls -1 ${workspace}/cppcheck-result-*.xml); do
        local result_path="$(basename ${file})"
        if [ -z "$(echo ${result_files} | grep ${result_path})" ]; then
            rm ${file}
        fi
    done
    make -j 2 -f ${make_file} all

    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

function get_cppcheck_result_path { (
    local relative_path=$(echo ${1} | sed "s:^${2}/\\(.*\\)\$:\\1:g")
    echo "cppcheck-result-$(echo ${relative_path} | tr / _ | tr . _ ).xml" | tr -d '\n'
) }

run_cppcheck


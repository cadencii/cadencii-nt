#!/bin/bash

function run_cppcheck { (
    local workspace=$(cd $(dirname $0)/../; pwd)
    . ${workspace}/ci-tool/which_qmake.sh
    local qmake=$(which_qmake)

    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)

    local temporary_make_file="${workspace}/Makefile.cppcheck.tmp"

    ${qmake} ${workspace}/Cadencii.pro -o ${temporary_make_file}
    echo "prepare_cppcheck:" >> ${temporary_make_file}
    echo "	echo \$(SOURCES) > ${workspace}/Makefile.cppcheck.conf" >> ${temporary_make_file}
    make -f ${temporary_make_file} prepare_cppcheck

    local source_files=""
    for source in $(cat ${workspace}/Makefile.cppcheck.conf); do
        if [ -n "$(echo ${source} | grep -v '/qrc_' | grep -v '/moc_')" ]; then
            source_files="${source_files} ${source}"
        fi
    done

    local make_file="${workspace}/Makefile.cppcheck"
    local result_files=""
    echo "XML_FILES=" | tr -d '\n' > ${make_file}
    for source in ${source_files}; do
        local result_path="$(get_cppcheck_result_path ${source})"
        result_files="${result_files} ${result_path}"
        echo "${result_path} " | tr -d '\n' >> ${make_file}
    done
    echo "" >> ${make_file}
    echo "" >> ${make_file}

    for source in ${source_files}; do
        echo "$(get_cppcheck_result_path ${source}): ${source}" >> ${make_file}
        echo "	cppcheck \"\$<\" --enable=performance,portability,information -q --xml 2>&1 | grep -v '"missingInclude"' > \"\$@\"" >> ${make_file}
        echo "" >> ${make_file}
    done
    echo "all: \$(XML_FILES)" >> ${make_file}

    for file in $(ls -1 ${workspace}/cppcheck-result-*.xml); do
        local result_path="$(get_cppcheck_result_path ${file})"
        if [ -z "$(echo ${result_files} | grep ${result_path})" ]; then
            rm ${file}
        fi
    done
    make -f ${make_file} all

    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

function get_cppcheck_result_path { (
    echo "$(basename ${1})" | sed 's:^\(.*\)[.]cpp$:cppcheck-result-\1.xml:g' | tr / _ | tr -d '\n'
) }

run_cppcheck


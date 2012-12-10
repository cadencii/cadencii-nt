#!/bin/bash

function run_cppcheck { (
    local workspace=$(cd $(dirname $0)/../; pwd)

    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)

    find ${workspace} 2>/dev/null \
        | sed "s:^${workspace}/\\(.*\\)\$:\\1:g" \
        | grep -v "^vsq/" \
        | egrep '[.]hpp$|[.]cpp$' \
        | grep -v '^qt-solutions/' \
        | grep -v '^qt-property-browser/' \
        | grep -v '^test/' \
        | grep -v '^ui-test/' \
        | grep -v '^bin/' \
        | sed 's:^\(.*\)[.]cpp$:\1:g' \
        | sed 's:^\(.*\)[.]hpp$:\1:g' \
        | sort \
        | uniq \
        | sed 's:^[ ]*\(.*\)$:\1:g' \
        > ${workspace}/Makefile.cppcheck.conf

    local result_files=""
    local make_file="${workspace}/Makefile.cppcheck"
    local make_file_body="${workspace}/Makefile.cppcheck.body"
    rm -f "${make_file}"
    rm -f "${make_file_body}"
    echo "XML_FILES=" | tr -d '\n' > "${make_file}"
    touch "${make_file_body}"
    while read source; do
        local file=$(echo ${source} | cut -f2 -d' ')
        local cpp_file="${file}.cpp"
        local hpp_file="${file}.hpp"
        local xml_file=""
        local dependent_file_1=""
        local dependent_file_2=""

        if [ -e ${cpp_file} -a -e ${hpp_file} ]; then
            # cpp and hpp
            xml_file="$(get_cppcheck_result_path ${file} ${workspace})"
            dependent_file_1="${cpp_file}"
            dependent_file_2="${hpp_file}"
        elif [ -e ${cpp_file} ]; then
            # cpp only
            xml_file="$(get_cppcheck_result_path ${cpp_file} ${workspace})"
            dependent_file_1="${cpp_file}"
        else
            # hpp only
            xml_file="$(get_cppcheck_result_path ${hpp_file} ${workspace})"
            dependent_file_1="${hpp_file}"
        fi

        result_files="${result_files} ${xml_file}"

        echo "${xml_file} " \
            | tr -d '\n' \
            >> "${make_file}"
        write_cppcheck_command "${xml_file}" "${dependent_file_1}" "${dependent_file_2}" "${make_file_body}"
    done < ${workspace}/Makefile.cppcheck.conf

    echo "" >> "${make_file}"
    echo "" >> "${make_file}"
    cat "${make_file_body}" >> "${make_file}"
    echo "" >> "${make_file}"
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

function write_cppcheck_command { (
    local xml_file=$1
    local dependent_file_1=$2
    local dependent_file_2=$3
    local make_file_body=$4

    local suppress_list='missingInclude unusedFunction'
    echo "${xml_file}: ${dependent_file_1} ${dependent_file_2}" >> "${make_file_body}"
    echo "	cppcheck ${dependent_file_1} ${dependent_file_2} --enable=all -q --xml 2>&1 \\" >> "${make_file_body}"
    for suppress in ${suppress_list}; do
       echo "	    | grep -v '\"${suppress}\"' \\" >> "${make_file_body}"
    done
    echo "	    > \"\$@\"" >> "${make_file_body}"
    echo "" >> "${make_file_body}"
) }

run_cppcheck


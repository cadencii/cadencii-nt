#!/bin/bash

function run_valgrind { (
    echo "---------------------------------------------------------"
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 start"
    local workspace=$(cd $(dirname $0)/../; pwd)
    . ${workspace}/ci-tool/which_ruby.sh
    . ${workspace}/ci-tool/which_valgrind.sh
    local ruby=$(which_ruby)
    local valgrind=$(which_valgrind)

    g++ ${workspace}/test/*Test.cpp ${workspace}/test/**/*Test.cpp ${workspace}/test/main.cpp -g -lcppunit -o ${workspace}/test/a.out
    $valgrind --leak-check=yes --xml=yes --xml-file=${workspace}/valgrind.xml ${workspace}/test/a.out -o ${workspace}/test/hoge.xml -f ${workspace}/test
    $ruby ${workspace}/ci-tool/filter_valgrind_result.rb ${workspace}/valgrind.xml
    echo "[$(date +"%Y-%m-%d %H:%M:%S")] $0 end"
) }

run_valgrind


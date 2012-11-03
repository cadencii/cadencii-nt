#!/bin/sh

g++ ${WORKSPACE}/test/*Test.cpp ${WORKSPACE}/test/**/*Test.cpp ${WORKSPACE}/test/main.cpp -g -lcppunit -o ${WORKSPACE}/test/a.out
valgrind --leak-check=yes --xml=yes --xml-file=${WORKSPACE}/valgrind.xml ${WORKSPACE}/test/a.out -o ${WORKSPACE}/test/hoge.xml -f ${WORKSPACE}/test
RUBY=${HOME}/.rvm/rubies/ruby-1.9.3-p286/bin/ruby
${RUBY} ${WORKSPACE}/ci-tool/filter_valgrind_result.rb ${WORKSPACE}/valgrind.xml


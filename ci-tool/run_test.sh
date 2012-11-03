#!/bin/sh
rm -f ${WORKSPACE}/test/cppunit.xml
rm -f ${WORKSPACE}/coverage.xml
g++ ${WORKSPACE}/test/*.cpp ${WORKSPACE}/test/**/*.cpp -w -coverage -ldl -lcppunit -o ${WORKSPACE}/test.exe
${WORKSPACE}/test.exe -o ${WORKSPACE}/test/cppunit.xml -f ${WORKSPACE}/test


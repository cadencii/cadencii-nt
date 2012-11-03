#!/bin/sh

cd ${WORKSPACE}; rm -f Makefile; qmake-qt4 -o Makefile Cadencii.pro; make
ignoreMocFileOption=""; for mocFile in $(find ${WORKSPACE} 2>&1 | grep moc_); do ignoreMocFileOption="${ignoreMocFileOption} -i ${mocFile}"; done
cppcheck ${WORKSPACE} -i ${WORKSPACE}/qt-solutions -i ${WORKSPACE}/vsq ${ignoreMocFileOption} --enable=all -q --xml 2>&1 | sed "s:${WORKSPACE}/::g" | tee ${WORKSPACE}/cppcheck-result.xml
cd ${WORKSPACE}; make clean; rm -f Makefile


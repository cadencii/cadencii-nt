#!/bin/sh

cd ${WORKSPACE}/ui-test; rm -f bin/*; rm -f Makefile; qmake-qt4 ui-test.pro -o Makefile; make
xterm -display localhost:1 -e ${WORKSPACE}/ui-test/bin/ui-test -xunitxml -o ${WORKSPACE}/ui-test/result.xml
cd ${WORKSPACE}/ui-test; make clean


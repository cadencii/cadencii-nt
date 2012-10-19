#!/bin/sh

workingDirectory=$(cd $(dirname $0);pwd)

(
    qmake ${workingDirectory}/../Cadencii.pro -o ${workingDirectory}/Makefile
    rm -f ${workingDirectory}/../bin/release/ui_*.h
    rm -f ${workingDirectory}/../bin/debug/ui_*.h
    cd ${workingDirectory}
    make compiler_uic_clean
    make compiler_uic_make_all

    rm -f ${workingDirectory}/default.pot.files
    cd ${workingDirectory}/../
    find . -type f -name \*.cpp -or -name \*.h | grep -v 'qt-solutions' | grep -v 'ui-test' | sed 's:^[.]/\(.*\)$:\1:g' > ${workingDirectory}/default.pot.files
    lupdate @${workingDirectory}/default.pot.files -ts ${workingDirectory}/../default.pot.ts
    lconvert ${workingDirectory}/../default.pot.ts -o ${workingDirectory}/../default.pot
    rm -f ${workingDirectory}/default.pot.files
    rm -f ${workingDirectory}/../default.pot.ts
)


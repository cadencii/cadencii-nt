#!/bin/sh

workingDirectory=$(cd $(dirname $0);pwd)

(
    rm -f ${workingDirectory}/default.pot.files
    cd ${workingDirectory}/../
    find . -type f -name \*.cpp -or -name \*.h | grep -v 'qt-solutions' | sed 's:^[.]/\(.*\)$:\1:g' > ${workingDirectory}/default.pot.files
    lupdate @${workingDirectory}/default.pot.files -ts ${workingDirectory}/../default.pot.ts
    lconvert ${workingDirectory}/../default.pot.ts -o ${workingDirectory}/../default.pot
    rm -f ${workingDirectory}/default.pot.files
    rm -f ${workingDirectory}/../default.pot.ts
)


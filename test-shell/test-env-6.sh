#!/bin/bash
source ./utils.sh
it "ENV-06: Enviroment variable expansion"

rm -f $CSH_OUT $BUSH_OUT
echo "setenv C ap" > $BUSH_IN
echo "setenv D le" >> $BUSH_IN
echo "echo I like \${C}p\${D}" >> $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT

grep "I like apple" $BUSH_OUT
check $?
succeed

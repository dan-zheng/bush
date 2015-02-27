#!/bin/bash
source ./utils.sh
it "ENV-03: Enviroment: replace variable"

rm -f $CSH_OUT $BUSH_OUT
echo "setenv aaa bbbb" > $BUSH_IN
echo "setenv aaa pppp" >> $BUSH_IN
echo "printenv" >> $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT

grep aaa=pppp $BUSH_OUT > out.tmp.txt 2>&1
check $?
succeed

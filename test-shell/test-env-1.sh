#!/bin/bash

source ./utils.sh
it "ENV-01: Enviroment: Set variable"

rm -f $BUSH_OUT
echo "setenv aaa bbbb" > $BUSH_IN
echo "printenv" >> $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT

grep aaa=bbbb $BUSH_OUT > out.tmp.txt 2>&1
check $?
succeed

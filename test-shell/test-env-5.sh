#!/bin/bash
source ./utils.sh
it "ENV-05: Enviroment variable expansion"

rm -f $CSH_OUT $BUSH_OUT
echo "setenv A hello" > $BUSH_IN
echo "setenv B world" >> $BUSH_IN
echo "echo \${A} \${B}" >> $BUSH_IN
echo "A=hello" > $CSH_IN
echo "B=world" >> $CSH_IN
echo "echo \${A} \${B}" >> $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

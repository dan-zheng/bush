#!/bin/bash
source ./utils.sh
it "ENV-02: Enviroment: Redirect printenv"

rm -f $CSH_OUT $BUSH_OUT
echo "/usr/bin/printenv | grep PATH" > $CSH_IN

#Make sure that printenv is a built in command
(
PATH=/bin
export PATH
echo "printenv | grep PATH" > $BUSH_IN
)

$SHELL < $BUSH_IN > $BUSH_OUT
/bin/sh < $CSH_IN > $CSH_OUT

sort $BUSH_OUT -o $BUSH_OUT
sort $CSH_OUT -o $CSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

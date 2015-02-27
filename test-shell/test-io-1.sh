#!/bin/bash

source utils.sh
it "IO-01: Output redirection..."

rm -f $BUSH_OUT $CSH_OUT
echo "ls files > $BUSH_OUT" > $BUSH_IN
echo "ls files > $CSH_OUT" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

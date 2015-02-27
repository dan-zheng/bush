#!/bin/bash

source utils.sh
it "PIPE-01: 2 Pipes"

rm -f $CSH_OUT $BUSH_OUT
echo "cat file1.cc | grep malloc" > $BUSH_IN
/bin/sh < $BUSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

#!/bin/bash

source utils.sh
it "PIPE-03: Pipes and output redirection"

rm -f $BUSH_OUT $CSH_OUT $CSH_OUT $BUSH_OUT
echo "cat file1.cc | grep malloc > $BUSH_OUT" > $BUSH_IN
echo "cat file1.cc | grep malloc > $CSH_OUT" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

#!/bin/bash

source utils.sh
it "PIPE-04: Pipes and error redirection"

rm -f $CSH_ERR $BUSH_ERR $CSH_OUT $BUSH_OUT
echo "cat file1.cc | ls nonexisting >& $CSH_ERR" > $BUSH_IN
echo "cat file1.cc | ls nonexisting > $BUSH_ERR 2>&1" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $CSH_ERR $BUSH_ERR
check $?
succeed

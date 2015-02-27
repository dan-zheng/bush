#!/bin/bash

source utils.sh
it "IO-03: Error redirection..."

rm -f $CSH_ERR $BUSH_ERR
rm -f $CSH_OUT $BUSH_OUT
echo "ls /none >& $CSH_ERR" > $BUSH_IN
echo "ls /none >$BUSH_ERR 2>&1" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $CSH_ERR $BUSH_ERR
check $?
succeed

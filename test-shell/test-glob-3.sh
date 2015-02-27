#!/bin/bash

source ./utils.sh
it "GLOB-03: echo /usr/lib/libt*"

rm -f $CSH_OUT $BUSH_OUT
echo "echo /usr/lib/libt*" > $BUSH_IN
echo "echo /usr/lib/libt*" > $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

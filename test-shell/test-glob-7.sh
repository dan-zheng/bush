#!/bin/bash

source ./utils.sh
it "GLOB-07: echo /usr/lib/lib?.a"

rm -f $CSH_OUT $BUSH_OUT
echo "echo /usr/lib/lib?.a" > $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

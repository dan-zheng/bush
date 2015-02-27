#!/bin/bash

source ./utils.sh
it "GLOB-01: echo *"

rm -f $CSH_OUT $BUSH_OUT
touch $BUSH_IN $CSH_IN $BUSH_OUT $CSH_OUT
echo "echo *" > $BUSH_IN
echo "echo *" > $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

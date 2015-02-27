#!/bin/bash

source ./utils.sh
it "GLOB-02: echo /usr/lib/*"

rm -f $CSH_OUT $BUSH_OUT
echo "echo /usr/lib/*" > $BUSH_IN
echo "echo /usr/lib/*" > $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

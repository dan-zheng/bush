#!/bin/bash

source ./utils.sh
it "BUG-02: cd pwd"

rm $CSH_OUT $BUSH_OUT
echo "echo files/*a*" > $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

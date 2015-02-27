#!/bin/bash

source ./utils.sh
it "WORD-01: Parsing words and special characters"

rm -f $BUSH_OUT $CSH_OUT $CSH_OUT $BUSH_OUT
echo "ls file1.cc>$BUSH_OUT" > $BUSH_IN
echo "ls file1.cc>$CSH_OUT" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

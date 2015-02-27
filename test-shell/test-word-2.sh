#!/bin/bash

source ./utils.sh
it "WORD-02: Parsing words and special characters"

rm $CSH_OUT $BUSH_OUT
echo "ls|grep fil" > $BUSH_IN
echo "ls|grep fil" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

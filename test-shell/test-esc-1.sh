#!/bin/bash

source ./utils.sh
it "ESC-01: cd; echo lll \">\" "

rm -f $CSH_OUT $BUSH_OUT
echo "cd" > $BUSH_IN
echo "echo lll \">\"" >> $BUSH_IN

echo "cd" >  $CSH_IN
echo "echo lll \">\"" >> $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

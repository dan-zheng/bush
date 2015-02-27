#!/bin/bash

source ./utils.sh
it "GLOB-05: cd; echo *"

rm -f $CSH_OUT $BUSH_OUT
echo "cd" > $BUSH_IN
echo "echo *" >> $BUSH_IN
echo "echo .*" >> $BUSH_IN

echo "cd" >  $CSH_IN
echo "echo *" >> $CSH_IN
echo "echo .*" >> $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

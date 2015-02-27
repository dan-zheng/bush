#!/bin/bash

source ./utils.sh
it "ESC-02: ls \"command.cc Makefile\""

rm -f $CSH_OUT $BUSH_OUT
echo "ls file1.cc files" > $BUSH_IN
echo "ls \"file1.cc files\"" >> $BUSH_IN

echo "ls file1.cc files" >  $CSH_IN
echo "ls \"file1.cc files\"" >> $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

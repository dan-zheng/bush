#!/bin/bash

source utils.sh
it "IO-04: Appending output..."

rm -f $CSH_ERR $BUSH_ERR $BUSH_OUT $CSH_OUT $CSH_OUT $BUSH_OUT
echo "ls files > $BUSH_OUT" > $BUSH_IN
echo "ls files > $BUSH_OUT" >> $BUSH_IN
echo "ls files >> $BUSH_OUT" >> $BUSH_IN
echo "ls files > $CSH_OUT" > $CSH_IN
echo "ls files > $CSH_OUT" >> $CSH_IN
echo "ls files >> $CSH_OUT" >> $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $BUSH_OUT $CSH_OUT
check $?
succeed

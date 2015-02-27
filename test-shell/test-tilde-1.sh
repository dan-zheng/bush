#!/bin/bash

source ./utils.sh
it "TILDE-01: Tilde expansion"

echo "ls ~ > $CSH_OUT" > $CSH_IN
echo "ls ~cs180 >> $CSH_OUT" >> $CSH_IN
echo "ls ~cs180/share >> $CSH_OUT" >> $CSH_IN

echo "ls ~ > $BUSH_OUT" > $BUSH_IN
echo "ls ~cs180 >> $BUSH_OUT" >> $BUSH_IN
echo "ls ~cs180/share >> $BUSH_OUT" >> $BUSH_IN

/bin/bash < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT


diff $BUSH_OUT $CSH_OUT > /dev/null 2>&1
check $?
succeed

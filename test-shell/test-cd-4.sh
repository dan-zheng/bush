#!/bin/bash

source ./utils.sh
it "CD-04: cd notfound >& $CSH_ERR"

rm -f $CSH_OUT $BUSH_OUT
echo "cd notfound >& $CSH_ERR" > $BUSH_IN
echo "pwd" >> $CSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

grep "No such file or directory" $CSH_ERR
check $?
succeed

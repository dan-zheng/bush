#!/bin/bash

source utils.sh
it "IO-06: Multiple redirection..."

rm -f $BUSH_OUT $CSH_OUT
rm -f $CSH_OUT $BUSH_OUT
echo "ls > $BUSH_OUT > $CSH_OUT" > $BUSH_IN
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

grep "Ambiguous output redirect" $BUSH_OUT
check $?
succeed

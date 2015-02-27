#!/bin/bash

source ./utils.sh
it "CD-01: cd /etc; pwd"

rm -f $CSH_OUT $BUSH_OUT
echo "cd /etc" > $BUSH_IN
echo "pwd" >> $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

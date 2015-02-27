#!/bin/bash

source ./utils.sh
it "CD-05: cd /etc"

rm -f $CSH_OUT $BUSH_OUT
echo "cd /etc" > $BUSH_IN
echo "pwd" >> $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $CSH_OUT $BUSH_OUT
check $?
succeed

#!/bin/bash

source utils.sh
it "IO-02: Test stdin redirection..."

echo "cat < files/aaa" > $BUSH_IN
echo "cat < files/aaa" > $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT
$SHELL < $BUSH_IN > $BUSH_OUT

diff $CSH_OUT $BUSH_OUT
check $?
succeed

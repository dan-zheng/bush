#!/bin/bash

source ./utils.sh
it "ESC-04: Escape Chars"

rm -f $CSH_OUT $BUSH_OUT
echo "echo lll\< kkkk\\\\\\ uuuu\&" > $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

grep "lll< kkkk[\] uuuu&" $BUSH_OUT
check $?
succeed

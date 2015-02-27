#!/bin/bash

source ./utils.sh
it "ESC-03: touch \"hello world.tmp.txt\""

rm -f $CSH_OUT $BUSH_OUT "hello world.tmp.txt"

echo "touch \"hello world.tmp.txt\"" > $BUSH_IN
echo "ls" >>  $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

if [ ! -f "hello world.tmp.txt" ]; then
  fail
fi
succeed

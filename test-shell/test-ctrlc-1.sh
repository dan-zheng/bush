#!/bin/bash

source ./utils.sh

it "CTRLC-01: test ctrl-c"

echo "sleep 10" > $BUSH_IN
$SHELL < $BUSH_IN > $BUSH_OUT &

# Send ctrl-c to process
sleep 1
kill -INT $!
sleep 1
/bin/ps -e -o pid | grep $! > out.tmp.txt 2>&1
if [ $? -ne 0 ]
then
  panic "No shell process"
  fail
fi

#Make sure that shell has gone away
kill -KILL $!
sleep 1
/bin/ps -e -o pid | grep $! > out.tmp.txt 2>&1
if [ $? -eq 0 ]
then
  panic "Shell process still there"
  fail
fi
succeed
grade9=5

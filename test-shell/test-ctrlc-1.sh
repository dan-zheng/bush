#!/bin/sh

source ./utils.sh

pgray "CTRLC-01: test ctrl-c"
echo "sleep 10" > shell-in

$SHELL < shell-in > shell-out &

# Send ctrl-c to process
sleep 1
kill -INT $!
sleep 1
/bin/ps -e -o pid | grep $! > out 2>&1
if [ $? -ne 0 ]
then
  echo "No shell process"
  fail
fi

#Make sure that shell has gone away
kill -KILL $!
sleep 1
/bin/ps -e -o pid | grep $! > out 2>&1
if [ $? -eq 0 ]
then
  echo "Shell process still there"
  fail
fi
succeed

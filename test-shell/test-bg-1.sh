#!/bin/sh

source utils.sh

rm -f csh-out shell-out
pgray "BG-01: Background"
echo "sleep 5 &" > shell-in
echo "touch bgfile" >> shell-in
$SHELL < shell-in > shell-out &

sleep 1
if [ ! -f bgfile ]
then
  fail
fi
succeed

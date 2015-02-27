#!/bin/bash

source utils.sh

it "BG-01: Background"

rm -f $CSH_OUT $BUSH_OUT
echo "sleep 5 &" > $BUSH_IN
echo "touch bgfile.tmp.txt" >> $BUSH_IN
$SHELL < $BUSH_IN > $BUSH_OUT &

sleep 1
if [ ! -f bgfile.tmp.txt ]
then
  fail
fi
succeed

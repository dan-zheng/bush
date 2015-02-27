#!/bin/bash
source ./utils.sh
it "ENV-04: Enviroment: delete variable"

rm -f $CSH_OUT $BUSH_OUT
echo "setenv aaa pppp" > $BUSH_IN
echo "printenv" >> $BUSH_IN
echo "unsetenv aaa" >> $BUSH_IN
echo "printenv" >> $BUSH_IN

$SHELL < $BUSH_IN > $BUSH_OUT

res=`grep -c aaa= $BUSH_OUT`
if [ $res -ne 1 ]
then
  fail
fi
succeed

#!/bin/bash

source utils.sh
it "BG-02: Zombie processes"

rm -f $BUSH_OUT
myname=`whoami`

before=`/bin/ps -u $myname | grep -c defunc`
echo "ls &" > $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "ls &" >> $BUSH_IN
echo "sleep 5" >> $BUSH_IN
$SHELL < $BUSH_IN > $BUSH_OUT &

sleep 1

grep test $BUSH_OUT > $CSH_OUT 2>&1
check $?

after=`/bin/ps -u $myname | grep -c defunc`

if [ $before -ne $after ]
then
    panic "Zombie processes still around ($before, $after)"
    fail
fi
succeed

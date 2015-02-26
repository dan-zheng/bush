#!/bin/sh

source utils.sh

rm -f shell-out
myname=`whoami`
pgray "BG-02: Zombie processes"

before=`/bin/ps -u $myname | grep -c defunc`
echo "ls &" > shell-in
echo "ls &" >> shell-in
echo "ls &" >> shell-in
echo "ls &" >> shell-in
echo "ls &" >> shell-in
echo "ls &" >> shell-in
echo "ls &" >> shell-in
echo "sleep 5" >> shell-in
$SHELL < shell-in > shell-out &

sleep 1

grep test shell-out > out2 2>&1
check $?

after=`/bin/ps -u $myname | grep -c defunc`

if [ $before -ne $after ]
then
    echo "Zombie processes still around ($before, $after)"
    fail
fi
succeed

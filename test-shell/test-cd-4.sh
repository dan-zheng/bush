#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "CD-04: cd notfound >& err1"
echo "cd notfound >& err1" > shell-in
echo "pwd" >> csh-in

$SHELL < shell-in > shell-out 2>&1

grep "No such file or directory" err1
check $?
succeed

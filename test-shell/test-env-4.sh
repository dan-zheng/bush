#!/bin/sh
source ./utils.sh

rm -f csh-out shell-out
pgray "ENV-04: Enviroment: delete variable"
echo "setenv aaa pppp" > shell-in
echo "printenv" >> shell-in
echo "unsetenv aaa" >> shell-in
echo "printenv" >> shell-in

$SHELL < shell-in > shell-out

res=`grep -c aaa= shell-out`
if [ $res -ne 1 ]
then
  fail
fi
succeed

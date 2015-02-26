#!/bin/sh
source ./utils.sh

rm -f csh-out shell-out
pgray "ENV-03: Enviroment: replace variable"
echo "setenv aaa bbbb" > shell-in
echo "setenv aaa pppp" >> shell-in
echo "printenv" >> shell-in

$SHELL < shell-in > shell-out

grep aaa=pppp shell-out > out 2>&1
check $?
succeed

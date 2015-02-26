#!/bin/sh

source ./utils.sh

rm -f shell-out
pgray "ENV-01: Enviroment: Set variable"
echo "setenv aaa bbbb" > shell-in
echo "printenv" >> shell-in

$SHELL < shell-in > shell-out

grep aaa=bbbb shell-out > out 2>&1
check $?
succeed

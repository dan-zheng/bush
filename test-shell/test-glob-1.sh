#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-01: echo *"
touch shell-in csh-in shell-out csh-out
echo "echo *" > shell-in
echo "echo *" > csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

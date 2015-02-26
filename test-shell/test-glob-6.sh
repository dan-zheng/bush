#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-06: echo /etc/r*/*s* "
echo "echo /etc/r*/*s*" > shell-in

/bin/sh < shell-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

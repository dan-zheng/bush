#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-07: echo /usr/lib/lib?.a"
echo "echo /usr/lib/lib?.a" > shell-in

/bin/sh < shell-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

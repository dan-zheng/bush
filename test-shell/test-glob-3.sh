#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-03: echo /usr/lib/libt*"
echo "echo /usr/lib/libt*" > shell-in
echo "echo /usr/lib/libt*" > csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

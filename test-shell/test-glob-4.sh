#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-04: echo /usr/lib/*ck*"
echo "echo /usr/lib/*ck*" > shell-in
echo "echo /usr/lib/*ck*" > csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

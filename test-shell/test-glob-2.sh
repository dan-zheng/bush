#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-02: echo /usr/lib/*"
echo "echo /usr/lib/*" > shell-in
echo "echo /usr/lib/*" > csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

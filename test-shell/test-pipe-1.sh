#!/bin/sh

source utils.sh

rm -f csh-out shell-out
pgray "PIPE-01: 2 Pipes"
echo "cat file1.cc | grep malloc" > shell-in
/bin/sh < shell-in > csh-out
$SHELL < shell-in > shell-out

diff shell-out csh-out
check $?
succeed

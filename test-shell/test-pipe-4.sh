#!/bin/sh

source utils.sh

rm -f err1 err2 csh-out shell-out
pgray "PIPE-04: Pipes and error redirection"
echo "cat file1.cc | ls nonexisting >& err1" > shell-in
echo "cat file1.cc | ls nonexisting > err2 2>&1" > csh-in
/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff err1 err2
check $?
succeed

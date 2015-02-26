#!/bin/sh

source utils.sh

rm -f out1 out2 csh-out shell-out
pgray "PIPE-03: Pipes and output redirection"
echo "cat file1.cc | grep malloc > out1" > shell-in
echo "cat file1.cc | grep malloc > out2" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff out1 out2
check $?
succeed

#!/bin/sh

source utils.sh

pgray "IO-01: Output redirection..."
rm -f out1 out2
echo "ls files > out1" > shell-in
echo "ls files > out2" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff out1 out2
check $?
succeed

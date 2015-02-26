#!/bin/sh

source ./utils.sh

rm -f out1 out2 csh-out shell-out
pgray "WORD-01: Parsing words and special characters"
echo "ls file1.cc>out1" > shell-in
echo "ls file1.cc>out2" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff out1 out2
check $?
succeed

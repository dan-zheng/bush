#!/bin/sh

source ./utils.sh

rm csh-out shell-out
pgray "WORD-02: Parsing words and special characters"
echo "ls|grep fil" > shell-in
echo "ls|grep fil" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff shell-out csh-out
check $?
succeed

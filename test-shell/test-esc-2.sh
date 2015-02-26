#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "ESC-02: ls \"command.cc Makefile\""
echo "ls file1.cc files" > shell-in
echo "ls \"file1.cc files\"" >> shell-in

echo "ls file1.cc files" >  csh-in
echo "ls \"file1.cc files\"" >> csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

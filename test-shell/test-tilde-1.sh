#!/bin/sh

source ./utils.sh

pgray "TILDE-01: Tilde expansion"

echo "ls ~ > out1" > csh-in
echo "ls ~cs180 >> out1" >> csh-in
echo "ls ~cs180/share >> out1" >> csh-in

echo "ls ~ > out2" > shell-in
echo "ls ~cs180 >> out2" >> shell-in
echo "ls ~cs180/share >> out2" >> shell-in

/bin/bash < csh-in > csh-out
$SHELL < shell-in > shell-out


diff out1 out2 > /dev/null 2>&1
check $?
succeed

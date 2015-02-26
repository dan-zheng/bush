#!/bin/sh

source ./utils.sh

rm csh-out shell-out
pgray "CD-02: cd pwd"
echo "cd" > shell-in
echo "/bin/pwd" >> shell-in
echo "cd" > csh-in
echo "/bin/pwd" >> csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

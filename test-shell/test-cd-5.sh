#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "CD-05: cd /etc"
echo "cd /etc" > shell-in
echo "pwd" >> shell-in

/bin/sh < shell-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff csh-out shell-out
check $?
succeed

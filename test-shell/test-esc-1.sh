#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "ESC-01: cd; echo lll \">\" "
echo "cd" > shell-in
echo "echo lll \">\"" >> shell-in

echo "cd" >  csh-in
echo "echo lll \">\"" >> csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

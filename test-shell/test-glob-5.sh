#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "GLOB-05: cd; echo *"
echo "cd" > shell-in
echo "echo *" >> shell-in
echo "echo .*" >> shell-in

echo "cd" >  csh-in
echo "echo *" >> csh-in
echo "echo .*" >> csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

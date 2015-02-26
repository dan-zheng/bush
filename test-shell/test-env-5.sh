#!/bin/sh
source ./utils.sh

rm -f csh-out shell-out
pgray "ENV-05: Enviroment variable expansion"
echo "setenv A hello" > shell-in
echo "setenv B world" >> shell-in
echo "echo \${A} \${B}" >> shell-in
echo "A=hello" > csh-in
echo "B=world" >> csh-in
echo "echo \${A} \${B}" >> csh-in

/bin/sh < csh-in > csh-out 2>&1
$SHELL < shell-in > shell-out

diff shell-out csh-out
check $?
succeed

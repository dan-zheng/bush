#!/bin/sh
source ./utils.sh

rm -f csh-out shell-out
pgray "ENV-02: Enviroment: Redirect printenv"
echo "/usr/bin/printenv | grep PATH" > csh-in

#Make sure that printenv is a built in command
(
PATH=/bin
export PATH
echo "printenv | grep PATH" > shell-in
)

$SHELL < shell-in > shell-out
/bin/sh < csh-in > csh-out

sort shell-out -o shell-out
sort csh-out -o csh-out

diff shell-out csh-out
check $?
succeed

#!/bin/sh
source ./utils.sh

rm -f csh-out shell-out
pgray "ENV-06: Enviroment variable expansion"
echo "setenv C ap" > shell-in
echo "setenv D le" >> shell-in
echo "echo I like \${C}p\${D}" >> shell-in

$SHELL < shell-in > shell-out

grep "I like apple" shell-out
check $?
succeed

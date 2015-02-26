#!/bin/sh

source ./utils.sh

pgray "SUBSH-01: test subshell"

echo "ls test3* > file-list " > shell-in
echo "ls -al \`cat file-list\` " >> shell-in
$SHELL < shell-in > shell-out

echo "ls test3* > file-list" > csh-in
echo "ls -al \`cat file-list\` " >> csh-in
/bin/sh < csh-in > csh-out

diff csh-out shell-out > /dev/null 2>&1
check $?
succeed

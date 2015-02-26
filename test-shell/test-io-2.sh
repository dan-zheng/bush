#!/bin/sh

source utils.sh

pgray "IO-02: Test stdin redirection..."
echo "cat < files/aaa" > shell-in
echo "cat < files/aaa" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff csh-out shell-out
check $?
succeed

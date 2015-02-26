#!/bin/sh

source utils.sh

rm -f err1 err2
rm -f csh-out shell-out
pgray "IO-03: Error redirection..."
echo "ls /none >& err1" > shell-in
echo "ls /none >err2 2>&1" > csh-in
/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff err1 err2
check $?
succeed

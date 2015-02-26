#!/bin/sh

source utils.sh

rm -f out1 out2
rm -f csh-out shell-out
pgray "IO-06: Multiple redirection..."
echo "ls > out1 > out2" > shell-in
$SHELL < shell-in > shell-out 2>&1

grep "Ambiguous output redirect" shell-out
check $?
succeed

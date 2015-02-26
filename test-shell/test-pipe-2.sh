#!/bin/sh

source utils.sh

rm -f csh-out shell-out
pgray "PIPE-02: 3 Pipes"
echo "cat file1.cc | grep malloc | grep char" > shell-in
/bin/sh < shell-in > csh-out
$SHELL < shell-in > shell-out
diff shell-out csh-out
check $?
succeed

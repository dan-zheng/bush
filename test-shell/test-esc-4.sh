#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out
pgray "ESC-04: Escape Chars"
echo "echo lll\< kkkk\\\\\\ uuuu\&" > shell-in

$SHELL < shell-in > shell-out 2>&1

grep "lll< kkkk[\] uuuu&" shell-out
check $?
succeed

#!/bin/sh

source ./utils.sh

rm -f csh-out shell-out "hello world"

pgray "ESC-03: touch \"hello world\""
echo "touch \"hello world\"" > shell-in
echo "ls" >>  shell-in

$SHELL < shell-in > shell-out 2>&1

if [ ! -f "hello world" ]; then
  fail
fi
succeed

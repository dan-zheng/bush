#!/bin/sh

source utils.sh

rm -f err1 err2 out1 out2 csh-out shell-out
pgray "IO-04: Appending output..."
echo "ls files > out1" > shell-in
echo "ls files > out1" >> shell-in
echo "ls files >> out1" >> shell-in
echo "ls files > out2" > csh-in
echo "ls files > out2" >> csh-in
echo "ls files >> out2" >> csh-in

/bin/sh < csh-in > csh-out
$SHELL < shell-in > shell-out

diff out1 out2
check $?
succeed

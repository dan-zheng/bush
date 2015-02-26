#!/bin/sh

source utils.sh

rm -f csh-out shell-out
pgray "IO-05: ls notfound >& err..."
echo "/bin/ls notfound >& err1" > shell-in
echo "/bin/ls notfound > err2 2>&1" > csh-in

/bin/sh < csh-in > csh-out 2>&1
../shell < shell-in > shell-out 2>&1

diff err1 err2
check $?

echo "ls notfound >>& err1" > shell-in
echo "ls notfound >> err2 2>&1" > csh-in

/bin/sh < csh-in > csh-out 2>&1
../shell < shell-in > shell-out 2>&1

diff err1 err2
check $?
succeed

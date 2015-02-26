#!/bin/sh

source ./utils.sh

rm csh-out shell-out
pgray "CD-03: cd ${HOME}; pwd"
echo "cd ${HOME}" > shell-in
echo "/bin/pwd" >> shell-in

/bin/sh < shell-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff shell-out csh-out
check $?
succeed

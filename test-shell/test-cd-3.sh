#!/bin/bash

source ./utils.sh
it "CD-03: cd ${HOME}; pwd"

rm $CSH_OUT $BUSH_OUT
echo "cd ${HOME}" > $BUSH_IN
echo "/bin/pwd" >> $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $BUSH_OUT $CSH_OUT
check $?
succeed

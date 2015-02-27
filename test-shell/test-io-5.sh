#!/bin/bash

source utils.sh
it "IO-05: ls notfound >& err..."

rm -f $CSH_OUT $BUSH_OUT
echo "/bin/ls notfound >& $CSH_ERR" > $BUSH_IN
echo "/bin/ls notfound > $BUSH_ERR 2>&1" > $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
../shell < $BUSH_IN > $BUSH_OUT 2>&1

diff $CSH_ERR $BUSH_ERR
check $?

echo "ls notfound >>& $CSH_ERR" > $BUSH_IN
echo "ls notfound >> $BUSH_ERR 2>&1" > $CSH_IN

/bin/sh < $CSH_IN > $CSH_OUT 2>&1
../shell < $BUSH_IN > $BUSH_OUT 2>&1

diff $CSH_ERR $BUSH_ERR
check $?
succeed

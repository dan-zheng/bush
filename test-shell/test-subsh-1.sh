#!/bin/bash

source ./utils.sh
it "SUBSH-01: test subshell"

filelist=filelist.tmp.txt

echo "ls test3* > $filelist " > $BUSH_IN
echo "ls -al \`cat $filelist\` " >> $BUSH_IN
$SHELL < $BUSH_IN > $BUSH_OUT

echo "ls test3* > $filelist" > $CSH_IN
echo "ls -al \`cat $filelist\` " >> $CSH_IN
/bin/sh < $CSH_IN > $CSH_OUT

diff $CSH_OUT $BUSH_OUT > /dev/null 2>&1
check $?
succeed

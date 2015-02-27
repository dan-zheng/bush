#!/bin/bash

source ./utils.sh
it "MISC-01: Robustness"

pgray "  The grade of this test will depend on how"
pgray "  difficult it is to grade your shell and"
pgray "  succesfully running this script."

rm -f core
echo "ls " > $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "cat file1.cc | grep char | grep c > out.tmp.txt " >> $BUSH_IN
echo "echo /d*/* | grep k" >> $BUSH_IN
echo "echo /d*/* | grep k" >> $BUSH_IN
echo "echo /d*/* | grep k" >> $BUSH_IN
echo "echo /d*/* | grep k" >> $BUSH_IN
echo "echo /d*/* | grep k" >> $BUSH_IN
echo "ls none | grep ppp" >> $BUSH_IN
echo "ls none | grep ppp" >> $BUSH_IN
echo "ls none | grep ppp" >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN
echo "ls " >> $BUSH_IN

/bin/sh < $BUSH_IN > $CSH_OUT 2>&1
$SHELL < $BUSH_IN > $BUSH_OUT 2>&1

diff $CSH_OUT $BUSH_OUT > temp-out 2>&1
if [ $? -ne 0 ]
then
  rm -fr temp-out
  fail
fi
rm -fr temp-out
succeed

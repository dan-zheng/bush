#!/bin/sh

source ./utils.sh

pgray "MISC-01: Robustness"
pgray "  The grade of this test will depend on how"
pgray "  difficult it is to grade your shell and"
pgray "  succesfully running this script."

rm -f core
echo "ls " > shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "cat file1.cc | grep char | grep c > out " >> shell-in
echo "echo /d*/* | grep k" >> shell-in
echo "echo /d*/* | grep k" >> shell-in
echo "echo /d*/* | grep k" >> shell-in
echo "echo /d*/* | grep k" >> shell-in
echo "echo /d*/* | grep k" >> shell-in
echo "ls none | grep ppp" >> shell-in
echo "ls none | grep ppp" >> shell-in
echo "ls none | grep ppp" >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in
echo "ls " >> shell-in

/bin/sh < shell-in > csh-out 2>&1
$SHELL < shell-in > shell-out 2>&1

diff csh-out shell-out > temp-out 2>&1
if [ $? -ne 0 ]
then
  rm -fr temp-out
  fail
fi
rm -fr temp-out
succeed

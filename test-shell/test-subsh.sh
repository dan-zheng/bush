#!/bin/sh

source utils.sh

grade11=0
grade11max=10
echo "--- Subshell ----------------------------------------"
./test-subsh-1.sh && grade11=`expr $grade11 "+" 10`
echo ""
suit $grade11 $grade11max
echo ""

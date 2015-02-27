#!/bin/bash

source utils.sh
describe "Subshell"

grade11=0
grade11max=10
./test-subsh-1.sh && grade11=`expr $grade11 "+" 10`
echo ""
suit $grade11 $grade11max
echo ""
